
/// @file ReadLineTest.cc
/// @brief ReadLineTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ReadLine.h"


BEGIN_NAMESPACE_YM

void
loop()
{
  const char* prompt{"% "};
  for ( ; ; ) {
    std::string linebuf;
    if ( !ReadLine::get_line(prompt, linebuf) ) {
      break;
    }
    std::cout << " " << linebuf << std::endl;
  }
}

END_NAMESPACE_YM


int
main(
  int argc,
  char** argv
)
{
  YM_NAMESPACE::loop();

  return 0;
}
