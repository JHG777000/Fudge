//
//  Example2.c
//  Fudge
//
//  Created by Jacob Gordon on 4/13/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "fudge.h"

void Example2( void ) {
    
    declare_cls(NULL) ;
    
    float area = 0 ;
    
    
    AnyClass circle = NULL ;
    
    sm(ShapeFactory, strings("Circle"), args(AnyClass*,&circle), floats(0.0f, 0.0f, 4.0f, 0.0)) ;
    
    m(circle, get area, args(float*,&area)) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass square = NULL ;
    
    sm(ShapeFactory, strings("Square"), args(AnyClass*,&square), floats(0.0f, 0.0f, 2.0f, 0.0)) ;
    
    m(square, get area, args(float*,&area)) ;
    
    printf("Area: %f\n", area) ;
    
    
    AnyClass rectangle = NULL ;
    
    sm(ShapeFactory, strings("Rectangle"), args(AnyClass*,&rectangle), floats(0.0f, 0.0f, 5.0f, 2.0)) ;
       
    m(rectangle, get area, args(float*,&area)) ;
    
    printf("Area: %f\n", area) ;

    
    sm(ShapeFactory, strings("Destroy"), args(AnyClass*,NULL), floats(0.0f, 0.0f, 0.0f, 0.0)) ;
    
}