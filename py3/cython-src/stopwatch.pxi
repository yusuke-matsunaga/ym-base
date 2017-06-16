#
# @file stopwatch.pxi
# @brief stopwatch の cython インターフェイス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from CXX_StopWatch cimport StopWatch as C_StopWatch
from CXX_USTime cimport USTime as C_USTime

## @brief StopWatch の python バージョン
cdef class StopWatch :
    cdef C_StopWatch _this

    ## @brief リセットする．
    def reset(self) :
        self._this.reset()

    ## @brief スタートする．
    def start(self) :
        self._this.start()

    ## @brief ストップする．
    def stop(self) :
        self._this.stop()

    ## @brief 現在の時間を得る．
    def time(self) :
        cdef C_USTime c_time = self._this.time()
        time = USTime()
        time._this = c_time
        return time
