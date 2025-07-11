#ifndef YM_MSGMGR_H
#define YM_MSGMGR_H

/// @file ym/MsgMgr.h
/// @brief MsgMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/MsgType.h"


BEGIN_NAMESPACE_YM

class FileRegion;
class MsgHandler;

//////////////////////////////////////////////////////////////////////
/// @class MsgMgr MsgMgr.h "ym/MsgMgr.h"
/// @ingroup ym
/// @brief メッセージを管理するクラス
/// このクラスのメンバ関数はすべて static 関数なので
/// このクラスをインスタンス化する必要はない．というかできない．
/// @sa MsgHandler
//////////////////////////////////////////////////////////////////////
class MsgMgr
{
public:
  //////////////////////////////////////////////////////////////////////
  // ハンドラの登録
  //////////////////////////////////////////////////////////////////////

  /// @brief ハンドラを登録する．
  static
  void
  attach_handler(
    MsgHandler* handler ///< [in] 登録するハンドラ
  );

  /// @brief ハンドラの登録を解除する．
  ///
  /// ハンドラの登録が解除されてもハンドラその物の削除は行わない．
  static
  void
  detach_handler(
    MsgHandler* handler ///< [in] 解除するハンドラ
  );

  /// @brief すべてのハンドラの登録を解除する．
  static
  void
  detach_all_handlers();


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージの出力
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを出力する(パーサー用)．
  static
  void
  put_msg(
    const char* src_file,       ///< [in] この関数を呼んでいるソースファイル名
    int src_line,               ///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& file_loc, ///< [in] ファイル位置
    MsgType type,               ///< [in] メッセージの種類
    const char* label,          ///< [in] メッセージラベル
    const char* msg             ///< [in] メッセージ本文
  );

  /// @brief メッセージを出力する(パーサー用)．
  static
  void
  put_msg(
    const char* src_file,       ///< [in] この関数を呼んでいるソースファイル名
    int src_line,		///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& file_loc, ///< [in] ファイル位置
    MsgType type,		///< [in] メッセージの種類
    const char* label,	        ///< [in] メッセージラベル
    const std::string& msg      ///< [in] メッセージ本文
  )
  {
    put_msg(src_file, src_line, file_loc, type, label, msg.c_str());
  }

  /// @brief メッセージを出力する(一般用)．
  static
  void
  put_msg(
    const char* src_file, ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	  ///< [in] この関数を呼んでいるソースの行番号
    MsgType type,         ///< [in] メッセージの種類
    const char* label,    ///< [in] メッセージラベル
    const char* msg       ///< [in] メッセージ本文
  );

  /// @brief メッセージを出力する(一般用)．
  static
  void
  put_msg(
    const char* src_file,  ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	   ///< [in] この関数を呼んでいるソースの行番号
    MsgType type,	   ///< [in] メッセージの種類
    const char* label,     ///< [in] メッセージラベル
    const std::string& msg ///< [in] メッセージ本文
  )
  {
    put_msg(src_file, src_line, type, label, msg.c_str());
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 統計情報の管理
  //////////////////////////////////////////////////////////////////////

  /// @brief カウント値をクリアする．
  static
  void
  clear_count();

  /// @brief 全メッセージ数を得る．
  static
  int
  msg_num();

  /// @brief エラーメッセージ数を得る．
  static
  int
  error_num();

  /// @brief 警告メッセージ数を得る．
  static
  int
  warning_num();

  /// @brief 情報メッセージ数を得る．
  static
  int
  info_num();

  /// @brief 失敗メッセージ数を得る．
  static
  int
  fail_num();

  /// @brief デバッグメッセージ数を得る．
  static
  int
  debug_num();

};

END_NAMESPACE_YM

#endif // YM_MSGMGR_H
