#ifndef INTERVALTIMER_H
#define INTERVALTIMER_H

/// @file IntervalTimer.h
/// @brief IntervalTimer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"
#include <chrono>
#include <functional>
#include <thread>


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class IntervalTimer IntervalTimer.h "IntervalTimer.h"
/// @brief 時間経過で割り込み処理を行うクラス
///
/// 意味的には別スレッドで指定された時間後に割り込み関数を呼び出すだけだが，
/// 本スレッドがそれ以前に終了しているとエラーとなるので，実際には
/// 1秒ごとに本スレッドが終了していないかチェックしている．
/// それでも最大で1秒は本スレッド終了後に子スレッドが生きていることになるが，
/// 許容範囲と考える．
//////////////////////////////////////////////////////////////////////
class IntervalTimer
{
public:

  /// @brief コンストラクタ
  IntervalTimer(
    SizeType interval,    ///< [in] 間隔(秒)
    bool periodic = false ///< [in] 周期的なイベントの時 true にするフラグ
  ) : mInterval{interval},
      mPeriodic{periodic}
  {
  }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief タイマーを起動する．
  void
  start(
    std::function<void()> callback ///< [in] コールバック関数
  )
  {
    mAlive = true;
    std::thread interval_thread([&] {
      for ( ; ; ) {
	for ( SizeType i = 0; i < mInterval; ++ i ) {
	  // 1秒間停止する．
	  std::this_thread::sleep_for(std::chrono::seconds(1));
	  if ( !mAlive ) {
	    // 本スレッドが終了していたら終わる．
	    return;
	  }
	}
	// コールバック関数を呼び出す．
	callback();
	if ( !mPeriodic ) {
	  break;
	}
      }
    });
    interval_thread.detach();
  }

  /// @brief タイマーを終了する．
  void
  stop()
  {
    mAlive = false;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 間隔(秒)
  SizeType mInterval{0};

  // 周期的なイベントの時 true にするフラグ
  bool mPeriodic{false};

  // 動作中を表すフラグ
  std::atomic<bool> mAlive{false};

};

END_NAMESPACE_YM

#endif // INTERVALTIMER_H
