#ifndef YM_STRBUFF_H
#define YM_STRBUFF_H

/// @file ym/StrBuff.h
/// @brief StrBuff のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrBuff StrBuff.h "ym/StrBuff.h"
/// @ingroup ym
/// @brief 文字列バッファ
///
/// 基本的にはただの文字列バッファなので string でも代用できるが
/// たぶんこちらのほうが効率がよい．
//////////////////////////////////////////////////////////////////////
class StrBuff
{
public:

  /// @brief サイズを表す型の定義
  ///
  /// std::basic_string のまね
  using SizeType = size_t;

  /// @brief 末尾を表す定数
  ///
  /// std::basic_string のまね
  static const SizeType npos = static_cast<SizeType>(-1);


public:

  /// @brief デフォルトのコンストラクタ
  StrBuff()
    : mSize{1},
      mEnd{0},
      mBuffer{new char[mSize]}
  {
    mBuffer[mEnd] = '\0';
  }

  /// @brief C文字列からの変換用コンストラクタ
  StrBuff(const char* str) ///< [in] 文字列
    : mSize{strlen(str) + 1},
      mEnd{mSize - 1},
      mBuffer{new char[mSize]}
  {
    copy(str, mBuffer);
  }

  /// @brief コピーコンストラクタ (StrBuff)
  StrBuff(const StrBuff& src) ///< [in] コピー元のオブジェクト (StrBuff)
    : mSize(src.mSize),
      mEnd(src.mEnd),
      mBuffer(new char[mSize])
  {
    copy(src.mBuffer, mBuffer);
  }

  /// @brief コピーコンストラクタ (string)
  StrBuff(const string& src) ///< [in] コピー元のオブジェクト (string)
    : mSize(src.size() + 1),
      mEnd(mSize - 1),
      mBuffer(new char[mSize])
  {
    copy(src.c_str(), mBuffer);
  }

  /// @brief 代入演算子 (StrBuff)
  /// @return 自分自身
  const StrBuff&
  operator=(const StrBuff& src) ///< [in] コピー元の文字列 (StrBuff)
  {
    clear();
    put_str(src);
    return *this;
  }

  /// @brief 代入演算子 (C文字列)
  /// @return 自分自身
  const StrBuff&
  operator=(const char* src) ///< [in] コピー元の文字列 (C文字列)
  {
    clear();
    put_str(src);
    return *this;
  }

  /// @brief 代入演算子 (string)
  /// @return 自分自身
  const StrBuff&
  operator=(const string& src) ///< [in] コピー元の文字列 (string)
  {
    clear();
    put_str(src);
    return *this;
  }

  /// @brief デストラクタ
  ~StrBuff()
  {
    delete [] mBuffer;
  }


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 制御用の関数
  /// @{

  /// @brief クリアする．
  void
  clear()
  {
    mBuffer[0] = '\0';
    mEnd = 0;
  }

  /// @brief 必要なサイズを指定する．
  void
  reserve(SizeType size) ///< [in] サイズ
  {
    if ( mSize < size ) {
      expand(size);
    }
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容を追加する関数
  /// @{

  /// @brief 一文字追加
  void
  put_char(int c) ///< [in] 追加する文字
  {
    if ( mEnd >= mSize - 1 ) {
      // バッファの大きさを2倍にする．
      expand(mSize << 1);
    }
    mBuffer[mEnd] = c;
    ++ mEnd;
    mBuffer[mEnd] = '\0';
  }

  /// @brief 文字列の追加 (StrBuff)
  void
  put_str(const StrBuff& str) ///< [in] 追加する文字列 (StrBuff)
  {
    put_str(str.c_str());
  }

  /// @brief 文字列の追加 (C文字列)
  void
  put_str(const char* str); ///< [in] 追加する文字列 (C文字列)

  /// @brief 文字列の追加 (string)
  void
  put_str(const string& str) ///< [in] 追加する文字列 (string)
  {
    put_str(str.c_str());
  }

  /// @brief 整数を文字列に変換して追加
  void
  put_digit(int d); ///< [in] 数値

  /// @}
  //////////////////////////////////////////////////////////////////////


public:
  //////////////////////////////////////////////////////////////////////
  /// @name 内容の取得
  /// @{

  /// @brief 文字列の長さの取得
  /// @return 文字列の長さ(末尾の '\\0' を含まない)を返す．
  SizeType
  size() const { return mEnd; }

  /// @brief pos 番目の文字の取得
  /// @return pos 番目の文字を返す．
  ///
  /// 範囲外の場合は '\\0' を返す．
  char
  operator[](SizeType pos) const ///< [in] 取得する文字の位置
  {
    if ( pos < mEnd ) {
      return mBuffer[pos];
    }
    else {
      // 範囲外
      return '\0';
    }
  }

  /// @brief c が最初に現れる位置の検索
  /// @retval c が最初に現れる位置
  /// @retval npos 見つからない場合
  SizeType
  find_first_of(char c) const; ///< [in] 検索対象の文字

  /// @brief 部分文字列の取得
  /// @return first から last までの部分文字列を切り出す．
  StrBuff
  substr(SizeType first,       ///< [in] 部分文字列の開始位置
	 SizeType last) const; ///< [in] 部分文字列の終了位置

  /// @brief Cスタイルの文字列への変換
  /// @return Cスタイルに変換した文字列を返す．
  const char*
  c_str() const
  {
    return mBuffer;
  }

  /// @brief string への変換
  operator string() const
  {
    return string(c_str());
  }

  /// @}
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部で用いられるコンストラクタ
  StrBuff(SizeType size) ///< [in] サイズ
    : mSize(size),
      mEnd(0),
      mBuffer(new char[mSize])
  {
    mBuffer[0] = '\0';
  }

  /// @brief src から dst にコピーする．
  void
  copy(const char* src, ///< [in] ソース文字列
       char* dst)       ///< [in] ディスティネーション文字列
  { while ( (*dst ++ = *src ++) ) ; }

  /// @brief バッファサイズを拡張する．
  void
  expand(SizeType new_size); ///< [in] サイズ


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // バッファのサイズ
  SizeType mSize;

  // 末尾の位置
  SizeType mEnd;

  // バッファ本体
  char* mBuffer;

};


/// @relates StrBuff
/// @brief 等価比較
/// @return 2つの文字列が等しいときに true を返す．
inline
bool
operator==(const StrBuff& src1, ///< [in] 第1オペランド
	   const StrBuff& src2) ///< [in] 第2オペランド
{
  return strcmp(src1.c_str(), src2.c_str()) == 0;
}

/// @relates StrBuff
/// @brief 非等価比較
/// @return 2つの文字列が等しくないときに true を返す．
inline
bool
operator!=(const StrBuff& src1, ///< [in] 第1オペランド
	   const StrBuff& src2) ///< [in] 第2オペランド
{
  return !operator==(src1, src2);
}

/// @relates StrBuff
/// @brief StrBuff の内容を出力する
/// @return 出力ストリームを返す．
inline
ostream&
operator<<(ostream& s,            ///< [in] 出力ストリーム
	   const StrBuff& strbuf) ///< [in] 出力対象のオブジェクト
{
  return s << strbuf.c_str();
}

END_NAMESPACE_YM


BEGIN_NAMESPACE_STD

// StrBuff をキーとしたハッシュを使うために必要なクラス定義

template <>
struct hash<YM_NAMESPACE::StrBuff> {
  SizeType
  operator()(const YM_NAMESPACE::StrBuff& __x) const {
    SizeType __h = 0;
    for ( const char* __s = __x.c_str(); *__s; ++ __s )
      __h = 37*__h + *__s;
    return static_cast<SizeType>(__h);
  }
};

END_NAMESPACE_STD

#endif // YM_STRBUFF_H
