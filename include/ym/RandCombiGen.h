#ifndef YM_RANDCOMBIGEN_H
#define YM_RANDCOMBIGEN_H

/// @file ym/RandCombiGen.h
/// @brief RandCombiGen のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
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
  RandCombiGen(
    SizeType n, ///< [in] 全要素数
    SizeType k  ///< [in] 組み合わせの要素数
  ) : mCombiNum{k},
      mArray(n)
  {
  }

  /// @brief デストラクタ
  ~RandCombiGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 全要素数を返す．
  SizeType
  num() const { return mArray.size(); }

  /// @brief 組み合わせの要素数を返す．
  SizeType
  combi_num() const { return mCombiNum; }

  /// @brief ランダムな組み合わせを生成する．
  template <class URNG>
  void
  generate(
    URNG& randgen ///< [in] 乱数発生器
  )
  {
    vector<int> src_array(combi_num());
    for ( int i = 0; i < num(); ++ i ) {
      src_array[i] = i;
    }
    int n = num() - 1;
    for ( int i = 0; i < combi_num(); ++ i ) {
      std::uniform_int_distribution<int> rd(0, n);
      int r = rd(randgen);
      mArray[i] = src_array[r];
      -- n;
      src_array[r] = src_array[n];
    }
  }

  /// @brief 組み合わせの要素を取り出す．
  int
  elem(
    SizeType pos ///< [in] 要素の位置番号 ( 0 <= pos < combi_num() )
  ) const
  {
    ASSERT_COND( 0 <= pos && pos < combi_num() );

    return mArray[pos];
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 組み合わせの要素数
  SizeType mCombiNum;

  // 現在の順列
  vector<int> mArray;

};

END_NAMESPACE_YM

#endif // YM_RANDCOMBIGEN_H
