﻿#ifndef YMUTILS_MULTISETPERMGEN_H
#define YMUTILS_MULTISETPERMGEN_H

/// @file ym/MultiSetPermGen.h
/// @brief MultiSetPermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class MultiSetPermGen MultiSetPermGen.h "ym/MultiSetPermGen.h"
/// @brief 重複を許した集合の組み合わせを作るクラス
//////////////////////////////////////////////////////////////////////
class MultiSetPermGen :
  public MultiSetGenBase
{
public:

  /// @brief コンストラクタ
  MultiSetPermGen(const vector<int>& num_array, ///< [in] 各要素の重複度を納めた配列
		  int k)                        ///< [in] 選び出す要素数
    : MultiSetGenBase(num_array, k)
  {
  }

  /// @brief コンストラクタ
  MultiSetPermGen(initializer_list<int>& num_array, ///< [in] 各要素の重複度を納めた初期化配列
		  int k)                            ///< [in] 選び出す要素数
    : MultiSetGenBase(num_array, k)
  {
  }

  /// @brief コピーコンストラクタ
  MultiSetPermGen(const MultiSetPermGen& src) ///< [in] コピー元のオブジェクト
    : MultiSetGenBase(src)
  {
  }

  /// @brief 代入演算子
  /// @return 自分自身
  const MultiSetPermGen&
  operator=(const MultiSetPermGen& src) ///< [in] コピー元のオブジェクト
  {
    copy(src);
    return *this;
  }

  /// @brief デストラクタ
  ~MultiSetPermGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_MULTISETPERMGEN_H
