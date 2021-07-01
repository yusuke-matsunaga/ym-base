
/// @file MsgHandler.cc
/// @brief MsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym/MsgHandler.h"
#include "ym/StreamMsgHandler.h"
#include "ym/StrListMsgHandler.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MsgHandler
//////////////////////////////////////////////////////////////////////

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
MsgHandler::put_msg(const char* src_file,
		    int src_line,
		    MsgType type,
		    const char* label,
		    const char* body)
{
  // 継承クラスが定義しなかった時のデフォルトフォールバック
  // 結局，event_proc() 中の条件分岐はなんなんだよ！，というコード
  put_msg(src_file, src_line, FileRegion(), type, label, body);
}

// @brief メッセージを文字列にまとめる．
string
MsgHandler::msg_to_string(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  ostringstream buf;
  buf << loc << ": " << type << " [" << label << "]: " << body << endl;
  return buf.str();
}

// @brief メッセージを文字列にまとめる．
string
MsgHandler::msg_to_string(const char* src_file,
			  int src_line,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  ostringstream buf;
  buf << type << " [" << label << "]: " << body << endl;
  return buf.str();
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
MsgHandler::event_proc(const char* src_file,
		       int src_line,
		       const FileRegion& loc,
		       MsgType type,
		       const char* label,
		       const char* body)
{
  MsgBitMask bit = conv2bitmask(type);
  if ( mMask & bit ) {
    if ( loc.is_valid() ) {
      put_msg(src_file, src_line, loc, type, label, body);
    }
    else {
      put_msg(src_file, src_line, type, label, body);
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス StreamMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  mStream << msg_to_string(src_file, src_line, loc, type, label, body);
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  mStream << msg_to_string(src_file, src_line, type, label, body);
}


//////////////////////////////////////////////////////////////////////
// クラス StrListMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
StrListMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  mMsgList.push_back(msg_to_string(src_file, src_line, loc, type, label, body));
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
void
StrListMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  mMsgList.push_back(msg_to_string(src_file, src_line, type, label, body));
}

END_NAMESPACE_YM
