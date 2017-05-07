//
//  StringClass.c
//  Fudge
//
//  Created by Jacob Gordon on 11/16/15.
//  Copyright © 2016 Jacob Gordon. All rights reserved.
//

#include <fudge.h>
#include <string.h>

use_class(BaseClass) ;

declare_class(StringClass) ;

define_data_protocol(StringData, RKLong size ; char* string ; ) ;

#define Self private_protocol_interface(StringData,StringClass)

start_method(my_init_method, argfromlist(string,const char*))

  alloc_private_data_protocol(StringData, StringClass) ;

  RKLong size = strlen(string) ;

  char* the_string = RKMem_CArray(size+1, char) ;

  strcpy(the_string, string) ;

  Self->string = the_string ;

  Self->size = size ;

end_method

start_method(my_deinit_method,)

  free(Self->string) ;

end_method

start_method(print_string,)

 printf("%s", Self->string) ;

end_method

start_method(get_size, arg(size, RKLong*))

 *size = Self->size ;

end_method

new_class(StringClass) {
    
    make_class_subclass_of(BaseClass) ;
    
    make_method_init(my_init_method) ;
    
    make_method_deinit(my_deinit_method) ;
    
    make_method_mask(print_string, print) ;
    
    make_method_mask(get_size, get the size of the string) ;
}