﻿#ifndef YMUTILS_MULTICOMBIGEN_H
#define YMUTILS_MULTICOMBIGEN_H

/// @file ym/MultiCombiGen.h
/// @brief MultiCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiCombiGen MultiCombiGen.h "ym/MultiCombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの組み合わせを生成するクラス
///
/// 意味的には (n_1, k_1), (n_2, k_2), ... という複数の CombiGen
/// の出力の直積を作るイメージ．
///
/// @code
/// vector<pair<int, int> > nk_array(2);
/// nk_array[0] = make_pair(3, 1);
/// nk_array[1] = make_pair(5, 2);
/// for (MultiCombiGen mcg(nk_array); !mcg.is_end(); ++ mcg) {
///    for (int g = 0; g < 2; ++ g) {
///        int k1 = mcg.k(g);
///        for (int i = 0; i < k1; ++ i) {
///            elem[g][i] = mcg(g, i);
///        }
///    }
///    // elem[0][0] に最初のグループの要素が
///    // elem[1][0] - elem[1][1] に次のグループの要素が入る．
/// }
/// @endcode
///
/// という風に使う．
//////////////////////////////////////////////////////////////////////
class MultiCombiGen :
  public MultiGenBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(const vector<pair<int, int>>& nk_array);

  /// @brief コンストラクタ
  /// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
  MultiCombiGen(std::initializer_list<pair<int, int>>& nk_array);

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  MultiCombiGen(const MultiCombiGen& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  /// @return 自分自身
  const MultiCombiGen&
  operator=(const MultiCombiGen& src);

  /// @brief デストラクタ
  ~MultiCombiGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTICOMBIGEN_H
