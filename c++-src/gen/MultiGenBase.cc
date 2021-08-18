
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiGenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiGenBase::MultiGenBase(
  const vector<pair<int, int>>& nk_array
) : mGroupNum{nk_array.size()},
    mNArray(mGroupNum),
    mKArray(mGroupNum),
    mOffsetArray(mGroupNum)
{
  SizeType g = 0;
  SizeType offset = 0;
  for ( auto& p: nk_array ) {
    mNArray[g] = p.first;
    mKArray[g] = p.second;
    mOffsetArray[g] = offset;
    offset += mKArray[g];
    ++ g;
  }
  mElemArray.resize(offset);
  init();
}

// @brief コンストラクタ
// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
MultiGenBase::MultiGenBase(
  initializer_list<pair<int, int>>& nk_array
) : mGroupNum{nk_array.size()},
    mNArray(mGroupNum),
    mKArray(mGroupNum),
    mOffsetArray(mGroupNum)
{
  SizeType g = 0;
  SizeType offset = 0;
  for ( auto& p: nk_array ) {
    mNArray[g] = p.first;
    mKArray[g] = p.second;
    mOffsetArray[g] = offset;
    offset += mKArray[g];
    ++ g;
  }
  mElemArray.resize(offset);
  init();
}

// @brief 初期化
void
MultiGenBase::init()
{
  for ( int group = 0; group < mGroupNum; ++ group ) {
    init_group(group);
  }
}

// @brief 要素配列の初期化
// @param[in] grp グループ番号
// @note grp 番目のグループの要素配列を初期化する．
void
MultiGenBase::init_group(
  SizeType grp
)
{
  for ( int i = 0; i < k(grp); ++ i ) {
    elem(grp, i) = i;
  }
}

END_NAMESPACE_YM
