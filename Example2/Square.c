/*
 Copyright (c) 2016-2023 Jacob Gordon. All rights reserved.

 Permission to redistribution and use this software in source and binary forms, with or without modification is hereby granted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fudge.h>

use_class(Shape);
declare_class(Square);
define_data_protocol(SquareData, float size;);

#define SquareSelf private_protocol_interface(SquareData,Square)

start_method(InitSquare, argfromlist(pos_x, float) argfromlist(pos_y, float) argfromlist(size, float))
 alloc_private_data_protocol(SquareData, Square);
 m(obj, InitShape, noargs);
 m(obj, set pos, floats(pos_x, pos_y));
 m(obj, set size, floats(size));
 m(obj, compute area, noargs);
end_method

start_method(DeinitSquare,)
 m(obj, DeinitShape, noargs);
end_method

start_method(set_size, arg(size,float))
 SquareSelf->size = size;
end_method

start_method(get_size, arg(size,float*))
 *size = SquareSelf->size;
end_method

start_method(compute_area, )
 m(obj, set area, floats(SquareSelf->size * SquareSelf->size));
end_method

new_class(Square) {
    make_class_subclass_of(Shape);
    make_method_init(InitSquare);
    make_method_deinit(DeinitSquare);
    make_method_mask(get_size, get size);
    make_method_mask(set_size, set size);
    make_method_mask(compute_area, compute area);
}

