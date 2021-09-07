#ifndef YMUTILS_MULTIPERMGEN_H
#define YMUTILS_MULTIPERMGEN_H

/// @file ym/MultiPermGen.h
/// @brief MultiPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiPermGen MultiPermGen.h "ym/MultiPermGen.h"
/// @ingroup GeneratorGroup
/// @brief 複数の要素のグループの順列を生成するクラス
///
/// 意味的には (n_1, k_1), (n_2, k_2), ... という複数の PemGen
/// の出力の直積を作るイメージ．
///
/// @code
/// vector<pair<int, int> > nk_array(2);
/// nk_array[0] = make_pair(3, 1);
/// nk_array[1] = make_pair(5, 2);
/// for (MultiPemGen mpg(nk_array); !mpg.is_end(); ++ mpg) {
///    for (int g = 0; g < 2; ++ g) {
///        int k1 = mpg.k(g);
///        for (int i = 0; i < k1; ++ i) {
///            elem[g][i] = mpg(g, i);
///        }
///    }
///    // elem[0][0] に最初のグループの要素が
///    // elem[1][0] - elem[1][1] に次のグループの要素が入る．
/// }
/// @endcode
///
/// という風に使う．
//////////////////////////////////////////////////////////////////////
class MultiPermGen :
  public MultiGenBase
{
public:

  /// @brief コンストラクタ
  MultiPermGen(
    const vector<pair<int, int>>& nk_array ///< [in] 要素数 n と選択する要素数 k のベクタ
  ) : MultiGenBase{nk_array}
  {
  }

  /// @brief コンストラクタ
  MultiPermGen(
    initializer_list<pair<int, int>>& nk_array ///< [in] 要素数 n と選択する要素数 k の初期化リスト
  ) : MultiGenBase{nk_array}
  {
  }

  /// @brief コピーコンストラクタ
  MultiPermGen(
    const MultiPermGen& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  /// @return 自分自身
  MultiPermGen&
  operator=(
    const MultiPermGen& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~MultiPermGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTIPERMGEN_H
