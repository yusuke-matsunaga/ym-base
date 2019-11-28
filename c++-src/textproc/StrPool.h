#ifndef STRPOOL_H
#define STRPOOL_H

/// @file StrPool.h
/// @brief StrPool のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StrPool StrPool.h "StrPool.h"
/// @ingroup ym
/// @brief 文字列を共有するためのプール
///
/// メンバ関数 reg() で文字列を登録する．
/// 文字列へのポインタが返ってくるが，これはこのオブジェクト内で共有化
/// されている．
///
/// このクラスではいったん登録した文字列を削除する方法はない．
//////////////////////////////////////////////////////////////////////
class StrPool
{
public:

  /// @brief コンストラクタ
  StrPool();

  /// @brief デストラクタ
  /// @note このオブジェクトが管理しているすべてのページが解放される．
  ~StrPool();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文字列を登録する．
  /// @param[in] str 入力となる文字列
  /// @return カノニカライズされた文字列を返す．
  const char*
  reg(const char* str);

  /// @brief 確保した文字列領域の総量を得る．
  /// @return 確保した文字列領域の総量を得る．
  ///
  /// デバッグ/解析用 -- 通常は使わない．
  SizeType
  accum_alloc_size() const;

  /// @brief メモリを全部開放する．
  ///
  /// 非常に破壊的なのでメモリリーク検査時の終了直前などの場合のみに使う．
  void
  destroy();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ハッシュ表の中で使われる要素
  struct
  Cell
  {
    // 次のセルを指すリンクポインタ
    Cell* mLink;

    // 文字数 (末尾の \0 を含む)
    SizeType mSize;

    // 文字列領域の先頭を指すダミー
    // 実際には必要なサイズの領域を確保する．
    char mStr[1];
  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テーブルを確保して初期化する．
  /// @param[in] new_size 新しいテーブルサイズ
  void
  alloc_table(SizeType new_size);

  /// @brief 新しい文字列を表す Cell を確保する．
  /// @param[in] str 文字列
  Cell*
  alloc_cell(const char* str);

  /// @brief セルをリンクに追加する．
  void
  add_cell(SizeType pos,
	   Cell* cell);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // すべての文字列を蓄えておくハッシュ表
  Cell** mTable;

  // ハッシュ表のサイズ
  SizeType mTableSize;

  // ハッシュ表の実効サイズ
  SizeType mHashSize;

  // 登録されている要素数
  SizeType mNum;

  // 次に拡張する基準
  SizeType mExpandLimit;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief セルをリンクに追加する．
inline
void
StrPool::add_cell(SizeType pos,
		  Cell* cell)
{
  cell->mLink = mTable[pos];
  mTable[pos] = cell;
}

END_NAMESPACE_YM

#endif // STRPOOL_H
