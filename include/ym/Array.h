#ifndef ARRAY_H
#define ARRAY_H

/// @file Array.h
/// @brief Array のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Array Array.h "Array.h"
/// @brief シンプルな配列クラス
//////////////////////////////////////////////////////////////////////
template<typename Obj_Type>
class Array
{
public:

  using iterator = Obj_Type*;

public:

  /// @brief コンストラクタ
  /// @param[in] body 配列本体
  /// @param[in] begin 開始位置
  /// @param[in] end 終了位置
  ///
  /// body[begin] 〜 body[end - 1] までの範囲を表す配列となる．
  Array(Obj_Type* body,
	SizeType begin,
	SizeType end);

  /// @brief デストラクタ
  ~Array();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const;

  /// @brief 要素を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < num() )
  Obj_Type
  operator[](SizeType pos) const;

  /// @breif 先頭の反復子を得る．
  iterator
  begin() const;

  /// @brief 末尾の反復子を得る．
  iterator
  end() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum;

  // 本体
  Obj_Type* mBody;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] body 配列本体
// @param[in] begin 開始位置
// @param[in] end 終了位置
//
// body[begin] 〜 body[end - 1] までの範囲を表す配列となる．
template<typename Obj_Type>
inline
Array<Obj_Type>::Array(Obj_Type* body,
		       SizeType begin,
		       SizeType end)
{
  mNum = end - begin;
  mBody = body + begin;
}

// @brief デストラクタ
template<typename Obj_Type>
inline
Array<Obj_Type>::~Array()
{
}

// @brief 要素数を返す．
template<typename Obj_Type>
inline
SizeType
Array<Obj_Type>::num() const
{
  return mNum;
}

// @brief 要素を返す．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
template<typename Obj_Type>
inline
Obj_Type
Array<Obj_Type>::operator[](SizeType pos) const
{
  ASSERT_COND( pos < num() );

  return mBody[pos];
}

// @breif 先頭の反復子を得る．
template<typename Obj_Type>
inline
Obj_Type*
Array<Obj_Type>::begin() const
{
  return mBody;
}

// @brief 末尾の反復子を得る．
template<typename Obj_Type>
inline
Obj_Type*
Array<Obj_Type>::end() const
{
  return mBody + num();
}

END_NAMESPACE_YM

#endif // ARRAY_H
