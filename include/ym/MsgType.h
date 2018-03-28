#ifndef YM_MSGTYPE_H
#define YM_MSGTYPE_H

/// @file ym/MsgType.h
/// @brief MsgMgr, MsgHandler 用の型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @brief メッセージの種類を表す列挙型
//////////////////////////////////////////////////////////////////////
enum class MsgType {
  /// @brief エラー
  /// @note 不正なデータなどの再現性のある致命的な不具合
  Error   = 1,
  /// @brief 警告
  /// @note 軽微な不具合
  Warning = 2,
  /// @brief 失敗
  /// @note メモリ不足，ファイル読み書き失敗のような外的要因の不具合
  Failure = 3,
  /// @brief 情報
  /// @note 付加的な情報
  Info    = 4,
  /// @brief デバッグ情報
  /// @note デバッグ用の付加的な情報
  Debug   = 5
};

/// @brief MsgType に対応するビットマスクの型
typedef ymuint8 MsgBitMask;

/// @brief メッセータイプからビットマスクを得る．
constexpr
MsgBitMask
conv2bitmask(MsgType type);

/// @brief tMsgType のストリーム出力演算子
/// @param[in] s 出力先のストリーム
/// @param[in] type メッセージの種類
ostream&
operator<<(ostream& s,
	   MsgType type);


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
const MsgBitMask kMsgMaskNone = 0U;


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief メッセータイプからビットマスクを得る．
inline
constexpr
MsgBitMask
conv2bitmask(MsgType type)
{
  return 1U << static_cast<int>(type);
}

END_NAMESPACE_YM

#endif // YM_MSGTYPE_H
