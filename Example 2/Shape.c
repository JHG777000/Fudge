//
//  Shape.c
//  Fudge
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "fudge.h"

declare_class(Shape) ;

define_data_protocol(ShapeData, float pos_x ; float pos_y ; float area ;) ;

#define ShapeSelf private_protocol_interface(ShapeData,Shape)

start_method(InitShape, )

 alloc_private_data_protocol(ShapeData, Shape) ;

end_method

start_method(DeinitShape, )

end_method

start_method(get_area, arg(retval, float*))

 *retval = ShapeSelf->area ;

end_method

start_method(set_area, arg(area, float))

 ShapeSelf->area = area ;

end_method

start_method(compute_area, )

end_method

start_method(get_pos, arg(ret_pos_x, float*) arg(ret_pos_y, float*))

 *ret_pos_x = ShapeSelf->pos_x ;

 *ret_pos_y = ShapeSelf->pos_y ;

end_method

start_method(set_pos, arg(pos_x, float) arg(pos_y, float))

 ShapeSelf->pos_x = pos_x ;

 ShapeSelf->pos_y = pos_y ;

end_method

new_class(Shape) {
    
    make_method_protected(InitShape) ;
    
    make_method_protected(DeinitShape) ;
    
    make_method_mask(get_area, get area) ;
    
    make_method_mask(set_area, set area) ;
    
    make_method_mask(compute_area, compute area) ;
    
    make_method_mask(get_pos, get pos) ;
    
    make_method_mask(set_pos, set pos) ;
}