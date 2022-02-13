
/// @file BzEngine.cc
/// @brief BzEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "BzEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス BzEngineGen
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のエンジンを作る．
CodecEngine*
BzEngineGen::new_engine(
  istream& is,
  SizeType buff_size
) const
{
  return new BzEngine{is, buff_size, mParam};
}

// @brief 圧縮用のエンジンを作る．
CodecEngine*
BzEngineGen::new_engine(
  ostream& os,
  SizeType buff_size
) const
{
  return new BzEngine{os, buff_size, mParam};
}


//////////////////////////////////////////////////////////////////////
// クラス BzEngine
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のコンストラクタ
BzEngine::BzEngine(
  istream& is,
  SizeType buff_size,
  const BzEngineGen::Param& param
) : CodecEngine{is, buff_size}
{
  mBzStream.bzalloc = param.alloc_func;
  mBzStream.bzfree = param.free_func;
  mBzStream.opaque = param.opaque;

  inflate_init(param.verbosity, param.small);
}

// @brief コンストラクタ
BzEngine::BzEngine(
  ostream& os,
  SizeType buff_size,
  const BzEngineGen::Param& param
) : CodecEngine{os, buff_size}
{
  mBzStream.bzalloc = param.alloc_func;
  mBzStream.bzfree = param.free_func;
  mBzStream.opaque = param.opaque;

  deflate_init(param.block_size_100k, param.verbosity, param.work_factor);
}

// @brief デストラクタ
BzEngine::~BzEngine()
{
  if ( read_mode() ) {
    inflate_end();
  }
  else if ( write_mode() ) {
    deflate_end();
  }
}

// @brief データを伸長して読み出す．
// @return 読み出したデータサイズを返す．
//
// エラーの場合には例外が送出される．
SizeType
BzEngine::read(
  ymuint8* buff, ///< [in] 読み出したデータを格納するバッファ
  SizeType size  ///< [in] 読み出すデータの最大サイズ
)
{
  ASSERT_COND( read_mode() );

  set_outbuf(buff, size);

  for ( ; ; ) {
    // 入力データをセットする．
    SizeType old_size = read_size();
    if ( old_size == 0 ) {
      // 入力の読み込みが末尾に達している．
      return 0;
    }

    // 入力データをセットする．
    set_inbuf(read_ptr(), old_size);

    // 伸長する．
    auto rcode = BZ2_bzDecompress(&mBzStream);

    // 今回の伸張で消費した分だけ入力バッファを空読みする．
    SizeType in_size = old_size - avail_in();
    read_seek(in_size);

    switch ( rcode ) {
    case BZ_OK:
      if ( avail_out() == 0 ) {
	// 出力バッファが満杯になったら今回の処理は終わり
	return size;
      }
      break;

    case BZ_STREAM_END:
      // データの末尾を読んだときの処理
      return size - avail_out();

    default:
      // それ以外はエラー
      throw BzError{rcode, "BZ2_bzDecompress"};
      break;
    }
  }
}

// @brief データを圧縮して書き込む．
//
// エラーの場合には例外が送出される．
void
BzEngine::write(
  const ymuint8* buff, ///< [in] 入力データのバッファ
  SizeType size        ///< [in] データサイズ
)
{
  ASSERT_COND( write_mode() );

  if ( size == 0 ) {
    return;
  }

  set_inbuf(buff, size);

  while ( avail_in() > 0 ) {
    // データを圧縮する．
    deflate_common(BZ_RUN);
  }
}

// @brief 圧縮用の初期化を行う．
void
BzEngine::deflate_init(
  int block_size_100k,
  int verbosity,
  int work_factor
)
{
  auto ret = BZ2_bzCompressInit(&mBzStream, block_size_100k, verbosity, work_factor);
  if ( ret != BZ_OK ) {
    throw BzError{ret, "BZ2_bzCompressInit"};
  }

  set_outbuf(write_ptr(), write_size());
}

// @brief 圧縮を行う．
int
BzEngine::deflate_common(
  int action ///< [in] 動作コード
  ///<  - BZ_RUN
  ///<  - BZ_FLUSH
  ///<  - BZ_FINISH
)
{
  auto ret = BZ2_bzCompress(&mBzStream, action);
  switch ( ret ) {
  case BZ_RUN_OK:
  case BZ_FLUSH_OK:
  case BZ_FINISH_OK:
  case BZ_STREAM_END:
    break;
  default:
    // それ以外はエラー
    throw BzError{ret, "BZ2_bzCompress"};
  }

  // バッファに書き込まれたデータを処理する．
  SizeType wr = write_size() - avail_out();
  write_seek(wr);
  if ( avail_out() == 0 ) {
    set_outbuf(write_ptr(), write_size());
  }

  return ret;
}

// @brief 圧縮を終了する．
void
BzEngine::deflate_end()
{
  // 入力データはないけど，内部で書き出されていないデータが残っている可能性がある．
  while ( deflate_common(BZ_FINISH) != BZ_STREAM_END ) {
    ;
  }

  auto ret = BZ2_bzCompressEnd(&mBzStream);
  if ( ret != BZ_OK ) {
    throw BzError{ret, "BZ2_bzCompressEnd"};
  }
}

// @brief 伸張用のの初期化を行う．
void
BzEngine::inflate_init(
  int verbosity,
  int small
)
{
  auto ret = BZ2_bzDecompressInit(&mBzStream, verbosity, small);
  if ( ret != BZ_OK ) {
    throw BzError{ret, "BZ2_bzDecompressInit"};
  }
}

// @brief 伸張を終了する．
void
BzEngine::inflate_end()
{
  auto ret = BZ2_bzDecompressEnd(&mBzStream);
  if ( ret != BZ_OK ) {
    throw BzError{ret, "BZ2_bzDecompressEnd"};
  }
}

END_NAMESPACE_YM
