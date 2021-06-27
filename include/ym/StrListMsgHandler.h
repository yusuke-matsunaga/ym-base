﻿#ifndef YM_STRLISTMSGHANDLER_H
#define YM_STRLISTMSGHANDLER_H

/// @file ym/StrListMsgHandler.h
/// @brief StrListMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MsgHandler.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class StrListMsgHandler StrListMsgHandler.h "ym/StrListMsgHandler.h"
/// @ingroup ym
/// @brief 結果をvector<string>に格納する MsgHandler の実装
//////////////////////////////////////////////////////////////////////
class StrListMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  StrListMsgHandler();

  /// @brief デストラクタ
  ~StrListMsgHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler (T6Binder) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] loc ファイル位置
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  const FileRegion& loc,
	  MsgType type,
	  const char* label,
	  const char* body) override;

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  /// @param[in] src_file この関数を読んでいるソースファイル名
  /// @param[in] src_line この関数を読んでいるソースの行番号
  /// @param[in] type メッセージの種類
  /// @param[in] label メッセージラベル
  /// @param[in] body メッセージ本文
  void
  put_msg(const char* src_file,
	  int src_line,
	  MsgType type,
	  const char* label,
	  const char* body) override;


public:
  //////////////////////////////////////////////////////////////////////
  // このクラス特有の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージリストをクリアする．
  void
  clear();

  /// @brief メッセージリストを返す．
  vector<string>
  message_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージリスト
  vector<string> mMsgList;

};

END_NAMESPACE_YM

#endif // YM_STRLISTMSGHANDLER_H
