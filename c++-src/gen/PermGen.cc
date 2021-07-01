
/// @file PermGen.cc
/// @brief PermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/PermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PermGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
PermGen::operator++()
{
  int n = this->n();
  int k = this->k();

  // 各値の使用回数(0/1)を数える．
  vector<bool> used(n, false);
  for ( int pos = 0; pos < k; ++ pos ) {
    used[elem(pos)] = true;
  }

  // 最後の要素から次の値にしていく．
  for ( int pos = k; pos -- > 0; ) {
    bool found = false;
    for ( int j = elem(pos); ++ j < n; ) {
      if ( !used[j] ) {
	// j と elem(pos) を入れ替える．
	used[elem(pos)] = false;
	elem(pos) = j;
	used[j] = true;
	found = true;
	break;
      }
    }
    if ( found ) {
      // pos + 1 以降の値をセットする．
      int val = 0;
      for ( int j = pos + 1; j < k; ++ j ) {
	// 使われていない値を探す．
	for ( ; used[val]; ++ val ) ;
	used[val] = true;
	elem(j) = val;
	++ val;
      }
      break;
    }
    if ( pos > 0 ) {
      // pos の値を未定にする．
      used[elem(pos)] = false;
    }
    else {
      // すべての順列を試した印を書き込む．
      elem(0) = n;
    }
  }
}

END_NAMESPACE_YM
