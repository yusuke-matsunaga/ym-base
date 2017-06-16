﻿
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

// コンストラクタ
// 全要素数 n と選択する要素数 k を必ず指定する．
PermGen::PermGen(ymuint n,
		 ymuint k) :
  GenBase(n, k)
{
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
PermGen::PermGen(const PermGen& src) :
  GenBase(src)
{
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
// @return 自分自身
const PermGen&
PermGen::operator=(const PermGen& src)
{
  copy(src);
  return *this;
}

// デストラクタ
PermGen::~PermGen()
{
}

// 次の要素を求める．
void
PermGen::operator++()
{
  ymuint n = num();
  ymuint k = combi_num();

  // 各値の使用回数(0/1)を数える．
  vector<int> bitmap(n, 0);
  for (ymuint pos = 0; pos < k; ++ pos) {
    bitmap[elem(pos)] = 1;
  }

  // 最後の要素から次の値にしていく．
  for (ymuint pos = k; pos -- > 0; ) {
    bool found = false;
    for (ymuint j = elem(pos); ++ j < n; ) {
      if ( bitmap[j] == 0 ) {
	bitmap[elem(pos)] = 0;
	elem(pos) = j;
	bitmap[j] = 1;
	found = true;
	break;
      }
    }
    if ( found ) {
      // pos + 1 以降の値をセットする．
      ymuint val = 0;
      for (ymuint j = pos + 1; j < k; ++ j) {
	// 使われていない値を探す．
	for ( ; bitmap[val] == 1; ++ val) ;
	bitmap[val] = 1;
	elem(j) = val;
	++ val;
      }
      break;
    }
    if ( pos > 0 ) {
      // pos の値を未定にする．
      bitmap[elem(pos)] = 0;
    }
    else {
      // すべての順列を試した印を書き込む．
      elem(0) = n;
    }
  }
}

END_NAMESPACE_YM
