
/// @file RandSamplerSimple.cc
/// @brief RandSamplerSimple の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/RandSamplerSimple.h"
#include "ym/RandGen.h"


BEGIN_NAMESPACE_YM

// @brief コンストラクタ
// @param[in] weight_array 重みの配列
RandSamplerSimple::RandSamplerSimple(const vector<ymuint>& weight_array)
{
  mNum = static_cast<ymuint>(weight_array.size());
  mWeightArray = new ymuint[mNum];
  for (ymuint i = 0; i < mNum; ++ i) {
    ymuint weight = weight_array[i];
    mWeightArray[i] = weight;
  }

  init();
}

// @brief コンストラクタ
// @param[in] num 要素数
// @param[in] weight_array 重みの配列
RandSamplerSimple::RandSamplerSimple(ymuint num,
				     ymuint weight_array[])
{
  mNum = num;
  mWeightArray = new ymuint[mNum];
  for (ymuint i = 0; i < mNum; ++ i) {
    ymuint weight = weight_array[i];
    mWeightArray[i] = weight;
  }

  init();
}

// @brief コンストラクタの共通処理
void
RandSamplerSimple::init()
{
  mAccumArray = new ymuint[mNum + 1];
  ymuint accum = 0;
  for (ymuint i = 0; i < mNum; ++ i) {
    mAccumArray[i] = accum;
    accum += mWeightArray[i];
  }
  mAccumArray[mNum] = accum;
}

// @brief デストラクタ
RandSamplerSimple::~RandSamplerSimple()
{
  delete [] mWeightArray;
  delete [] mAccumArray;
}

// @brief 要素数を返す．
ymuint
RandSamplerSimple::num() const
{
  return mNum;
}

// @brief 要素の重みを返す．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
ymuint
RandSamplerSimple::weight(ymuint pos) const
{
  ASSERT_COND( pos < num() );

  return mWeightArray[pos];
}

// @brief サンプリングを行う．
// @param[in] randgen 乱数発生器
// @return サンプリング結果を返す．
ymuint
RandSamplerSimple::get_sample(RandGen& randgen)
{
  ymuint val = randgen.int32() % mAccumArray[mNum];

  ymuint left = 0;
  ymuint right = mNum;
  // left <= i < right && mAccumArray[i] <= val < mAccumArray[i + 1]
  // を満たす i を求める．
  while ( (left + 1) < right ) {
    ymuint half = (left + right) / 2;
    if ( mAccumArray[half] <= val ) {
      left = half;
    }
    else {
      right = half;
    }
  }
  ASSERT_COND( mAccumArray[left] <= val );
  ASSERT_COND( mAccumArray[left + 1] > val );
  return left;
}

END_NAMESPACE_YM
