
/// @file zstream.cc
/// @brief zstream の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "zstream.h"


BEGIN_NAMESPACE_YM

// @brief 入力用のコンストラクタ
zstream_buff::zstream_buff(
  istream* s,
  SizeType size
) : mInStream{s},
    mOutStream{nullptr},
    mBuff{new Bytef[size]},
    mSize{size},
    mInFlush(Z_NO_FLUSH)
{
  inflate_init();

  set_inbuf(Z_NULL, 0);
}

// @brief 出力用のコンストラクタ
zstream_buff::zstream_buff(
  ostream* s,
  SizeType size
) : mInStream{nullptr},
    mOutStream{s},
    mBuff{new Bytef[size]},
    mSize{size},
    mInFlush{Z_NO_FLUSH}
{
  deflate_init(Z_DEFAULT_COMPRESSION);

  set_outbuf(mBuff, mSize);
}

// @brief デストラクタ
zstream_buff::~zstream_buff()
{
  delete [] mBuff;
}

// @brief データの圧縮を行う．
void
zstream_buff::compress(
  Bytef* buff,
  SizeType size,
  int flush
)
{
  set_inbuf(buff, size);
  for( ; ; ){
    int status = deflate(flush);
    if ( status == Z_STREAM_END ) {
      // 完了
      write();
      break;
    }
    if ( avail_out() == 0 ) {
      // 出力バッファが尽きた
      write();
      set_outbuf(mBuff, mSize);
    }
    if ( avail_in() == 0 && flush != Z_FINISH ) {
      // 入力バッファが尽きた
      break;
    }
  }
}

// @brief データの伸長を行う．
int
zstream_buff::decompress(
  Bytef* buff,
  SizeType size
)
{
  set_outbuf(buff, size);
  int orig_size = avail_out();
  for( ; ; ){
    if ( avail_in() == 0 ) {
      // 入力バッファが尽きた
      mInStream->read(reinterpret_cast<char*>(mBuff), mSize);
      set_inbuf(mBuff, mInStream->gcount());
      if ( avail_in() == 0 ) {
	mInFlush = Z_FINISH;
      }
    }
    int status = inflate(mInFlush);
    if ( status == Z_STREAM_END ) {
      // 完了
      return orig_size - avail_out();
    }
    if ( avail_out() == 0 ) {
      // 出力バッファが尽きた
      return orig_size;
    }
  }
}

END_NAMESPACE_YM
