#ifndef YM_RANDCOMBIGEN_H
#define YM_RANDCOMBIGEN_H

/// @file ym/RandCombiGen.h
/// @brief RandCombiGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/Range.h"
#include <random>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class RandCombiGen RandGen.h "ym/RandGen.h"
/// @brief ランダムな組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class RandCombiGen
{
public:

  /// @brief コンストラクタ
  RandCombiGen(int n, ///< [in] 全要素数
	       int k) ///< [in] 組み合わせの要素数
    : mNum{n},
      mCombiNum{k},
      mArray{new int[n]}
  {
  }

  /// @brief デストラクタ
  ~RandCombiGen()
  {
    delete [] mArray;
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全要素数を返す．
  int
  num() const { return mNum; }

  /// @brief 組み合わせの要素数を返す．
  int
  combi_num() const { return mCombiNum; }

  /// @brief ランダムな組み合わせを生成する．
  template <class URNG>
  void
  generate(URNG& randgen) ///< [in] 乱数発生器
  {
    vector<int> src_array(mNum);
    for ( int i: Range(mNum) ) {
      src_array[i] = i;
    }
    int n = mNum - 1;
    for ( int i: Range(mCombiNum) ) {
      std::uniform_int_distribution<int> rd(0, n);
      int r = rd(randgen);
      mArray[i] = src_array[r];
      for ( int j: Range(r, n) ) {
	src_array[j] = src_array[j + 1];
      }
      -- n;
    }
  }

  /// @brief 組み合わせの要素を取り出す．
  int
  elem(int pos) const ///< [in] 要素の位置番号 ( 0 <= pos < combi_num() )
  {
    ASSERT_COND( 0 <= pos && pos < combi_num() );

    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

  // 組み合わせの要素数
  int mCombiNum;

  // 現在の順列
  int* mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDCOMBIGEN_H
