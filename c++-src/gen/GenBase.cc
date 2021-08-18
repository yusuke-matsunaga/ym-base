
/// @file GenBase.cc
/// @brief GenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
GenBase::GenBase(
  SizeType n,
  SizeType k
) : mN{n},
    mK{k},
    mElemList(n)
{
  init();
}

// @brief 最初の要素を指すように初期化する．
void
GenBase::init()
{
}

END_NAMESPACE_YM
