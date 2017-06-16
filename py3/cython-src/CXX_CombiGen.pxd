#
# @file CXX_CombiGen.pxd
# @brief CombiGen 用の pxd ファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.

from libcpp cimport bool
ctypedef unsigned int ymuint


cdef extern from "ym/CombiGen.h" namespace "nsYm" :

    # CombiGen の cython インターフェイス
    cdef cppclass CombiGen :
        CombiGen(ymuint, ymuint)
        ymuint n()
        ymuitn k()
        void init()
        ymuint operator()(ymuint)
        bool is_end()
        void operator++()
