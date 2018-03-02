
/// @file FragAlloc.cc
/// @brief FragAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym/FragAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス FragAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
FragAlloc::FragAlloc(SizeType max_size) :
  mMaxSize(max_size)
{
  // double 型の整列境界値
#if defined(WIN32)
  const int ALIGNOF_DOUBLE = __alignof(double);
#else
  const int ALIGNOF_DOUBLE = __alignof__(double);
#endif

  mMinSize = 1;
  mMinLogSize = 0;
  for ( ; mMinSize < ALIGNOF_DOUBLE; mMinSize <<= 1, ++ mMinLogSize) {
    ; // ダミー
  }
  ASSERT_COND( mMinSize == ALIGNOF_DOUBLE );

  ASSERT_COND( max_size > 0 );
  mMaxPowerSize = 1;
  mMaxLogSize = 0;
  for ( ; mMaxPowerSize < max_size; mMaxPowerSize <<= 1, ++ mMaxLogSize) {
    ; // ダミー
  }

  mBlockListArray = new Block*[mMaxLogSize - mMinLogSize + 1];
  for ( int i = mMinLogSize; i <= mMaxLogSize; ++ i ) {
    mBlockListArray[i - mMinLogSize] = nullptr;
  }
}

// デストラクタ
FragAlloc::~FragAlloc()
{
  destroy();
  delete [] mBlockListArray;
}

// @brief n バイトの領域を確保する．
void*
FragAlloc::_get_memory(SizeType n)
{
  if ( n > mMaxSize ) {
    // デフォルトのアロケータを使う．
    return alloc(n);
  }

  // 2の巾乗のサイズに整える．
  SizeType alloc_size = mMinSize;
  int pos0 = mMinLogSize;
  while ( alloc_size < n ) {
    alloc_size <<= 1;
    ++ pos0;
  }
  char* ans = alloc_block(pos0);
  return static_cast<void*>(ans);
}

// @brief n バイトの領域を開放する．
void
FragAlloc::_put_memory(SizeType n,
		       void* block)
{
  if ( n > mMaxSize ) {
    free(n, block);
  }
  else {
    char* cblock = static_cast<char*>(block);

    // 2の巾乗のサイズに整える．
    SizeType alloc_size = mMinSize;
    int pos0 = mMinLogSize;
    while ( alloc_size < n ) {
      alloc_size <<= 1;
      ++ pos0;
    }
    put_block(pos0, cblock);
  }
}

// @brief 今までに確保した全ての領域を破棄する．
void
FragAlloc::_destroy()
{
  for ( int i = mMinLogSize; i <= mMaxLogSize; ++ i ) {
    mBlockListArray[i - mMinLogSize] = nullptr;
  }
  for ( auto chunk: mAllocList ) {
    free(mMaxPowerSize, static_cast<void*>(chunk));
  }
  mAllocList.clear();
}

// サイズ 2^p のブロックを確保する．
char*
FragAlloc::alloc_block(int p)
{
  char* block = get_block(p);
  if ( block ) {
    return block;
  }
  else if ( p == mMaxLogSize ) {
    void* p = alloc(mMaxPowerSize);
    char* block = static_cast<char*>(p);
    mAllocList.push_back(block);
    return block;
  }
  else {
    char* block = alloc_block(p + 1);
    char* block1 = &block[(1ULL << p)];
    put_block(p, block1);
    return block;
  }
}

// サイズ 2^p のブロックがあれば返す．
// なければ nullptr を返す．
char*
FragAlloc::get_block(int p)
{
  Block* b = mBlockListArray[p - mMinLogSize];
  if ( b ) {
    mBlockListArray[p - mMinLogSize] = b->mLink;
    return reinterpret_cast<char*>(b);
  }
  return nullptr;
}

// サイズ 2^p のブロックをリストに戻す．
void
FragAlloc::put_block(int p,
		     char* block)
{
  Block* b = reinterpret_cast<Block*>(block);
  b->mLink = mBlockListArray[p - mMinLogSize];
  mBlockListArray[p - mMinLogSize] = b;
}

END_NAMESPACE_YM
