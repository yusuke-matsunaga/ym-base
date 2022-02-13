
/// @file GzEngine.cc
/// @brief GzEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "GzEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GzEngineGen
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のエンジンを作る．
CodecEngine*
GzEngineGen::new_engine(
  istream& is,
  SizeType buff_size
) const
{
  return new GzEngine{is, buff_size, mParam};
}

// @brief 圧縮用のエンジンを作る．
CodecEngine*
GzEngineGen::new_engine(
  ostream& os,
  SizeType buff_size
) const
{
  return new GzEngine{os, buff_size, mParam};
}


//////////////////////////////////////////////////////////////////////
// クラス GzEngine
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

// gzip 形式のファイルフォーマットに関する定数定義
const ymuint8 GZIP_MAGIC0  = 0x1FU;
const ymuint8 GZIP_MAGIC1  = 0x8BU;
const ymuint8 GZIP_OMAGIC1 = 0x9EU;

const off_t GZIP_TIMESTAMP = (off_t)4;
const off_t GZIP_ORIGNAME  = (off_t)10;

const ymuint8 HEAD_CRC     = 0x02U;
const ymuint8 EXTRA_FIELD  = 0x04U;
const ymuint8 ORIG_NAME	   = 0x08U;
const ymuint8 COMMENT      = 0x10U;

const ymuint8 OS_CODE      = 3;	/* Unix */

// 32ビットの符号なし整数を4バイトのデータに変換する．
inline
void
conv_to_4bytes(
  ymuint32 data,
  ymuint8 buff[]
)
{
  buff[0] = (data >>  0) & 0xFF;
  buff[1] = (data >>  8) & 0xFF;
  buff[2] = (data >> 16) & 0xFF;
  buff[3] = (data >> 24) & 0xFF;
}

// 4バイトのデータを32ビットの符号なし整数に変換する．
inline
ymuint32
conv_from_4bytes(
  ymuint8 buff[]
)
{
  ymuint32 val0 = static_cast<ymuint32>(buff[0]);
  ymuint32 val1 = static_cast<ymuint32>(buff[1]);
  ymuint32 val2 = static_cast<ymuint32>(buff[2]);
  ymuint32 val3 = static_cast<ymuint32>(buff[3]);
  return val0 | (val1 << 8) | (val2 << 16) | (val3 << 24);
}

END_NONAMESPACE

// @brief 伸張用のコンストラクタ
GzEngine::GzEngine(
  istream& is,
  SizeType buff_size,
  const GzEngineGen::Param& param
) : CodecEngine{is, buff_size}
{
  mZ.zalloc = param.alloc_func;
  mZ.zfree = param.free_func;
  mZ.opaque = param.opaque;

  inflate_init();
}

/// @brief 圧縮用のコンストラクタ
GzEngine::GzEngine(
  ostream& os,
  SizeType buff_size,
  const GzEngineGen::Param& param
) : CodecEngine{os, buff_size}
{
  mZ.zalloc = param.alloc_func;
  mZ.zfree = param.free_func;
  mZ.opaque = param.opaque;

  deflate_init(param.level);
}

// @brief デストラクタ
GzEngine::~GzEngine()
{
  if ( write_mode() ) {
    deflate_end();
  }
  else if ( read_mode() ) {
    inflate_end();
  }
}

// @brief データを伸長して読み出す．
SizeType
GzEngine::read(
  ymuint8* buff,
  SizeType size
)
{
  ASSERT_COND( read_mode() );

  // 出力バッファを設定する．
  set_outbuf(buff, size);

  int ret = 0;
  for ( ; ; ) {
    // 読み出し可能なサイズ
    SizeType size1 = read_size();
    if ( size1 == 0 ) {
      // データがなければ終わる．
      break;
    }

    // 入力データをセットする．
    set_inbuf(read_ptr(), size1);

    // 伸張する．
    ret = ::inflate(&mZ, Z_FINISH);

    // 諸元を更新する．
    SizeType nr = size1 - avail_in();
    read_seek(nr);

    switch ( ret ) {
    case Z_OK:
    case Z_STREAM_END:
      break;
    case Z_BUF_ERROR:
      if ( avail_out() > 0 ) {
	// 出力バッファが満杯でないのに BUF_ERROR ということは
	// 入力データが枯渇したということ．
	ASSERT_COND( avail_in() == 0 );
	// もう一回繰り返す．
	continue;
      }
      // わざと次に続く．
    default:
      cerr << "inflate" << endl;
      throw GzError{ret, "inflate"};
    }
    // 基本的にはループは回らない．
    break;
  }

  // 実際に読み出されたサイズ
  SizeType n = size - avail_out();

  // CRC 値を更新しておく．
  mCRC = crc32(mCRC, (const Bytef*)buff, n);
  mOutSize += n;

  if ( ret == Z_STREAM_END ) {
    // データの末尾を読んだときの処理

    // データ末尾の次の4バイトは CRC コード
    ymuint8 tmp_buff[4];
    if ( raw_read(tmp_buff, 4) != 4 ) {
      // truncated input
      cerr << "Truncated input(1)" << endl;
      throw GzError{-1, "Truncated input"};
    }

    ymuint32 orig_crc = conv_from_4bytes(tmp_buff);
    if ( orig_crc != mCRC ) {
      cerr << "CRC Error" << endl;
      throw GzError{-1, "CRC Error"};
    }

    // その次の4バイトは データ長
    if ( raw_read(tmp_buff, 4) != 4 ) {
      // truncated input
      cerr << "Truncated input(2)" << endl;
      throw GzError{-1, "Truncated input"};
    }

    SizeType orig_len = conv_from_4bytes(tmp_buff);
    if ( orig_len != mOutSize ) {
      cerr << "data-length Error" << endl;
      throw GzError{-1, "data-length Error"};
    }
  }

  return n;
}

// @brief データを圧縮して書き込む．
void
GzEngine::write(
  const ymuint8* buff,
  SizeType size
)
{
  ASSERT_COND( write_mode() );

  if ( size == 0 ) {
    return;
  }

  // 入力をセットする．
  set_inbuf(buff, size);

  // CRC の値を更新する．
  mCRC = crc32(mCRC, buff, size);
  mOutSize += size;

  // 入力が残っている限り圧縮処理を繰り返す．
  while ( avail_in() > 0 ) {
    // データを圧縮する．
    deflate_common(Z_NO_FLUSH);
  }
}

void
GzEngine::deflate_init(
  int level
)
{
  auto ret = deflateInit2(&mZ, level, Z_DEFLATED, (-MAX_WBITS),
			  8, Z_DEFAULT_STRATEGY);
  if ( ret != Z_OK ) {
    cerr << "deflateInit2" << endl;
    throw GzError{ret, "defaultInit2"};
  }

  // ヘッダを書き込む．
  // gzip 形式のファイルヘッダ
  static ymuint8 header[] = {
    GZIP_MAGIC0, GZIP_MAGIC1, Z_DEFLATED, 0,
    0, 0, 0, 0,
    0, OS_CODE
  };

  raw_write(header, sizeof(header));

  // CRC と書き込みサイズの初期化
  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0;

  set_outbuf(write_ptr(), write_size());
}

// @brief 圧縮用の終了処理を行なう．
void
GzEngine::deflate_end()
{
  // 内部で処理されていないデータが残っている可能性がある．
  while ( deflate_common(Z_FINISH) != Z_STREAM_END ) {
    ;
  }

  // 終了処理
  auto ret = deflateEnd(&mZ);
  if ( ret != Z_OK ) {
    cerr << "deflateEnd" << endl;
    throw GzError{ret, "deflateEnd"};
  }

  { // gzip 形式のファイルの末尾データを作る．
    ymuint8 trail[8];
    conv_to_4bytes(mCRC, &trail[0]);
    conv_to_4bytes(mOutSize, &trail[4]);
    raw_write(trail, sizeof trail);
  }
}

// @brief deflate の共通処理
int
GzEngine::deflate_common(
  int flash
)
{
  int ret = ::deflate(&mZ, flash);
  if ( ret != Z_OK && ret != Z_STREAM_END ) {
    // それ以外はエラー
    cerr << "deflate" << endl;
    throw GzError{ret, "deflate"};
  }

  // バッファに書き込まれたデータを処理する．
  SizeType wr = write_size() - avail_out();
  write_seek(wr);
  if ( avail_out() == 0 ) {
    set_outbuf(write_ptr(), write_size());
  }

  return ret;
}

// @brief 伸張用の初期化を行う．
void
GzEngine::inflate_init()
{
  auto ret = inflateInit2(&mZ, -MAX_WBITS);
  if ( ret != Z_OK ) {
    cerr << "inflateInit2" << endl;
    throw GzError{ret, "inflateInit2"};
  }

  // CRC と書き込みサイズの初期化
  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0;

  // ヘッダを解釈する．
  ymuint8 header[10];
  SizeType hsize = raw_read(header, sizeof(header));
  if ( hsize != sizeof(header) ||
       header[0] != GZIP_MAGIC0 ||
       ((header[1] != GZIP_MAGIC1) && (header[1] != GZIP_OMAGIC1)) ) {
    // ファイル形式が違う．
    cerr << "incorrect gzip header" << endl
	 << "hsize = " << hsize << endl
	 << "header[0] = " << hex << header[0] << dec << endl
	 << "header[1] = " << hex << header[1] << dec << endl;
    throw GzError{-1, "incorrect gzip header"};
  }

  // CM(Compression Method)
  if ( header[2] != Z_DEFLATED ) {
    // maybe_err("unknown cpression method");
    cerr << "unknown compression method" << endl;
    throw GzError{-1, "unknown cmpression method"};
  }

  // FLG(FLaGs)
  ymuint8 flags = header[3];

  // MTIME(Modification TIME)
  // XFL (eXtra FLags)
  // OS(Operationg Systeam)
  // 全部無視

  if ( flags & EXTRA_FIELD ) {
    // EXTRA_FIELD がセットされていたら次の2バイトにそのサイズが書いてある．
    ymuint8 tmp_buff[2];
    if ( raw_read(tmp_buff, 2) != 2 ) {
      cerr << "wrong EXTRA_FIELD field" << endl;
      throw GzError{-1, "wrong EXTRA_FIELD field."};
    }
    // ただし内容は読み飛ばす．
    SizeType size = static_cast<SizeType>(tmp_buff[0]) |
      (static_cast<SizeType>(tmp_buff[1]) << 8);
    if ( raw_read(nullptr, size) != size ) {
      cerr << "wrong EXTRA_FIELD field" << endl;
      throw GzError{-1, "wrong EXTRA_FIELD field."};
    }
  }

  if ( flags & ORIG_NAME ) {
    // ORIG_NAME がセットされていたら0終端の文字列が書いてある．
    // ただし内容は読み飛ばす．
    for ( ymuint8 c = ' '; c != '\0'; ) {
      if ( raw_read(&c, 1) != 1 ) {
	cerr << "wrong ORIG_NAME field" << endl;
	throw GzError{-1, "wrong ORIG_NAME field."};
      }
    }
  }

  if ( flags & COMMENT ) {
    // COMMENT がセットされていたら0終端の文字列が書いてある．
    // ただし内容は読み飛ばす．
    for ( ymuint8 c = ' '; c != '\0'; ) {
      if ( raw_read(&c, 1) != 1 ) {
	cerr << "wrong COMMENT field" << endl;
	throw GzError{-1, "wrong COMMENT field."};
      }
    }
  }

  if ( flags & HEAD_CRC ) {
    // HEAD_CRC がセットされていたら2バイトのCRCコードが書いてある．
    // でも無視する．
    ymuint8 tmp_buff[2];
    if ( raw_read(tmp_buff, 2) != 2 ) {
      cerr << "wrong HEAD_CRC field" << endl;
      throw GzError{-1, "wrong HEAD_CRC field."};
    }
  }
}

// @brief 伸張用の終了処理を行なう．
void
GzEngine::inflate_end()
{
  auto ret = inflateEnd(&mZ);
  if ( ret != Z_OK ) {
    cerr << "inflateEnd" << endl;
    throw GzError{ret, "inflateEnd"};
  }
}

END_NAMESPACE_YM
