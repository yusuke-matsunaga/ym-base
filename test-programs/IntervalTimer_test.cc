
/// @file IntervalTimer_test.cc
/// @brief IntervalTimer_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2024 Yusuke Matsunaga
/// All rights reserved.

#include "ym/IntervalTimer.h"
#include <libgen.h>


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
  if ( argc != 2 ) {
    cerr << "USAGE: " << basename(argv[0]) << " <n>" << endl;
    return 1;
  }

  SizeType n = atoi(argv[1]);

  std::atomic<bool> stop = false;

  IntervalTimer itimer{10};

  itimer.start([&](){ stop = true; });

  for ( int i = 0; i < n; ++ i ) {
    auto v = fib(i);
    std::cout << "fib(" << i << ") = " << v << std::endl;
    if ( stop ) {
      break;
    }
  }

  itimer.stop();


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
