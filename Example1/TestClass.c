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

    fm(obj, my_private_method, ints(myint2)) ;

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
    
    make_method_init(my_init_method) ;
    
    make_method_public(my_first_method) ;
    
    make_method_private(my_private_method) ;
    
    make_method_public(my_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_class_method_public(print_hello) ;
    
    make_class_method_public(print_hi) ;
    
    make_method_protected(print_hello_world_from_test) ;
}