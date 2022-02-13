#ifndef BASIC_CODEC_FSTREAM_H
#define BASIC_CODEC_FSTREAM_H

/// @file basic_codec_fstream.h
/// @brief basic_codec_fstream のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#incoude "ym/CodecEngine.h"
#include <iosteam>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class basic_codec_fstream basic_codec_stream.h "ym/basic_codec_fstream.h"
/// @brief CODEC を用いた streambuf の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_fstreambuf :
  public std::basic_streambuf<Ch, Tr>
{
public:

  using superclass = std::basic_streambuf<Ch, Tr>;

public:

  /// @brief 出力用コンストラクタ
  basic_codec_fstreambuf(
    std::basic_ostream<Ch, Tr>& os, ///< [in] 真の出力ストリーム
    CodecType type                  ///< [in] CODEC のタイプ
  ) : mEngine{unique_ptr<CodecEngin>{new CodecEngine{type, nullptr, &os}}}
  {
  }

  /// @brief 入力用コンストラクタ
  basic_codec_streambuf(
    std::basic_istream<Ch, Tr>& is, ///< [in] 真の入力ストリーム
    CodecType type                  ///< [in] CODEC のタイプ
  ) : mEngine{unique_ptr<CodecEngin>{new CodecEngine{type, &is, nullptr}}}
  {
  }

  /// @brief デストラクタ
  ~basic_codec_streambuf() = default;


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
    mEngine->compress();
    if ( c != Tr::eof() ) {
      // バッファに書き込む．
      *pptr() = Tr::to_char_type(c);
      pbump(1);
      return Tr::not_eof(c);
    }
    else {
      return Tr::eof();
    }
  }

  /// @brief 読み出しバッファが空になった時の処理
  int_type
  underflow()
  {
    if ( egptr() <= gptr() ) {
      if ( iflush == Z_FINISH ) {
	return Tr::eof();
      }
      mEngine->decompress();
      if ( egptr() <= gptr() ) {
	return Tr::eof();
      }
    }
    // バッファから読み出す．
    return Tr::to_int_type(*gptr());
  }

  /// @brief バッファをセットする．
  std::basic_streambuf<Ch,Tr>*
  setbuf(
    Ch* buffer,   ///< [in] バッファの先頭アドレス
    SizeType size ///< [in] バッファのサイズ
  )
  {
    if ( mOutStream != nullptr ) {
      mSrcBuffer.resize(0);
      setp(buffer, buffer, buffer + size);
    }
    if ( mInStream != nullptr ) {
      mDstBuffer.resize(0);
      setg(buffer, buffer, buffer + size);
    }
    mBuffer = buffer;
    mSize = size;
    return this;
  }

  // sync
  int
  sync()
  {
    if ( mInStream != nullptr ) {
      // 何もしない．
      ;
    }
    if ( mOutStream != nullptr ) {
      mEngine->compress();
    }

    return 0;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 圧縮データを入力するストリーム
  istream* mInStream{nullptr};

  // 圧縮データを出力するストリーム
  ostream* mOutStream{nullptr};

  // 圧縮器
  unique_ptr<CodecEngine> mEngine{nullptr};

};


//////////////////////////////////////////////////////////////////////
/// @class basic_codec_ofstream basic_codec_fstream.h "ym/basic_codec_fstream.h"
/// @brief CODEC を用いた ostream の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_ofstream :
  public std::basic_osteam<Ch, Tr>
{
public:

  /// @brief コンストラクタ
  basic_codec_ofstream(
    CodecType type ///< [in] CODEC タイプ
  ) : std::basic_ostream<Ch, Tr>{new basic_codec_fstreambuf<Ch, Tr>{type}}
  {
  }

  /// @brief デストラクタ
  ~basic_codec_ofstream() = default;

};


//////////////////////////////////////////////////////////////////////
/// @class basic_codec_ifstream basic_codec_fstream.h "ym/basic_codec_fstream.h"
/// @brief CODEC を用いた istream の拡張クラス
//////////////////////////////////////////////////////////////////////
template <class Ch,
	  class Tr = std::char_traits<Ch>>
class basic_codec_ifstream :
  public std::basic_isteam<Ch, Tr>
{
public:

  /// @brief コンストラクタ
  basic_codec_ifstream(
    CodecType type ///< [in] CODEC タイプ
  ) : std::basic_istream<Ch, Tr>{new basic_codec_fstreambuf<Ch, Tr>{type}}
  {
  }

  /// @brief デストラクタ
  ~basic_codec_ifstream() = default;

};

using codec_ifstream = basic_codec_ifstream<char>;
using codec_ofstream = basic_codec_ofstream<char>;

END_NAMESPACE_YM

#endif // BASIC_CODEC_STREAMBUF_H
