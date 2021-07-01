#ifndef YM_ITVLMGR_H
#define YM_ITVLMGR_H

/// @file ym/ItvlMgr.h
/// @brief ItvlMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

// ポインタを使うだけなのでこのクラス定義は見せる必要がない．
class ItvlMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ItvlMgr ItvlMgr.h "ym/ItvlMgr.h"
/// @ingroup YmUtils
/// @brief インターバルを管理するクラス
///
/// 最初は 0 から numeric_limits<int>::max() までの区間が利用可能で，
/// 数字を使用するとその部分を利用可能な区間から取り除いてゆく．
/// あとで，使用していた数字や区間を利用可能に戻すことも可能である．
///
/// 内部的には効率化のために AVL 木を用いて実装している．
//////////////////////////////////////////////////////////////////////
class ItvlMgr
{
public:

  /// @brief コンストラクタ
  /// @note 初期値として [0〜numeric_limits<int>::max()) の区間を持つ
  ItvlMgr();

  /// @brief デストラクタ
  ~ItvlMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアして全ての区間を使用可能にする．
  void
  clear();

  /// @brief 使用可能な数字を得る．
  /// @return 使用可能な数字
  /// @note 内容は変化しない．
  int
  avail_num() const;

  /// @brief d を使用可能な区間から削除する．
  void
  erase(int d); ///< [in] 使用不可能となった要素

  /// @brief [d1, d2] を使用可能な区間から削除する．
  void
  erase(int d1,  ///< [in] 区間の開始点
	int d2); ///< [in] 区間の終了点

  /// @brief d を使用可能な区間に追加する．
  void
  add(int d); ///< [in] 使用可能となった要素

  /// @brief [d1, d2] を使用可能な区間に追加する．
  void
  add(int d1,  ///< [in] 区間の開始点
      int d2); ///< [in] 区間の終了点

  /// @brief [d1, d2] が使用可能な区間かどうか調べる．
  /// @retval true 使用可能
  /// @retval false 使用可能でない
  bool
  check(int d1,        ///< [in] 区間の開始点
	int d2) const; ///< [in] 区間の終了点

  /// @brief 使用されている区間の最小値を求める．
  /// @retval 使用されている区間の最小値
  /// @retval -1 全区間が未使用の場合
  int
  min_id() const;

  /// @brief 使用されている区間の最大値を求める．
  /// @retval 使用されている区間の最大値
  /// @retval -1 全区間が未使用の場合
  int
  max_id() const;

  /// @brief 内部構造が正しいかチェックする．
  /// @note おかしい時は例外を投げる．
  void
  sanity_check() const;

  /// @brief 内容を表示する
  void
  print(ostream& s) const; ///< [in] 出力ストリーム

  /// @brief 木構造を表示する
  void
  print_tree(ostream& s) const; ///< [in] 出力ストリーム

  /// @brief バイナリファイルに書き出す．
  void
  dump(ostream& s) const; ///< [in] 出力ストリーム

  /// @brief バイナリファイルを読み込む．
  void
  restore(istream& s); ///< [in] 入力ストリーム


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  unique_ptr<ItvlMgrImpl> mImpl;

};

END_NAMESPACE_YM

#endif // YM_ITVLMGR_H
