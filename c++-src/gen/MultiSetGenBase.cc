
/// @file MultiSetGenBase.cc
/// @brief MultiSetGenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiSetGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiSetGenBase
//////////////////////////////////////////////////////////////////////

// @brief 初期化する．
void
MultiSetGenBase::init()
{
  int pos = 0;
  int count = 0;
  for (int i = 0; i < mK; ++ i) {
    if ( count >= mNumArray[pos] ) {
      ++ pos;
      count = 0;
    }
    ASSERT_COND( count < mNumArray[pos] );

    mElem[i] = pos;
    ++ count;
  }
}

// @brief 内容をコピーする関数
// @param[in] src コピー元のオブジェクト
void
MultiSetGenBase::copy(const MultiSetGenBase& src)
{
  mNumArray = src.mNumArray;
  mK = src.mK;
  mElem = src.mElem;
}

END_NAMESPACE_YM
