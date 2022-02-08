
/// @file GzFile.cc
/// @brief GzFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "GzFile.h"


// gzip 形式のファイルフォーマットに関する定数定義
#define GZIP_MAGIC0	0x1F
#define GZIP_MAGIC1	0x8B
#define GZIP_OMAGIC1	0x9E

#define GZIP_TIMESTAMP	(off_t)4
#define GZIP_ORIGNAME	(off_t)10

#define HEAD_CRC	0x02
#define EXTRA_FIELD	0x04
#define ORIG_NAME	0x08
#define COMMENT		0x10

#define OS_CODE		3	/* Unix */


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GzFile
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
GzFile::GzFile(
  SizeType buff_size,
  alloc_func af,
  free_func ff,
  voidp op
) : ZStream{af, ff, op},
    mFileBuff{buff_size}
{
}

// @brief デストラクタ
GzFile::~GzFile()
{
  close();
}

// @brief ファイルを圧縮用に開く
bool
GzFile::deflate_open(
  const char* filename,
  mode_t mode,
  int level
)
{
  if ( deflate_mode() || inflate_mode() ) {
    // すでにどちらかのモードでオープン済み
    return false;
  }

  int oflags = O_WRONLY | O_CREAT | O_TRUNC;
  auto ret = mFileBuff.open(filename, oflags, mode);
  if ( !ret ) {
    return false;
  }

  // ヘッダを書き込む．
  // gzip 形式のファイルヘッダ
  static ymuint8 header[] = {
    GZIP_MAGIC0, GZIP_MAGIC1, Z_DEFLATED, 0,
    0, 0, 0, 0,
    0, OS_CODE
  };

  mFileBuff.write(header, sizeof(header));

  deflate_init2(level, Z_DEFLATED, (-MAX_WBITS),
		8, Z_DEFAULT_STRATEGY);

  // CRC と書き込みサイズの初期化
  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0;

  set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());

  return true;
}

// @brief ファイルを伸張用に開く
// @retval true 成功した
// @retval false 失敗した．
//
// 失敗する理由は以下の通り
//  - ファイルが存在しないか読み出し許可がない．
bool
GzFile::inflate_open(
  const char* filename ///< [in] ファイル名
)
{
  if ( deflate_mode() || inflate_mode() ) {
    // すでにどちらかのモードでオープン済み
    return false;
  }

  int oflags = O_RDONLY;
  bool ret = mFileBuff.open(filename, oflags);
  if ( !ret ) {
    return false;
  }

  // ヘッダを解釈する．
  ymuint8 header[10];
  SizeType hsize = mFileBuff.read(header, sizeof(header));
  if ( hsize != sizeof(header) ||
       header[0] != GZIP_MAGIC0 ||
       ((header[1] != GZIP_MAGIC1) && (header[1] != GZIP_OMAGIC1)) ) {
    // ファイル形式が違う．
    throw ZlibError{-1, "incorrect gzip header"};
  }

  // CM(Compression Method)
  if ( header[2] != Z_DEFLATED ) {
    // maybe_err("unknown cpression method");
    throw ZlibError{-1, "unknown cmpression method"};
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
    if ( mFileBuff.read(tmp_buff, 2) != 2 ) {
      throw ZlibError{-1, "wrong EXTRA_FIELD field."};
    }
    // ただし内容は読み飛ばす．
    int size = static_cast<int>(tmp_buff[0]) |
      (static_cast<int>(tmp_buff[1]) << 8);
    if ( !mFileBuff.dummy_read(size) ) {
      throw ZlibError{-1, "wrong EXTRA_FIELD field."};
    }
  }

  if ( flags & ORIG_NAME ) {
    // ORIG_NAME がセットされていたら0終端の文字列が書いてある．
    // ただし内容は読み飛ばす．
    for ( ymuint8 c = ' '; c != '\0'; ) {
      if ( mFileBuff.read(&c, 1) != 1 ) {
	throw ZlibError{-1, "wrong ORIG_NAME field."};
      }
    }
  }

  if ( flags & COMMENT ) {
    // COMMENT がセットされていたら0終端の文字列が書いてある．
    // ただし内容は読み飛ばす．
    for ( ymuint8 c = ' '; c != '\0'; ) {
      if ( mFileBuff.read(&c, 1) != 1 ) {
	throw ZlibError{-1, "wrong COMMENT field."};
      }
    }
  }

  if ( flags & HEAD_CRC ) {
    // HEAD_CRC がセットされていたら2バイトのCRCコードが書いてある．
    // でも無視する．
    ymuint8 tmp_buff[2];
    if ( mFileBuff.read(tmp_buff, 2) != 2 ) {
      throw ZlibError{-1, "wrong HEAD_CRC field."};
    }
  }

  // zstream の初期化
  inflate_init2(- MAX_WBITS);

  mCRC = crc32(0L, Z_NULL, 0);
  mOutSize = 0U;

  return true;
}


BEGIN_NONAMESPACE

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

// @brief ファイルを閉じる．
// @note 以降の書き込みは行われない．
void
GzFile::close()
{
  if ( !mFileBuff.is_ready() ) {
    return;
  }

  if ( deflate_mode() ) {
    // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
    for ( auto rcode = Z_OK; rcode != Z_STREAM_END; ) {
      rcode = deflate(Z_FINISH);

      SizeType wr = mFileBuff.write_size() - avail_out();
      mFileBuff.write_seek(wr);
      if ( avail_out() == 0 ) {
	set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
      }
    }

    // 終了処理
    deflate_end();

    { // gzip 形式のファイルの末尾データを作る．
      ymuint8 trail[8];
      conv_to_4bytes(mCRC, &trail[0]);
      conv_to_4bytes(mOutSize, &trail[4]);
      mFileBuff.write(trail, sizeof(trail));
    }
  }
  else if ( inflate_mode() ) {
    inflate_end();
  }
  mFileBuff.close();
}

// @brief データを書き出す．
void
GzFile::write(
  const ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && deflate_mode() );

  if ( num == 0 ) {
    return;
  }

  set_inbuf(buff, num);
  mCRC = crc32(mCRC, buff, num);
  mOutSize += num;

  while ( avail_in() > 0 ) {
    // データを圧縮する．
    auto ret = deflate(Z_NO_FLUSH);
    SizeType wr = mFileBuff.write_size() - avail_out();
    mFileBuff.write_seek(wr);
    if ( avail_out() == 0 ) {
      set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
    }
  }
}

// @brief 圧縮されたデータを伸長してバッファに書き込む．
SizeType
GzFile::read(
  ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && inflate_mode() );

  set_outbuf(buff, num);

  int ret = 0;
  for ( ; ; ) {
    // ここの for ループが回るのは Z_BUF_ERROR の continue の時のみ

    // 入力バッファが空なら新たなデータを読み込む．
    mFileBuff.fill();

    SizeType old_size = mFileBuff.read_size();
    if ( old_size == 0 ) {
      break;
    }

    // 入力データをセットする．
    set_inbuf(mFileBuff.read_ptr(), old_size);

    // 伸長する．
    ret = inflate(Z_FINISH);

    // 今回の inflate で消費した分だけ入力バッファを空読みする．
    SizeType in_size = old_size - avail_in();
    mFileBuff.read_seek(in_size);

    switch ( ret ) {
    case Z_OK:
      break;

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
      // こちらは出力バッファが満杯になってしまった．
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
    // 基本的にはループを回らない．
    break;
  }

  // バッファに書き込まれた量を計算する．
  SizeType wr = num - avail_out();
  if ( wr != 0 ) {
    // CRC 値を更新しておく．
    mCRC = crc32(mCRC, (const Bytef*)buff, wr);
    mOutSize += wr;
  }

  if ( ret == Z_STREAM_END ) {
    // データの末尾を読んだときの処理

    // データ末尾の次の4バイトは CRC コード
    ymuint8 tmp_buff[4];
    if ( mFileBuff.read(tmp_buff, 4) != 4 ) {
      // truncated input
      throw ZlibError{-1, "Truncated input"};
    }

    ymuint32 orig_crc = conv_from_4bytes(tmp_buff);
    if ( orig_crc != mCRC ) {
      throw ZlibError{-1, "CRC Error"};
    }

    // その次の4バイトは データ長
    if ( mFileBuff.read(tmp_buff, 4) != 4 ) {
      // truncated input
      throw ZlibError{-1, "Truncated input"};
    }
    int orig_len = static_cast<int>(conv_from_4bytes(tmp_buff));
    if ( orig_len != mOutSize ) {
      throw ZlibError{-1, "data-length Error"};
    }
  }

  return wr;
}

END_NAMESPACE_YM
