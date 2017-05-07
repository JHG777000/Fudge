//
//  Example.c
//  Fudge
//
//  Created by Jacob Gordon on 4/12/16.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include <fudge.h>

use_class(TestClass) ;

use_class(StringClass) ;

void Example1( void ) {
    
    declare_cls(NULL) ;
    
    cm(TestClass, print_hi, noargs) ;
    
    TestClass myobject = new_object(TestClass, floats(10.0)) ; //new_object(Class, arguments...)
    
    m(myobject, my_first_method, ints(7)) ; //m is method invoke: m(object, method name, arguments...)
    
    m(myobject, my_method, ints(10, 33)) ;
    
    RKLong retval = m(myobject, print_hello_world_from_test, noargs) ; //protected method can not be accessed from here returns 0, method unresolved
    
    printf("%ld\n",retval) ;
    
    cm(TestClass, print_hello, myobject) ;
    
    free_object(myobject) ;
    
    StringClass string = new_object(StringClass, strings("This is my String!\n")) ;
    
    m(string,print,noargs) ;
    
    RKLong size = 0 ;
    
    m(string, get the size of the string, args(RKLong*,&size)) ;
    
    printf("%ld\n",size) ;
    
    m(string, print size of the object on the stack in bytes, noargs) ;
    
    if (!m(string, hello world, noargs)) { //methods by default return 1, if a method is not found 0 is returned
        
        printf("hello world: not found! \n") ;
        
        if ( get_method(string, hello world) == get_null_method  ) printf("hello world: not found! 2 \n") ; //unresolved methods are equal to get_null_method
        
    }
    
    free_object(string) ;

}

