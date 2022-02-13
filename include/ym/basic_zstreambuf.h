#ifndef BASIC_ZSTREAMBUF_H
#define BASIC_ZSTREAMBUF_H

/// @file basic_zstreambuf.h
/// @brief basic_zstreambuf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <iosteam>


BEGIN_NAMESPACE_YM

template <class Ch,class Tr=std::char_traits<Ch> >
class basic_zfilterbuf :
  public std::basic_streambuf<Ch,Tr>
{
public:

  using superclass = std::basic_streambuf<Ch,Tr>;


public:

  // 出力コンストラクタ
  basic_zstreambuf(
    std::basic_ostream<Ch,Tr>& os ///< [in] 真の出力ストリーム
  ) : mOut{&os},
      mIn{nullptr},
      mSize{4096}
      mSrcBuffer(mSize),
      mDstBuffer(mSize)
  {
    mBuffer = mSrcBuffer.begin();

    setp(sbuffer.begin(),sbuffer.begin(),sbuffer.end());

    // zlib 初期化
    zs.zalloc   =Z_NULL;
    zs.zfree    =Z_NULL;
    zs.opaque   =Z_NULL;
    if( deflateInit(&zs,Z_DEFAULT_COMPRESSION) != Z_OK ) {
      throw zlib_exception(zs.msg);
    }
    iflush = Z_NO_FLUSH;
    oflush = Z_NO_FLUSH;
  }

  // 入力コンストラクタ
  basic_zstreambuf(
    std::basic_istream<Ch,Tr>& is ///< [in] 真の入力ストリーム
  ) : mOut{nullptr},
      mIn{&is},
      mSize{4096},
      mSrcBuffer(mSize),
      mDstBuffer(mSize)
  {
    mBuffer = mDstBuffer.begin();

    setg(dbuffer.begin(),dbuffer.end(),dbuffer.end());

    // zlib 初期化
    zs.zalloc   =Z_NULL;
    zs.zfree    =Z_NULL;
    zs.opaque   =Z_NULL;
    zs.next_in  =Z_NULL;
    zs.avail_in =0;
    if ( inflateInit(&zs) != Z_OK ) {
      throw zlib_exception(zs.msg);
    }
    iflush = Z_NO_FLUSH;
    oflush = Z_NO_FLUSH;
    }

  // デストラクタ
  ~basic_zstreambuf()
  {
    oflush = Z_FINISH;
    iflush = Z_FINISH;
    sync();

    if ( mOut != nullptr ) {
      if ( deflateEnd(&zs) != Z_OK ) {
	throw zlib_exception(zs.msg);
      }
    }
    if( mIn != nullptr ) {
      if ( inflateEnd(&zs) != Z_OK ) {
	throw zlib_exception(zs.msg);
      }
    }
  }


protected:

  // setbuf
  std::basic_streambuf<Ch,Tr>*
  setbuf(
    Ch* buffer,
    SizeType size
  )
  {
    if ( mOut != nullptr ) {
      mSrcBuffer.resize(0);
      setp(buffer, buffer, buffer + size);
    }
    if ( in != nullptr ) {
      mDstBuffer.resize(0);
      setg(buffer, buffer, buffer + size);
    }
    mBuffer = buffer;
    mSize =size;
    return this;
  }

  // overflow
  int_type
  overflow(
    int_type c =Tr::eof()
  )
  {
    compress();
    if ( c != Tr::eof() ) {
      *pptr() = Tr::to_char_type(c);
      pbump(1);
      return Tr::not_eof(c);
    }
    else {
      return Tr::eof();
    }
  }

  // underflow
  int_type
  underflow()
  {
    if ( egptr() <= gptr() ) {
      if ( iflush == Z_FINISH ) {
	return Tr::eof();
      }
      decompress();
      if ( egptr() <= gptr() ) {
	return Tr::eof();
      }
    }
    return Tr::to_int_type(*gptr());
  }

  // sync
  int
  sync()
  {
    if ( mIn != nullptr ) {
      ;
    }
    if ( mOut != nullptr ) {
      compress();
    }

    return 0;
  }


protected:

  // compress
  void
  compress()
  {
    zs.next_in  =(unsigned char*)buffer;
    zs.avail_in =pptr()-buffer;
    zs.next_out =(unsigned char*)dbuffer.begin();
    zs.avail_out=dbuffer.size();

    for(;;){
      int status = deflate(&zs, oflush);
      if ( status == Z_STREAM_END ) {
	// 完了
	out->write(dbuffer.begin(),dbuffer.size()-zs.avail_out);
	return;
      }
      if ( status != Z_OK ) {
	throw zlib_exception(zs.msg);
      }
      if ( zs.avail_in == 0 ) {
	// 入力バッファが尽きた
	out->write(dbuffer.begin(),dbuffer.size()-zs.avail_out);
	setp(buffer,buffer,buffer+size);
	return;
      }
      if ( zs.avail_out == 0 ) {
	// 出力バッファが尽きた
	out->write(dbuffer.begin(),dbuffer.size());
	zs.next_out = static_cast<unsigned char*>(dbuffer.begin());
	zs.avail_out = dbuffer.size();
      }
    }
  }

  // decompress
  void
  decompress()
  {
    zs.next_out = static_cast<unsigned char*>(buffer);
    zs.avail_out = size;

    for(;;){
      if ( zs.avail_in == 0 ) {
	// 入力バッファが尽きた
	in->read(sbuffer.begin(),sbuffer.size());
	zs.next_in = static_cast<unsigned char*>(sbuffer.begin());
	zs.avail_in = in->gcount();
	if ( zs.avail_in == 0 ) {
	  iflush=Z_FINISH;
	}
      }
      if ( zs.avail_out == 0 ) {
	// 出力バッファが尽きた
	setg(buffer,buffer,buffer+size);
	return;
      }
      int status = inflate(&zs, iflush);
      if ( status == Z_STREAM_END ) {
	// 完了
	setg(buffer,buffer,buffer+size-zs.avail_out);
	return;
      }
      if ( status != Z_OK ) {
	throw zlib_exception(zs.msg);
      }
    }
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  std::basic_ostream<Ch,Tr>* mOut;
  std::basic_istream<Ch,Tr>* mIn;

  Ch* mBuffer;
  int mSize;

  std::vector<Ch> mSrcBuffer;
  std::vector<Ch> mDstBuffer;

  z_stream    zs;
  int         iflush;
  int         oflush;

};

END_NAMESPACE_YM

#endif // BASIC_ZSTREAMBUF_H
