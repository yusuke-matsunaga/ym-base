#
# @file CXX_StopWatch.pxd
# @brief StopWatch 用の pxd ファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.


from CXX_USTime cimport USTime


cdef extern from "ym/StopWatch.h" namespace "nsYm" :

    # StopWatch クラスの cython バージョン
    cdef cppclass StopWatch :
        StopWatch()
        void reset()
        void start()
        void stop()
        USTime time()
