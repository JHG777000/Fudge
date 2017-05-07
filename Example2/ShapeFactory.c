//
//  ShapeFactory.c
//  Fudge
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include <fudge.h>

use_class(Circle) ;

use_class(Square) ;

use_class(Rectangle) ;

declare_private_class(ShapeFactoryClass) ;

start_static_method(ShapeFactory, arg(shape, const char*) arg(object, AnyClass*) arg(pos_x, float) arg(pos_y, float) arg(value1, float) arg(value2, float))

 static ShapeFactoryClass factory = NULL ;

 AnyClass any_class = NULL ;

 if ( factory == NULL ) factory = new_object(ShapeFactoryClass, noargs) ;

 RKLong retval = send_object_msg(factory, shape, args(AnyClass*,&any_class), floats(pos_x, pos_y, value1, value2)) ;

 if ( retval ) {
    
     if ( retval == method_returned_error ) {
         
         free_object(factory) ;
         
         factory = NULL ;
         
         return_from_method ;
     }
     
     *object = any_class ;
     
     return_from_method ;
 }

end_method

start_method(new_circle, arg(ret_circle, AnyClass*) arg(pos_x, float) arg(pos_y, float) arg(radius, float))

 *ret_circle = strong(new_any_object(Circle, floats(pos_x, pos_y, radius))) ;

end_method

start_method(new_square, arg(ret_square, AnyClass*) arg(pos_x, float) arg(pos_y, float) arg(size, float))

 *ret_square = strong(new_any_object(Square, floats(pos_x, pos_y, size))) ;

end_method

start_method(new_rectangle, arg(ret_rectangle, AnyClass*) arg(pos_x, float) arg(pos_y, float) arg(width, float) arg(length, float))

 *ret_rectangle = strong(new_any_object(Rectangle, floats(pos_x, pos_y, width, length))) ;

end_method

start_method(destroy_factory,)

 return_error_from_method ;

end_method

new_private_class(ShapeFactoryClass) {
    
    make_method_mask(new_circle, Circle) ;
    
    make_method_mask(new_square, Square) ;
    
    make_method_mask(new_rectangle, Rectangle) ;
    
    make_method_mask(destroy_factory, Destroy) ;
}
