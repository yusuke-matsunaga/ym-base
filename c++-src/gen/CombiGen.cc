
/// @file CombiGen.cc
/// @brief CombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/CombiGen.h"


BEGIN_NAMESPACE_YM

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
