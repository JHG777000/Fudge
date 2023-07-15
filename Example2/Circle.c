/*
 Copyright (c) 2016-2023 Jacob Gordon. All rights reserved.

 Permission to redistribution and use this software in source and binary forms, with or without modification is hereby granted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#define _USE_MATH_DEFINES
#include <math.h>
#include <fudge.h>

use_class(Shape);
declare_class(Circle);
define_data_protocol(CircleData, float radius ;);

#define CircleSelf private_protocol_interface(CircleData,Circle)

start_method(InitCircle, argfromlist(pos_x, float) argfromlist(pos_y, float) argfromlist(radius, float))
 alloc_private_data_protocol(CircleData,Circle);
 m(obj, InitShape, noargs);
 m(obj, set pos, floats(pos_x, pos_y));
 m(obj, set radius, floats(radius));
 m(obj, compute area, noargs);
end_method

start_method(DeinitCircle,)
 m(obj, DeinitShape, noargs);
end_method

start_method(set_radius, arg(radius,float))
 CircleSelf->radius = radius;
end_method

start_method(get_radius, arg(radius,float*))
 *radius = CircleSelf->radius;
end_method

start_method(compute_area, )
 m(obj, set area, floats(CircleSelf->radius * CircleSelf->radius * M_PI));
end_method

new_class(Circle) {
    make_class_subclass_of(Shape);
    make_method_init(InitCircle);
    make_method_deinit(DeinitCircle);
    make_method_mask(get_radius, get radius);
    make_method_mask(set_radius, set radius);
    make_method_mask(compute_area, compute area);
}
