
/// @file MultiCombiGen.cc
/// @brief MultiCombiGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MultiCombiGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiCombiGen
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
MultiCombiGen::operator++()
{
  for (int g = group_num(); g -- > 0; ) {
    for (int pos = k(g); pos -- > 0; ) {
      if ( elem(g)[pos] < n(g) - k(g) + pos ) {
	++ elem(g)[pos];
	for (int pos1 = pos + 1; pos1 < k(g); ++ pos1) {
	  elem(g)[pos1] = elem(g)[pos1 - 1] + 1;
	}
	break;
      }
      else if ( pos == 0 ) {
	elem(g)[0] = n(g);
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
