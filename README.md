# Fudge

Fudge is a framework that enables dynamic and explicit object-oriented programming in C.

### Features:

* Dynamic dispatch, and late binding for methods, references(other objects), and data(pointers).

* Support for private methods.

* Support for final methods, make a method immutable(can not be overridden).

* Support for static methods, methods that need no object instance.

* Support for class methods, methods that are called on the class.

* Support for protected methods, protected methods that can only be accessed by a subclass.

* Support for data to be held in a class.

* Classes can be altered durring runtime(adding methods, data to a class) via the class pointer.

* Support for subclasses(multiple inheritance).

* Support for private classes.

* Support for manual memory management, manual reference counting and, run-time automatic reference counting.

### Example:

     #include <fudge.h>

     use_class(TestClass) ;

     use_class(StringClass) ;

    void example( void ) {
    
     declare_cls(NULL) ;
    
     cm(TestClass, print_hi, noargs) ;
    
     TestClass myobject = new_object(TestClass, floats(10.0)) ; //new_object(Class, arguments...)
    
     m(myobject, my_first_method, ints(7)) ; 
     //m is method invoke: m(object, method name, arguments...)
    
     m(myobject, my_method, ints(10, 33)) ;
    
     RKLong retval = m(myobject, print_hello_world_from_test, noargs) ; 
     //protected method can not be accessed from here returns 0, method unresolved
    
     printf("%ld\n",retval) ;
    
     cm(TestClass, print_hello, myobject) ;
    
     free_object(myobject) ;
    
     StringClass string = new_object(StringClass, strings("This is my String!\n")) ;
    
     m(string,print,noargs) ;
     
     RKLong size = 0 ;
    
     m(string, get the size of the string, args(RKLong*,&size)) ;
    
     printf("%ld\n",size) ;
    
     m(string, print size of the object on the stack in bytes, noargs) ;
    
     if (!m(string, hello world, noargs)) { 
     //methods by default return 1, if a method is not found 0 is returned
        
         printf("hello world: not found! \n") ;
        
         if ( get_method(string, hello world) == get_null_method  ) 
           printf("hello world: not found! 2 \n") ; 
          //unresolved methods are equal to get_null_method
        
     }
    
     free_object(string) ;

    }


#### Dependencies: 

 -RKLib https://github.com/JHG777000/RKLib