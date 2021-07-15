#ifndef YM_MSGTYPE_H
#define YM_MSGTYPE_H

/// @file ym/MsgType.h
/// @brief MsgMgr, MsgHandler 用の型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief メッセージの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class MsgType {
  Error   = 0, ///< エラー
               ///< 不正なデータなどの再現性のある致命的な不具合
  Warning = 1, ///< 警告
               ///< 軽微な不具合
  Failure = 2, ///< 失敗
               ///< メモリ不足，ファイル読み書き失敗のような外的要因の不具合
  Info    = 3, ///< 情報
               ///< 付加的な情報
  Debug   = 4, ///< デバッグ情報
               ///< デバッグ用の付加的な情報
  End     = 5  ///< メッセージの種類(MsgBitMask のサイズ用．実際には使わない)
};

/// @brief MsgType に対応するビットマスクの型
using MsgBitMask = bitset<static_cast<size_t>(MsgType::End)>;

/// @brief メッセージタイプからビットマスクを得る．
constexpr
MsgBitMask
conv2bitmask(
  MsgType type ///< [in] メッセージタイプ
)
{
  return {(1U << static_cast<int>(type))};
}

/// @brief MsgType のストリーム出力演算子
/// @return s を返す．
inline
ostream&
operator<<(
  ostream& s,  ///< [in] 出力先のストリーム
  MsgType type ///< [in] メッセージの種類
)
{
  switch ( type ) {
  case MsgType::Error:   s << "(ERROR  )"; break;
  case MsgType::Warning: s << "(WARNING)"; break;
  case MsgType::Info:    s << "(INFO   )"; break;
  case MsgType::Failure: s << "(FAILURE)"; break;
  case MsgType::Debug:   s << "(DEBUG  )"; break;
  case MsgType::End:     s << "(-------)"; break; // この値は使わない
  }
  return s;
}


//////////////////////////////////////////////////////////////////////
// マスク用の定数
//////////////////////////////////////////////////////////////////////

/// @brief MsgType::Error 用のビットマスク
const MsgBitMask kMsgMaskError = conv2bitmask(MsgType::Error);

/// @brief MsgType::Warning 用のビットマスク
const MsgBitMask kMsgMaskWarning = conv2bitmask(MsgType::Warning);

/// @brief MsgType::Info 用のビットマスク
const MsgBitMask kMsgMaskInfo = conv2bitmask(MsgType::Info);

/// @brief MsgType::Failure 用のビットマスク
const MsgBitMask kMsgMaskFailure = conv2bitmask(MsgType::Failure);

/// @brief MsgType::Debug 用のビットマスク
const MsgBitMask kMsgMaskDebug = conv2bitmask(MsgType::Debug);

/// @brief 全てのを含むビットマスク
const MsgBitMask kMsgMaskAll =
	  kMsgMaskError |
	  kMsgMaskWarning |
	  kMsgMaskInfo |
	  kMsgMaskFailure |
	  kMsgMaskDebug;

/// @brief 全てを含まないビットマスク
const MsgBitMask kMsgMaskNone{0U};

END_NAMESPACE_YM

#endif // YM_MSGTYPE_H
