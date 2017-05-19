//
//  Circle.c
//  Fudge
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//


#define _USE_MATH_DEFINES
#include <math.h>
#include <fudge.h>

use_class(Shape) ;

declare_class(Circle) ;

define_data_protocol(CircleData, float radius ;) ;

#define CircleSelf private_protocol_interface(CircleData,Circle)

start_method(InitCircle, argfromlist(pos_x, float) argfromlist(pos_y, float) argfromlist(radius, float))

 alloc_private_data_protocol(CircleData,Circle) ;

 m(obj, InitShape, noargs) ;

 m(obj, set pos, floats(pos_x, pos_y)) ;

 m(obj, set radius, floats(radius)) ;

 m(obj, compute area, noargs) ;

end_method

start_method(DeinitCircle,)

 m(obj, DeinitShape, noargs) ;

end_method

start_method(set_radius, arg(radius,float))

 CircleSelf->radius = radius ;

end_method

start_method(get_radius, arg(radius,float*))

 *radius = CircleSelf->radius ;

end_method

start_method(compute_area, )

 m(obj, set area, floats(CircleSelf->radius * CircleSelf->radius * M_PI)) ;

end_method

new_class(Circle) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitCircle) ;
    
    make_method_deinit(DeinitCircle) ;
    
    make_method_mask(get_radius, get radius) ;
    
    make_method_mask(set_radius, set radius) ;
    
    make_method_mask(compute_area, compute area) ;
}
