#
# @file CXX_USTime.pxd
# @brief USTime 用の pxd ファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.


cdef extern from "ym/USTime.h" namespace "nsYm" :

    # USTime クラスの cython バージョン
    cdef cppclass USTime :
        USTime()
        USTime(double, double, double)
        void set(double, double, double)
        double usr_time_usec()
        double sys_time_usec()
        double real_time_usec()
        double usr_time()
        double sys_time()
        double real_time()
