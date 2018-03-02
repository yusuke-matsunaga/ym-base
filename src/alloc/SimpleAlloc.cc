
/// @file SimpleAlloc.cc
/// @brief SimpleAlloc の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym/SimpleAlloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス SimpleAlloc
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
SimpleAlloc::SimpleAlloc(SizeType page_size)
{
  ASSERT_COND( page_size > 0 );

  mPageSize = align(page_size);
  mBlockTop = nullptr;
}

// デストラクタ
SimpleAlloc::~SimpleAlloc()
{
  destroy();
}

// @brief n バイトの領域を確保する．
void*
SimpleAlloc::_get_memory(SizeType n)
{
  if ( n > mPageSize ) {
    // mPageSize を越えるものは普通にアロケートする．
    SizeType n1 = n + sizeof(BigBlock) - sizeof(char);
    void* p = alloc(n1);
    BigBlock* block = new (p) BigBlock();
    block->mSize = n;
    block->mNext = mBlockTop;
    mBlockTop = block;
    return &block->mDummy[0];
  }

  // ワード境界に乗るようにサイズに整える．
  SizeType alloc_size = align(n);

  // alloc_size 以上の余りがあるページを探す．
  list<Page>::iterator p = mAvailList.begin();
  for ( ; p != mAvailList.end(); ++ p) {
    Page& page = *p;
    if ( page.mNextPos + alloc_size <= mPageSize ) {
      // 見つけた．
      break;
    }
  }
  if ( p == mAvailList.end() ) {
    // 適当なページがなければ新しいページを確保する．
    void* chunk = alloc(mPageSize);
    mUsedList.push_back(static_cast<char*>(chunk));
    mAvailList.push_back(Page(chunk));
    p = mAvailList.end();
    -- p;
  }

  Page& page = *p;
  char* s = &page.mChunk[page.mNextPos];
  page.mNextPos += alloc_size;

  // page の余りがなくなったら mAvailList から取り除く．
  if ( page.mNextPos + align(1) > mPageSize ) {
    mAvailList.erase(p);
  }

  return static_cast<void*>(s);
}

// @brief n バイトの領域を開放する．
void
SimpleAlloc::_put_memory(SizeType n,
			 void* block)
{
  // このクラスでは領域の再利用はしない．
  return;
}

// @brief 今までに確保した全ての領域を破棄する．
void
SimpleAlloc::_destroy()
{
  mAvailList.clear();

  for ( auto chunk: mUsedList ) {
    free(mPageSize, chunk);
  }
  mUsedList.clear();

  for ( BigBlock* block = mBlockTop; block; ) {
    BigBlock* next = block->mNext;
    free(block->mSize + sizeof(BigBlock) - sizeof(char), block);
    block = next;
  }
  mBlockTop = nullptr;
}

// @brief アラインメントを考慮してサイズを調節する．
inline
Alloc::SizeType
SimpleAlloc::align(SizeType req_size)
{
  // double 型の整列境界値
#if defined(WIN32)
  const int ALIGNOF_DOUBLE = __alignof(double);
#else
  const int ALIGNOF_DOUBLE = __alignof__(double);
#endif
  return ((req_size + ALIGNOF_DOUBLE - 1) / ALIGNOF_DOUBLE) * ALIGNOF_DOUBLE;
}

END_NAMESPACE_YM
