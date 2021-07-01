
/// @file MultiPermGen.cc
/// @brief MultiPermGen の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MultiPermGen.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiPermGen
//////////////////////////////////////////////////////////////////////

// 次の要素を求める．
void
MultiPermGen::operator++()
{
  for (int g = group_num(); g -- > 0; ) {
    vector<int> bitmap(n(g), 0);
    for (int pos = 0; pos < k(g); ++ pos) {
      bitmap[elem(g)[pos]] = 1;
    }
    for (int pos = k(g); pos -- > 0; ) {
      bool found = false;
      for (int val = elem(g)[pos]; ++ val < n(g); ) {
	if ( bitmap[val] == 0 ) {
	  bitmap[elem(g)[pos]] = 0;
	  elem(g)[pos] = val;
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
	  elem(g)[j] = val;
	  ++ val;
	}
	break;
      }
      if ( pos > 0 ) {
	bitmap[elem(g)[pos]] = 0;
      }
      else {
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
