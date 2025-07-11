#ifndef YMUTILS_MULTISETCOMBIGEN_H
#define YMUTILS_MULTISETCOMBIGEN_H

/// @file ym/MultiSetCombiGen.h
/// @brief MultiSetCombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetCombiGen MultiSetCombiGen.h "ym/MultiSetCombiGen.h"
/// @brief 重複を許した集合の順列を作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetCombiGen :
  public MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  MultiSetCombiGen(
    const std::vector<SizeType>& num_array, ///< [in] 各要素の重複度を納めた配列
    SizeType k                              ///< [in] 選び出す要素数
  ) : MultiSetGenBase(num_array, k)
  {
  }

  /// @brief コンストラクタ
  MultiSetCombiGen(
    std::initializer_list<SizeType>& num_array, ///< [in] 各要素の重複度を納めた初期化リスト
    SizeType k                                  ///< [in] 選び出す要素数
  ) : MultiSetGenBase(num_array, k)
  {
  }

  /// @brief コピーコンストラクタ
  MultiSetCombiGen(
    const MultiSetCombiGen& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief 代入演算子
  /// @return 自分自身
  MultiSetCombiGen&
  operator=(
    const MultiSetCombiGen& src ///< [in] コピー元のオブジェクト
  ) = default;

  /// @brief デストラクタ
  ~MultiSetCombiGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTISETCOMBIGEN_H
