
/// @file IntervalTimer_test.cc
/// @brief IntervalTimer_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/IntervalTimer.h"


BEGIN_NAMESPACE_YM

int
fib(
  int x
)
{
  if ( x == 0 ) {
    return 0;
  }
  if ( x == 1 ) {
    return 1;
  }
  return fib(x - 1) + fib(x - 2);
}

int
itimer_test(
  int argc,
  char** argv
)
{

  bool stop = false;
  auto interval = IntervalTimer::millisec(1000);

  IntervalTimer::start(interval,
		       [&]() {
			 stop = true;
		       });

  for ( int i = 0; i < 10000; ++ i ) {
    auto v = fib(i);
    std::cout << "fib(" << i << ") = " << v << std::endl;
    if ( stop ) {
      break;
    }
  }

  return 0;
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  return YM_NAMESPACE::itimer_test(argc, argv);
}
