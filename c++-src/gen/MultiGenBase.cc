
/// @file MultiGenBase.cc
/// @brief MultiGenBase の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MultiGenBase.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MultiGenBase
//////////////////////////////////////////////////////////////////////

// コンストラクタ
// 全要素数 n と選択する要素数 k のベクタを指定する．
MultiGenBase::MultiGenBase(const vector<pair<int, int>>& nk_array)
  : mGroupNum{static_cast<int>(nk_array.size())},
    mNArray{new int[mGroupNum]},
    mKArray{new int[mGroupNum]},
    mOffsetArray{new int[mGroupNum]}
{
  int g = 0;
  int offset = 0;
  for ( auto p: nk_array ) {
    mNArray[g] = p.first;
    mKArray[g] = p.second;
    mOffsetArray[g] = offset;
    offset += mKArray[g];
    ++ g;
  }
  mElemArray = new int[offset];
  init();
}

// @brief コンストラクタ
// @param[in] nk_array 全要素数 n と選択する要素数 k のベクタ
MultiGenBase::MultiGenBase(initializer_list<pair<int, int>>& nk_array)
  : mGroupNum{static_cast<int>(nk_array.size())},
    mNArray{new int[mGroupNum]},
    mKArray{new int[mGroupNum]},
    mOffsetArray{new int[mGroupNum]}
{
  int g = 0;
  int offset = 0;
  for ( auto p: nk_array ) {
    mNArray[g] = p.first;
    mKArray[g] = p.second;
    mOffsetArray[g] = offset;
    ++ g;
    offset += mKArray[g];
  }
  mElemArray = new int[offset];
  init();
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
MultiGenBase::MultiGenBase(const MultiGenBase& src)
  : mGroupNum{src.mGroupNum},
    mNArray{new int[mGroupNum]},
    mKArray{new int[mGroupNum]},
    mOffsetArray{new int[mGroupNum]}
{
  int offset = 0;
  for ( int g = 0; g < mGroupNum; ++ g ) {
    mNArray[g] = src.mNArray[g];
    mKArray[g] = src.mKArray[g];
    mOffsetArray[g] = offset;
    offset += mKArray[g];
  }
  mElemArray = new int[offset];
  for ( int i = 0; i < offset; ++ i ) {
    mElemArray[i] = src.mElemArray[i];
  }
}

// デストラクタ
MultiGenBase::~MultiGenBase()
{
  delete [] mNArray;
  delete [] mKArray;
  delete [] mOffsetArray;
  delete [] mElemArray;
}

// @brief 初期化
void
MultiGenBase::init()
{
  for ( int group = 0; group < mGroupNum; ++ group ) {
    init_group(group);
  }
}

// コピーする．
void
MultiGenBase::copy(const MultiGenBase& src)
{
  if ( this != &src ) {
    delete [] mNArray;
    delete [] mKArray;
    delete [] mOffsetArray;
    delete [] mElemArray;
    mGroupNum = src.mGroupNum;
    mNArray = new int[mGroupNum];
    mKArray = new int[mGroupNum];
    mOffsetArray = new int[mGroupNum];
    int offset = 0;
    for ( int g = 0; g < mGroupNum; ++ g ) {
      mNArray[g] = src.mNArray[g];
      mKArray[g] = src.mKArray[g];
      mOffsetArray[g] = offset;
      offset += mKArray[g];
    }
    mElemArray = new int[offset];
    for ( int i = 0; i < offset; ++ i ) {
      mElemArray[i] = src.mElemArray[i];
    }
  }
}

// @brief 要素配列の初期化
// @param[in] grp グループ番号
// @note grp 番目のグループの要素配列を初期化する．
void
MultiGenBase::init_group(int grp)
{
  for ( int i = 0; i < k(grp); ++ i ) {
    elem(grp)[i] = i;
  }
}

END_NAMESPACE_YM
