﻿#ifndef YM_ITVLMGR_H
#define YM_ITVLMGR_H

/// @file ym/ItvlMgr.h
/// @brief ItvlMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/IDO.h"
#include "ym/ODO.h"


BEGIN_NAMESPACE_YM

// ポインタを使うだけなのでこのクラス定義は見せる必要がない．
class ItvlMgrImpl;

//////////////////////////////////////////////////////////////////////
/// @class ItvlMgr ItvlMgr.h "ym/ItvlMgr.h"
/// @ingroup YmUtils
/// @brief インターバルを管理するクラス
///
/// 最初は 0 から INT_MAX までの区間が利用可能で，数字を使用するとその
/// 部分を利用可能な区間から取り除いてゆく．
/// あとで，使用していた数字や区間を利用可能に戻すことも可能である．
///
/// 内部的には効率化のために AVL 木を用いて実装している．
//////////////////////////////////////////////////////////////////////
class ItvlMgr
{
public:

  /// @brief コンストラクタ
  /// @note 初期値として [0〜INT_MAX) の区間を持つ
  ItvlMgr();

  /// @brief デストラクタ
  ~ItvlMgr();


public:

  /// @brief クリアして全ての区間を使用可能にする．
  void
  clear();

  /// @brief 使用可能な数字を得る．
  /// @return 使用可能な数字
  /// @note 内容は変化しない．
  int
  avail_num() const;

  /// @brief d を使用可能な区間から削除する．
  /// @param[in] d 使用不可能となった要素
  void
  erase(int d);

  /// @brief [d1, d2] を使用可能な区間から削除する．
  /// @param[in] d1 区間の開始点
  /// @param[in] d2 区間の終了点
  void
  erase(int d1,
	int d2);

  /// @brief d を使用可能な区間に追加する．
  /// @param[in] d 使用可能となった要素
  void
  add(int d);

  /// @brief [d1, d2] を使用可能な区間に追加する．
  /// @param[in] d1 区間の開始点
  /// @param[in] d2 区間の終了点
  void
  add(int d1,
      int d2);

  /// @brief [d1, d2] が使用可能な区間かどうか調べる．
  /// @param[in] d1 区間の開始点
  /// @param[in] d2 区間の終了点
  /// @retval true 使用可能
  /// @retval false 使用可能でない
  bool
  check(int d1,
	int d2) const;

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
  /// @param[in] s 出力ストリーム
  void
  print(ostream& s) const;

  /// @brief 木構造を表示する
  /// @param[in] s 出力ストリーム
  void
  print_tree(ostream& s) const;

  /// @brief バイナリファイルに書き出す．
  void
  dump(ODO& s) const;

  /// @brief バイナリファイルを読み込む．
  void
  restore(IDO& s);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  unique_ptr<ItvlMgrImpl> mImpl;

};


/// @relates ItvlMgr
/// @brief バイナリファイルに書き出す．
/// @param[in] s 出力先のストリーム
/// @param[in] itvlmgr 対象のオブジェクト
/// @return s を返す．
inline
ODO&
operator<<(ODO& s,
	   const ItvlMgr& itvlmgr)
{
  itvlmgr.dump(s);
  return s;
}

/// @relates ItvlMgr
/// @brief バイナリファイルを読み込む．
/// @param[in] s 入力元のストリーム
/// @param[in] itvlmgr 読み込む先のオブジェクト
/// @return s を返す．
inline
IDO&
operator>>(IDO& s,
	   ItvlMgr& itvlmgr)
{
  itvlmgr.restore(s);
  return s;
}

END_NAMESPACE_YM

#endif // YM_ITVLMGR_H
