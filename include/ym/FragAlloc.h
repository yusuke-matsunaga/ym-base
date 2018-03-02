#ifndef YM_FRAGALLOC_H
#define YM_FRAGALLOC_H

/// @file ym/FragAlloc.h
/// @brief FragAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class FragAlloc FragAlloc.h "ym/FragAlloc.h"
/// @brief 2の巾乗の単位で管理するメモリアロケータ
//////////////////////////////////////////////////////////////////////
class FragAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  /// @param[in] max_size このオブジェクトが管理する最大サイズ
  /// @note max_size 以上のメモリ領域はデフォルトのアロケーターを
  /// 使用する．
  explicit
  FragAlloc(SizeType max_size = 4096);

  /// @brief デストラクタ
  virtual
  ~FragAlloc();


private:
  //////////////////////////////////////////////////////////////////////
  // AllocBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  _get_memory(SizeType n);

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  _put_memory(SizeType n,
	      void* blk);

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  _destroy();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる下請け関数
  //////////////////////////////////////////////////////////////////////

  // サイズ 2^p のブロックを確保する．
  char*
  alloc_block(int p);

  // サイズ 2^p のブロックがあれば返す．
  // なければ nullptr を返す．
  char*
  get_block(int p);

  // サイズ 2^p のブロックをリストに戻す．
  void
  put_block(int p,
	    char* block);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // 利用可能なメモリ領域を管理するための構造体
  struct Block
  {
    // 次の要素を指すポインタ
    Block* mLink;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 最小のサイズ
  SizeType mMinSize;

  // mMinSize の log
  int mMinLogSize;

  // コンストラクタの引数
  SizeType mMaxSize;

  // mMaxSize を越えない2の巾乗の数．
  SizeType mMaxPowerSize;

  // mMaxPowerSize の log
  int mMaxLogSize;

  // サイズごとに分けられたブロックリストの配列
  // 配列の大きさは mMaxLogSize + 1
  Block** mBlockListArray;

  // 確保して使用中のメモリ領域のリスト
  list<char*> mAllocList;

};

END_NAMESPACE_YM

#endif // YM_FRAGALLOC_H
