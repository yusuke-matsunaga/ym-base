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
class ItvlTree;

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
  ///
  /// 初期値として [0〜numeric_limits<int>::max()) の区間を持つ
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
  ///
  /// 内容は変化しない．
  int
  avail_num() const;

  /// @brief d を使用可能な区間から削除する．
  void
  erase(
    int d ///< [in] 使用不可能となった要素
  );

  /// @brief d を使用可能な区間に追加する．
  void
  add(
    int d ///< [in] 使用可能となった要素
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 実装クラス
  std::unique_ptr<ItvlTree> mImpl;

};

END_NAMESPACE_YM

#endif // YM_ITVLMGR_H
