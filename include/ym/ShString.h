﻿#ifndef YM_SHSTRING_H
#define YM_SHSTRING_H

/// @file ym/ShString.h
/// @brief ShString のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ShString ShString.h "ym/ShString.h"
/// @ingroup ShStringGroup
/// @brief StrPool で共有された文字列へのオートポインタ
/// @sa StrPool
//////////////////////////////////////////////////////////////////////
class ShString
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  /// @note nullptr がセットされる．
  ShString();

  /// @brief C文字列を指定したコンストラクタ
  /// @param[in] str C文字列
  explicit
  ShString(const char* str);

  /// @brief string を指定したコンストラクタ
  /// @param[in] str 文字列 (string)
  explicit
  ShString(const string& str);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  ShString(const ShString& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const ShString& src);

  /// @brief C文字列からのからの代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const char* src);

  /// @brief string からのからの代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身を返す．
  const ShString&
  operator=(const string& src);

  /// @brief デストラクタ
  ~ShString();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief const char* に変換する．
  operator const char*() const;

  /// @brief string に変換する．
  operator string() const;

  /// @brief id を返す．
  /// @note 実際には文字列へのポインタをキャストしたもの
  ympuint
  id() const;

  /// @brief ハッシュ用のキーを返す．
  HashType
  hash() const;

  /// @brief ShString 関連でアロケートされたメモリサイズ
  static
  int
  allocated_size();

  /// @brief ShString 関連でアロケートされたメモリをすべて開放する．
  /// @note 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
  static
  void
  free_all_memory();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 共有文字列を作ってセットする．
  /// @param[in] str 入力の文字列
  void
  set(const char* str);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列へのポインタ
  // ただし StrPool 上で共有化されている．
  const char* mPtr;

};

/// @name 共有文字列関係の関数
/// @{

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列の時 true を返す．
bool
operator==(const ShString& a,
	   const ShString& b);

/// @relates ShString
/// @ingroup ym
/// @brief 非等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が異なる文字列の時 true を返す．
bool
operator!=(const ShString& a,
	   const ShString& b);

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列を表しているとき true を返す．
/// @note b を ShString() に変換するよりも strcmp() を使ったほうがよい．
bool
operator==(const ShString& a,
	   const char* b);

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列を表しているとき true を返す．
/// @note a を ShString() に変換するよりも strcmp() を使ったほうがよい．
bool
operator==(const char* a,
	   const ShString& b);

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列を表しているとき true を返す．
/// @note b を ShString() に変換するよりも strcmp() を使ったほうがよい．
bool
operator!=(const ShString& a,
	   const char* b);

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @param[in] a, b 比較対象の文字列
/// @return a と b が同じ文字列を表しているとき true を返す．
/// @note a を ShString() に変換するよりも strcmp() を使ったほうがよい．
bool
operator!=(const char* a,
	   const ShString& b);

/// @relates ShString
/// @ingroup ym
/// @brief ShString 用のストリーム出力演算子
/// @param[in] s 出力ストリーム
/// @param[in] str 出力対象の文字列
/// @return s をそのまま返す．
ostream&
operator<<(ostream& s,
	   const ShString& str);

/// @}


// ShString をキーにしたハッシュ関数クラスの定義
template <>
struct HashFunc<ShString>
{
  HashType
  operator()(const ShString& str) const
  {
    return str.hash();
  }
};


//////////////////////////////////////////////////////////////////////
// ShString のインライン関数
//////////////////////////////////////////////////////////////////////

// 空のコンストラクタ
inline
ShString::ShString() :
  mPtr(nullptr)
{
}

// コピーコンストラクタ
inline
ShString::ShString(const ShString& src) :
  mPtr(src.mPtr)
{
}

// C文字列を指定したコンストラクタ
inline
ShString::ShString(const char* str)
{
  set(str);
}

// string を指定したコンストラクタ
inline
ShString::ShString(const string& str)
{
  set(str.c_str());
}

// 代入演算子
inline
const ShString&
ShString::operator=(const ShString& src)
{
  mPtr = src.mPtr;
  return *this;
}

// C文字列からの代入演算子
inline
const ShString&
ShString::operator=(const char* src)
{
  set(src);
  return *this;
}

// string からの代入演算子
inline
const ShString&
ShString::operator=(const string& src)
{
  set(src.c_str());
  return *this;
}

// デストラクタ
inline
ShString::~ShString()
{
}

// @brief const char* に変換する．
inline
ShString::operator const char*() const
{
  return mPtr;
}

// string に変換する．
inline
ShString::operator string() const
{
  if ( mPtr ) {
    return string(mPtr);
  }
  else {
    return string();
  }
}

// ID の取得
inline
ympuint
ShString::id() const
{
  return reinterpret_cast<ympuint>(mPtr);
}

// @brief ハッシュ用のキーを返す．
inline
HashType
ShString::hash() const
{
  ympuint tmp = reinterpret_cast<ympuint>(mPtr)/sizeof(void*);
  return static_cast<HashType>(tmp);
}

// 等価比較演算子
inline
bool
operator==(const ShString& a,
	   const ShString& b)
{
  return a.id() == b.id();
}

// 非等価比較演算子
inline
bool
operator!=(const ShString& a,
	   const ShString& b)
{
  return !(a == b);
}

// @brief 等価比較演算子
// @param[in] a, b 比較対象の文字列
// @return a と b が同じ文字列を表しているとき true を返す．
// @note b を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator==(const ShString& a,
	   const char* b)
{
  if ( b == nullptr ) {
    // 境界条件のチェック
    return false;
  }
  return strcmp(a.operator const char*(), b) == 0;
}

// @brief 等価比較演算子
// @param[in] a, b 比較対象の文字列
// @return a と b が同じ文字列を表しているとき true を返す．
// @note a を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator==(const char* a,
	   const ShString& b)
{
  return operator==(b, a);
}

// @brief 等価比較演算子
// @param[in] a, b 比較対象の文字列
// @return a と b が同じ文字列を表しているとき true を返す．
// @note b を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator!=(const ShString& a,
	   const char* b)
{
  return !operator==(a, b);
}

// @brief 等価比較演算子
// @param[in] a, b 比較対象の文字列
// @return a と b が同じ文字列を表しているとき true を返す．
// @note a を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator!=(const char* a,
	   const ShString& b)
{
  return !operator==(b, a);
}

END_NAMESPACE_YM

#endif // YM_SHSTRING_H
