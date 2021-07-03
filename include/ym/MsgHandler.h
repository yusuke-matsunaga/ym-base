#ifndef YM_MSGHANDLER_H
#define YM_MSGHANDLER_H

/// @file ym/MsgHandler.h
/// @brief MsgHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2018, 2021 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include "ym/MsgType.h"
#include "ym/Binder.h"


BEGIN_NAMESPACE_YM

class FileRegion;

//////////////////////////////////////////////////////////////////////
/// @class MsgHandler MsgHandler.h "ym/MsgHandler.h"
/// @ingroup ym
/// @brief メッセージハンドラを表す基底クラス
//////////////////////////////////////////////////////////////////////
class MsgHandler :
  public T6Binder<const char*,
		  int,
		  const FileRegion&,
		  MsgType,
		  const char*,
		  const char*>
{
public:

  /// @brief コンストラクタ
  MsgHandler(
    MsgBitMask mask = kMsgMaskAll ///< [in] メッセージマスク
  ) : mMask{mask}
  {
  }

  /// @brief デストラクタ
  ///
  /// 登録されていたら削除する．
  virtual
  ~MsgHandler() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // メッセージマスクの管理
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージマスクの設定
  void
  set_mask(
    MsgBitMask mask ///< [in] メッセージマスク
  )
  {
    mMask = mask;
  }

  /// @brief メッセージマスクの取得
  MsgBitMask
  mask() const { return mMask; }

  /// @brief マスクの付加
  ///
  /// type はビットマスクではない．
  void
  add_mask(
    MsgType type ///< [in] 付加するメッセージタイプ
  )
  {
    mMask |= conv2bitmask(type);
  }

  /// @brief マスクの削除
  ///
  /// type はビットマスクではない．
  void
  delete_mask(
    MsgType type ///< [in] 削除するメッセージタイプ
  )
  {
    mMask &= ~(conv2bitmask(type));
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスが実装する仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  virtual
  void
  put_msg(
    const char* src_file,  ///< [in] この関数を呼んでいるソースファイル名
    int src_line,          ///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& loc, ///< [in] ファイル位置
    MsgType type,          ///< [in] メッセージの種類
    const char* label,     ///< [in] メッセージラベル
    const char* body       ///< [in] メッセージ本文
  ) = 0;

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  virtual
  void
  put_msg(
    const char* src_file, ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	  ///< [in] この関数を呼んでいるソースの行番号
    MsgType type,         ///< [in] メッセージの種類
    const char* label,    ///< [in] メッセージラベル
    const char* body      ///< [in] メッセージ本文
  );


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスで用いられる便利関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージを文字列にまとめる．
  string
  msg_to_string(
    const char* src_file,  ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	   ///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& loc, ///< [in] ファイル位置
    MsgType type,	   ///< [in] メッセージの種類
    const char* label,     ///< [in] メッセージラベル
    const char* body       ///< [in] メッセージ本文
  );

  /// @brief メッセージを文字列にまとめる．
  string
  msg_to_string(
    const char* src_file, ///< [in] この関数を呼んでいるソースファイル名
    int src_line,         ///< [in] この関数を呼んでいるソースの行番号
    MsgType type,         ///< [in] メッセージの種類
    const char* label,    ///< [in] メッセージラベル
    const char* body      ///< [in] メッセージ本文
  );


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるメンバ関数
  //////////////////////////////////////////////////////////////////////

  /// @brief メッセージが登録されるたびに呼ばれる仮想関数
  virtual
  void
  event_proc(
    const char* src_file,  ///< [in] この関数を呼んでいるソースファイル名
    int src_line,	   ///< [in] この関数を呼んでいるソースの行番号
    const FileRegion& loc, ///< [in] ファイル位置
    MsgType type,	   ///< [in] メッセージの種類
    const char* label,	   ///< [in] メッセージラベル
    const char* body  	   ///< [in] メッセージ本文
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メッセージマスク
  MsgBitMask mMask;

};

END_NAMESPACE_YM

#endif // YM_MSGHANDLER_H
