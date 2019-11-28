
/// @file StrPool.cc
/// @brief StrPool と ShString の実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/ShString.h"
#include "StrPool.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

inline
SizeType
calc_maxprime(SizeType num)
{
  static struct Data
  {
    SizeType size;
    SizeType mask;
  } data_list[] = {
    {1024, 1021},
    {2048, 2039},
    {4096, 4093},
    {8192, 8191},
    {16384, 16381},
    {32768, 32749},
    {65536, 65521},
    {131072, 131071},
    {262144, 262139},
    {524288, 524287},
    {1048576, 1048573},
    {2097152, 2097143},
    {4194304, 4194301},
    {8388608, 8388593},
    {16777216, 16777213},
    {33554432, 33554393},
    {67108864, 67108859},
    {134217728, 134217689},
    {268435456, 268435399},
    {536870912, 536870909},
    {1073741824, 1073741789},
    {2147483648, 2147483647},
    {4294967296, 4294967291},
    {8589934592, 8589934583},
    {17179869184, 17179869143},
    {34359738368, 34359738337},
    {68719476736, 68719476731},
    {137438953472, 137438953447},
    {274877906944, 274877906899},
    {549755813888, 549755813881},
  };

  for ( auto data: data_list ) {
    if ( data.size == num ) {
      return data.mask;
    }
  }
  return num;
}


// C文字列のハッシュ関数
inline
SizeType
hash_func(const char* key)
{
  SizeType h = 0;
  char c;
  while ( (c = *key ++) ) {
    h = h * 33 + static_cast<SizeType>(c);
  }
  return h;
}

END_NONAMESPACE


//////////////////////////////////////////////////////////////////////
// 文字列を共有するためのプール
//////////////////////////////////////////////////////////////////////

// コンストラクタ
StrPool::StrPool() :
  mTable{nullptr},
  mTableSize{0},
  mNum{0}
{
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
  // まず str と同一の文字列が登録されていないか調べる．
  SizeType hash_value = hash_func(str);
  SizeType pos = hash_value % mHashSize;
  for ( auto cell = mTable[pos]; cell != nullptr; cell = cell->mLink ) {
    if ( memcmp(str, cell->mStr, cell->mSize) == 0 ) {
      return cell->mStr;
    }
  }

  // なければ新しい Cell を用意する．
  // とその前にテーブルの拡張が必要かどうか調べる．
  if ( mNum >= mExpandLimit ) {
    alloc_table(mTableSize << 1);
    // サイズを拡張したので pos が古くなっている．
    pos = hash_value % mHashSize;
  }

  Cell* new_cell = alloc_cell(str);
  add_cell(pos, new_cell);
  ++ mNum;

  return new_cell->mStr;
}

// 確保した文字列領域の総量を得る．
SizeType
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
StrPool::alloc_table(SizeType new_size)
{
  Cell** old_table = mTable;
  SizeType old_size = mTableSize;
  mTableSize = new_size;
  mHashSize = calc_maxprime(mTableSize);
  mExpandLimit = static_cast<SizeType>(mTableSize * 1.8);
  mTable = new Cell*[mTableSize];
  for ( SizeType i = 0; i < mTableSize; ++ i ) {
    mTable[i] = nullptr;
  }

  for ( SizeType i = 0; i < old_size; ++ i ) {
    for ( auto cell = old_table[i]; cell != nullptr; ) {
      auto tmp = cell;
      cell = cell->mLink;
      SizeType pos = hash_func(tmp->mStr) % mHashSize;
      add_cell(pos, tmp);
    }
  }
  delete [] old_table;
}

// @brief 新しい文字列を表す Cell を確保する．
// @param[in] str 文字列
StrPool::Cell*
StrPool::alloc_cell(const char* str)
{
  SizeType len = strlen(str) + 1;
  SizeType cell_size = sizeof(Cell) + sizeof(char) * (len - 1);
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
SizeType
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
