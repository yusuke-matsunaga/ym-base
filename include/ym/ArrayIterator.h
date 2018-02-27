#ifndef YM_ARRAYITERATOR_H
#define YM_ARRAYITERATOR_H

/// @file ym/ArrayIterator.h
/// @brief ArrayIterator のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
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
  /// @param[in] obj_ptr 要素へのポインタ
  ArrayIterator(T* obj_ptr) :
    mObjPtr(obj_ptr)
  {
  }

  /// @brief デストラクタ
  ~ArrayIterator()
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief dereference 演算子
  const T&
  operator*() const
  {
    return *mObjPtr;
  }

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
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素へのポインタ
  T* mObjPtr;

};

END_NAMESPACE_YM

#endif // YM_ARRAYITERATOR_H
