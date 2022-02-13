﻿
/// @file ZEngine.cc
/// @brief ZEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ZEngine.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
// クラス ZEngineGen
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のエンジンを作る．
CodecEngine*
ZEngineGen::new_engine(
  istream& is,
  SizeType buff_size
) const
{
  return new ZEngine{is, buff_size, mParam};
}

// @brief 圧縮用のエンジンを作る．
CodecEngine*
ZEngineGen::new_engine(
  ostream& os,
  SizeType buff_size
) const
{
  return new ZEngine{os, buff_size, mParam};
}


//////////////////////////////////////////////////////////////////////
// クラス ZEngine
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// magic number
const ZEngine::char_type k_MAGICHEADER[] = { 037, 0235 };

// 左端(msb)から1を埋めていくビットパタン
const ZEngine::char_type lmask[] = {
  0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00
};

// 右端(lsb)から1を埋めていくビットパタン
const ZEngine::char_type rmask[] = {
  0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff
};

const ZEngine::char_type k_BIT_MASK = 0x1f;

const ZEngine::char_type k_BLOCK_MASK = 0x80;

// Initial number of bits/code
const ymuint32 k_INIT_BITS = 9;

// Ratio check interval
const ZEngine::count_int k_CHECK_GAP = 10000;

// First free entry
const ZEngine::code_int k_FIRST = 257;

// Table clear output code
const ZEngine::code_int k_CLEAR = 256;

inline
ymuint32
MAXCODE(
  ymuint32 n_bits
)
{
  return (1UL << n_bits) - 1;
}

// デバッグ用の表時間数
inline
void
print_code(
  ostream& s,
  ZEngine::char_type suffix
)
{
  s << static_cast<int>(suffix);
  if ( isprint(suffix) ) {
    s << " \"" << suffix << "\"";
  }
  else if ( suffix == '\t' ) {
    s << " TAB";
  }
  else if ( suffix == '\r' ) {
    s << " CR";
  }
  else if ( suffix == '\n' ) {
    s << " LF";
  }
}

END_NONAMESPACE

// @brief 伸張用のコンストラクタ
ZEngine::ZEngine(
  istream& is,
  SizeType buff_size,
  const ZEngineGen::Param& param
) : CodecEngine{is, buff_size}
{
  inflate_init();
}

// @brief 圧縮用のコンストラクタ
ZEngine::ZEngine(
  ostream& os,
  SizeType buff_size,
  const ZEngineGen::Param& param
) : CodecEngine{os, buff_size}
{
  deflate_init(param.level);
}

// @brief デストラクタ
ZEngine::~ZEngine()
{
  if ( write_mode() ) {
    output(m_ent);

    ++ m_out_count;

    output(static_cast<code_int>(-1));
  }
}

// @brief 圧縮されたファイルを読んで最大 num バイトをバッファに格納する．
SizeType
ZEngine::read(
  ymuint8* rbuff,
  SizeType num
)
{
  ASSERT_COND( read_mode() );

  if ( num == 0 ) {
    return 0;
  }

  SizeType count = num;
  ymuint8* bp = rbuff;

  switch ( m_state ) {
  case START_STATE:
    m_state = MIDDLE_STATE;
    break;

  case MIDDLE_STATE:
    goto middle;

  case EOF_STATE:
    goto eof;
  }

  // 先頭のマジックナンバーと最大ビット数を読む．
  {
    ymuint8 header[3];
    int n = raw_read(header, sizeof(header));
    if ( n != sizeof(header) ||
	 memcmp(header, k_MAGICHEADER, sizeof(k_MAGICHEADER)) != 0 ) {
      // ファイルタイプミスマッチ
      throw ZError{"invalid magic header"};
    }
    m_maxbits = header[2];
    m_block_compress = m_maxbits & k_BLOCK_MASK;
    m_maxbits &= k_BIT_MASK;
    m_maxmaxcode = 1ULL << m_maxbits;
    if ( m_maxbits > k_BITS || m_maxbits < 12 ) {
      // EFTYPE
      throw ZError{"incorrect m_maxbits"};
    }

    m_n_bits = k_INIT_BITS;
    m_maxcode = MAXCODE(m_n_bits);
  }

  for (code_int code = 255; code >= 0; -- code) {
    tab_prefixof(code) = 0;
    tab_suffixof(code) = static_cast<char_type>(code);
  }
  m_free_ent = m_block_compress ? k_FIRST : 256;

  m_oldcode = getcode();
  m_finchar = static_cast<char_type>(m_oldcode & 0xFF);
  if ( m_oldcode == -1 ) {
    return 0;
  }

  *bp ++ = m_finchar;
  -- count;
  init_stack();

  for (code_int code; (code = getcode()) > -1; ) {
    if ( (code == k_CLEAR) && m_block_compress ) {
      for (code = 255; code >= 0; -- code) {
	tab_prefixof(code) = 0;
      }
      m_clear_flg = 1;
      m_free_ent = k_FIRST;
      m_oldcode = -1;
      continue;
    }
    m_incode = code;

    if ( code >= m_free_ent ) {
      if ( code > m_free_ent || m_oldcode == -1 ) {
	// Bad stream
	// EINVAL;
	throw ZError{"Bad stream"};
      }
      push_stack(m_finchar);
      code = m_oldcode;
    }

    while ( code >= 256 ) {
      push_stack(tab_suffixof(code));
      code = tab_prefixof(code);
    }
    m_finchar = tab_suffixof(code);
    push_stack(m_finchar);

  middle:
    do {
      if ( count -- == 0 ) {
	return num;
      }
      *bp ++ = pop_stack();
    } while ( !is_empty() );

    {
      code_int code = m_free_ent;
      if ( code < m_maxmaxcode && m_oldcode != -1 ) {
	tab_prefixof(code) = m_oldcode;
	tab_suffixof(code) = m_finchar;
	m_free_ent = code + 1;
      }
    }

    m_oldcode = m_incode;
  }

  m_state = EOF_STATE;

 eof:
  return num - count;
}

// @brief 最大 num バイトのデータを圧縮してファイルに書き込む．
void
ZEngine::write(
  const ymuint8* wbuff,
  SizeType num
)
{
  ASSERT_COND( write_mode() );

  if ( num == 0 ) {
    return;
  }

  SizeType count = num;
  const ymuint8* bp = wbuff;

  if ( m_state == START_STATE ) {
    m_state = MIDDLE_STATE;

    m_maxmaxcode = 1ULL << m_maxbits;
    raw_write(k_MAGICHEADER, sizeof(k_MAGICHEADER));

    ymuint8 tmp = static_cast<ymuint8>(m_maxbits) | m_block_compress;
    raw_write(&tmp, 1);

    m_offset = 0;
    m_bytes_out = 3;
    m_out_count = 0;
    m_clear_flg = 0;
    m_ratio = 0;
    m_in_count = 1;
    m_checkpoint = k_CHECK_GAP;
    m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
    m_free_ent = m_block_compress ? k_FIRST : 256;

    m_ent = *(bp ++);
    -- count;

    m_hshift = 0;
    for (code_int fcode = m_hsize; fcode < 65536L; fcode *= 2L) {
      ++ m_hshift;
    }
    m_hshift = 8 - m_hshift;

    m_hsize_reg = m_hsize;
    cl_hash(static_cast<count_int>(m_hsize_reg));
  }

  for ( code_int i = 0; count -- > 0; ) {
    code_int disp;
    ymuint8 c = *(bp ++ );
    ++ m_in_count;
    code_int fcode = (static_cast<code_int>(c) << m_maxbits) + m_ent;
    i = (c << m_hshift) ^ m_ent;

    if ( htabof(i) == fcode ) {
      m_ent = codetabof(i);
      continue;
    }
    else if ( static_cast<long>(htabof(i)) < 0 ) {
      goto nomatch;
    }

    if ( i == 0 ) {
      disp = 1;
    }
    else {
      disp = m_hsize_reg - i;
    }

  probe:
    if ( (i -= disp) < 0 ) {
      i += m_hsize_reg;
    }

    if ( htabof(i) == fcode ) {
      m_ent = codetabof(i);
      continue;
    }
    if ( static_cast<long>(htabof(i)) >= 0 ) {
      goto probe;
    }

  nomatch:
    output(m_ent);

    ++ m_out_count;
    m_ent = c;

    if ( m_free_ent < m_maxmaxcode ) {
      codetabof(i) = m_free_ent ++;
      htabof(i) = fcode;
    }
    else if ( m_in_count >= m_checkpoint && m_block_compress ) {
      cl_block();
    }
  }
}

// @brief 圧縮用の初期化
void
ZEngine::deflate_init(
  int level
)
{
  m_state = START_STATE;

  m_maxbits = level ? level : k_BITS;	/* User settable max # bits/code. */
  m_maxmaxcode = 1ULL << m_maxbits;	/* Should NEVER generate this code. */
  m_free_ent = 0;			/* First unused entry. */
  m_block_compress = k_BLOCK_MASK;
  m_clear_flg = 0;

  m_hsize = k_HSIZE;			/* For dynamic table sizing. */
  m_ratio = 0;
  m_checkpoint = k_CHECK_GAP;
  m_in_count = 1;			/* Length of input. */
  m_out_count = 0;			/* # of codes output (for debugging). */
}

// @brief 伸張用の初期化
void
ZEngine::inflate_init()
{
  m_state = START_STATE;

  m_maxbits = k_BITS;	/* User settable max # bits/code. */
  m_maxmaxcode = 1ULL << m_maxbits;	/* Should NEVER generate this code. */
  m_free_ent = 0;			/* First unused entry. */
  m_block_compress = k_BLOCK_MASK;
  m_clear_flg = 0;

  m_roffset = 0;
  m_size = 0;
}

// Table clear for block compress
void
ZEngine::cl_block()
{
  m_checkpoint = m_in_count + k_CHECK_GAP;

  count_int rat;
  if ( m_in_count > 0x007fffff ) { // Shift will overflow.
    rat = m_bytes_out >> 8;
    if ( rat == 0 ) {
      rat = 0x7fffffff;
    }
    else {
      rat = m_in_count / rat;
    }
  }
  else {
    rat = (m_in_count << 8) / m_bytes_out;
  }
  if ( rat > m_ratio ) {
    m_ratio = rat;
  }
  else {
    m_ratio = 0;
    cl_hash(m_hsize);
    m_free_ent = k_FIRST;
    m_clear_flg = 1;
    output(k_CLEAR);
  }
}

void
ZEngine::cl_hash(
  count_int cl_hsize
)
{
  long m1 = -1;
  count_int* htab_p = m_htab + cl_hsize;
  count_int i = cl_hsize - 16;
  do {			/* Might use Sys V memset(3) here. */
    *(htab_p - 16) = m1;
    *(htab_p - 15) = m1;
    *(htab_p - 14) = m1;
    *(htab_p - 13) = m1;
    *(htab_p - 12) = m1;
    *(htab_p - 11) = m1;
    *(htab_p - 10) = m1;
    *(htab_p - 9) = m1;
    *(htab_p - 8) = m1;
    *(htab_p - 7) = m1;
    *(htab_p - 6) = m1;
    *(htab_p - 5) = m1;
    *(htab_p - 4) = m1;
    *(htab_p - 3) = m1;
    *(htab_p - 2) = m1;
    *(htab_p - 1) = m1;
    htab_p -= 16;
  } while ( (i -= 16) >= 0 );
  for (i += 16; i > 0; i--) {
    *(-- htab_p) = m1;
  }
}

void
ZEngine::output(
  code_int ocode
)
{
  SizeType r_off = m_offset;
  SizeType bits = m_n_bits;
  char_type* bp = m_buf;

  if ( ocode >= 0 ) {
    // Get to the first byte.
    bp += (r_off >> 3);
    r_off &= 7;

    // Since ocode is always >= 8 bits, only need to mask the first
    // hunk on the left.
    *bp = (*bp & rmask[r_off]) | ((ocode << r_off) & lmask[r_off]);
    bp++;
    bits -= (8 - r_off);
    ocode >>= 8 - r_off;

    // Get any 8 bit parts in the middle (<=1 for up to 16 bits).
    if ( bits >= 8 ) {
      *bp ++ = static_cast<char_type>(ocode & 0xFFL);
      ocode >>= 8;
      bits -= 8;
    }

    // Last bits.
    if ( bits ) {
      *bp = static_cast<char_type>(ocode);
    }
    m_offset += m_n_bits;
    if ( m_offset == (m_n_bits << 3) ) {
      bp = m_buf;
      bits = m_n_bits;
      m_bytes_out += bits;
      raw_write(bp, bits);
      bp += bits;
      bits = 0;
      m_offset = 0;
    }

    // If the next entry is going to be too big for the ocode size,
    // then increase it, if possible.
    if ( m_free_ent > m_maxcode || (m_clear_flg > 0)) {
      // Write the whole buffer, because the input side won't
      // discover the size increase until after it has read it.
      if ( m_offset > 0 ) {
	raw_write(m_buf, m_n_bits);
	m_bytes_out += m_n_bits;
      }
      m_offset = 0;

      if ( m_clear_flg ) {
	m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
	m_clear_flg = 0;
      }
      else {
	++ m_n_bits;
	if ( m_n_bits == m_maxbits ) {
	  m_maxcode = m_maxmaxcode;
	}
	else {
	  m_maxcode = MAXCODE(m_n_bits);
	}
      }
    }
  }
  else {
    // At EOF, write the rest of the buffer.
    if ( m_offset > 0 ) {
      m_offset = (m_offset + 7) / 8;
      raw_write(m_buf, m_offset);
      m_bytes_out += m_offset;
    }
    m_offset = 0;
  }
}

ZEngine::code_int
ZEngine::getcode()
{
  ymuint8* bp = m_gbuf;
  if ( m_clear_flg > 0 || m_roffset >= m_size || m_free_ent > m_maxcode ) {
    if ( m_free_ent > m_maxcode ) {
      ++ m_n_bits;
      if ( m_n_bits == m_maxbits ) {
	m_maxcode = m_maxmaxcode;
      }
      else {
	m_maxcode = MAXCODE(m_n_bits);
      }
    }
    if ( m_clear_flg > 0 ) {
      m_maxcode = MAXCODE(m_n_bits = k_INIT_BITS);
      m_clear_flg = 0;
    }
    auto n = raw_read(m_gbuf, m_n_bits);
    if ( n == 0 ) {
      return -1;
    }
    m_roffset = 0;
    int n1 = n;
    m_size = (n1 << 3) - (m_n_bits - 1);
  }
  auto r_off = m_roffset;
  auto bits = m_n_bits;

  // Get to the first byte.
  bp += static_cast<ymuint8>(r_off >> 3);
  r_off &= 7;

  // Get first part (low order bits).
  code_int gcode = *(bp ++) >> r_off;
  bits -= (8 - r_off);
  r_off = 8 - r_off;

  // Get any 8 bit parts in the middle (<= 1 for up to 16 bits).
  if ( bits >= 8 ) {
    gcode |= *(bp ++ ) << r_off;
    r_off += 8;
    bits -= 8;
  }

  // High order bits.
  gcode |= (*bp & rmask[bits]) << r_off;
  m_roffset += m_n_bits;

  return gcode;
}

END_NAMESPACE_YM
