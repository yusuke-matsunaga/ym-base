
/// @file XzFile.cc
/// @brief XzFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "XzFile.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
XzFile::XzFile(
  SizeType buff_size,
  lzma_allocator* af
) : LzmaStream{af},
    mFileBuff{buff_size}
{
}

// @brief デストラクタ
XzFile::~XzFile()
{
  close();
}

// @brief ファイルを圧縮用にオープンする．
bool
XzFile::deflate_open(
  const char* filename,
  mode_t mode,
  int preset,
  lzma_check check
)
{
  if ( mMode != None ) {
    // すでにどちらかのモードでオープン済み
    return false;
  }
  mMode = Deflate;

  int oflags = O_WRONLY | O_CREAT | O_TRUNC;
  auto ret = mFileBuff.open(filename, oflags, mode);
  if ( !ret ) {
    return false;
  }

  easy_encoder(preset, check);
  set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());

  return true;
}

// @brief ファイルを伸張用にオープンする．
bool
XzFile::inflate_open(
  const char* filename,
  SizeType memlimit,
  ymuint32 flags
)
{
  if ( mMode != None ) {
    // すでにどちらかのモードでオープン済み
    return false;
  }
  mMode = Inflate;

  int oflags = O_RDONLY;
  auto ret = mFileBuff.open(filename, oflags);
  if ( !ret ) {
    return false;
  }

  stream_decoder(memlimit, flags);
  set_inbuf(mFileBuff.read_ptr(), mFileBuff.read_size());

  return true;
}

// @brief ファイルをクローズする．
void
XzFile::close()
{
  if ( !mFileBuff.is_ready() ) {
    return;
  }

  if ( mMode == Deflate ) {
    for ( auto rcode = LZMA_OK; rcode != LZMA_STREAM_END; ) {
      rcode = code(LZMA_FINISH);
      SizeType wr = mFileBuff.write_size() - avail_out();
      mFileBuff.write_seek(wr);
      if ( avail_out() == 0 ) {
	set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
      }
    }
  }

  // 終了処理
  end();

  mFileBuff.close();
}

// @brief buff[0] - buff[num - 1] の内容を書き込む．
void
XzFile::write(
  const ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && mMode == Deflate );

  set_inbuf(buff, num);

  auto action = (num > 0) ? LZMA_RUN : LZMA_FINISH;
  while ( avail_in() > 0 ) {
    // データを圧縮する．
    auto rcode = code(action);
    // 今の処理で書き込まれた分だけバッファのポインタを進める．
    SizeType wr = mFileBuff.write_size() - avail_out();
    mFileBuff.write_seek(wr);
    if ( avail_out() == 0 ) {
      set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
    }
  }
}

// @brief num バイトを読み込み buff[] に格納する．
// @return 読み込んだバイト数を返す．
SizeType
XzFile::read(
  ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && mMode == Inflate );

  set_outbuf(buff, num);

  for ( ; ; ) {
    // 入力バッファが空なら新たなデータを読み込む．
    mFileBuff.fill();

    // 入力データをセットする．
    SizeType old_size = mFileBuff.read_size();
    if ( old_size == 0 ) {
      // 入力の読み込みが末尾に達している．
      return 0;
    }

    // 入力データをセットする．
    set_inbuf(mFileBuff.read_ptr(), old_size);

    // 伸張する．
    auto ret = code(LZMA_RUN);

    // 今回の伸張で消費した分だけ入力バッファを空読みする．
    SizeType in_size = old_size - avail_in();
    mFileBuff.read_seek(in_size);

    switch ( ret ) {
    case LZMA_OK:
      if ( avail_out() == 0 ) {
	// 出力バッファが満杯になった．
	return num;
      }
      break;
    case LZMA_STREAM_END:
      // 処理すべきデータが残っていない．
      return num - avail_out();
    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

END_NAMESPACE_YM
