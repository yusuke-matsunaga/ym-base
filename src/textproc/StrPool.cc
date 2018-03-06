
/// @file StrPool.cc
/// @brief StrPool と ShString の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ShString.h"
#include "StrPool.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// 文字列を共有するためのプール
//////////////////////////////////////////////////////////////////////

// コンストラクタ
StrPool::StrPool() :
  mTable(nullptr),
  mTableSize(0),
  mNum(0),
  mCellAlloc(4096)
{
  cout << "StrPool::StrPool()" << endl;

  alloc_table(1024);
}

// デストラクタ
StrPool::~StrPool()
{
  delete [] mTable;
}

// 文字列を登録する．
const char*
StrPool::reg(const char* str)
{
  HashFunc<const char*> hash_func;

  cout << "StrPool::reg(" << str << ")" << endl;

  // まず str と同一の文字列が登録されていないか調べる．
  HashType hash_value = hash_func(str);
  int pos = hash_value & mHashMask;
  for ( Cell* cell = mTable[pos]; cell != nullptr; cell = cell->mLink ) {
    cout << "  cell->mStr = " << cell->mStr << endl;
    if ( memcmp(str, cell->mStr, cell->mSize) == 0 ) {
      return cell->mStr;
    }
  }

  // なければ新しい Cell を用意する．
  // とその前にテーブルの拡張が必要かどうか調べる．
  if ( mNum >= mExpandLimit ) {
    alloc_table(mTableSize << 1);
    // サイズを拡張したので pos が古くなっている．
    pos = hash_value & mHashMask;
  }

  Cell* new_cell = alloc_cell(str);
  new_cell->mLink = mTable[pos];
  mTable[pos] = new_cell;
  ++ mNum;

  return new_cell->mStr;
}

// 確保した文字列領域の総量を得る．
int
StrPool::accum_alloc_size() const
{
  return mCellAlloc.allocated_size();
}

// @brief メモリを全部開放する．
// @note 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
void
StrPool::destroy()
{
  delete [] mTable;
  mCellAlloc.destroy();
  mTableSize = 0;
  mTable = nullptr;
}

// テーブルを確保して初期化する．
void
StrPool::alloc_table(int new_size)
{
  Cell** old_table = mTable;
  int old_size = mTableSize;
  mTableSize = new_size;
  mHashMask = mTableSize - 1;
  mExpandLimit = static_cast<int>(mTableSize * 1.8);
  mTable = new Cell*[mTableSize];
  for ( int i = 0; i < mTableSize; ++ i ) {
    mTable[i] = nullptr;
  }

  HashFunc<const char*> hash_func;
  for ( int i = 0; i < old_size; ++ i ) {
    for ( Cell* cell = old_table[i]; cell != nullptr; ) {
      Cell* tmp = cell;
      cell = cell->mLink;
      HashType pos = hash_func(tmp->mStr) & mHashMask;
      tmp->mLink = mTable[pos];
      mTable[pos] = tmp;
    }
  }
  delete [] old_table;
}

// @brief 新しい文字列を表す Cell を確保する．
// @param[in] str 文字列
StrPool::Cell*
StrPool::alloc_cell(const char* str)
{
  int len = strlen(str) + 1;
  Alloc::SizeType cell_size = sizeof(Cell) + sizeof(char) * (len - 1);
  void* p = mCellAlloc.get_memory(cell_size);
  Cell* new_cell = new (p) Cell;
  memcpy(new_cell->mStr, str, len);
  new_cell->mSize = len;

  return new_cell;
}


//////////////////////////////////////////////////////////////////////
// ShString
//////////////////////////////////////////////////////////////////////

BEGIN_NONAMESPACE

StrPool thePool;

END_NONAMESPACE

// 共有文字列を作ってセットする．
void
ShString::set(const char* str)
{
  mPtr = thePool.reg(str);
}

// @brief ShString 関連でアロケートされたメモリサイズ
int
ShString::allocated_size()
{
  return thePool.accum_alloc_size();
}

// @brief ShString 関連でアロケートされたメモリをすべて開放する．
// @note 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
void
ShString::free_all_memory()
{
  thePool.destroy();
}

// ShString 用ストリーム出力演算子
ostream&
operator<<(ostream& s,
	   const ShString& str)
{
  // ちょっと見にくいけど ShString から const char* への変換演算子
  const char* p = str.operator const char*();
  if ( p ) {
    return s << p;
  }
  else {
    return s << "";
  }
}

END_NAMESPACE_YM
