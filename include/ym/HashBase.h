#ifndef YM_HASHBASE_H
#define YM_HASHBASE_H

/// @file ym/HashBase.h
/// @brief HaseBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/HashFunc.h"


BEGIN_NAMESPACE_YM

template<typename Key_Type>
class HashBaseIterator;

//////////////////////////////////////////////////////////////////////
/// @class HashBaseCell HashBase.h "ym/HashBase.h"
/// @brief HashBase 用のセル
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
struct HashBaseCell
{
  HashBaseCell() { }

  ~HashBaseCell() { }

  /// @brief キー
  Key_Type mKey;

  /// @brief 次のセルへのリンク
  HashBaseCell* mLink;
};


//////////////////////////////////////////////////////////////////////
/// @class HashBase HashBase.h "ym/HashBase.h"
/// @brief ハッシュ表の基底クラス
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
class HashBase
{
  typedef HashBaseCell<Key_Type> Cell;

protected:

  /// @brief コンストラクタ
  /// @param[in] size 表の初期サイズ
  HashBase(int size);

  /// @brief デストラクタ
  ~HashBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容をクリアする．
  void
  clear();

  /// @brief 要素数を返す．
  int
  num() const;

  /// @brief 要素の有無をチェックする．
  /// @param[in] key 調べる要素のキー
  /// @retval true key というキーを持つ要素が存在する．
  /// @retval false key というキーを持つ要素が存在しない．
  bool
  check(Key_Type key) const;

  /// @brief 要素を削除する．
  /// @param[in] key 削除する要素のキー
  ///
  /// 該当の要素が存在しない場合にはなにもしない．
  void
  erase(Key_Type key);


protected:
  //////////////////////////////////////////////////////////////////////
  // HashSet/HashMapで用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief キーが一致する要素を返す．
  /// @param[in] key キー
  /// @return key と一致する要素を返す．
  ///
  /// 見つからなければ nullptr を返す．
  Cell*
  find_cell(Key_Type key) const;

  /// @brief セルの先頭の反復子を返す．
  HashBaseIterator<Key_Type>
  begin() const;

  /// @brief セルの末尾の反復子を返す．
  HashBaseIterator<Key_Type>
  end() const;

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  void
  reg_cell(Cell* cell);

  /// @brief ハッシュ表を確保する．
  /// @param[in] req_size 表のサイズ
  void
  alloc_table(int req_size);

  /// @brief 要素を登録する．
  /// @param[in] cell 登録する要素
  ///
  /// 同じ名前の要素がすでに登録されていたら動作は不定
  /// reg_cell() との違いはハッシュ表のサイズチェックをしない．
  void
  _reg_cell(Cell* cell);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスのデストラクタ用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表のサイズを得る．
  int
  hash_size();

  /// @brief ハッシュ表の先頭のセルを得る．
  Cell*
  hash_top(int pos);

  /// @brief テーブルを削除する．
  void
  _clear();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表のサイズ
  int mHashSize;

  // ハッシュ表
  Cell** mHashTable;

  // 拡大する目安
  int mNextLimit;

  // 要素数
  int mNum;

};


//////////////////////////////////////////////////////////////////////
/// @class HashBaseIterator HashBase.h "ym/HashBase.h"
/// @brief HashBase の反復子
//////////////////////////////////////////////////////////////////////
template<typename Key_Type>
class HashBaseIterator
{
  friend class HashBase<Key_Type>;
public:

  /// @brief 空のコンストラクタ
  HashBaseIterator();

  /// @brief コピーコンストラクタ
  /// @param[in] src コピー元のオブジェクト
  HashBaseIterator(const HashBaseIterator& src);

  /// @brief 代入演算子
  /// @param[in] src コピー元のオブジェクト
  const HashBaseIterator&
  operator=(const HashBaseIterator& src);

  /// @brief デストラクタ
  ~HashBaseIterator();

  /// @brief 指しているセルを返す．
  HashBaseCell<Key_Type>*
  cell() const;

  /// @brief 一つ進める(前置演算子)
  /// @return 進めた後の反復子を返す．
  HashBaseIterator
  operator++();

  /// @brief 一つ進める(後置演算子)
  /// @return 進める前の反復子を返す．
  /// @note int は後置演算子を表すためのダミー
  HashBaseIterator
  operator++(int);

  /// @brief 等価比較演算子
  bool
  operator==(const HashBaseIterator& src) const;

  /// @brief 非等価比較演算子
  bool
  operator!=(const HashBaseIterator& src) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使用する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を指定するコンストラクタ
  HashBaseIterator(HashBaseCell<Key_Type>** table,
		   int table_size,
		   int index,
		   HashBaseCell<Key_Type>* cell);

  /// @brief ポインタを進める．
  void
  next_ptr();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ハッシュ表
  HashBaseCell<Key_Type>** mTable;

  // ハッシュ表のサイズ
  int mTableSize;

  // 現在のインデックス
  int mIndex;

  // 現在のセル
  HashBaseCell<Key_Type>* mCell;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
template<typename Key_Type>
inline
HashBase<Key_Type>::HashBase(int size)
{
  mHashSize = 0;
  mHashTable = nullptr;
  mNum = 0;
  alloc_table(size);
}

// @brief デストラクタ
template<typename Key_Type>
inline
HashBase<Key_Type>::~HashBase()
{
  clear();
  delete [] mHashTable;
}

// @brief 内容をクリアする．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::clear()
{
  for ( int i = 0; i < mHashSize; ++ i ) {
    for ( Cell* cell = mHashTable[i]; cell != nullptr; ) {
      Cell* tmp = cell;
      cell = tmp->mLink;
      delete tmp;
    }
    mHashTable[i] = nullptr;
  }
  mNum = 0;
}

// @brief 要素数を返す．
template<typename Key_Type>
inline
int
HashBase<Key_Type>::num() const
{
  return mNum;
}

// @brief 要素の有無をチェックする．
// @param[in] key 調べる要素のキー
// @retval true key というキーを持つ要素が存在する．
// @retval false key というキーを持つ要素が存在しない．
template<typename Key_Type>
inline
bool
HashBase<Key_Type>::check(Key_Type key) const
{
  return find_cell(key) != nullptr;
}

// @brief 要素を削除する．
// @param[in] key 削除する要素のキー
//
// 該当の要素が存在しない場合にはなにもしない．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::erase(Key_Type key)
{
  HashFunc<Key_Type> hash_func;
  HashType h = hash_func(key) % mHashSize;
  Cell** pprev = &mHashTable[h];
  Cell* cell;
  while ( (cell = *pprev) != nullptr ) {
    if ( cell->mKey == key ) {
      // 見つけた
      *pprev = cell->mLink;
      delete cell;
      -- mNum;
      break;
    }
    pprev = &cell->mLink;
  }
}

// @brief キーが一致する要素を返す．
// @param[in] key キー
// @return key と一致する要素を返す．
//
// 見つからなければ nullptr を返す．
template<typename Key_Type>
inline
HashBaseCell<Key_Type>*
HashBase<Key_Type>::find_cell(Key_Type key) const
{
  HashFunc<Key_Type> hash_func;
  HashType h = hash_func(key) % mHashSize;
  for ( Cell* cell = mHashTable[h]; cell != nullptr; cell = cell->mLink) {
    if ( cell->mKey == key ) {
      return cell;
    }
  }
  return nullptr;
}

// @brief セルの先頭の反復子を返す．
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>
HashBase<Key_Type>::begin() const
{
  for ( int i = 0; i < mHashSize; ++ i ) {
    if ( mHashTable[i] != nullptr ) {
      return HashBaseIterator<Key_Type>(mHashTable, mHashSize, i, mHashTable[i]);
    }
  }
  return HashBaseIterator<Key_Type>(nullptr, 0, 0, nullptr);
}

// @brief セルの末尾の反復子を返す．
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>
HashBase<Key_Type>::end() const
{
  return HashBaseIterator<Key_Type>(nullptr, 0, 0, nullptr);
}

// @brief 要素を登録する．
// @param[in] cell 登録する要素
//
// 同じ名前の要素がすでに登録されていたら動作は不定
template<typename Key_Type>
inline
void
HashBase<Key_Type>::reg_cell(Cell* cell)
{
  if ( check(cell->mKey) ) {
    return;
  }
  if ( mNum >= mNextLimit ) {
    alloc_table(mHashSize * 2);
  }
  _reg_cell(cell);
  ++ mNum;
}

// @brief ハッシュ表を確保する．
// @param[in] req_size 表のサイズ
template<typename Key_Type>
inline
void
HashBase<Key_Type>::alloc_table(int req_size)
{
  int old_size = mHashSize;
  Cell** old_table = mHashTable;
  mHashSize = req_size;
  mNextLimit = static_cast<int>(mHashSize * 1.8);
  mHashTable = new Cell*[mHashSize];
  for ( int i = 0; i < mHashSize; ++ i ) {
    mHashTable[i] = nullptr;
  }
  for ( int i = 0; i < old_size; ++ i ) {
    for ( Cell* cell = old_table[i]; cell != nullptr; ) {
      Cell* next = cell->mLink;
      _reg_cell(cell);
      cell = next;
    }
  }
  delete [] old_table;
}

// @brief 要素を登録する．
// @param[in] cell 登録する要素
//
// 同じ名前の要素がすでに登録されていたら動作は不定
// reg_cell() との違いはハッシュ表のサイズチェックをしない．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::_reg_cell(Cell* cell)
{
  HashFunc<Key_Type> hash_func;
  HashType h = hash_func(cell->mKey) % mHashSize;
  cell->mLink = mHashTable[h];
  mHashTable[h] = cell;
}

// @brief ハッシュ表のサイズを得る．
template<typename Key_Type>
inline
int
HashBase<Key_Type>::hash_size()
{
  return mHashSize;
}

// @brief ハッシュ表の先頭のセルを得る．
template<typename Key_Type>
inline
HashBaseCell<Key_Type>*
HashBase<Key_Type>::hash_top(int pos)
{
  return mHashTable[pos];
}

// @brief テーブルを削除する．
template<typename Key_Type>
inline
void
HashBase<Key_Type>::_clear()
{
  delete [] mHashTable;
  mHashTable = nullptr;
  mHashSize = 0;
  mNum = 0;
}

// @brief 空のコンストラクタ
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>::HashBaseIterator()
{
  mTable = nullptr;
  mTableSize = 0;
  mIndex = 0;
  mCell = nullptr;
}

// @brief コピーコンストラクタ
// @param[in] src コピー元のオブジェクト
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>::HashBaseIterator(const HashBaseIterator& src)
{
  mTable = src.mTable;
  mTableSize = src.mTableSize;
  mIndex = src.mIndex;
  mCell = src.mCell;
}

// @brief 代入演算子
// @param[in] src コピー元のオブジェクト
template<typename Key_Type>
inline
const HashBaseIterator<Key_Type>&
HashBaseIterator<Key_Type>::operator=(const HashBaseIterator& src)
{
  if ( this != &src ) {
    mTable = src.mTable;
    mTableSize = src.mTableSize;
    mIndex = src.mIndex;
    mCell = src.mCell;
  }
  return *this;
}

// @brief デストラクタ
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>::~HashBaseIterator()
{
}

// @brief 指しているセルを返す．
template<typename Key_Type>
inline
HashBaseCell<Key_Type>*
HashBaseIterator<Key_Type>::cell() const
{
  return mCell;
}

// @brief 一つ進める(前置演算子)
// @return 進めた後の反復子を返す．
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>
HashBaseIterator<Key_Type>::operator++()
{
  next_ptr();
  return *this;
}

// @brief 一つ進める(後置演算子)
// @return 進める前の反復子を返す．
// @note int は後置演算子を表すためのダミー
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>
HashBaseIterator<Key_Type>::operator++(int)
{
  int cur_index = mIndex;
  HashBaseCell<Key_Type>* cur_cell = mCell;
  next_ptr();
  return HashBaseIterator<Key_Type>(mTable, mTableSize, cur_index, cur_cell);
}

// @brief 等価比較演算子
template<typename Key_Type>
inline
bool
HashBaseIterator<Key_Type>::operator==(const HashBaseIterator& src) const
{
  return mCell == src.mCell;
}

// @brief 非等価比較演算子
template<typename Key_Type>
inline
bool
HashBaseIterator<Key_Type>::operator!=(const HashBaseIterator& src) const
{
  return !operator==(src);
}

// @brief 内容を指定するコンストラクタ
template<typename Key_Type>
inline
HashBaseIterator<Key_Type>::HashBaseIterator(HashBaseCell<Key_Type>** table,
					     int size,
					     int index,
					     HashBaseCell<Key_Type>* cell)
{
  mTable = table;
  mTableSize = size;
  mIndex = index;
  mCell = cell;
}

// @brief ポインタを進める．
template<typename Key_Type>
inline
void
HashBaseIterator<Key_Type>::next_ptr()
{
  HashBaseCell<Key_Type>* next = mCell->mLink;
  while ( next == nullptr ) {
    ++ mIndex;
    if ( mIndex < mTableSize ) {
      next = mTable[mIndex];
    }
    else {
      break;
    }
  }
  mCell = next;
}

END_NAMESPACE_YM

#endif // YM_HASHBASE_H
