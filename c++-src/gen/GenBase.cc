
/// @file GenBase.cc
/// @brief GenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/GenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
GenBase::GenBase(int n,
		 int k) :
  mN{n},
  mK{k},
  mElemList{new int[mK]}
{
  init();
}

// @brief コピーコンストラクタ
GenBase::GenBase(const GenBase& src) :
  mN{src.mN},
  mK{src.mK},
  mElemList{new int[mK]}
{
  for ( int i = 0; i < mK; ++ i ) {
    mElemList[i] = src.mElemList[i];
  }
}

// デストラクタ
GenBase::~GenBase()
{
  delete [] mElemList;
}

// @brief 最初の要素を指すように初期化する．
void
GenBase::init()
{
  for ( int i = 0; i < mK; ++ i ) {
    mElemList[i] = i;
  }
}

// 内容をコピーする関数
void
GenBase::copy(const GenBase& src)
{
  if ( mK != src.mK ) {
    delete [] mElemList;
    mElemList = new int[src.mK];
  }
  mN = src.mN;
  mK = src.mK;
  for ( int i = 0; i < mK; ++ i ) {
    mElemList[i] = src.mElemList[i];
  }
}

END_NAMESPACE_YM
