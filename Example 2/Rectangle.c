//
//  Rectangle.c
//  Fudge
//
//  Created by Jacob Gordon on 4/16/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "fudge.h"

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

