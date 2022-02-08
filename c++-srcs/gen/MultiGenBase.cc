
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiGenBase.h"
#include "ym/MultiPermGen.h"
#include "ym/MultiCombiGen.h"


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


//////////////////////////////////////////////////////////////////////
// クラス MultiPermGen
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
MultiPermGen::operator++()
{
  for ( int g = group_num(); g -- > 0; ) {
    vector<int> bitmap(n(g), 0);
    for ( int pos = 0; pos < k(g); ++ pos) {
      bitmap[elem(g, pos)] = 1;
    }
    for ( int pos = k(g); pos -- > 0; ) {
      bool found = false;
      for ( int val = elem(g, pos); ++ val < n(g); ) {
	if ( bitmap[val] == 0 ) {
	  bitmap[elem(g, pos)] = 0;
	  elem(g, pos) = val;
	  bitmap[val] = 1;
	  found = true;
	  break;
	}
      }
      if ( found ) {
	int val = 0;
	for (int j = pos + 1; j < k(g); ++ j) {
	  for ( ; bitmap[val] == 1; ++ val) ;
	  bitmap[val] = 1;
	  elem(g, j) = val;
	  ++ val;
	}
	break;
      }
      if ( pos > 0 ) {
	bitmap[elem(g, pos)] = 0;
      }
      else {
	elem(g, 0) = n(g);
      }
    }
    if ( !is_end_sub(g) ) {
      break;
    }
    if ( g > 0 ) {
      init_group(g);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MultiCombiGen
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
MultiCombiGen::operator++()
{
  for ( int g = group_num(); g -- > 0; ) {
    for ( int pos = k(g); pos -- > 0; ) {
      if ( elem(g, pos) < n(g) - k(g) + pos ) {
	++ elem(g, pos);
	for (int pos1 = pos + 1; pos1 < k(g); ++ pos1) {
	  elem(g, pos1) = elem(g, pos1 - 1) + 1;
	}
	break;
      }
      else if ( pos == 0 ) {
	elem(g, 0) = n(g);
      }
    }
    if ( !is_end_sub(g) ) {
      break;
    }
    if ( g > 0 ) {
      init_group(g);
    }
  }
}

END_NAMESPACE_YM
