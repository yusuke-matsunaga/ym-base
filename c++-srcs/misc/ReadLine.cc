
/// @file ReadLine.cc
/// @brief ReadLine の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "ym/ReadLine.h"
#ifdef HAS_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif


BEGIN_NAMESPACE_YM

bool
ReadLine::get_line(
  const std::string& prompt,
  std::string& linebuf
)
{
#ifdef HAS_READLINE
  auto line_read = readline(prompt.c_str());
  if ( line_read == nullptr ) {
    // EOF
    return false;
  }
  if ( line_read[0] ) {
    add_history(line_read);
  }
  linebuf = string{line_read};
  free(line_read);
  return true;
#else
  std::cerr << prompt;
  std::cerr.flush();
  if ( getline(std::cin, linebuf) ) {
    return true;
  }
  else {
    return false;
  }
#endif
}

END_NAMESPACE_YM
