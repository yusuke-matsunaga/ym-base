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
  Error   = 1, ///< エラー
               ///< 不正なデータなどの再現性のある致命的な不具合
  Warning = 2, ///< 警告
               ///< 軽微な不具合
  Failure = 3, ///< 失敗
               ///< メモリ不足，ファイル読み書き失敗のような外的要因の不具合
  Info    = 4, ///< 情報
               ///< 付加的な情報
  Debug   = 5  ///< デバッグ情報
               ///< デバッグ用の付加的な情報
};

/// @brief MsgType に対応するビットマスクの型
using MsgBitMask = bitset<5>;

/// @brief メッセージタイプからビットマスクを得る．
constexpr
MsgBitMask
conv2bitmask(
  MsgType type ///< [in] メッセージタイプ
)
{
  return bitset<5>(1U << static_cast<int>(type));
}

/// @brief MsgType のストリーム出力演算子
/// @return s を返す．
extern
ostream&
operator<<(
  ostream& s,  ///< [in] 出力先のストリーム
  MsgType type ///< [in] メッセージの種類
);


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
