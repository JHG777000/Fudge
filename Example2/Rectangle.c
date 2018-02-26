/*
 Copyright (c) 2016 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fudge.h>

use_class(Shape) ;

declare_class(Rectangle) ;

define_data_protocol(RectangleData, float length ; float width ;) ;

#define RectangleSelf private_protocol_interface(RectangleData,Rectangle)

start_method(InitRectangle, argfromlist(pos_x, float) argfromlist(pos_y, float) argfromlist(width, float) argfromlist(length, float))

 alloc_private_data_protocol(RectangleData, Rectangle) ;

 m(obj, InitShape, noargs) ;

 m(obj, set pos, floats(pos_x, pos_y)) ;

 m(obj, set size, floats(width, length)) ;

 m(obj, compute area, noargs) ;

end_method

start_method(DeinitRectangle,)

 m(obj, DeinitShape, noargs) ;

end_method

start_method(set_size, arg(width, float) arg(length, float))

 RectangleSelf->width = width ;

 RectangleSelf->length = length ;

end_method

start_method(get_size, arg(width, float*) arg(length, float*))

 *width = RectangleSelf->width ;

 *length = RectangleSelf->length ;

end_method

start_method(compute_area, )

 m(obj, set area, floats(RectangleSelf->width * RectangleSelf->length)) ;

end_method

new_class(Rectangle) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitRectangle) ;
    
    make_method_deinit(DeinitRectangle) ;
    
    make_method_mask(get_size, get size) ;
    
    make_method_mask(set_size, set size) ;
    
    make_method_mask(compute_area, compute area) ;
}

