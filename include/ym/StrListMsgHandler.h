#ifndef YM_STRLISTMSGHANDLER_H
#define YM_STRLISTMSGHANDLER_H

/// @file ym/StrListMsgHandler.h
/// @brief StrListMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
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
  StrListMsgHandler() = default;

  /// @brief デストラクタ
  ~StrListMsgHandler() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler (T6Binder) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  void
  put_msg(const char* src_file,        ///< [in] この関数を呼んでいるソースファイル名
	  int src_line,		       ///< [in] この関数を呼んでいるソースの行番号
	  const FileRegion& loc,       ///< [in] ファイル位置
	  MsgType type,		       ///< [in] メッセージの種類
	  const char* label,	       ///< [in] メッセージラベル
	  const char* body) override;  ///< [in] メッセージ本文

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  void
  put_msg(const char* src_file,        ///< [in] この関数を呼んでいるソースファイル名
	  int src_line,		       ///< [in] この関数を呼んでいるソースの行番号
	  MsgType type,		       ///< [in] メッセージの種類
	  const char* label,	       ///< [in] メッセージラベル
	  const char* body) override;  ///< [in] メッセージ本文


public:
  //////////////////////////////////////////////////////////////////////
  // このクラス特有の外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージリストをクリアする．
  void
  clear() { mMsgList.clear(); }

  /// @brief メッセージリストを返す．
  vector<string>
  message_list() const { return mMsgList; }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージリスト
  vector<string> mMsgList;

};

END_NAMESPACE_YM

#endif // YM_STRLISTMSGHANDLER_H
