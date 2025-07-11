
/// @file MultiSetGenBase.cc
/// @brief MultiSetGenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiSetGenBase.h"
#include "ym/MultiSetPermGen.h"
#include "ym/MultiSetCombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiSetGenBase
//////////////////////////////////////////////////////////////////////

// @brief 初期化する．
void
MultiSetGenBase::init()
{
  // 先頭から若い番号を埋めてゆく
  // その番号の要素数が上限に達したら次の番号を割り当てる．
  // いわゆる辞書式順序で最初の順列となる．
  SizeType grp = 0;
  SizeType count = 0;
  for ( int i = 0; i < mK; ++ i ) {
    if ( count >= n(grp) ) {
      // 上限に達したので次の要素にする．
      ++ grp;
      count = 0;
    }
    mElem[i] = grp;
    ++ count;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MultiSetPermGen
//////////////////////////////////////////////////////////////////////

// @brief (辞書式順序で)次の要素を求める．
void
MultiSetPermGen::operator++()
{
  // 各要素の使用数を求める．
  auto ng = group_num();
  std::vector<int> count(ng, 0);
  for ( int i = 0; i < k(); ++ i ) {
    int v = elem(i);
    ++ count[v];
  }

  // 末尾からインクリメントしてゆく．
  for ( int pos = k(); pos -- > 0; ) {
    bool found = false;
    // 今の要素
    int g0 = elem(pos);
    for ( int g1 = g0 + 1; g1 < ng; ++ g1 ) {
      if ( count[g1] < n(g1) ) {
	// 使用可能な要素があった．
	-- count[g0];
	elem(pos) = g1;
	++ count[g1];
	found = true;
	break;
      }
    }
    if ( found ) {
      // pos 番目の要素を更新したのでそれより後ろの要素を初期化する．
      // 具体的には可能な限り先頭の要素から割り当てる．
      int g = 0;
      for ( int pos1 = pos + 1; pos1 < k(); ++ pos1 ) {
	for ( ; count[g] == n(g); ++ g) ;
	++ count[g];
	elem(pos1) = g;
      }
      break;
    }
    else {
      if ( pos > 0 ) {
	-- count[g0];
      }
      else {
	// 終わり
	elem(0) = ng;
      }
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス MultiSetCombiGen
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を求める．
void
MultiSetCombiGen::operator++()
{
  auto ng = group_num();
  std::vector<int> count(ng, 0);
  for ( int i = 0; i < k(); ++ i ) {
    int v = elem(i);
    ++ count[v];
  }

  for ( int pos = k(); pos -- > 0; ) {
    bool found = false;
    for ( int v = elem(pos) + 1; v < ng; ++ v ) {
      if ( count[v] < n(v) ) {
	-- count[elem(pos)];
	elem(pos) = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      auto g = elem(pos);
      for ( int pos1 = pos + 1; pos1 < k(); ++ pos1 ) {
	for ( ; g < ng; ++ g) {
	  if ( count[g] < n(g) ) {
	    break;
	  }
	}
	if ( g == ng ) {
	  elem(0) = ng;
	  return;
	}
	++ count[g];
	elem(pos1) = g;
      }
      break;
    }
    if ( pos > 0 ) {
      -- count[elem(pos)];
    }
    else {
      elem(0) = ng;
    }
  }
}

END_NAMESPACE_YM
