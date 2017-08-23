#
# @file CXX_RandGen.pxd
# @brief RandGen 用のpxdファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2016, 2017 Yusuke Matsunaga
# All rights reserved.


cdef extern from "ym/RandGen.h" namespace "nsYm" :

    # RandGen クラスの cython バージョン
    cdef cppclass RandGen :
        RandGen()
        void init(unsigned int seed)
        void init_by_array(unsigned int* init_key, unsigned int key_length)
        unsigned int int32()
        int int31()
        unsigned long uint64()
        double real1()
        double real2()
        double real3()
        double res53()
