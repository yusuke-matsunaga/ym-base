#ifndef YM_RANDPERMGEN_H
#define YM_RANDPERMGEN_H

/// @file ym/RandPermGen.h
/// @brief RandPermGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/Range.h"
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
    int n ///< [in] 要素数
  ) : mNum{n},
      mArray{new int[n]}
  {
  }

  /// @brief デストラクタ
  ~RandPermGen()
  {
    delete [] mArray;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  int
  num() const { return mNum; }

  /// @brief ランダムな順列を生成する．
  template<class URNG>
  void
  generate(
    URNG& randgen ///< [in] 乱数発生器
  )
  {
    vector<int> src_array(mNum);
    for ( int i: Range(mNum) ) {
      src_array[i] = i;
    }
    int n = mNum - 1;
    for ( int i: Range(mNum) ) {
      std::uniform_int_distribution<int> rd(0, n);
      int r = rd(randgen);
      mArray[i] = src_array[r];
      for ( int j: Range(r, n) ) {
	src_array[j] = src_array[j + 1];
      }
      -- n;
    }
  }

  /// @brief 順列の要素を取り出す．
  int
  elem(
    int pos ///< [in] 要素の位置番号 ( 0 <= pos < num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < num() );

    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

  // 現在の順列
  int* mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDPERMGEN_H
