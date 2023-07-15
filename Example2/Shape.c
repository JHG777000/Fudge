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

declare_class(Shape);
define_data_protocol(ShapeData, float pos_x ; float pos_y ; float area ;);

#define ShapeSelf private_protocol_interface(ShapeData,Shape)

start_method(InitShape, )
 alloc_private_data_protocol(ShapeData, Shape);
end_method

start_method(DeinitShape, )
end_method

start_method(get_area, arg(retval, float*))
 *retval = ShapeSelf->area;
end_method

start_method(set_area, arg(area, float))
 ShapeSelf->area = area;
end_method

start_method(compute_area, )
end_method

start_method(get_pos, arg(ret_pos_x, float*) arg(ret_pos_y, float*))
 *ret_pos_x = ShapeSelf->pos_x;
 *ret_pos_y = ShapeSelf->pos_y;
end_method

start_method(set_pos, arg(pos_x, float) arg(pos_y, float))
 ShapeSelf->pos_x = pos_x;
 ShapeSelf->pos_y = pos_y;
end_method

new_class(Shape) {
    make_method_protected(InitShape);
    make_method_protected(DeinitShape);
    make_method_mask(get_area, get area);
    make_method_mask(set_area, set area);
    make_method_mask(compute_area, compute area);
    make_method_mask(get_pos, get pos);
    make_method_mask(set_pos, set pos);
}