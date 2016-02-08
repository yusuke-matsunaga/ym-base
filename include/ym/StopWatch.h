﻿#ifndef YM_STOPWATCH_H
#define	YM_STOPWATCH_H

/// @file ym/StopWatch.h
/// @brief StopWatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym_config.h"
#include "ym/USTime.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class StopWatch StopWatch.h "ym/StopWatch.h"
/// @ingroup YmUtils
/// @brief ストップウォッチのクラス
/// @note
/// - {"停止中", "計時中"}の2つの内部状態を持つ．
/// - 計時中の累積時間を内部に保持している．
/// @sa USTime, MStopWatch
//////////////////////////////////////////////////////////////////////
class StopWatch
{
  friend class MStopWatch;

public:

  /// @brief コンストラクタ．
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態で初期化される．
  StopWatch();

  /// @brief デストラクタ
  ~StopWatch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief "リセット" する.
  /// @note
  /// - 停止中
  /// - 累積時間は 0
  /// の状態にリセットする．計測途中の場合には今までの累積値は捨てられる．
  void
  reset();

  /// @brief 計測開始.
  /// @note すでに計時中だったらなにもしない．
  void
  start();

  /// @brief 計測を止める.
  /// @note
  /// - すでに停止状態だったらなにもしない．
  /// - 計測状態だったら直前の start() から今までの経過時間を
  /// 累積時間に足す．
  void
  stop();

  /// @brief 累積時間の取得
  /// @retval ラップタイム 計測状態の場合
  /// @retval 経過累積時間 停止状態
  USTime
  time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 現在の時間をとってくる関数
  static
  USTime
  cur_time();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ストップウォッチの状態を表す
  enum {
    // 停止状態
    OFF,
    // 計測状態
    ON
  } mState;

  // 開始時刻
  USTime mStartTime;

  // 経過時間
  USTime mTotalTime;

};

END_NAMESPACE_YM

#endif // YM_STOPWATCH_H
