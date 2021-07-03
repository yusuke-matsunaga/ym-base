#ifndef ARRAY_H
#define ARRAY_H

/// @file Array.h
/// @brief Array のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018, 2020, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Array Array.h "Array.h"
/// @brief シンプルな配列クラス
///
/// このクラスではメモリアロケーションは行わない．
/// ただ単に古典的な C配列に対して反復子を提供するのがこのクラスの目的
///
/// @code
/// SizeType n{100};
/// int* chunk{new int[n]};
/// Array<int> int_array{chunk, 0, n};
/// for ( int i: int_array ) {
///   ...
/// }
/// @endcode
/// のように用いることができる．
//////////////////////////////////////////////////////////////////////
template<typename ObjType>
class Array
{
public:

  using iterator = ObjType*;

public:

  /// @brief コンストラクタ
  ///
  /// body[begin] 〜 body[end - 1] までの範囲を表す配列となる．
  Array(
    ObjType* body,  ///< [in] 配列本体
    SizeType begin, ///< [in] 開始位置
    SizeType end    ///< [in] 終了位置
  ) : mNum{end - begin},
      mBody{body + begin}
  {
  }

  /// @brief デストラクタ
  ~Array() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const { return mNum; }

  /// @brief 要素を返す．
  ObjType&
  operator[](
    SizeType pos ///< [in] 位置番号 ( 0 <= pos < num() )
  ) const
  {
    ASSERT_COND( pos < num() );

    return mBody[pos];
  }

  /// @breif 先頭の反復子を得る．
  iterator
  begin() const { return mBody; }

  /// @brief 末尾の反復子を得る．
  iterator
  end() const { return mBody + num(); }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  SizeType mNum;

  // 本体
  ObjType* mBody;

};

END_NAMESPACE_YM

#endif // ARRAY_H
