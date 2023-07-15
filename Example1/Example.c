/*
 Copyright (c) 2016-2023 Jacob Gordon. All rights reserved.

 Permission to redistribution and use this software in source and binary forms, with or without modification is hereby granted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <fudge.h>

use_class(TestClass);
use_class(StringClass);

void Example1( void ) {
    
    declare_cls(NULL);
    cm(TestClass, print_hi, noargs);
    TestClass myobject = new_object(TestClass, floats(10.0)); //new_object(Class, arguments...)
    m(myobject, my_first_method, ints(7)); //m is method invoke: m(object, method name, arguments...)
    m(myobject, my_method, ints(10, 33));
    vortex_long retval = m(myobject, print_hello_world_from_test, noargs); //protected method can not be accessed from here returns 0, method unresolved
    printf("%lld\n",retval);
    cm(TestClass, print_hello, myobject);
    free_object(myobject);
    StringClass string = new_object(StringClass, strings("This is my String!\n"));
    m(string,print,noargs);
    vortex_ulong size = 0;
    m(string, get the size of the string, args(vortex_ulong*,&size));
    printf("%lld\n",size);
    m(string, print size of the object on the stack in bytes, noargs);
    if (!m(string, hello world, noargs)) { //methods by default return 1, if a method is not found 0 is returned
        printf("hello world: not found! \n");
        if ( get_method(string, hello world) == get_null_method  ) 
         printf("hello world: not found! 2 \n"); //unresolved methods are equal to get_null_method 
    }
    free_object(string);
}

