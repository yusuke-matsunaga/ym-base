#ifndef YM_RANDPERMGEN_H
#define YM_RANDPERMGEN_H

/// @file ym/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <random>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RandPermGen RandGen.h "RandGen.h"
/// @brief ランダムな順列を作るクラス
//////////////////////////////////////////////////////////////////////
class RandPermGen
{
public:

  /// @brief コンストラクタ
  RandPermGen(
    SizeType n ///< [in] 要素数
  ) : mArray(n)
  {
  }

  /// @brief デストラクタ
  ~RandPermGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  SizeType
  num() const { return mArray.size(); }

  /// @brief ランダムな順列を生成する．
  template<class URNG>
  void
  generate(
    URNG& randgen ///< [in] 乱数発生器
  )
  {
    std::vector<int> src_array(num());
    for ( int i = 0; i < num(); ++ i ) {
      src_array[i] = i;
    }
    int n = num() - 1;
    for ( int i = 0; i < num(); ++ i ) {
      std::uniform_int_distribution<int> rd(0, n);
      int r = rd(randgen);
      mArray[i] = src_array[r];
      src_array[r] = src_array[n];
      -- n;
    }
  }

  /// @brief 順列の要素を取り出す．
  int
  elem(
    SizeType pos ///< [in] 要素の位置番号 ( 0 <= pos < num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < num() );

    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 現在の順列
  std::vector<int> mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDPERMGEN_H
