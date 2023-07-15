/*
 Copyright (c) 2016-2023 Jacob Gordon. All rights reserved.

 Permission to redistribution and use this software in source and binary forms, with or without modification is hereby granted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <string.h>
#include <fudge.h>

struct fudge_class_s {
  VortexArgs_Dynamic_Type_Protocol;
  VortexAny fast_data_structure;
  VortexAny fudge_type_array;
  fudge_class_init_and_deinit_type classinit;
  fudge_class_init_and_deinit_type classdeinit;
  fudge_method init;
  fudge_method deinit;
  fudge_method final_init;
  fudge_method final_deinit;
  VortexStore methods;
  VortexStore final_methods;
  VortexStore class_methods;
  VortexStore final_class_methods;
  VortexStore data;
  fudge_classdef the_classdef;
  VortexList superclass_refs;
};

struct fudge_object_s {
  VortexArgs_Dynamic_Type_Protocol;
  VortexAny fast_data_structure;
  fudge_class class_of_object;
  VortexStore public_data;
  VortexStore private_data;
  VortexStore protected_data;
  VortexStore public_refs;
  VortexStore private_refs;
  VortexStore protected_refs; 
  vortex_ulong refcount;
};

struct fudge_ref_s { 
  vortex_int strong;
  AnyClass obj;
  AnyClass base_obj;
  fudge_class class_of_origin;
};

typedef struct fudge_ref_s* fudge_ref;

typedef struct fudge_data_ref_s {
  VortexAny data;
  fudge_class class_of_origin;
}* fudge_data_ref ;

typedef struct fudge_method_ref_s {
  fudge_method method;
  vortex_int mode; 
  fudge_class class_of_origin; 
}* fudge_method_ref;

 vortex_long fudge_default_func(
     VortexArgs external_arglist,
      const AnyClass obj,
       const fudge_class iclass,
        VortexArgs method_arglist) {
    return 0 ;
}

void fudge_default_class_init_and_deinit(const fudge_class cls) {
}

AnyClass fudge_object_alloc(fudge_classdef the_classdef, VortexArgs arglist) {
    AnyClass obj = the_classdef(0, NULL);
    obj->verify = 12345423;
    obj->refcount = 0;
    obj->public_refs = VortexStore_New();
    obj->private_refs = VortexStore_New();
    obj->protected_refs = VortexStore_New();
    obj->public_data = VortexStore_New();
    obj->private_data = VortexStore_New();
    obj->protected_data = VortexStore_New();
    start_arglist(arglist);
    fudge_method init = 
     (obj->class_of_object->final_init != get_null_method) 
      ? obj->class_of_object->final_init : obj->class_of_object->init;
    fudge_call_method(init, arglist, (AnyClass)obj, noargs);
    end_arglist(arglist);
    return obj ;
}

AnyClass fudge_new_object(fudge_class cls) {
    AnyClass obj = vortex_new_mem_of_type(struct fudge_object_s);
    obj->class_of_object = cls;
    obj->base_type = VortexArgs_Dynamic_type;
    obj->type_array = cls->fudge_type_array;
    return obj;
}

void fudge_delete_object(AnyClass obj) {  
    free(obj);
}

static void fudge_dealloc_refs(VortexAny data) {
    fudge_ref ref = (fudge_ref)data;
    if ( (ref->strong) && (ref->obj != ref->base_obj) ) sub_ref_count(ref->obj);
    free(ref);
}

static void fudge_dealloc_data_store(VortexAny data) {
    fudge_data_ref ref = data;
    free(ref->data);
    free(ref);
}

void fudge_object_dealloc(AnyClass obj) {
    fudge_method deinit = 
     (obj->class_of_object->final_deinit != get_null_method)
      ? obj->class_of_object->final_deinit : obj->class_of_object->deinit;
    fudge_call_method(deinit, NULL, (AnyClass)obj, noargs);
    obj->class_of_object->the_classdef(-1, NULL);
    VortexStore_IterateWith(fudge_dealloc_refs, obj->public_refs);
    VortexStore_IterateWith(fudge_dealloc_refs, obj->private_refs);
    VortexStore_IterateWith(fudge_dealloc_refs, obj->protected_refs);
    VortexStore_IterateWith(fudge_dealloc_data_store, obj->public_data);
    VortexStore_IterateWith(fudge_dealloc_data_store, obj->private_data);
    VortexStore_IterateWith(fudge_dealloc_data_store, obj->protected_data);
    VortexStore_Destroy(obj->public_refs);
    VortexStore_Destroy(obj->private_refs);
    VortexStore_Destroy(obj->protected_refs);
    VortexStore_Destroy(obj->public_data);
    VortexStore_Destroy(obj->private_data);
    VortexStore_Destroy(obj->protected_data);
    free(obj) ;
}

AnyClass fudge_store_object_public(
    AnyClass obj1,
     AnyClass obj2,
     const char* name,
      vortex_int strong) {
    if ( name != NULL ) {
      if ( VortexStore_ItemExists(obj1->public_refs, name) ) {
          fudge_ref ref = VortexStore_GetItem(obj1->public_refs, name);
          if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) 
            sub_ref_count(ref->obj);
          free(ref);
      }
        
    }
    fudge_ref ref = vortex_new_mem_of_type(struct fudge_ref_s);
    ref->base_obj = obj1;
    ref->strong = strong;
    ref->obj = obj2;
    ref->class_of_origin = NULL;
    if ( (ref->strong) && (obj1 != obj2) ) add_ref_count(ref->obj);
    if ( name == NULL ) {
        VortexStore_AddItemToList(obj1->public_refs, ref);
        return obj2;
    }
    VortexStore_AddItem(obj1->public_refs, ref, name);
    return obj2 ;
}

AnyClass fudge_store_object_private(
    AnyClass obj1,
     AnyClass obj2,
      const char* name,
       vortex_int strong,
        fudge_class cls) {
            
    if ( name == NULL ) return obj2;
        if ( VortexStore_ItemExists(obj1->private_refs, name) ) {
            fudge_ref ref = VortexStore_GetItem(obj1->private_refs, name);
            if (ref->class_of_origin != cls) return obj2;
            if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) 
              sub_ref_count(ref->obj);
            free(ref);
        }
    fudge_ref ref = vortex_new_mem_of_type(struct fudge_ref_s);
    ref->base_obj = obj1;
    ref->strong = strong;
    ref->obj = obj2;
    ref->class_of_origin = cls;
    if ( (ref->strong) && (obj1 != obj2) ) 
      add_ref_count(ref->obj);
    VortexStore_AddItem(obj1->private_refs, ref, name);
    return obj2;
}

AnyClass fudge_store_object_protected(
    AnyClass obj1,
     AnyClass obj2,
      const char* name,
       vortex_int strong,
        fudge_class cls) {
            
    if (obj1->class_of_object != cls) return obj2;
    if ( name == NULL ) return obj2;
        if ( VortexStore_ItemExists(obj1->public_refs, name) ) {
            fudge_ref ref = VortexStore_GetItem(obj1->protected_refs, name);
            if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) 
              sub_ref_count(ref->obj);
            free(ref);
        }
    fudge_ref ref = vortex_new_mem_of_type(struct fudge_ref_s);
    ref->base_obj = obj1;
    ref->strong = strong;
    ref->obj = obj2;
    ref->class_of_origin = NULL;
    if ( (ref->strong) && (obj1 != obj2) ) 
      add_ref_count(ref->obj);
    VortexStore_AddItem(obj1->protected_refs, ref, name);
    return obj2;
}

AnyClass fudge_get_object_public(AnyClass obj, const char* name) {
    fudge_ref ref = VortexStore_GetItem(obj->public_refs, name);
    if ( ref == NULL ) return NULL;
    return ref->obj;
}

AnyClass fudge_get_object_private(
    AnyClass obj,
     const char* name,
      fudge_class access_class) {
    
    fudge_ref ref = VortexStore_GetItem(obj->private_refs, name);
    if ( ref == NULL ) return NULL;
    if ( ref->class_of_origin != access_class ) return NULL;
    return ref->obj;
}

AnyClass fudge_get_object_protected(
    AnyClass obj,
     const char* name,
      fudge_class access_class) {
    
    if ( obj->class_of_object != access_class ) return NULL;
    fudge_ref ref = VortexStore_GetItem(obj->protected_refs, name);
    if ( ref == NULL ) return NULL;
    return ref->obj;
}

void fudge_add_ref_count(AnyClass obj) {
    if ( obj != NULL ) obj->refcount++;
}

void fudge_sub_ref_count(AnyClass obj) {
    if ( obj != NULL ) obj->refcount--;
    if ( obj != NULL ) if ( obj->refcount <= 0 ) 
      free_object(obj);
}

fudge_class fudge_class_alloc(
    fudge_classdef the_classdef,
     const char* classname) {
    
    fudge_class cls = vortex_new_mem_of_type(struct fudge_class_s);
    cls->verify = 12345423;
    cls->base_type = VortexArgs_Dynamic_type;
    cls->type_array = vortex_c_array(2, VortexAny);
    const char* fudgeclass = "fudge_class";
    vortex_ulong size = strlen(fudgeclass);
    char* the_string = vortex_c_array(size+1, char);
    strcpy(the_string, fudgeclass);
    ((VortexAny*)cls->type_array)[0] = the_string;
    ((VortexAny*)cls->type_array)[1] = NULL;
    cls->fudge_type_array = vortex_c_array(3, VortexAny) ;
    vortex_ulong size2 = strlen(classname);
    char* the_string2 = vortex_c_array(size2+1, char);
    strcpy(the_string2, classname);
    const char* anyclass = "AnyClass";
    vortex_ulong size3 = strlen(anyclass);
    char* the_string3 = vortex_c_array(size3+1, char);
    strcpy(the_string3, anyclass);
    ((VortexAny*)cls->fudge_type_array)[0] = the_string2;
    ((VortexAny*)cls->fudge_type_array)[1] = the_string3;
    ((VortexAny*)cls->fudge_type_array)[2] = NULL;
    cls->final_init = fudge_default_func;
    cls->final_deinit = fudge_default_func;
    cls->init = fudge_default_func;
    cls->deinit = fudge_default_func;
    cls->classinit = fudge_default_class_init_and_deinit;
    cls->classdeinit = fudge_default_class_init_and_deinit;
    cls->final_class_methods = VortexStore_New();
    cls->class_methods = VortexStore_New();
    cls->final_methods = VortexStore_New();
    cls->methods = VortexStore_New();
    cls->data = VortexStore_New();
    cls->the_classdef = the_classdef;
    cls->superclass_refs = VortexList_New();
    return cls;
}

static void fudge_dealloc_method_refs(VortexAny data) {
    fudge_method_ref ref = data;
    free(ref);
}

static void fudge_dealloc_superclass_refs(VortexAny data) {
    fudge_class cls = (fudge_class)data;
    cls->the_classdef(-1, NULL);
}

void fudge_class_dealloc(fudge_class cls) {
    cls->classdeinit(cls);
    int i = 0;
    while (((VortexAny*)cls->type_array)[i] != NULL) {
        free(((VortexAny*)cls->type_array)[i]);
        i++;
    }
    free(cls->type_array);
    i = 0;
    while (((VortexAny*)cls->fudge_type_array)[i] != NULL) {
        free(((VortexAny*)cls->fudge_type_array)[i]);
        i++;
    }
    free(cls->fudge_type_array);
    VortexStore_Destroy(cls->final_class_methods);
    VortexStore_IterateWith(fudge_dealloc_method_refs, cls->class_methods) ;
    VortexStore_Destroy(cls->class_methods);
    VortexStore_Destroy(cls->final_methods);
    VortexStore_IterateWith(fudge_dealloc_method_refs, cls->methods);
    VortexStore_Destroy(cls->methods);
    VortexStore_Destroy(cls->data);
    VortexList_IterateWith(fudge_dealloc_superclass_refs, cls->superclass_refs);
    VortexList_Destroy(cls->superclass_refs);
    free(cls);
}

void fudge_add_class_ref_to_subclass(fudge_class cls, fudge_class subclass) {
    VortexList_AddToList(subclass->superclass_refs, cls);
}

void fudge_invoke_classinit_method(fudge_class cls) {
    cls->classinit(cls) ;
}

void fudge_add_classinit_method(
    fudge_class_init_and_deinit_type classinit,
     fudge_class cls) {
    
    if ( classinit == NULL ) classinit = 
      fudge_default_class_init_and_deinit;
    cls->classinit = classinit;
}

void fudge_add_classdeinit_method(
    fudge_class_init_and_deinit_type classdeinit,
     fudge_class cls) {
    
    if ( classdeinit == NULL ) 
     classdeinit = 
      fudge_default_class_init_and_deinit;
    cls->classdeinit = classdeinit;
}

void fudge_add_init_method(
    fudge_method method,
     fudge_class cls) {
    
    if ( cls->final_init == get_null_method ) 
      cls->init = method;
}

void fudge_add_deinit_method(
    fudge_method method,
     fudge_class cls) {
    
    if ( cls->final_deinit == get_null_method ) 
      cls->deinit = method;
}

void fudge_add_final_init_method(
    fudge_method method,
     fudge_class cls) {
         
    if ( cls->final_init == get_null_method )
     cls->final_init = method;
}

void fudge_add_final_deinit_method(
    fudge_method method,
     fudge_class cls) {
         
    if ( cls->final_deinit == get_null_method )
     cls->final_deinit = method;
}

void fudge_add_required_method(
    const char* name,
     fudge_class cls,
      vortex_int mode) {
    fudge_method method = NULL;
    
    if ( !(VortexStore_ItemExists(cls->final_methods, name)) )  {     
      fudge_method_ref ref = vortex_new_mem_of_type(struct fudge_method_ref_s);
      ref->class_of_origin = cls; 
      ref->method = method;
      ref->mode = mode;
      if ( (VortexStore_ItemExists(cls->methods, name)) ) 
       free(VortexStore_GetItem(cls->methods, name));
      VortexStore_AddItem(cls->methods, ref, name);
        
    }
}

void fudge_add_method(
   fudge_method method, 
    const char* name, 
    fudge_class cls,
     vortex_int mode) {
    
     if ( method == NULL ) return;
     if ( !(VortexStore_ItemExists(cls->final_methods, name)) )  {
      fudge_method_ref ref = vortex_new_mem_of_type(struct fudge_method_ref_s);
      ref->class_of_origin = cls;
      ref->method = method;
      ref->mode = mode;

      if ( (VortexStore_ItemExists(cls->methods, name)) ) 
       free(VortexStore_GetItem(cls->methods, name));
      VortexStore_AddItem(cls->methods, ref, name);
    }
}

fudge_method fudge_get_method(
    AnyClass obj,
     const char* name,
      fudge_class access_class) {
    
    fudge_method_ref ref = NULL;
    ref = VortexStore_GetItem(obj->class_of_object->methods, name);
    if ( ref == NULL ) return fudge_default_func;
    switch (ref->mode) {
        case 0:
            return ref->method;
        break;
        case 1:
            if ( ref->class_of_origin == access_class ) return ref->method;
        break;
        case 2:
            if ( obj->class_of_object == access_class ) return ref->method;
        break;
        default:
            break;
    }
    return fudge_default_func;
}

void fudge_add_final_method( const char* name, fudge_class cls ) {
    VortexStore_AddItem(cls->final_methods, NULL, name);
}

void fudge_add_required_class_method(
    const char* name,
     fudge_class cls,
      vortex_int mode ) {
    
    fudge_method method = NULL;
    if ( !(VortexStore_ItemExists(cls->final_class_methods, name)) )  {
        fudge_method_ref ref = vortex_new_mem_of_type(struct fudge_method_ref_s);
        ref->class_of_origin = cls;
        ref->method = method;
        ref->mode = mode;
        if ( (VortexStore_ItemExists(cls->class_methods, name)) )
         free(VortexStore_GetItem(cls->class_methods, name));
        VortexStore_AddItem(cls->class_methods, ref, name);
    }
}

void fudge_add_class_method(
    fudge_method method,
     const char* name,
      fudge_class cls,
       vortex_int mode ) {
    
    if ( method == NULL ) return;
    if ( !(VortexStore_ItemExists(cls->final_class_methods, name)) )  {
        fudge_method_ref ref = vortex_new_mem_of_type(struct fudge_method_ref_s);
        ref->class_of_origin = cls;
        ref->method = method;
        ref->mode = mode;
        if ( (VortexStore_ItemExists(cls->class_methods, name)) ) 
         free(VortexStore_GetItem(cls->class_methods, name));
        VortexStore_AddItem(cls->class_methods, ref, name);
    }
}

vortex_int fudge_is_class_superclass_of_class(
    fudge_class superclass,
     fudge_class subclass) {
    
    VortexListNode node = VortexList_GetFirstNode(subclass->superclass_refs);
    while ( node != NULL ) {
        if ( VortexList_GetData(node) == superclass ) return 1;
        node = VortexList_GetNextNode(node);
    }
    return 0;
}

fudge_method fudge_get_class_method(
    fudge_class cls,
     fudge_class access_class,
      const char* name) {
    
    fudge_method_ref ref = NULL;
    ref = VortexStore_GetItem(cls->class_methods, name);
    if ( ref == NULL ) return fudge_default_func;
    switch (ref->mode) {
        case 0:
            return ref->method;
            break;
        case 1:
            if ( ref->class_of_origin == access_class ) 
             return ref->method;
            break;
        case 2:
            if ( (fudge_is_class_superclass_of_class(ref->class_of_origin, access_class)) 
                || (ref->class_of_origin == access_class) ) return ref->method;
            break;
        default:
            break;
    }
    return fudge_default_func;
}

void fudge_add_final_class_method(const char* name, fudge_class cls) {
    VortexStore_AddItem(cls->final_class_methods, NULL, name);
}

fudge_classdef obj_get_classdef_from_class(fudge_class cls) {
    return cls->the_classdef;
}

vortex_int fudge_verify_object_is_of_class(
    AnyClass obj,
     fudge_class cls) {
    
    if ( obj->class_of_object == cls ) return 1;
    return 0;
}

VortexAny fudge_get_data_from_object(
    const char* protocol_name,
     AnyClass obj,
      fudge_class cls,
       fudge_get_set_cls_type get_set_cls,
        vortex_int mode) {
            
    fudge_data_ref data_ref = NULL;
    switch (mode) {
        case 0:
                data_ref = VortexStore_GetItem(obj->public_data, protocol_name);
            break;
        case 1:
                data_ref = VortexStore_GetItem(obj->private_data, protocol_name);
                if (data_ref != NULL) if (data_ref->class_of_origin != get_set_cls(NULL)) data_ref = NULL;
            break;
        case 2:
               if (obj->class_of_object == cls) data_ref = VortexStore_GetItem(obj->protected_data, protocol_name) ;
            break;
        default:
            break;
    }
    if ( data_ref == NULL ) return NULL;
    return data_ref->data;
}

void fudge_set_data_for_object(
    const char* protocol_name,
     VortexAny ref, 
     AnyClass obj, 
     fudge_class cls,
      fudge_get_set_cls_type get_set_cls, 
      vortex_int mode ) {
    
    fudge_data_ref data_ref = vortex_new_mem_of_type(struct fudge_data_ref_s);
    data_ref->class_of_origin = NULL;
    if (get_set_cls != NULL) data_ref->class_of_origin = get_set_cls(NULL);
    data_ref->data = ref;
    switch (mode) {
        case 0:
            if ( !VortexStore_ItemExists(obj->public_data, protocol_name) ) {
                VortexStore_AddItem(obj->public_data, data_ref, protocol_name);
                return;
            }
        break;
        case 1:
            if ( !VortexStore_ItemExists(obj->private_data, protocol_name) ) {
                VortexStore_AddItem(obj->private_data, data_ref, protocol_name);
                return;
            }
        break;
        case 2:
            if ( !VortexStore_ItemExists(obj->protected_data, protocol_name) ) {
                VortexStore_AddItem(obj->protected_data, data_ref, protocol_name);
                return;
            }
        break;
        default:
            break;
    }
    free(ref);
    free(data_ref);
}

VortexAny fudge_get_data_from_class(
    const char* protocol_name,
     fudge_class cls) {
    
    VortexAny data_ref = NULL;
    data_ref = VortexStore_GetItem(cls->data, protocol_name);
    if ( data_ref == NULL ) return NULL;
    return data_ref;
}

void fudge_set_data_for_class(
    const char* protocol_name,
     VortexAny ref, fudge_class cls) {
    
    if ( !VortexStore_ItemExists(cls->data, protocol_name) ) {
        VortexStore_AddItem(cls->data, ref, protocol_name);
    }
}

vortex_long fudge_call_method(
    fudge_method method,
     VortexArgs external_arglist,
      const AnyClass obj,
       VortexArgs method_arglist) {
    
    fudge_class iclass = ( obj != NULL ) ? obj->class_of_object : NULL;
    start_arglist(method_arglist);
    vortex_long retval = method(external_arglist,obj,iclass,method_arglist);
    end_arglist(method_arglist);
    return retval;
}

vortex_long fudge_call_class_method(
    fudge_classmethod method, 
     VortexArgs external_arglist,
      fudge_class cls,
       VortexArgs method_arglist) {
    
    return method(external_arglist,cls,method_arglist);
}

vortex_long fudge_call_class_method_with_cls(
    fudge_classmethod method, 
     VortexArgs external_arglist, 
      fudge_class cls, 
       VortexArgs method_arglist) {
    
    start_arglist(method_arglist);
    vortex_long retval = method(external_arglist,cls,method_arglist);
    end_arglist(method_arglist);
    return retval;
}

void fudge_add_typestring_to_class(
    fudge_class cls,
     const char* typestring) {
    
    int i = 0;
    char** type_array = cls->fudge_type_array;
    while ( type_array[i] != NULL ) {
        i++;
    }
    int size = i + 1;
    cls->fudge_type_array = 
     vortex_realloc(cls->fudge_type_array, size+1, size, char*, 1);
    vortex_long size2 = strlen(typestring);
    char* the_string = vortex_c_array(size2+1, char) ;
    strcpy(the_string, typestring);
    type_array = cls->fudge_type_array;
    type_array[size-1] = the_string;
    type_array[size] = NULL;
}