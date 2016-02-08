﻿
/// @file StopWatch.cc
/// @brief 時間計測用のクラスの実装ファイル
/// @author Yusuke Matsunaga
///
/// Copyright (C) 2005-2011, 2014, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "ym/USTime.h"
#include "ym/StopWatch.h"
#include "ym/MStopWatch.h"


#if defined(YM_HAVE_SYS_TIME_H) && defined(YM_HAVE_TIME_H)
#  include <sys/time.h>
#  include <time.h>
#elif defined(YM_HAVE_SYS_TIME_H)
#  include <sys/time.h>
#elif defined(YM_HAVE_TIME_H)
#  include <time.h>
#else
#  error "Neither <sys/time.h> nor <time.h> are not found."
#endif

#if defined(YM_HAVE_GETRUSAGE)
#  include <sys/resource.h>
#elif defined(YM_HAVE_TIMES)
// System V 系では rusage() の代りに times() を使う．
#  include <sys/param.h>
#  include <sys/times.h>
#elif defined(YM_WIN32)
#  include <windows.h>
#  include <mmsystem.h>
#  include <sys/types.h>
#  include <sys/timeb.h>
#  pragma comment(lib, "winmm.lib")
#else
#  error "Neither getrusage() nor times() are found."
#endif


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス StopWatch
//////////////////////////////////////////////////////////////////////

// コンストラクタ．実はリセット
StopWatch::StopWatch()
{
  reset();
}

// @brief デストラクタ
StopWatch::~StopWatch()
{
}

// ”リセット”
// 計測途中の場合にはその値は捨てられる．
void
StopWatch::reset()
{
  mTotalTime.set(0, 0, 0);
  mState = OFF;
}

// 計測開始
// 今まで計測状態だったらなにもしない．
void
StopWatch::start()
{
  if ( mState == OFF ) {
    mState = ON;
    mStartTime = cur_time();
  }
}

// 計測を止める．
// 今まで停止状態だったらなにもしない．
// 計測状態だったら直前のstart()から今までの経過時間をtotal_timeに足す．
void
StopWatch::stop()
{
  if ( mState == ON ) {
    mState = OFF;
    USTime tmp = cur_time();
    mTotalTime += (tmp - mStartTime);
  }
}

#if !defined(YM_WIN32)
BEGIN_NONAMESPACE

// timeval構造体をdoubleに変換する関数
inline
double
xchg(struct timeval& tv)
{
  return (double)tv.tv_sec * 1000.0 * 1000.0 + (double)tv.tv_usec;
}

END_NONAMESPACE
#endif

// 計測状態の場合にはラップタイムを求める．
// 停止状態の場合には経過時間を返す．
USTime
StopWatch::time() const
{
  USTime ans = mTotalTime;
  if ( mState == ON ) {
    USTime tmp = cur_time();
    ans += (tmp - mStartTime);
  }
  return ans;
}

// 時間をとってくる関数
USTime
StopWatch::cur_time()
{
  double u, s, r;
#if defined(YM_HAVE_GETRUSAGE)
  struct rusage ru;
  getrusage(RUSAGE_SELF, &ru);
  u = xchg(ru.ru_utime);
  s = xchg(ru.ru_stime);
#elif defined(YM_HAVE_TIMES)
  struct tms buffer;
  (void) times(&buffer);
  u = (double)buffer.tms_utime * 1000.0 * 1000.0 / (double)CLK_TCK;
  s = (double)buffer.tms.stime * 1000.0 * 1000.0 / (double)CLK_TCK;
#elif defined(YM_WIN32)
  DWORD cur_time = timeGetTime();
  u = (double) cur_time / 1000.0;
  s = 0.0;
#else
# error "neither getrusage() nor times() are found"
#endif
#if defined(YM_WIN32)
  _timeb tv;
  _ftime_s(&tv);
  r = (double)tv.time + (double)tv.millitm / 1000.0;
#else
  struct timeval tv;
  (void) gettimeofday(&tv, nullptr);
  // オーバーフローさせないためのオフセット
  const int kRTimeOffset = ((2000 - 1970)*365+(2000-1969)/4)*24*3600;
  tv.tv_sec -= kRTimeOffset;
  r = xchg(tv);
#endif
  return USTime(u, s, r);
}


//////////////////////////////////////////////////////////////////////
// クラス MStopWatch
//////////////////////////////////////////////////////////////////////


// コンストラクタ，時計の数と最初にアクティブな時計番号
MStopWatch::MStopWatch(ymuint n,
		       ymuint id) :
  mNum(n),
  mCur(id),
  mWarray(new StopWatch[n])
{
  // 範囲外の場合は補正する．
  if ( mCur >= mNum ) {
    mCur = 0;
  }
  mWarray[mCur].start();
}

// @brief デストラクタ
MStopWatch::~MStopWatch()
{
  delete [] mWarray;
}

// アクティブな時計を切替える．昔の時計番号を返す
ymuint
MStopWatch::change(ymuint new_id)
{
  ymuint old_id = mCur;

  // 範囲外の場合は何もしない．
  if ( new_id < mNum ) {
    mCur = new_id;
    if ( new_id != old_id ) {
      USTime tmp_time = StopWatch::cur_time();

      StopWatch& old_sw = mWarray[old_id];
      old_sw.mState = StopWatch::OFF;
      old_sw.mTotalTime += tmp_time - old_sw.mStartTime;

      StopWatch& new_sw = mWarray[new_id];
      new_sw.mState = StopWatch::ON;
      new_sw.mStartTime = tmp_time;
    }
  }
  return old_id;
}

// 時間をストリームに出力する．
ostream&
operator<<(ostream& s,
	   const USTime& time)
{
  ios::fmtflags save = s.flags();
  s.setf(ios::fixed, ios::floatfield);
  s << setw(8) << setprecision(2)
    << time.usr_time() << "u "
    << setw(8) << setprecision(2)
    << time.sys_time() << "s";
  s.flags(save);
  return s;
}

END_NAMESPACE_YM
