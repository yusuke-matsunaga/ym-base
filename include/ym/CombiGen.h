﻿#ifndef YMUTILS_COMBIGEN_H
#define YMUTILS_COMBIGEN_H

/// @file ym/CombiGen.h
/// @brief CombiGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class CombiGen CombiGen.h "ym/CombiGen.h"
/// @ingroup GeneratorGroup
/// @brief 組み合わせ生成器を表すクラス
///
/// @code
/// int n = 10;
/// int k =  3;
/// for ( CombiGen cg(n, k); !cg.is_end(); ++ cg )
///    for ( int i = 0; i < k; ++ i ) {
///       elem[i] = cg(i);
///    }
///    // elem[0] - elem[k - 1] に選ばれた要素(番号)が入る．
/// }
/// @endcode
///
/// という風に使う．
//////////////////////////////////////////////////////////////////////
class CombiGen :
  public GenBase
{
public:

  /// @brief コンストラクタ
  CombiGen(int n, ///< [in] 全要素数
	   int k) ///< [in] 選び出す要素数
    : GenBase(n, k)
  {
  }

  /// @brief コピーコンストラクタ
  CombiGen(const CombiGen& src) ///< [in] コピー元のオブジェクト
    : GenBase{src}
  {
  }

  /// @brief 代入演算子
  /// @return 自分自身を返す．
  const CombiGen&
  operator=(const CombiGen& src) ///< [in] コピー元のオブジェクト
  {
    copy(src);
    return *this;
  }

  /// @brief デストラクタ
  ~CombiGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_COMBIGEN_H
