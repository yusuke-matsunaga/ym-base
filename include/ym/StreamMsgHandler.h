#ifndef YM_STREAMMSGHANDLER_H
#define YM_STREAMMSGHANDLER_H

/// @file ym/StreamMsgHandler.h
/// @brief StreamMsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
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
  StreamMsgHandler(
    ostream& stream ///< [in] 出力先ストリーム
  ) : mStream{stream}
  {
  }

  /// @brief デストラクタ
  ~StreamMsgHandler() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // MsgHandler (T6Binder) の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  void
  put_msg(
    const char* src_file,  ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	   ///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& loc, ///< [in] ファイル位置
    MsgType type,	   ///< [in] メッセージの種類
    const char* label,	   ///< [in] メッセージラベル
    const char* body       ///< [in] メッセージ本文
  ) override;

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  void
  put_msg(
    const char* src_file, ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	  ///< [in] この関数を呼んでいるソースの行番号
    MsgType type,	  ///< [in] メッセージの種類
    const char* label,	  ///< [in] メッセージラベル
    const char* body      ///< [in] メッセージ本文
  ) override;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 出力先のストリーム
  ostream& mStream;

};

END_NAMESPACE_YM

#endif // YM_STREAMMSGHANDLER_H
