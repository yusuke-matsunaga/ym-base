#ifndef YM_UNITALLOC_H
#define YM_UNITALLOC_H

/// @file ym/UnitAlloc.h
/// @brief UnitAlloc のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/Alloc.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class UnitAlloc UnitAlloc.h "ym/UnitAlloc.h"
/// @brief 単一サイズのメモリ領域の管理を行うクラス
//////////////////////////////////////////////////////////////////////
class UnitAlloc :
  public Alloc
{
public:

  /// @brief コンストラクタ
  /// @param[in] unit_size メモリ割り当ての単位となるサイズ
  /// @param[in] block_size 一度に確保する個数
  explicit
  UnitAlloc(SizeType unit_size,
	    SizeType block_size = 1024);

  /// @brief デストラクタ
  virtual
  ~UnitAlloc();


private:
  //////////////////////////////////////////////////////////////////////
  // AllocBase の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief n バイトの領域を確保する．
  /// @param[in] n 確保するメモリ量(単位はバイト)
  virtual
  void*
  _get_memory(SizeType n) override;

  /// @brief n バイトの領域を開放する．
  /// @param[in] n 確保したメモリ量(単位はバイト)
  /// @param[in] blk 開放するメモリ領域の先頭番地
  virtual
  void
  _put_memory(SizeType n,
	      void* blk) override;

  /// @brief 今までに確保した全ての領域を破棄する．
  /// 個々のオブジェクトのデストラクタなどは起動されない
  /// ので使用には注意が必要
  virtual
  void
  _destroy() override;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


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

  // 単位サイズ
  SizeType mUnitSize;

  // ブロックサイズ
  SizeType mBlockSize;

  // 利用可能なメモリ領域のリスト
  Block* mAvailTop;

  // 確保して使用中のメモリ領域のリスト
  vector<void*> mAllocList;

};

END_NAMESPACE_YM

#endif // YM_UNITALLOC_H
