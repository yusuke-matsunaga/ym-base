
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
  SizeType grp = 0;
  SizeType count = 0;
  for ( int i = 0; i < mK; ++ i ) {
    if ( n(grp) <= count ) {
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

// @brief 次の要素を求める．
void
MultiSetPermGen::operator++()
{
  SizeType ng = group_num();
  vector<int> count(ng, 0);
  for ( int i = 0; i < k(); ++ i ) {
    int v = elem(i);
    ++ count[v];
  }

  // 唯一，後置型の演算子の意味のある使い方
  // 符号なし数なので pos >= 0 は常に成り立ってしまう．
  // あえて pos <= k という使い方もできないことはないが，，，，
  for ( int pos = k(); pos -- > 0; ) {
    bool found = false;
    for ( int v = elem(pos); ++ v < ng; ++ v ) {
      if ( count[v] < n(v) ) {
	-- count[elem(pos)];
	elem(pos) = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      int g = 0;
      for ( int pos1 = pos + 1; pos1 < k(); ++ pos1 ) {
	for ( ; count[g] == n(g); ++ g) ;
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


//////////////////////////////////////////////////////////////////////
// クラス MultiSetCombiGen
//////////////////////////////////////////////////////////////////////

// @brief 次の要素を求める．
void
MultiSetCombiGen::operator++()
{
  SizeType ng = group_num();
  vector<int> count(ng, 0);
  for ( int i = 0; i < k(); ++ i ) {
    int v = elem(i);
    ++ count[v];
  }

  // 唯一，後置型の演算子の意味のある使い方
  // 符号なし数なので pos >= 0 は常に成り立ってしまう．
  // あえて pos <= k という使い方もできないことはないが，，，，
  for ( int pos = k(); pos -- > 0; ) {
    bool found = false;
    for ( int v = elem(pos); ++ v < ng; ++ v ) {
      if ( count[v] < n(v) ) {
	-- count[elem(pos)];
	elem(pos) = v;
	++ count[v];
	found = true;
	break;
      }
    }
    if ( found ) {
      int g = elem(pos);
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
