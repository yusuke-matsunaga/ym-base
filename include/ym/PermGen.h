#ifndef YMUTILS_PERMGEN_H
#define YMUTILS_PERMGEN_H

/// @file ym/PermGen.h
/// @brief PermGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PermGen PermGen.h "ym/PermGen.h"
/// @ingroup GeneratorGroup
/// @brief 順列生成器を表すクラス
///
/// @code
/// int n = 10;
/// int k =  3;
/// for ( PermGen pg(n, k); !pg.is_end(); ++ pg )
///    for ( int i = 0; i < k; ++ i ) {
///       elem[i] = pg(i);
///    }
///    // elem[0] - elem[k - 1] に選ばれた要素(番号)が入る．
/// }
/// @endcode
///
/// という風に使う．
//////////////////////////////////////////////////////////////////////
class PermGen :
  public GenBase
{
public:

  /// @brief コンストラクタ
  PermGen(
    int n, ///< [in] 全要素数
    int k  ///< [in] 選択する要素数
  ) : GenBase(n, k)
  {
  }

  /// @brief コピーコンストラクタ
  PermGen(
    const PermGen& src ///< [in] コピー元のオブジェクト
  ) : GenBase(src)
  {
  }

  /// @brief 代入演算子
  /// @return 自分自身
  const PermGen&
  operator=(
    const PermGen& src ///< [in] コピー元のオブジェクト
  )
  {
    copy(src);
    return *this;
  }

  /// @brief デストラクタ
  ~PermGen() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を求める．
  void
  operator++();

};

END_NAMESPACE_YM

#endif // YMUTILS_PERMGEN_H
