#ifndef YM_STREAMMSGHANDLER_H
#define YM_STREAMMSGHANDLER_H

/// @file ym/StreamMsgHandler.h
/// @brief StreamMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MsgHandler.h"


BEGIN_NAMESPACE_YM


//////////////////////////////////////////////////////////////////////
/// @class StreamMsgHandler StreamMsgHandler.h "ym/StreamMsgHandler.h"
/// @ingroup ym
/// @brief ストリーム出力する MsgHandler の実装
//////////////////////////////////////////////////////////////////////
class StreamMsgHandler :
  public MsgHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] stream_ptr ストリームへのポインタ
  StreamMsgHandler(ostream* stream_ptr);

  /// @brief デストラクタ
  ~StreamMsgHandler();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream* mStreamPtr;

};

END_NAMESPACE_YM

#endif // YM_STREAMMSGHANDLER_H
