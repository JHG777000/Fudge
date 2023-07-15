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
#include <string.h>

use_class(BaseClass);
declare_class(StringClass);
define_data_protocol(StringData, vortex_ulong size; char* string;);

#define Self private_protocol_interface(StringData,StringClass)

start_method(my_init_method, argfromlist(string,const char*))
  alloc_private_data_protocol(StringData, StringClass);
  vortex_ulong size = strlen(string);
  char* the_string = vortex_c_array(size+1, char);
  strcpy(the_string, string);
  Self->string = the_string;
  Self->size = size;
end_method

start_method(my_deinit_method,)
  free(Self->string);
end_method

start_method(print_string,)
 printf("%s", Self->string);
end_method

start_method(get_size, arg(size, vortex_ulong*))
 *size = Self->size;
end_method

new_class(StringClass) {
    make_class_subclass_of(BaseClass);
    make_method_init(my_init_method);
    make_method_deinit(my_deinit_method);
    make_method_mask(print_string, print);
    make_method_mask(get_size, get the size of the string);
}