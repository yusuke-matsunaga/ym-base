﻿
/// @file MFSet.cc
/// @brief MFSet の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MFSet.h"


BEGIN_NAMESPACE_YM

/// @class MFSetCell MFSet.cc
/// @brief MFSet 内部で用いられるクラス
class MFSetCell
{
  friend class MFSet;
private:

  // コンストラクタ
  MFSetCell();

  // この要素の属する集合の代表元を返す．
  MFSetCell*
  find();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一つの母集合の中でユニークなID番号
  ymuint32 mId;

  // 親を指すポインタ
  MFSetCell* mParent;

  // 根本までのレベル
  ymint32 mRank;

};


// コンストラクタ
MFSetCell::MFSetCell()
{
  mParent = this;
  mRank = 0;
}

// 検索処理
MFSetCell*
MFSetCell::find()
{
  MFSetCell* tmp = this;

  // 検索の途中で道の圧縮を行なう．
  MFSetCell* parent = tmp->mParent;
  while ( parent->mParent != parent ) {
    tmp = tmp->mParent = parent->mParent;
    parent = tmp->mParent;
  }

  return parent;
}

// n 個の要素を持つ集合を作るコンストラクタ．
MFSet::MFSet(ymuint n) :
  mNum(n),
  mCellArray(new MFSetCell[mNum])
{
  for (ymuint i = 0; i < n; ++ i) {
    mCellArray[i].mId = i;
  }
}

// デストラクタ
MFSet::~MFSet()
{
  delete [] mCellArray;
}

// @brief 要素数を返す．
ymuint
MFSet::num() const
{
  return mNum;
}

// x を含む集合の代表元を返す．
// x が存在しない時には 0 を返す．
ymuint
MFSet::find(ymuint id)
{
  MFSetCell* x = get(id);
  if ( x ) {
    x = x->find();
    return x->mId;
  }
  else {
    return kBadID;
  }
}

// x_id を含む集合と y_id を含む集合を併合し，新しい集合の代表元を返す．
// x_id や y_id が存在しない時には 0 を返す．
ymuint
MFSet::merge(ymuint x_id,
	     ymuint y_id)
{
  MFSetCell* x = get(x_id);
  if ( !x ) return kBadID;

  MFSetCell* y = get(y_id);
  if ( !y ) return kBadID;

  if ( x->mParent != x ) {
    // xは代表点ではない．
    x = x->find();
  }

  if ( y->mParent != y ) {
    // yは代表点ではない．
    y = y->find();
  }

  if ( x == y ) {
    // 同一の集合のマージはできない．
    // ただし，違反ではないのでその代表元を返す．
    return x->mId;
  }

  // ランクによるマージ
  if ( x->mRank > y->mRank ) {
    y->mParent = x;
    return x->mId;
  }
  else if ( x->mRank == y->mRank ) {
    ++ x->mRank;
    y->mParent = x;
    return x->mId;
  }
  else {
    x->mParent = y;
    return y->mId;
  }
}

// x 番めのセルを取り出す．
MFSetCell*
MFSet::get(ymuint id)
{
  if ( id >= mNum ) {
    // 範囲外の場合はnullptrを返す．
    return nullptr;
  }
  return &mCellArray[id];
}

END_NAMESPACE_YM
