# @file CXX_vector.pxd
# @brief CXX_vector の cython インターフェイス
# @author Yusuke Matsunaga (松永 裕介)
#
# Copyright (C) 2017 Yusuke Matsunaga
# All rights reserved.


cdef extern from "<vector>" namespace "std" :

    cdef cppclass vector[T] :
        vector() except +
        vector(vector&) except +
        vector(size_t) except +
        vector(size_t, T&) except +
        T& operator[](size_t)
        void clear()
        void push_back(T&)
        void reserve(size_t)
        size_t size()
