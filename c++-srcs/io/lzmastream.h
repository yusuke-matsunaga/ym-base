#ifndef LZMASTREAM_H
#define	LZMASTREAM_H

/// @file lzmastream.h
/// @brief lzma を使った圧縮/伸長機能つきストリーム
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "LzmaEngine.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class lzmastream_buff lzmastream.h "lzmastream.h"
/// @brief LZMA 圧縮付きのバッファ
//////////////////////////////////////////////////////////////////////
class lzmastream_buff
{
public:

  /// @brief 入力用のコンストラクタ
  explicit
  lzmastream_buff(
    istream* s,              ///< [in] 入力ストリーム
    SizeType size = BUFFSIZE ///< [in] バッファサイズ
  );

  /// @brief 出力用のコンストラクタ
  explicit
  lzmastream_buff(
    ostream* s,              ///< [in] 出力ストリーム
    SizeType size = BUFFSIZE ///< [in] バッファサイズ
  );

  /// @brief デストラクタ
  ~lzmastream_buff();


public:

  /// @brief データの圧縮を行う．
  ///
  /// 結果は出力ストリームに書き込まれる．
  void
  compress(
    Bytef* buff,   ///< [in] 圧縮するデータを格納しているバッファ
    SizeType size, ///< [in] バッファ中の有効なデータのサイズ
  );

  /// @brief データの伸長を行う．
  /// @return バッファに書き出されたサイズ(in bytes)を返す．
  ///
  /// データは入力ストリームから読み込まれる．
  int
  decompress(
    Bytef* buff,  ///< [in] 伸長したデータを格納するバッファ
    SizeType size ///< [in] バッファ中の空きサイズ(in bytes)
  );

  /// @brief 入力ストリームを取り出す．
  istream*
  in()
  {
    return mInStream;
  }

  /// @brief 出力ストリームを取り出す．
  ostream*
  out()
  {
    return mOutStream;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で呼ばれる下請け関数
  //////////////////////////////////////////////////////////////////////

  // バッファの内容を書き出す．
  void
  write()
  {
    mOutStream->write(reinterpret_cast<char*>(mBuff), mSize - avail_out());
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 定数
  //////////////////////////////////////////////////////////////////////

  // バッファサイズ
  static
  const int BUFFSIZE = 4096;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 圧縮データを入力するストリーム
  istream* mInStream;

  // 圧縮データを出力するストリーム
  ostream* mOutStream;

  // 内側の(圧縮されている)バッファ
  Bytef* mBuff;

  // mBuff のサイズ
  SizeType mSize;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_lzma_streambuf lzmastream.h "lzmastream.h"
/// @brief lzma を使ったストリームバッファのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_lzma_streambuf :
  public std::basic_streambuf<_CharT, _Traits>
{
public:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;

public:

  /// @brief 入力用のコンストラクタ
  explicit
  basic_lzma_streambuf(
    istream& s,         ///< [in] 入力ストリーム
    SizeType size       ///< [in] バッファサイズ
    = lzmastream_buff::BUFFSIZE
  ) : mLzmaBuff(&s, size),
      mBuff{nullptr},
      mSize{size},
      mBuff0{new char_type[size]}
  {
    mBuff = mBuff0;
    int wsize = mLzmaBuff.decompress(byte_buff(), byte_size(mSize));
    this->setg(mBuff, mBuff, mBuff + char_size(wsize));
  }

  /// @brief 出力用のコンストラクタ
  explicit
  basic_lzma_streambuf(
    ostream& s,    ///< [in] 出力ストリーム
    SizeType size  ///< [in] バッファサイズ
    = lzmastream_buf::BUFFSIZE
  ) : mLzmaBuff(&s, size),
      mBuff{nullptr},
      mSize{size},
      mBuff0{new char_type[size]}
  {
    mBuff = mBuff0;
    this->setp(mBuff, mBuff + mSize);
  }

  /// @brief デストラクタ
  ~basic_lzma_streambuf()
  {
    sync();

    mLzmaBuff.end();

    delete [] mBuff0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // basic_streambuf<_CharT, _Traits> の仮想関数
  //////////////////////////////////////////////////////////////////////

  streambuf_type*
  setbuf(
    char_type* buf,
    sizeType size
  ) override
  {
    if ( mLzmaBuff.in() ) {
      this->setg(buf, buf, buf + size);
    }
    if ( mLzmaBuff.out() ) {
      this->setp(buf, buf + size);
    }
    mBuff = buf;
    mSize = size;
    return this;
  }

  int_type
  overflow(
    int_type c = traits_type::eof()
  ) override
  {
    // バッファに溜っているデータを圧縮する．
    mLzmaBuff.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
    this->setp(mBuff, mBuff + mSize);

    if ( c == traits_type::eof() ) {
      return c;
    }

    // 一文字書き込む
    *(this->pptr()) = traits_type::to_char_type(c);
    this->pbump(1);

    return traits_type::not_eof(c);
  }

  int_type
  underflow() override
  {
    if ( this->egptr() <= this->gptr() ) {
      // データを伸長してバッファに入れる．
      int wsize = mLzmaBuff.decompress(byte_buff(), byte_size(mSize));
      this->setg(mBuff, mBuff, mBuff + char_size(wsize));
      if ( wsize == 0 ) {
	return traits_type::eof();
      }
    }

    // 一文字読み出す．
    return traits_type::to_int_type(*(this->gptr()));
  }

  int
  sync() override
  {
    if ( mLzmaBuff.out() ) {
      mLzmaBuff.compress(byte_buff(), byte_size(this->pptr() - mBuff), mOutFlush);
      this->setp(mBuff, mBuff + mSize);
    }
    return 0;
  }


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  // mBuff を lzmastream 用にキャストする．
  Bytef*
  byte_buff()
  {
    return reinterpret_cast<Bytef*>(mBuff);
  }

  // char_type 単位のサイズを Bytef 単位のサイズに変換する
  static
  SizeType
  byte_size(
    SizeType size
  )
  {
    return size * sizeof(char_type) / sizeof(Bytef);
  }

  // Bytef 単位のサイズを char_type 単位のサイズに変換する
  static
  SizeType
  char_size(
    SizeType size
  )
  {
    return size * sizeof(Bytef) / sizeof(char_type);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // lzma 圧縮を行なうバッファ
  lzmastream_buff mLzmaBuff;

  // 外側の(plainな)バッファ
  char_type* mBuff;

  // mBuff のサイズ
  SizeType mSize;

  // デフォルトのバッファ
  char_type* mBuff0;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_ilzmastream lzmastream.h "lzmastream.h"
/// @brief lzma を使った入力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_ilzmastream :
  public std::basic_istream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;
  using istream_type = std::basic_istream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  explicit
  basic_ilzmastream(
    istream& s ///< [in] 伸長元の入力ストリーム
  ) : istream_type(new basic_lzma_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_ilzmastream()
  {
    delete this->rdbuf();
  }
};


//////////////////////////////////////////////////////////////////////
/// @class basic_olzmastream lzmastream.h "lzmastream.h"
/// @brief lzma を使った出力ストリームのテンプレート
//////////////////////////////////////////////////////////////////////
template <typename _CharT,
	  typename _Traits = std::char_traits<_CharT> >
class basic_olzmastream :
  public std::basic_ostream<_CharT, _Traits>
{
public:
  using char_type = _CharT;
  using traits_type = _Traits;
  using ostream_type = std::basic_ostream<char_type, traits_type>;

public:

  /// @brief コンストラクタ
  explicit
  basic_olzmastream(
    ostream& s ///< [in] 圧縮前の出力ストリーム
  ) : ostream_type(new basic_lzma_streambuf<char_type, traits_type>(s))
  {
  }

  /// @brief デストラクタ
  ~basic_olzmastream()
  {
    this->flush();
    delete this->rdbuf();
  }
};

using ilzmastream = basic_ilzmastream<char>;
using olzmastream = basic_olzmastream<char>;

END_NAMESPACE_YM

#endif // YM_UTILS_LZMASTREAM_H
