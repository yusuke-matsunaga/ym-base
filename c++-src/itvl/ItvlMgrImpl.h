#ifndef ITVLMGRIMPL_H
#define ITVLMGRIMPL_H

/// @file ItvlMgrImpl.h
/// @brief ItvlMgrImpl のヘッダファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2019 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class ItvlMgrImpl ItvlMgrImpl.h "ItvlMgrImpl.h"
/// @brief ItvlMgr の実際の処理を行うクラス
//////////////////////////////////////////////////////////////////////
class ItvlMgrImpl
{
public:

  /// @brief コンストラクタ
  ItvlMgrImpl();

  /// @brief デストラクタ
  ~ItvlMgrImpl();


public:

  /// @brief クリアする．
  void
  clear();

  /// @brief 使用可能な数字を得る．
  /// @note 内容は変化しない．
  /// @note 使用可能な区間がない場合(!!!)，-1を返す．
  int
  avail_num();

  /// @brief [d1, d2]という区間が使用可能などうか調べる．
  bool
  check(int d1,
	int d2);

  /// @brief 使用されている区間の最小値を求める．
  /// @note 全区間が未使用の場合は -1 を返す．
  int
  min_id();

  /// @brief 使用されている区間の最大値を求める．
  /// @note 全区間が未使用の場合は -1 を返す．
  int
  max_id();

  /// @brief d を使用可能な区間から削除する．
  void
  erase(int d);

  /// @brief [d1, d2] を使用可能な区間から削除する．
  void
  erase(int d1,
	int d2);

  /// @brief d を使用可能区間に追加する．
  void
  add(int d);

  /// @brief [d1, d2] を使用可能区間に追加する．
  void
  add(int d1,
      int d2);

  /// @brief 内部構造が正しいかチェックする．
  /// @note おかしい時は例外を投げる．
  void
  sanity_check() const;

  /// @brief 内容を表示する
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;

  /// @brief 木構造を表示する
  /// @param[in] s 出力ストリーム
  void
  print_tree(ostream& s) const;

  /// @brief バイナリファイルに書き出す．
  void
  dump(ostream& s) const;

  /// @brief バイナリファイルを読み込む．
  void
  restore(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ型
  //////////////////////////////////////////////////////////////////////

  struct Cell
  {

    // 開始点
    int mStart;

    // 終了点
    int mEnd;

    // 平衡度
    ymint8 mBalance{0};

    // 左の子供
    Cell* mLchd{nullptr};

    // 右の子供
    Cell* mRchd{nullptr};

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief d を含む区間を求める．
  Cell*
  find(int d);

  /// @brief d よりも小さくもっとも右側にある区間を求める．
  Cell*
  find_left(int d);

  /// @brief d よりも大きくもっとも左側にある区間を求める．
  Cell*
  find_right(int d);

  /// @brief 最左端にあるセルを求める．
  Cell*
  leftmost_cell();

  /// @brief 最右端にあるセルを求める．
  Cell*
  rightmost_cell();

  /// @brief 区間を追加する．
  void
  add_itvl(int start,
	   int end);

  /// @brief セルを追加するためのサブルーティン．
  bool
  add_cell(Cell* cell,
	   Cell*& ptr);

  /// @brief 左の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_left(Cell*& ptr);

  /// @brief 右の部分木の高さが減少したときの処理
  /// @return 自分自身の高さも減少する時に true を返す．
  bool
  balance_right(Cell*& ptr);

  /// @brief もっとも右にある節点の内容を cell にコピーして削除する．
  /// @return 木の高さが変化した時には true を返す．
  bool
  remove_right(Cell* cell,
	       Cell*& ptr);

  /// @brief delete のためのサブルーティン
  /// @note ptr を根とする部分木から cell を削除する．
  /// @return この部分木の高さが変わった時には true を返す．
  bool
  remove_cell(Cell* cell,
	      Cell*& ptr);

  /// @brief 新しいセルを確保する．
  /// @param[in] start 開始位置
  /// @param[in] end 終了位置
  Cell*
  new_cell(int start,
	   int end);

  /// @brief セルを削除する．
  void
  delete_cell(Cell* cell);

  /// @brief sanity_check() の下請け関数
  int
  check_cell(Cell* cell,
	     int& l,
	     int& r) const;

  /// @brief print() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  void
  print_cell(ostream& s,
	     Cell* cell) const;

  /// @brief print_tree() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  /// @param[in] level レベル
  void
  print_tree_cell(ostream& s,
		  Cell* cell,
		  int level) const;

  /// @brief dump() の下請け関数
  /// @param[in] s 出力先のストリーム
  /// @param[in] cell 対象のセル
  void
  dump_cell(ostream& s,
	    Cell* cell) const;

  /// @brief restore() の下請け関数
  /// @param[in] s 入力元のストリーム
  /// @return 作成したセルを返す．
  Cell*
  restore_cell(istream& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // フリーセルリストの先頭
  Cell* mFreeTop{nullptr};

  // 根のポインタ
  Cell* mRoot{nullptr};

};

END_NAMESPACE_YM

#endif // ITVLMGRIMPL_H
