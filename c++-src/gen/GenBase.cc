
/// @file GenBase.cc
/// @brief GenBase の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/GenBase.h"
#include "ym/PermGen.h"
#include "ym/CombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス GenBase
//////////////////////////////////////////////////////////////////////

// @brief 最初の要素を指すように初期化する．
void
GenBase::init()
{
  for ( int i = 0; i < k(); ++ i ) {
    elem(i) = i;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス PermGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
PermGen::operator++()
{
  SizeType n = this->n();
  SizeType k = this->k();

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


//////////////////////////////////////////////////////////////////////
// クラス CombiGenIterator
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
CombiGen::operator++()
{
  SizeType n = this->n();
  SizeType k = this->k();
  for ( int pos = k; pos -- > 0; ) {
    if ( elem(pos) < n - k + pos ) {
      ++ elem(pos);
      for ( int pos1 = pos + 1; pos1 < k; ++ pos1 ) {
	elem(pos1) = elem(pos1 - 1) + 1;
      }
      break;
    }
    else if ( pos == 0 ) {
      elem(0) = n;
    }
  }
}

END_NAMESPACE_YM
