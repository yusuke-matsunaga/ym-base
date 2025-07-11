#ifndef YM_SHSTRING_H
#define YM_SHSTRING_H

/// @file ym/ShString.h
/// @brief ShString のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/BinDec.h"
#include "ym/BinEnc.h"


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
  ///
  /// nullptr がセットされる．
  ShString() = default;

  /// @brief C文字列を指定したコンストラクタ
  explicit
  ShString(
    const char* str ///< [in] C文字列
  )
  {
    set(str);
  }

  /// @brief string を指定したコンストラクタ
  explicit
  ShString(
    const std::string& str ///< [in] 文字列 (string)
  )
  {
    set(str.c_str());
  }

  /// @brief コピーコンストラクタ
  ShString(
    const ShString& src ///< [in] コピー元のオブジェクト
  ) : mPtr(src.mPtr)
  {
  }

  /// @brief 代入演算子
  /// @return 自分自身を返す．
  const ShString&
  operator=(
    const ShString& src ///< [in] コピー元のオブジェクト
  )
  {
    mPtr = src.mPtr;
    return *this;
  }

  /// @brief C文字列からのからの代入演算子
  /// @return 自分自身を返す．
  const ShString&
  operator=(
    const char* src ///< [in] コピー元のC文字列
  )
  {
    set(src);
    return *this;
  }

  /// @brief string からのからの代入演算子
  /// @return 自分自身を返す．
  const ShString&
  operator=(
    const std::string& src ///< [in] コピー元の string
  )
  {
    set(src.c_str());
    return *this;
  }

  /// @brief デストラクタ
  ~ShString() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief const char* に変換する．
  operator const char*() const { return mPtr; }

  /// @brief string に変換する．
  operator std::string() const
  {
    if ( mPtr ) {
      return std::string(mPtr);
    }
    return std::string{};
  }

  /// @brief id を返す．
  /// @note 実際には文字列へのポインタをキャストしたもの
  PtrIntType
  id() const { return reinterpret_cast<PtrIntType>(mPtr); }

  /// @brief ハッシュ用のキーを返す．
  SizeType
  hash() const
  {
    auto tmp = reinterpret_cast<PtrIntType>(mPtr)/sizeof(void*);
    return static_cast<SizeType>(tmp);
  }

  /// @brief ShString 関連でアロケートされたメモリサイズ
  static
  SizeType
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
  void
  set(
    const char* str ///< [in] 入力の文字列
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文字列へのポインタ
  // ただし StrPool 上で共有化されている．
  const char* mPtr{nullptr};

};

/// @name 共有文字列関係の関数
/// @{

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @return a と b が同じ文字列の時 true を返す．
inline
bool
operator==(
  const ShString& a, ///< [in] 左のオペランド
  const ShString& b  ///< [in] 右のオペランド
)
{
  return a.id() == b.id();
}

/// @relates ShString
/// @ingroup ym
/// @brief 非等価比較演算子
/// @return a と b が異なる文字列の時 true を返す．
inline
bool
operator!=(
  const ShString& a, ///< [in] 左のオペランド
  const ShString& b  ///< [in] 右のオペランド
)
{
  return !(a == b);
}

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @return a と b が同じ文字列を表しているとき true を返す．
///
/// b を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator==(
  const ShString& a, ///< [in] 左のオペランド
  const char* b      ///< [in] 右のオペランド
)
{
  if ( b == nullptr ) {
    // 境界条件のチェック
    return false;
  }
  return strcmp(a.operator const char*(), b) == 0;
}

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @return a と b が同じ文字列を表しているとき true を返す．
///
/// a を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator==(
  const char* a,    ///< [in] 左のオペランド
  const ShString& b ///< [in] 右のオペランド
)
{
  return operator==(b, a);
}

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @return a と b が同じ文字列を表していないとき true を返す．
///
/// b を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator!=(
  const ShString& a, ///< [in] 左のオペランド
  const char* b      ///< [in] 右のオペランド
)
{
  return !operator==(a, b);
}

/// @relates ShString
/// @ingroup ym
/// @brief 等価比較演算子
/// @return a と b が同じ文字列を表していないとき true を返す．
///
/// a を ShString() に変換するよりも strcmp() を使ったほうがよい．
inline
bool
operator!=(
  const char* a,     ///< [in] 左のオペランド
  const ShString& b) ///< [in] 右のオペランド
{
  return !operator==(a, b);
}

/// @relates ShString
/// @ingroup ym
/// @brief ShString 用のストリーム出力演算子
/// @return s をそのまま返す．
std::ostream&
operator<<(
  std::ostream& s,    ///< [in] 出力ストリーム
  const ShString& str ///< [in] 出力対象の文字列
);

/// @relates ShString
/// @ingroup ym
/// @brief ShString 用のバイナリ出力演算子
/// @return s をそのまま返す．
inline
BinEnc&
operator<<(
  BinEnc& s,          ///< [in] 出力ストリーム
  const ShString& str ///< [in] 出力対象の文字列
)
{
  s << static_cast<std::string>(str);
  return s;
}

/// @relates ShString
/// @ingroup ym
/// @brief ShString 用のバイナリ入力演算子
/// @return s をそのまま返す．
inline
BinDec&
operator>>(
  BinDec& s,    ///< [in] 入力ストリーム
  ShString& str ///< [out] 対象の文字列
)
{
  std::string tmp;
  s >> tmp;
  str = ShString{tmp};
  return s;
}

/// @}

END_NAMESPACE_YM

BEGIN_NAMESPACE_STD

// ShString をキーにしたハッシュ関数クラスの定義
template <>
struct hash<YM_NAMESPACE::ShString>
{
  SizeType
  operator()(const YM_NAMESPACE::ShString& str) const
  {
    return str.hash();
  }
};

END_NAMESPACE_STD

#endif // YM_SHSTRING_H
