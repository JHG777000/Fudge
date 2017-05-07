//
//  Square.c
//  Fudge
//
//  Created by Jacob Gordon on 4/16/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include <fudge.h>

use_class(Shape) ;

declare_class(Square) ;

define_data_protocol(SquareData, float size ;) ;

#define SquareSelf private_protocol_interface(SquareData,Square)

start_method(InitSquare, argfromlist(pos_x, float) argfromlist(pos_y, float) argfromlist(size, float))

 alloc_private_data_protocol(SquareData, Square) ;

 m(obj, InitShape, noargs) ;

 m(obj, set pos, floats(pos_x, pos_y)) ;

 m(obj, set size, floats(size)) ;

 m(obj, compute area, noargs) ;

end_method

start_method(DeinitSquare,)

 m(obj, DeinitShape, noargs) ;

end_method

start_method(set_size, arg(size,float))

 SquareSelf->size = size ;

end_method

start_method(get_size, arg(size,float*))

 *size = SquareSelf->size ;

end_method

start_method(compute_area, )

 m(obj, set area, floats(SquareSelf->size * SquareSelf->size)) ;

end_method

new_class(Square) {
    
    make_class_subclass_of(Shape) ;
    
    make_method_init(InitSquare) ;
    
    make_method_deinit(DeinitSquare) ;
    
    make_method_mask(get_size, get size) ;
    
    make_method_mask(set_size, set size) ;
    
    make_method_mask(compute_area, compute area) ;
}

