#ifndef BASIC_CODEC_STREAMBUF_H
#define BASIC_CODEC_STREAMBUF_H

/// @file basic_codec_streambuf.h
/// @brief basic_codec_streambuf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2021, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/CodecEngine.h"
#include "ym/CodecGen.h"
#include <streambuf>
#include <iostream>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class basic_codec_streambuf basic_codec_streambuf.h "ym/basic_codec_streambuf.h"
/// @brief CODEC を用いた streambuf の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_streambuf :
  public std::basic_streambuf<Ch, Tr>
{
public:

  using superclass = std::basic_streambuf<Ch, Tr>;
  using char_type = typename Tr::char_type;
  using int_type = typename Tr::int_type;

public:

  /// @brief コンストラクタ
  basic_codec_streambuf(
    CodecEngine* engine ///< [in] 圧縮/伸張エンジン
  ) : mEngine{engine},
      mBuffSize{4096},
      mBuff{new ymuint8[mBuffSize]}
  {
    if ( mEngine->read_mode() ) {
      this->setg(reinterpret_cast<char_type*>(mBuff),
		 reinterpret_cast<char_type*>(mBuff + mBuffSize),
		 reinterpret_cast<char_type*>(mBuff + mBuffSize));
    }
    else {
      this->setp(reinterpret_cast<char_type*>(mBuff),
		 reinterpret_cast<char_type*>(mBuff + mBuffSize));
    }
  }

  /// @brief デストラクタ
  ~basic_codec_streambuf()
  {
    delete mEngine;
  }


protected:

  std::streampos
  seekoff(
    std::streamoff off,
    std::ios::seek_dir dir,
    int nmode = std::ios::in | std::ios::out
  )
  {
    // 実際の処理は何もしない．
    return Tr::eof();
  }

  std::streampos
  seekpos(
    std::streampos pos,
    int nmode = std::ios::in | std::ios::out
  )
  {
    // 実際の処理は何もしない．
    return Tr::eof();
  }

  /// @brief 書き込みバッファが溢れた時の処理
  int_type
  overflow(
    int_type c = Tr::eof() ///< [in] 次に書き込むデータ
  )
  {
    mEngine->write(mBuff, mBuffSize);
    this->setp(reinterpret_cast<char_type*>(mBuff),
	       reinterpret_cast<char_type*>(mBuff + mBuffSize));
    if ( c == Tr::eof() ) {
      return Tr::eof();
    }

    // バッファに書き込む．
    *this->pptr() = Tr::to_char_type(c);
    this->pbump(1);
    return Tr::not_eof(c);
  }

  /// @brief 読み出しバッファが空になった時の処理
  int_type
  underflow()
  {
    if ( this->egptr() <= this->gptr() ) {
      auto size = mEngine->read(mBuff, mBuffSize);
      this->setg(reinterpret_cast<char_type*>(mBuff),
		 reinterpret_cast<char_type*>(mBuff),
		 reinterpret_cast<char_type*>(mBuff + size));
      if ( size == 0 ) {
	return Tr::eof();
      }
    }

    // バッファから読み出す．
    return Tr::to_int_type(*this->gptr());
  }

  // sync
  int
  sync()
  {
    if ( mEngine->write_mode() ) {
      SizeType size = reinterpret_cast<ymuint8*>(this->pptr()) - mBuff;
      mEngine->write(mBuff, size);
      this->setp(reinterpret_cast<char_type*>(mBuff),
		 reinterpret_cast<char_type*>(mBuff + mBuffSize));
    }

    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 圧縮器
  CodecEngine* mEngine{nullptr};

  // バッファサイズ
  SizeType mBuffSize;

  // バッファ
  ymuint8* mBuff;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_codec_ostream basic_codec_ostream.h "ym/basic_codec_ostream.h"
/// @brief CODEC を用いた ostream の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_ostream :
  public std::basic_ostream<Ch, Tr>
{
public:

  /// @brief コンストラクタ
  basic_codec_ostream(
    ostream& os,        ///< [in] 真の出力ストリーム
    const CodecGen& gen ///< [in] エンジン生成器
  ) : std::basic_ostream<Ch, Tr>{new basic_codec_streambuf<Ch, Tr>{gen.new_engine(os)}}
  {
  }

  /// @brief デストラクタ
  ~basic_codec_ostream()
  {
    this->flush();
    delete this->rdbuf();
  }

};


//////////////////////////////////////////////////////////////////////
/// @class basic_codec_istream basic_codec_istream.h "ym/basic_codec_istream.h"
/// @brief CODEC を用いた istream の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_istream :
  public std::basic_istream<Ch, Tr>
{
public:

  /// @brief コンストラクタ
  basic_codec_istream(
    istream& is,        ///< [in] 真の入力ストリーム
    const CodecGen& gen ///< [in] エンジン生成器
  ) : std::basic_istream<Ch, Tr>{new basic_codec_streambuf<Ch, Tr>{gen.new_engine(is)}}
  {
  }

  /// @brief デストラクタ
  ~basic_codec_istream()
  {
    delete this->rdbuf();
  }

};

using codec_istream = basic_codec_istream<char>;
using codec_ostream = basic_codec_ostream<char>;

END_NAMESPACE_YM

#endif // BASIC_CODEC_STREAMBUF_H
