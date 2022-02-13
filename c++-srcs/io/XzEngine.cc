
/// @file XzEngine.cc
/// @brief XzEngine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "XzEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス XzEngineGen
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のエンジンを作る．
CodecEngine*
XzEngineGen::new_engine(
  istream& is,
  SizeType buff_size
) const
{
  return new XzEngine{is, buff_size, mParam};
}

// @brief 圧縮用のエンジンを作る．
CodecEngine*
XzEngineGen::new_engine(
  ostream& os,
  SizeType buff_size
) const
{
  return new XzEngine{os, buff_size, mParam};
}


//////////////////////////////////////////////////////////////////////
// クラス XzEngine
//////////////////////////////////////////////////////////////////////

// @brief 伸張用のコンストラクタ
XzEngine::XzEngine(
  istream& is,
  SizeType buff_size,
  const XzEngineGen::Param& param
) : CodecEngine{is, buff_size}
{
  mLzmaStream = (lzma_stream)LZMA_STREAM_INIT;
  if ( param.allocator != nullptr ) {
    mLzmaStream.allocator = param.allocator;
  }
  inflate_init(param.memlimit, param.flags);
}

// @brief 圧縮用のコンストラクタ
XzEngine::XzEngine(
  ostream& os,
  SizeType buff_size,
  const XzEngineGen::Param& param
) : CodecEngine{os, buff_size}
{
  mLzmaStream = (lzma_stream)LZMA_STREAM_INIT;
  if ( param.allocator != nullptr ) {
    mLzmaStream.allocator = param.allocator;
  }
  deflate_init(param.preset, param.check);
}

// @brief デストラクタ
XzEngine::~XzEngine()
{
  if ( write_mode() ) {
    deflate_end();
  }
  lzma_end(&mLzmaStream);
}

// @brief データを伸長して読み出す．
SizeType
XzEngine::read(
  ymuint8* buff,
  SizeType size
)
{
  ASSERT_COND( read_mode() );

  set_outbuf(buff, size);

  while ( avail_out() > 0 ) {
    // 入力データをセットする．
    SizeType old_size = read_size();
    if ( old_size == 0 ) {
      // 入力の読み込みが末尾に達している．
      return 0;
    }

    // 入力データをセットする．
    set_inbuf(read_ptr(), old_size);

    // 伸張する．
    auto ret = lzma_code(&mLzmaStream, LZMA_RUN);
    if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
      throw LzmaError{ret, "lzma_code"};
    }

    // 今回の伸張で消費した分だけ入力バッファを空読みする．
    SizeType in_size = old_size - avail_in();
    read_seek(in_size);

    if ( ret == LZMA_STREAM_END ) {
      break;
    }
  }
  return size - avail_out();
}

// @brief データを圧縮して書き込む．
//
// エラーの場合には例外が送出される．
void
XzEngine::write(
  const ymuint8* buff,
  SizeType size
)
{
  ASSERT_COND( write_mode() );

  if ( size == 0 ) {
    return;
  }

  set_inbuf(buff, size);

  while ( avail_in() > 0 ) {
    deflate_common(LZMA_RUN);
  }
}

// @brief 圧縮用の初期化
void
XzEngine::deflate_init(
  int preset,
  lzma_check check
)
{
  auto ret = lzma_easy_encoder(&mLzmaStream, preset, check);
  if ( ret != LZMA_OK ) {
    throw LzmaError{ret, "lzma_easy_encoder"};
  }
  set_outbuf(write_ptr(), write_size());
}

// @brief deflate の終了処理
void
XzEngine::deflate_end()
{
  while ( deflate_common(LZMA_FINISH) != LZMA_STREAM_END ) {
    ;
  }
}

// @brief deflate の共通処理
lzma_ret
XzEngine::deflate_common(
  lzma_action action
)
{
  auto ret = lzma_code(&mLzmaStream, action);
  if ( ret != LZMA_OK && ret != LZMA_STREAM_END ) {
    throw LzmaError{ret, "lzma_code"};
  }

  SizeType wr = write_size() - avail_out();
  write_seek(wr);
  if ( avail_out() == 0 ) {
    set_outbuf(write_ptr(), write_size());
  }
  return ret;
}

// @brief 伸張用の初期化
void
XzEngine::inflate_init(
  SizeType memlimit,
  ymuint32 flags
)
{
  auto ret = lzma_stream_decoder(&mLzmaStream, memlimit, flags);
  if ( ret != LZMA_OK ) {
    throw LzmaError{ret, "lzma_stream_decoder"};
  }
  set_inbuf(read_ptr(), read_size());
}

END_NAMESPACE_YM
