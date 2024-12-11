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
/// 実際には名前空間として使っている．
//////////////////////////////////////////////////////////////////////
class IntervalTimer
{
public:

  /// @brief 時間単位(ミリ秒)
  using millisec = std::chrono::duration<double, std::milli>;

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief タイマーを起動する．
  static
  void
  start(
    millisec interval,              ///< [in] 間隔(ミリ秒)
    std::function<void()> callback, ///< [in] コールバック関数
    bool periodic = false           ///< [in] 周期的なイベントの時 true にするフラグ
  )
  {
    std::thread interval_thread([&] {
      for ( ; ; ) {
	std::this_thread::sleep_for(interval);
	callback();
	if ( !periodic ) {
	  break;
	}
      }
    });
    interval_thread.detach();
  }

};

END_NAMESPACE_YM

#endif // INTERVALTIMER_H
