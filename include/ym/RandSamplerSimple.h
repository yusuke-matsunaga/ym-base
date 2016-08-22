#ifndef YM_RANDSAMPLERSIMPLE_H
#define YM_RANDSAMPLERSIMPLE_H

/// @file ym/RandSamplerSimple.h
/// @brief RandSamplerSimple のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

class RandGen;

//////////////////////////////////////////////////////////////////////
/// @class RandSamplerSimple RandSamplerSimple.h "ym/RandSamplerSimple.h"
/// @brief ランダムサンプリングを行うクラス
//////////////////////////////////////////////////////////////////////
class RandSamplerSimple
{
public:

  /// @brief コンストラクタ
  /// @param[in] weight_array 重みの配列
  RandSamplerSimple(const vector<ymuint>& weight_array);

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @param[in] weight_array 重みの配列
  RandSamplerSimple(ymuint num,
		    ymuint weight_array[]);

  /// @brief デストラクタ
  ~RandSamplerSimple();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  ymuint
  num() const;

  /// @brief 要素の重みを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < num() )
  ymuint
  weight(ymuint pos) const;

  /// @brief サンプリングを行う．
  /// @param[in] randgen 乱数発生器
  /// @return サンプリング結果を返す．
  ymuint
  get_sample(RandGen& randgen);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタの共通処理
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mNum;

  // 各要素の重みの配列
  // サイズは mNum
  ymuint *mWeightArray;

  // 各要素の重みの累積和の配列
  // サイズは mNum + 1
  // mAccumArray[i] = sum mWeightArray[j] ( 0 <= j < i )
  ymuint *mAccumArray;

};

END_NAMESPACE_YM

#endif // YM_RANDSAMPLERSIMPLE_H
