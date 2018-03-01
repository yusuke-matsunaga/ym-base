#ifndef YM_RANDSAMPLER_H
#define YM_RANDSAMPLER_H

/// @file ym/RandSampler.h
/// @brief RandSampler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

class RandGen;

//////////////////////////////////////////////////////////////////////
/// @class RandSampler RandSampler.h "ym/RandSampler.h"
/// @brief ランダムサンプリングを行うクラス
//////////////////////////////////////////////////////////////////////
class RandSampler
{
public:

  /// @brief コンストラクタ
  /// @param[in] weight_array 重みの配列
  RandSampler(const vector<int>& weight_array);

  /// @brief コンストラクタ
  /// @param[in] num 要素数
  /// @param[in] weight_array 重みの配列
  RandSampler(int num,
	      int weight_array[]);

  /// @brief デストラクタ
  ~RandSampler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief 要素の重みを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < num() )
  int
  weight(int pos) const;

  /// @brief サンプリングを行う．
  /// @param[in] randgen 乱数発生器
  /// @return サンプリング結果を返す．
  int
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
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct RsNode
  {
    // 要素番号
    int mIndex;

    // 重み
    int mWeight;

    // 左の子供
    RsNode* mLeft;

    // 右の子供
    RsNode* mRight;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // RsNode 用のヒープ木を扱う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素を上に移動する．
  /// @param[in] heap ヒープ木の配列
  /// @param[in] pos 対象の位置
  void
  move_up(RsNode** heap,
	  int pos);

  /// @brief 要素を下に移動する．
  /// @param[in] heap ヒープ木の配列
  /// @param[in] heap_size ヒープ木のサイズ
  /// @param[in] pos 対象の位置
  void
  move_down(RsNode** heap,
	    int heap_size,
	    int pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  int mNum;

  // 各要素の重みの配列
  // サイズは mNum;
  int *mWeightArray;

  // 重みの総和
  int mTotalWeight;

  // サンプリング用の2分木の根
  RsNode* mRoot;

  // 2分木のノードの実体の配列
  // サイズは mNum * 2 - 1
  RsNode* mNodeArray;

};

END_NAMESPACE_YM

#endif // YM_RANDSAMPLER_H
