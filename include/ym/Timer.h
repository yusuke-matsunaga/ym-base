#ifndef TIMER_H
#define TIMER_H

/// @file Timer.h
/// @brief Timer のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Timer Timer.h "Timer.h"
/// @brief 時間計測を行うクラス
//////////////////////////////////////////////////////////////////////
class Timer
{
public:
  //////////////////////////////////////////////////////////////////////
  // 計時用の定義
  //////////////////////////////////////////////////////////////////////

  using Clock = std::chrono::system_clock;
  using Duration = std::chrono::milliseconds;
  using TimePoint = std::chrono::time_point<Clock>;

public:

  /// @brief コンストラクタ
  Timer() = default;

  /// @brief デストラクタ
  ~Timer() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 計時を開始
  void
  start()
  {
    mStartTime = now();
  }

  /// @brief 計時の終了
  double
  end()
  {
    auto end_time = now();
    return calc_time(mStartTime, end_time);
  }

  /// @brief 現時刻の取得
  static
  TimePoint
  now()
  {
    return Clock::now();
  }

  /// @brief 時間を求める．
  static
  double
  calc_time(
    TimePoint start,
    TimePoint end
  )
  {
    auto duration = std::chrono::duration_cast<Duration>(end - start);
    return static_cast<double>(duration.count() / 1000.0);
  }


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 計時の開始時刻
  TimePoint mStartTime;

};

END_NAMESPACE_YM

#endif // TIMER_H
