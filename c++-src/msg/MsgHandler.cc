
/// @file MsgHandler.cc
/// @brief MsgHandler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "ym/MsgHandler.h"
#include "ym/StreamMsgHandler.h"
#include "ym/StrListMsgHandler.h"
#include "ym/FileRegion.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス MsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
MsgHandler::MsgHandler(MsgBitMask mask) :
  mMask(mask)
{
}

// @brief デストラクタ
MsgHandler::~MsgHandler()
{
}

// @brief メッセージマスクの設定
void
MsgHandler::set_mask(MsgBitMask mask)
{
  mMask = mask;
}

// @brief メッセージマスクの取得
MsgBitMask
MsgHandler::mask() const
{
  return mMask;
}

// @brief マスクの付加
// @param[in] type 付加するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::add_mask(MsgType type)
{
  mMask |= conv2bitmask(type);
}

// @brief マスクの削除
// @param[in] type 削除するメッセージタイプ
// @note type はビットマスクではない．
void
MsgHandler::delete_mask(MsgType type)
{
  mMask &= ~(conv2bitmask(type));
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
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
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] file_loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
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

// @brief コンストラクタ
StreamMsgHandler::StreamMsgHandler(ostream* stream_ptr) :
  mStreamPtr(stream_ptr)
{
}

// @brief デストラクタ
StreamMsgHandler::~StreamMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  const FileRegion& loc,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  (*mStreamPtr) << msg_to_string(src_file, src_line, loc, type, label, body);
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
StreamMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  (*mStreamPtr) << msg_to_string(src_file, src_line, type, label, body);
}


//////////////////////////////////////////////////////////////////////
// クラス StrListMsgHandler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
StrListMsgHandler::StrListMsgHandler()
{
}

// @brief デストラクタ
StrListMsgHandler::~StrListMsgHandler()
{
}

// @brief メッセージが登録されるたびに呼ばれる仮想関数
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] loc ファイル位置
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
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
// @param[in] src_file この関数を読んでいるソースファイル名
// @param[in] src_line この関数を読んでいるソースの行番号
// @param[in] type メッセージの種類
// @param[in] label メッセージラベル
// @param[in] body メッセージ本文
void
StrListMsgHandler::put_msg(const char* src_file,
			  int src_line,
			  MsgType type,
			  const char* label,
			  const char* body)
{
  mMsgList.push_back(msg_to_string(src_file, src_line, type, label, body));
}

// @brief メッセージリストをクリアする．
void
StrListMsgHandler::clear()
{
  mMsgList.clear();
}

// @brief メッセージリストを返す．
vector<string>
StrListMsgHandler::message_list() const
{
  return mMsgList;
}

END_NAMESPACE_YM
