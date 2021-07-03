#ifndef YM_ARRAYITERATOR_H
#define YM_ARRAYITERATOR_H

/// @file ym/ArrayIterator.h
/// @brief ArrayIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ArrayIterator ArrayIterator.h "ym/ArrayIterator.h"
/// @brief 汎用のリスト反復子
//////////////////////////////////////////////////////////////////////
template<typename T>
class ArrayIterator
{
public:

  /// @brief コンストラクタ
  ArrayIterator(
    T* obj_ptr ///< [in] 要素へのポインタ
  ) : mObjPtr(obj_ptr)
  {
  }

  /// @brief デストラクタ
  ~ArrayIterator() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  const T&
  operator*() const { return *mObjPtr; }

  /// @brief increment 演算子
  const ArrayIterator&
  operator++()
  {
    ++ mObjPtr;
    return *this;
  }

  /// @brief 等価比較演算子
  bool
  operator==(const ArrayIterator& right) const
  {
    return mObjPtr == right.mObjPtr;
  }

  /// @brief 非等価比較演算子
  bool
  operator!=(const ArrayIterator& right) const
  {
    return !operator==(right);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素へのポインタ
  T* mObjPtr;

};

END_NAMESPACE_YM

#endif // YM_ARRAYITERATOR_H
