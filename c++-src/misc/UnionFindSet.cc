
/// @file UnionFindSet.cc
/// @brief UnionFindSet の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2010, 2014, 2018, 2019, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/UnionFindSet.h"


BEGIN_NAMESPACE_YM

/// @class UFCell UnionFindSet.cc
/// @brief UnionFindSet 内部で用いられるクラス
class UFCell
{
  friend class UnionFindSet;
private:

  // コンストラクタ
  UFCell(
  ) : mParent{this},
      mRank{0}
  {
  }

  // この要素の属する集合の代表元を返す．
  UFCell*
  find()
  {
    auto tmp = this;

    // 検索の途中で道の圧縮を行なう．
    auto parent = tmp->mParent;
    while ( parent->mParent != parent ) {
      tmp = tmp->mParent = parent->mParent;
      parent = tmp->mParent;
    }

    return parent;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 一つの母集合の中でユニークなID番号
  int mId;

  // 親を指すポインタ
  UFCell* mParent;

  // 根本までのレベル
  int mRank;

};


/// @brief コンストラクタ
UnionFindSet::UnionFindSet(
  SizeType n ///< [in] 確保したい要素の数．
) : mNum{n},
    mCellArray{new UFCell[mNum]}
{
  for ( int i = 0; i < mNum; ++ i ) {
    mCellArray[i].mId = i;
  }
}

// デストラクタ
UnionFindSet::~UnionFindSet()
{
  delete [] mCellArray;
}

// x を含む集合の代表元を返す．
// x が存在しない時には 0 を返す．
int
UnionFindSet::find(
  int id
)
{
  auto x = _get(id);
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
int
UnionFindSet::merge(
  int x_id,
  int y_id
)
{
  auto x = _get(x_id);
  if ( !x ) return kBadID;

  auto y = _get(y_id);
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
UFCell*
UnionFindSet::_get(
  int id
)
{
  if ( id < 0 || id >= mNum ) {
    // 範囲外の場合はnullptrを返す．
    return nullptr;
  }
  return &mCellArray[id];
}

END_NAMESPACE_YM
