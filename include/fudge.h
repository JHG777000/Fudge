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

#ifndef fudge_h
#define fudge_h

#include <stdio.h>
#include <stdlib.h>
#include <RKLib/RKMem.h>
#include <RKLib/RKArgs.h>

typedef struct fudge_class_s* fudge_class ;

typedef struct fudge_object_s* AnyClass ;

typedef AnyClass (*fudge_classdef)(int mode, fudge_class subclass) ;

typedef struct { RKArgs_Dynamic_Type_Protocol ; void* fast_data_structure ; } *fudge_fds_type ;

typedef RKLong (*fudge_method)(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) ;

typedef RKLong (*fudge_classmethod)(RKArgs external_arglist, const fudge_class iclass, RKArgs method_arglist) ;

typedef void (*fudge_class_init_and_deinit_type)(const fudge_class cls) ;

typedef fudge_class (*fudge_get_set_cls_type)(fudge_class* cls) ;

#define fudge_bitsizeof(type) sizeof(type) * CHAR_BIT

#define alloc_class(classname) fudge_classdef_Def##classname(2, NULL)

#define dealloc_class(classname) fudge_classdef_Def##classname(-1, NULL)

#define for_fudge_runtime_get_set_cls_for_class(classname,cls) classname##_fudge_get_set_cls( cls )

#define for_fudge_runtime_make_get_set_cls_for_class(classname) static fudge_class classname##_fudge_get_set_cls( fudge_class* cls ) {\
static fudge_class* the_cls = NULL ;\
 if ( (the_cls == NULL) && (cls != NULL) ) the_cls = cls ;\
 if ( (the_cls == NULL) && (cls == NULL) ) alloc_class(classname) ;\
 if (the_cls != NULL ) return *the_cls ;\
return NULL ;\
}

#define for_fudge_runtime_get_class_method_from_classname(classname,methodname,cls) fudge_get_class_method_from_class_##classname(#methodname,cls)

#define for_fudge_runtime_make_get_class_method_from_class(classname) fudge_method fudge_get_class_method_from_class_##classname( const char* methodname, fudge_class cls ) {\
return fudge_get_class_method(for_fudge_runtime_get_set_cls_for_class(classname,NULL),cls,methodname) ; \
}

#define for_fudge_runtime_is_object_of_class_with_classname(obj,classname) fudge_is_object_of_class_##classname((AnyClass)obj)

#define for_fudge_runtime_make_is_object_of_class_with_classname(classname) int fudge_is_object_of_class_##classname( AnyClass obj ) {\
if ( fudge_verify_object_is_of_class(obj,for_fudge_runtime_get_set_cls_for_class(classname,NULL)) ) return 1 ;\
return 0 ;\
}

#define for_fudge_runtime_call_class_method_from_classname(classname,method,external_arglist,...) fudge_call_class_method_from_class_##classname(method,external_arglist,__VA_ARGS__)

#define for_fudge_runtime_make_call_class_method_from_class(classname) RKLong fudge_call_class_method_from_class_##classname( fudge_classmethod method, RKArgs external_arglist, RKArgs method_arglist ) {\
start_arglist(method_arglist) ;\
return fudge_call_class_method(method,external_arglist,for_fudge_runtime_get_set_cls_for_class(classname,NULL),method_arglist) ;\
end_arglist(method_arglist) ;\
}

#define new_class(classname)\
static void classname##_fudge_class_constructor_method(const fudge_class cls) ;\
AnyClass fudge_classdef_Def##classname( int mode, fudge_class subclass ) {\
static fudge_class cls = NULL ;\
static int count = 0 ;\
 if (mode == -1) {\
  count-- ;\
  if ( count <= 0 ) {\
  count = 0 ;\
  fudge_class_dealloc(cls) ;\
  cls = NULL ;\
  }\
 }\
 if (mode == 0) {\
  count++ ;\
  if (cls == NULL){\
   cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
   classname##_fudge_class_constructor_method(cls) ;\
   for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
   fudge_invoke_classinit_method(cls) ;\
   }\
 return fudge_new_object(cls) ;\
 }\
 if (mode == 1) {\
  fudge_delete_object(fudge_classdef_Def##classname(0, NULL)) ;\
  classname##_fudge_class_constructor_method(subclass) ;\
  fudge_add_classinit_method(NULL,cls) ;\
  fudge_add_class_ref_to_subclass( cls, subclass ) ;\
  return NULL ;\
 }\
 if (mode == 2) {\
   if (cls == NULL){\
    cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
    classname##_fudge_class_constructor_method(cls) ;\
    for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
    fudge_invoke_classinit_method(cls) ;\
   }\
  return NULL ;\
 }\
return NULL ;\
}\
static void classname##_fudge_class_constructor_method(const fudge_class cls)

#define new_private_class(classname)\
static void classname##_fudge_class_constructor_method(const fudge_class cls) ;\
static AnyClass fudge_classdef_Def##classname( int mode, fudge_class subclass ) {\
static fudge_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
fudge_class_dealloc(cls) ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return fudge_new_object(cls) ;\
}\
if (mode == 1) {\
fudge_delete_object(fudge_classdef_Def##classname(0, NULL)) ;\
classname##_fudge_class_constructor_method(subclass) ;\
fudge_add_classinit_method(NULL,cls) ;\
fudge_add_class_ref_to_subclass( cls, subclass ) ;\
return NULL ;\
}\
if (mode == 2) {\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return NULL ;\
}\
return NULL ;\
}\
static void classname##_fudge_class_constructor_method(const fudge_class cls)

#define new_final_class(classname)\
static void classname##_fudge_class_constructor_method(const fudge_class cls) ;\
AnyClass fudge_classdef_Def##classname( int mode, fudge_class subclass ) {\
static fudge_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
fudge_class_dealloc(cls) ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return fudge_new_object(cls) ;\
}\
if (mode == 1) {\
return NULL ;\
}\
if (mode == 2) {\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return NULL ;\
}\
return NULL ;\
}\
static void classname##_fudge_class_constructor_method(const fudge_class cls)

#define new_abstract_class(classname)\
static void classname##_fudge_class_constructor_method(const fudge_class cls) ;\
AnyClass fudge_classdef_Def##classname( int mode, fudge_class subclass ) {\
static fudge_class cls = NULL ;\
static int count = 0 ;\
if (mode == -1) {\
count-- ;\
if ( count <= 0 ) {\
count = 0 ;\
fudge_class_dealloc(cls) ;\
cls = NULL ;\
}\
}\
if (mode == 0) {\
count++ ;\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return NULL ;\
}\
if (mode == 1) {\
fudge_classdef_Def##classname(0, NULL) ;\
classname##_fudge_class_constructor_method(subclass) ;\
fudge_add_classinit_method(NULL,cls) ;\
fudge_add_class_ref_to_subclass( cls, subclass ) ;\
return NULL ;\
}\
if (mode == 2) {\
if (cls == NULL){\
cls = fudge_class_alloc(fudge_classdef_Def##classname,#classname) ;\
classname##_fudge_class_constructor_method(cls) ;\
for_fudge_runtime_get_set_cls_for_class(classname,&cls) ;\
fudge_invoke_classinit_method(cls) ;\
}\
return NULL ;\
}\
return NULL ;\
}\
static void classname##_fudge_class_constructor_method(const fudge_class cls)


#define make_class_subclass_of(superclass) fudge_classdef_Def##superclass(1, cls)

#define get_cls_for(class) for_fudge_runtime_get_set_cls_for_class(class,NULL)

#define set_cls_for_method(class) cls = class

#define set_class_for_method(class) set_cls_for_method(get_cls_for(class))

#define use_class(classname) AnyClass fudge_classdef_Def##classname( int mode, fudge_class subclass ) ; \
fudge_method fudge_get_class_method_from_class_##classname( const char* methodname, fudge_class cls ) ;\
RKLong fudge_call_class_method_from_class_##classname( fudge_classmethod method, RKArgs external_arglist, RKArgs method_arglist ) ;\
int fudge_is_object_of_class_##classname( AnyClass obj ) ;\
typedef struct Obj##classname##_s* classname

#define use_class_type(classname) int fudge_is_object_of_class_##classname( AnyClass obj ) ;\
typedef struct Obj##classname##_s* classname

#define declare_class(classname) use_class(classname) ; \
for_fudge_runtime_make_get_set_cls_for_class(classname) \
for_fudge_runtime_make_get_class_method_from_class(classname)\
for_fudge_runtime_make_call_class_method_from_class(classname)\
for_fudge_runtime_make_is_object_of_class_with_classname(classname)

#define declare_private_class(classname) static use_class(classname) ; \
for_fudge_runtime_make_get_set_cls_for_class(classname) \
for_fudge_runtime_make_get_class_method_from_class(classname)\
for_fudge_runtime_make_call_class_method_from_class(classname)\
for_fudge_runtime_make_is_object_of_class_with_classname(classname)

#define declare_cls(class) fudge_class cls = class

#define make_method_public(method) fudge_add_method(method,#method,cls,0)

#define make_method_mask(method,methodname) fudge_add_method(method,#methodname,cls,0)

#define make_method_required(methodname) fudge_add_required_method(#methodname,cls,0)

#define make_method_private(method) fudge_add_method(method,#method,cls,1)

#define make_method_mask_private(method,methodname) fudge_add_method(method,#methodname,cls,1)

#define make_method_required_private(methodname) fudge_add_required_method(#methodname,cls,1)

#define make_method_protected(method) fudge_add_method(method,#method,cls,2)

#define make_method_mask_protected(method,methodname) fudge_add_method(method,#methodname,cls,2)

#define make_method_required_protected(methodname) fudge_add_required_method(#methodname,cls,2)

#define make_method_final(methodname) fudge_add_final_method(#methodname,cls)

#define make_method_init(method) fudge_add_init_method(method,cls)

#define make_method_deinit(method) fudge_add_deinit_method(method,cls)

#define make_method_final_init(method) fudge_add_final_init_method(method,cls)

#define make_method_final_deinit(method) fudge_add_final_deinit_method(method,cls)


#define make_class_method_public(method) fudge_add_class_method((fudge_method)method,#method,cls,0)

#define make_class_method_mask(method,methodname) fudge_add_class_method((fudge_method)method,#methodname,cls,0)

#define make_class_method_required(methodname) fudge_add_required_class_method(#methodname,cls,0)

#define make_class_method_private(method) fudge_add_class_method((fudge_method)method,#method,cls,1)

#define make_class_method_mask_private(method,methodname) fudge_add_class_method((fudge_method)method,#methodname,cls,1)

#define make_class_method_required_private(methodname) fudge_add_required_class_method(#methodname,cls,1)

#define make_class_method_protected(method) fudge_add_class_method((fudge_method)method,#method,cls,2)

#define make_class_method_mask_protected(method,methodname) fudge_add_class_method((fudge_method)method,#methodname,cls,2)

#define make_class_method_required_protected(methodname) fudge_add_required_class_method(#methodname,cls,2)

#define make_class_method_final(methodname) fudge_add_final_class_method(#methodname,cls)

#define make_method_class_init(method) fudge_add_classinit_method(method,cls)

#define make_method_class_deinit(method) fudge_add_classdeinit_method(method,cls)


#define new_object(class,...) (class)fudge_object_alloc(fudge_classdef_Def##class,RKArgs_NewArgs(__VA_ARGS__))

#define new_any_object(class,...) fudge_object_alloc(fudge_classdef_Def##class,RKArgs_NewArgs(__VA_ARGS__))

#define new_object_with_classdef(classdef,...) fudge_object_alloc(classdef,RKArgs_NewArgs(__VA_ARGS__))

#define free_object(obj) fudge_object_dealloc((AnyClass)obj)

#define get_classdef(class) fudge_classdef_Def##class

#define get_classdef_from_cls(cls) fudge_get_classdef_from_class(cls)


#define keep_object(object1,object2) fudge_store_object_public((AnyClass)object1,(AnyClass)object2,NULL,1)

#define ko(object1,object2) keep_object(object1,object2)

#define strong(obj1) ko(obj,obj1) 

#define store_object_strong(object1,name,object2) fudge_store_object_public((AnyClass)object1,(AnyClass)object2,#name,1)

#define store_object_weak(object1,name,object2) fudge_store_object_public((AnyClass)object1,(AnyClass)object2,#name,0)

#define sos(object1,name,object2) store_object_strong(object1,name,object2)

#define sow(object1,name,object2) store_object_weak(object1,name,object2)

#define get_object(object,name) fudge_get_object_public(object, #name)

#define getobj(object,name) get_object(object,name)


#define store_object_strong_private(object1,name,object2) fudge_store_object_private((AnyClass)object1,(AnyClass)object2,#name,1,cls)

#define store_object_weak_private(object1,name,object2) fudge_store_object_private((AnyClass)object1,(AnyClass)object2,#name,0,cls)

#define psos(object1,name,object2) store_object_strong_private(object1,name,object2)

#define psow(object1,name,object2) store_object_weak_private(object1,name,object2)

#define get_object_private(object,name) fudge_get_object_private(object, #name, cls)

#define gop(object,name) get_object_private(object,name)


#define store_object_strong_protected(object1,name,object2) fudge_store_object_protected((AnyClass)object1,(AnyClass)object2,#name,1,cls)

#define store_object_weak_protected(object1,name,object2) fudge_store_object_protected((AnyClass)object1,(AnyClass)object2,#name,0,cls)

#define prsos(object1,name,object2) store_object_strong_protected(object1,name,object2)

#define prsow(object1,name,object2) store_object_weak_protected(object1,name,object2)

#define get_object_protected(object,name) fudge_get_object_protected(object, #name, cls)

#define gopr(object,name) get_object_protected(object,name)


#define add_ref_count(object) fudge_add_ref_count((AnyClass)object)

#define sub_ref_count(object) fudge_sub_ref_count((AnyClass)object)


#define arg(name,type) type name ; if (method_arglist != NULL) {\
name = RKArgs_GetNextArg(method_arglist,type) ;\
} else { return -1 ;}

#define item(name,type) type name ; if (method_arglist != NULL) {\
name = RKArgs_GetNextItem(method_arglist,type) ;\
if (name == NULL) abort();\
} else { return -1 ;}

#define argfromlist(name,type) type name ; if (external_arglist != NULL) {\
name = RKArgs_GetNextArg(external_arglist,type) ;\
} else { return -1 ;}

#define itemfromlist(name,type) type name ; if (external_arglist != NULL) {\
name = RKArgs_GetNextItem(external_arglist,type) ;\
if (name == NULL) abort();\
} else { return -1 ;}

#define noargs ints(0)

#define define_class_init_method(method_name) static void method_name(fudge_class cls)

#define define_class_deinit_method(method_name) static void method_name(fudge_class cls)

#define is_object_of_class_with_cls(obj,cls) fudge_verify_object_is_of_class((AnyClass)obj,cls)

#define is_object_of_class_with_classname(obj,classname) for_fudge_runtime_is_object_of_class_with_classname(obj,classname)

#define start_method(method_name,method_args) static RKLong method_name(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) {\
fudge_class cls = iclass ;\
if (cls == NULL) return -1 ;\
if (!is_object_of_class_with_cls(obj,cls)) return -1 ;\
method_args\

// static method equals non static function
#define start_static_method(method_name,method_args) RKLong method_name(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) {\
fudge_class cls = iclass ;\
if (cls != NULL) return -1 ;\
if (obj != NULL) return -1 ; \
method_args\

#define start_class_method(method_name,method_args) static RKLong method_name(RKArgs external_arglist, const fudge_class iclass, RKArgs method_arglist) {\
fudge_class cls = iclass ;\
if (cls == NULL) return -1 ;\
method_args\

#define end_method return 1 ; }

#define start_arglist(arglist) RKArgs_UseArgs(arglist)

#define end_arglist(arglist) RKArgs_UseArgs(arglist)


#define get_null_method fudge_default_func

#define method_runtime_error -1

#define return_error_from_method return -2

#define method_returned_error -2

#define return_from_method return 1

#define get_method(obj,method) fudge_get_method((AnyClass)obj,#method,cls)

#define get_class_method(class,method) fudge_get_class_method(class,cls,#method)

#define get_class_method_from_classname(classname,methodname,cls) for_fudge_runtime_get_class_method_from_classname(classname,methodname,cls)

#define call_class_method_from_classname(classname,method,external_arglist,...) for_fudge_runtime_call_class_method_from_classname(classname,method,external_arglist,RKArgs_NewArgs(__VA_ARGS__))

#define method_invoke(obj,method,...) fudge_call_method(fudge_get_method((AnyClass)obj,#method,cls),NULL,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))
#define m(obj,method,...) method_invoke(obj,method,__VA_ARGS__)

#define method_invoke_with_arglist(obj,method,arglist,...) fudge_call_method(fudge_get_method((AnyClass)obj,#method,cls),arglist,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))
#define ma(obj,method,arglist,...) method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)

#define fast_method_invoke(obj,method,...) fudge_call_method(method,NULL,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))
#define fm(obj,method,...) fast_method_invoke(obj,method,__VA_ARGS__)

#define fast_method_invoke_with_arglist(obj,method,arglist,...) fudge_call_method(method,arglist,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))
#define fma(obj,method,arglist,...) fast_method_invoke_with_arglist(obj,method,arglist,__VA_ARGS__)

#define static_method_invoke(method,...) RKLong method(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) ;\
fm(NULL,method,__VA_ARGS__)
#define sm(method,...) static_method_invoke(method,__VA_ARGS__)

#define static_method_invoke_with_arglist(method,arglist,...) RKLong method(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) ;\
fma(NULL,method,arglist,__VA_ARGS__)
#define sma(method,...) static_method_invoke_with_arglist(method,__VA_ARGS__)

#define class_method_invoke(classname,method,...)\
call_class_method_from_classname(classname,((fudge_classmethod)get_class_method_from_classname(classname,method,cls)),NULL,__VA_ARGS__)
#define cm(classname,method,...) class_method_invoke(classname,method,__VA_ARGS__)

#define class_method_invoke_with_arglist(classname,method,arglist,...)\
call_class_method_from_classname(classname,((fudge_classmethod)get_class_method_from_classname(classname,method,cls)),arglist,__VA_ARGS__)
#define cma(classname,method,arglist,...) class_method_invoke_with_arglist(classname,method,arglist,__VA_ARGS__)

#define fast_class_method_invoke(classname,method,...) call_class_method_from_classname(classname,((fudge_classmethod)method),NULL,__VA_ARGS__)
#define fcm(classname,method,...) fast_class_method_invoke(classname,method,__VA_ARGS__)

#define fast_class_method_invoke_with_arglist(classname,method,arglist,...) call_class_method_from_classname(classname,((fudge_classmethod)method),arglist,__VA_ARGS__)
#define fcma(classname,method,arglist,...) fast_class_method_invoke_with_arglist(method,arglist,__VA_ARGS__)

#define call_class(class,method,...)\
fudge_call_class_method_with_cls((fudge_classmethod)get_class_method(class,method),NULL,class,RKArgs_NewArgs(__VA_ARGS__))
#define cc(class,method,...) call_class(class,method,__VA_ARGS__)

#define call_class_with_arglist(class,method,arglist,...)\
fudge_call_class_method_with_cls((fudge_classmethod)get_class_method(class,method),arglist,class,RKArgs_NewArgs(__VA_ARGS__))
#define cca(class,method,arglist,...) call_class_with_arglist(class,method,arglist,__VA_ARGS__)


#define fast_data_store ((fudge_fds_type)obj)->fast_data_structure

#define fast_data_store_class ((fudge_fds_type)cls)->fast_data_structure

#define fast_data_store_of(entity) ((fudge_fds_type)entity)->fast_data_structure

#define get_fast_data_store(type) ((type)fast_data_store)

#define get_fast_data_store_class(type) ((type)fast_data_store_class)

#define get_fast_data_store_of(entity,type) ((type)fast_data_store_of(entity))

#define fds fast_data_store

#define fds_class fast_data_store_class

#define fds_of(entity) fast_data_store_of(entity)

#define get_fds(type) get_fast_data_store(type)

#define get_fds_class(type) get_fast_data_store_class(type)

#define get_fds_of(entity,type) get_fast_data_store_of(entity,type)


#define define_record_type(recordtype,...) typedef struct recordtype##_s { __VA_ARGS__ } *recordtype

#define new_record(recordtype) RKMem_NewMemOfType(struct recordtype##_s)



#define GetPublicFor(object,protocol) fudge_get_data_from_object(#protocol,object,NULL,NULL,0)

#define GetPublic(protocol) GetPublicFor(obj,protocol)


#define GetPrivateFor(object,protocol,classname) fudge_get_data_from_object(#protocol,object,NULL,classname##_fudge_get_set_cls,1)

#define GetPrivate(protocol,classname) GetPrivateFor(obj,protocol,classname)


#define GetProtectedFor(object,protocol) fudge_get_data_from_object(#protocol,object,cls,NULL,2)

#define GetProtected(protocol) GetProtectedFor(obj,protocol)


#define GetClassData(protocol) fudge_get_data_from_class(#protocol,cls)


#define public_protocol_interface(protocol) ((protocol##protocol_type) GetPublic(protocol))

#define public_protocol_interface_of(object,protocol) ((protocol##protocol_type) GetPublicFor(object,protocol))


#define private_protocol_interface(protocol,classname) ((protocol##protocol_type) GetPrivate(protocol,classname))

#define private_protocol_interface_of(object,protocol,classname) ((protocol##protocol_type) GetPrivateFor(object,protocol,classname))


#define protected_protocol_interface(protocol) ((protocol##protocol_type) GetProtected(protocol))

#define protected_protocol_interface_of(object,protocol) ((protocol##protocol_type) GetProtectedFor(object,protocol))


#define class_protocol_interface(protocol) (protocol##protocol_type) GetClassData(protocol))


#define define_data_protocol(protocol,...) define_record_type(protocol##protocol_type,__VA_ARGS__)

#define alloc_public_data_protocol(protocol) fudge_set_data_for_object(#protocol,new_record(protocol##protocol_type),obj,NULL,NULL,0)

#define alloc_private_data_protocol(protocol,classname) fudge_set_data_for_object(#protocol,new_record(protocol##protocol_type),obj,NULL,classname##_fudge_get_set_cls,1)

#define alloc_protected_data_protocol(protocol) fudge_set_data_for_object(#protocol,new_record(protocol##protocol_type),obj,cls,NULL,2)

#define alloc_class_data_protocol(protocol) fudge_set_data_for_class(#protocol,new_record(protocol##protocol_type),cls)


#define send_object_msg(obj,msg,...) fudge_call_method(fudge_get_method((AnyClass)obj,msg,cls),NULL,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))

#define send_object_msg_with_arglist(obj,msg,arglist,...) fudge_call_method(fudge_get_method((AnyClass)obj,msg,cls),arglist,(AnyClass)obj,RKArgs_NewArgs(__VA_ARGS__))

#define som(obj,msg,...) send_object_msg(obj,msg,__VA_ARGS__)

#define soma(obj,msg,arglist,...) send_object_msg_with_arglist(obj,msg,arglist,__VA_ARGS__)

#define store_object_with_msg(object1,msg,object2) fudge_store_object((AnyClass)object1,(AnyClass)object2,msg,1)

#define store_weak_object_with_msg(object1,msg,object2) fudge_store_object((AnyClass)object1,(AnyClass)object2,msg,0)

#define get_object_with_msg(object, msg) fudge_get_object((AnyClass)object, msg)

#define make_class_type_of(type) fudge_add_typestring_to_class(((fudge_class)((type)cls)),#type)


RKLong fudge_default_func(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) ;

AnyClass fudge_object_alloc( fudge_classdef the_classdef, RKArgs arglist ) ;

AnyClass fudge_new_object( fudge_class cls ) ;

void fudge_delete_object( AnyClass obj ) ;

void fudge_object_dealloc( AnyClass obj ) ;

AnyClass fudge_store_object_public( AnyClass obj1, AnyClass obj2, const char* name, int strong ) ;

AnyClass fudge_store_object_private( AnyClass obj1, AnyClass obj2, const char* name, int strong, fudge_class cls ) ;

AnyClass fudge_store_object_protected( AnyClass obj1, AnyClass obj2, const char* name, int strong, fudge_class cls ) ;

AnyClass fudge_get_object_public( AnyClass obj, const char* name ) ;

AnyClass fudge_get_object_private( AnyClass obj, const char* name, fudge_class access_class ) ;

AnyClass fudge_get_object_protected( AnyClass obj, const char* name, fudge_class access_class ) ;

void fudge_add_ref_count( AnyClass obj ) ;

void fudge_sub_ref_count( AnyClass obj ) ;

fudge_class fudge_class_alloc( fudge_classdef the_classdef, const char* classname ) ;

void fudge_class_dealloc( fudge_class cls ) ;

void fudge_add_class_ref_to_subclass( fudge_class cls, fudge_class subclass ) ;

void fudge_invoke_classinit_method( fudge_class cls ) ;

void fudge_add_classinit_method( fudge_class_init_and_deinit_type classinit, fudge_class cls ) ;

void fudge_add_classdeinit_method( fudge_class_init_and_deinit_type classdeinit, fudge_class cls ) ;

void fudge_add_init_method( fudge_method method, fudge_class cls ) ;

void fudge_add_deinit_method( fudge_method method, fudge_class cls ) ;

void fudge_add_final_init_method( fudge_method method, fudge_class cls ) ;

void fudge_add_final_deinit_method( fudge_method method, fudge_class cls ) ;

void fudge_add_required_method( const char* name, fudge_class cls, int mode ) ;

void fudge_add_method( fudge_method method, const char* name, fudge_class cls, int mode ) ;

fudge_method fudge_get_method( AnyClass obj, const char* name, fudge_class access_class) ;

void fudge_add_final_method( const char* name, fudge_class cls ) ;

void fudge_add_required_class_method( const char* name, fudge_class cls, int mode ) ;

void fudge_add_class_method(fudge_method method, const char* name, fudge_class cls, int mode ) ;

int fudge_is_class_superclass_of_class( fudge_class superclass, fudge_class subclass ) ;

fudge_method fudge_get_class_method( fudge_class cls, fudge_class access_class, const char* name ) ;

void fudge_add_final_class_method( const char* name, fudge_class cls ) ;

fudge_classdef fudge_get_classdef_from_class( fudge_class cls ) ;

int fudge_verify_object_is_of_class( AnyClass obj, fudge_class cls ) ;

void* fudge_get_data_from_object( const char* protocol_name, AnyClass obj, fudge_class cls, fudge_get_set_cls_type get_set_cls, int mode ) ;

void fudge_set_data_for_object( const char* protocol_name, void* ref, AnyClass obj, fudge_class cls, fudge_get_set_cls_type get_set_cls, int mode ) ;

void* fudge_get_data_from_class( const char* protocol_name, fudge_class cls ) ;

void fudge_set_data_for_class( const char* protocol_name, void* ref, fudge_class cls ) ;

RKLong fudge_call_method( fudge_method method, RKArgs external_arglist, const AnyClass obj, RKArgs method_arglist ) ;

RKLong fudge_call_class_method( fudge_classmethod method, RKArgs external_arglist, fudge_class cls, RKArgs method_arglist ) ;

RKLong fudge_call_class_method_with_cls( fudge_classmethod method, RKArgs external_arglist, fudge_class cls, RKArgs method_arglist ) ;

void fudge_add_typestring_to_class( fudge_class cls, const char* typestring ) ;

#endif /* fudge_h */
