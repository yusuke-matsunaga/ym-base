#ifndef PYM_YMBASE_H
#define PYM_YMBASE_H

/// @file ymbase.h
/// @brief ymbase の初期化関数のエクスポート
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2023 Yusuke Matsunaga
/// All rights reserved.

#include "ym_config.h"


BEGIN_NAMESPACE_YM

/// @brief ymbase モジュールの初期化関数
extern "C" PyObject* PyInit_ymbase();

END_NAMESPACE_YM

#endif // PYM_YMBASE_H
