//
//  TestClass.c
//  Fudge
//
//  Created by Jacob Gordon on 10/10/15.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include "fudge.h"

use_class(BaseClass) ;

use_class(StringClass) ;

declare_class(TestClass) ;

define_record_type( TestClassFDS, int value ; ) ;

define_data_protocol(TestClassPrivate, int value ; float myfloat ; ) ;

#define Self private_protocol_interface(TestClassPrivate,TestClass)

start_method( my_init_method, argfromlist(myfloat, float))

    printf("my init method: %f\n", myfloat) ;

    fds = new_record(TestClassFDS) ;

    get_fds(TestClassFDS)->value = 22 ;

    alloc_private_data_protocol(TestClassPrivate, TestClass) ;

    Self->value = 34 ;

    Self->myfloat = myfloat ;

end_method

start_method( my_deinit_method, )

    RKFloat myfloat = Self->myfloat ;

    printf("my deinit method: %f\n", myfloat) ;

    free(fds) ;

    printf("my private value: %d\n", Self->value) ;

    m(getobj(obj, mystring),print,noargs) ;

end_method

start_method( my_first_method, arg(myint, int) )

    printf("my first method: %d\n", myint) ;

end_method

start_method( my_private_method, arg(myint, int) )

    printf("my private method: %d\n", myint) ;

    printf("my private method fds: %d\n", get_fds(TestClassFDS)->value) ;

end_method

start_method( my_method, arg(myint,int) arg(myint2, int) )

    m(obj, my_first_method, ints(myint)) ;

    fm(obj, my_first_method, ints(myint2)) ;

    m(obj, my_private_method, ints(myint)) ;

    m(obj, my_private_method, ints(myint2)) ;

    StringClass string = new_object(StringClass, strings("My String!\n")) ;

    sos(obj, mystring, string) ;

end_method

start_method(print_hello_world_from_test,)

   m(obj, print_hello_world, noargs) ;

end_method

start_class_method( print_hello, item(TestObj, BaseClass) )

   m(TestObj, print_hello_world_from_test, noargs) ;

   cc(iclass, print_hi, noargs) ;

end_method

start_class_method( print_hi, )

 printf("Hi!!!!\n") ;

end_method

new_class(TestClass) {
    
    make_class_subclass_of(BaseClass) ;
    
    make_class_type_of(BaseClass) ;
    
    make_method_init(my_init_method) ;
    
    make_method_public(my_first_method) ;
    
    make_method_private(my_private_method) ;
    
    make_method_public(my_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_class_method_public(print_hello) ;
    
    make_class_method_public(print_hi) ;
    
    make_method_protected(print_hello_world_from_test) ;
}