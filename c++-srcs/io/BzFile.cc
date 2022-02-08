
/// @file BzFile.cc
/// @brief BzFile の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BzFile.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BzFile
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
BzFile::BzFile(
  SizeType buff_size,
  alloc_func af,
  free_func ff,
  void* op
) : BzStream{af, ff, op},
    mFileBuff{buff_size}
{
}

// @brief デストラクタ
BzFile::~BzFile()
{
  close();
}

// @brief ファイルを圧縮用に開く
bool
BzFile::deflate_open(
  const char* filename,
  mode_t mode,
  int block_size_100k,
  int verbosity,
  int work_factor
)
{
  if ( mFileBuff.is_ready() ) {
    // すでに開かれていた．
    return false;
  }

  int oflags = O_WRONLY | O_CREAT | O_TRUNC;
  bool stat = mFileBuff.open(filename, oflags, mode);
  if ( !stat ) {
    return false;
  }

  deflate_init(block_size_100k, verbosity, work_factor);

  set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());

  return true;
}

// @brief ファイルを伸張用に開く
bool
BzFile::inflate_open(
  const char* filename,
  int verbosity,
  int small
)
{
  if ( mFileBuff.is_ready() ) {
    // すでに開かれていた．
    return false;
  }

  int oflags = O_RDONLY;
  bool stat = mFileBuff.open(filename, oflags);
  if ( !stat ) {
    return false;
  }

  inflate_init(verbosity, small);

  return true;
}

// @brief ファイルを閉じる．
void
BzFile::close()
{
  if ( !mFileBuff.is_ready() ) {
    return;
  }

  if ( deflate_mode() ) {
    // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
    for ( auto rcode = BZ_OK; rcode != BZ_STREAM_END; ) {
      rcode = deflate(BZ_FINISH);
      SizeType wr = mFileBuff.write_size() - avail_out();
      mFileBuff.write_seek(wr);
      if ( avail_out() == 0 ) {
	set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
      }
    }

    // 終了処理
    deflate_end();
  }
  else if ( inflate_mode() ) {
    // 終了処理
    inflate_end();
  }

  mFileBuff.close();
}

// @brief データを書き出す．
void
BzFile::write(
  const ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && deflate_mode() );

  set_inbuf(buff, num);

  auto action = num > 0 ? BZ_RUN : BZ_FINISH;
  while ( avail_in() > 0 ) {
    // データを圧縮する．
    auto rcode = deflate(action);
    // 今の処理で書き込まれた分だけバッファのポインタを進める．
    SizeType wr = mFileBuff.write_size() - avail_out();
    mFileBuff.write_seek(wr);
    if ( avail_out() == 0 ) {
      set_outbuf(mFileBuff.write_ptr(), mFileBuff.write_size());
    }
  }
}

// @brief 圧縮されたデータを伸長してバッファに書き込む．
SizeType
BzFile::read(
  ymuint8* buff,
  SizeType num
)
{
  ASSERT_COND( mFileBuff.is_ready() && inflate_mode() );

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

    // 伸長する．
    auto rcode = inflate();

    // 今回の伸張で消費した分だけ入力バッファを空読みする．
    SizeType in_size = old_size - avail_in();
    mFileBuff.read_seek(in_size);

    switch ( rcode ) {
    case BZ_OK:
      if ( avail_out() == 0 ) {
	// 出力バッファが満杯になったら今回の処理は終わり
	return num;
      }
      break;

    case BZ_STREAM_END:
      // データの末尾を読んだときの処理
      return num - avail_out();

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }
}

END_NAMESPACE_YM
