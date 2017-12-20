/*
 Copyright (c) 2017 Jacob Gordon. All rights reserved.
 
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

#include <string.h>
#include <fudge.h>

struct fudge_class_s { RKArgs_Dynamic_Type_Protocol ; void* fast_data_structure ; void* fudge_type_array ;
    
fudge_class_init_and_deinit_type classinit ; fudge_class_init_and_deinit_type classdeinit ; fudge_method init ; fudge_method deinit ; fudge_method final_init ;
    
fudge_method final_deinit ; RKStore methods ; RKStore final_methods ; RKStore class_methods ; RKStore final_class_methods ; RKStore data ;
    
fudge_classdef the_classdef ; RKList superclass_refs ; } ;

struct fudge_object_s { RKArgs_Dynamic_Type_Protocol ; void* fast_data_structure ; fudge_class class_of_object ; RKStore public_data ; RKStore private_data ;
    
RKStore protected_data ; RKStore public_refs ; RKStore private_refs ; RKStore protected_refs ; int refcount ; } ;

struct fudge_ref_s { int strong ; AnyClass obj ; AnyClass base_obj ; fudge_class class_of_origin ; } ;

typedef struct fudge_ref_s* fudge_ref ;

typedef struct fudge_data_ref_s { void* data ; fudge_class class_of_origin ; } *fudge_data_ref ;

typedef struct fudge_method_ref_s { fudge_method method ; int mode ; fudge_class class_of_origin ; } *fudge_method_ref ;

 RKLong fudge_default_func(RKArgs external_arglist, const AnyClass obj, const fudge_class iclass, RKArgs method_arglist) {
    
    return 0 ;
}

void fudge_default_class_init_and_deinit( const fudge_class cls ) {
    
    
}

AnyClass fudge_object_alloc( fudge_classdef the_classdef, RKArgs arglist ) {
    
    AnyClass obj = the_classdef(0, NULL) ;
    
    obj->verify = 12345423 ;
    
    obj->refcount = 0 ;
    
    obj->public_refs = RKStore_NewStore() ;
    
    obj->private_refs = RKStore_NewStore() ;
    
    obj->protected_refs = RKStore_NewStore() ;
    
    obj->public_data = RKStore_NewStore() ;
    
    obj->private_data = RKStore_NewStore() ;
    
    obj->protected_data = RKStore_NewStore() ;
    
    start_arglist(arglist) ;
    
    fudge_method init = (obj->class_of_object->final_init != get_null_method) ? obj->class_of_object->final_init : obj->class_of_object->init ;
    
    fudge_call_method(init, arglist, (AnyClass)obj, noargs) ;
    
    end_arglist(arglist) ;
    
    return obj ;
}

AnyClass fudge_new_object( fudge_class cls ) {
    
    AnyClass obj = RKMem_NewMemOfType(struct fudge_object_s) ;
    
    obj->class_of_object = cls ;
    
    obj->base_type = RKArgs_Dynamic_type ;
    
    obj->type_array = cls->fudge_type_array ;
    
    return obj ;
}

void fudge_delete_object( AnyClass obj ) {
    
    free(obj) ;
}

static void fudge_dealloc_refs( void* data ) {
    
    fudge_ref ref = (fudge_ref)data ;
    
    if ( (ref->strong) && (ref->obj != ref->base_obj) ) sub_ref_count(ref->obj) ;
    
    free(ref) ;
}

static void fudge_dealloc_data_store( void* data ) {
    
    fudge_data_ref ref = data ;
    
    free(ref->data) ;
    
    free(ref) ;
}

void fudge_object_dealloc( AnyClass obj ) {
    
    fudge_method deinit = (obj->class_of_object->final_deinit != get_null_method) ? obj->class_of_object->final_deinit : obj->class_of_object->deinit ;
    
    fudge_call_method(deinit, NULL, (AnyClass)obj, noargs) ;
    
    obj->class_of_object->the_classdef(-1, NULL) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_refs, obj->public_refs) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_refs, obj->private_refs) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_refs, obj->protected_refs) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_data_store, obj->public_data) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_data_store, obj->private_data) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_data_store, obj->protected_data) ;
    
    RKStore_DestroyStore(obj->public_refs) ;
    
    RKStore_DestroyStore(obj->private_refs) ;
    
    RKStore_DestroyStore(obj->protected_refs) ;
    
    RKStore_DestroyStore(obj->public_data) ;
    
    RKStore_DestroyStore(obj->private_data) ;
    
    RKStore_DestroyStore(obj->protected_data) ;
    
    free(obj) ;
}

AnyClass fudge_store_object_public( AnyClass obj1, AnyClass obj2, const char* name, int strong ) {
    
    if ( name != NULL ) {
    
      if ( RKStore_ItemExists(obj1->public_refs, name) ) {
        
          fudge_ref ref = RKStore_GetItem(obj1->public_refs, name) ;
        
          if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) sub_ref_count(ref->obj) ;
        
          free(ref) ;
      }
        
    }
    
    fudge_ref ref = RKMem_NewMemOfType(struct fudge_ref_s) ;
    
    ref->base_obj = obj1 ;
    
    ref->strong = strong ;
    
    ref->obj = obj2 ;
    
    ref->class_of_origin = NULL ;
    
    if ( (ref->strong) && (obj1 != obj2) ) add_ref_count(ref->obj) ;
    
    if ( name == NULL ) {
        
        RKStore_AddItemToList(obj1->public_refs, ref) ;
        
        return obj2 ;
    }
    
    RKStore_AddItem(obj1->public_refs, ref, name) ;
    
    return obj2 ;
}

AnyClass fudge_store_object_private( AnyClass obj1, AnyClass obj2, const char* name, int strong, fudge_class cls ) {
    
    if ( name == NULL ) return obj2 ;
        
        if ( RKStore_ItemExists(obj1->private_refs, name) ) {
            
            fudge_ref ref = RKStore_GetItem(obj1->private_refs, name) ;
            
            if (ref->class_of_origin != cls) return obj2 ;
            
            if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) sub_ref_count(ref->obj) ;
            
            free(ref) ;
        }
    
    fudge_ref ref = RKMem_NewMemOfType(struct fudge_ref_s) ;
    
    ref->base_obj = obj1 ;
    
    ref->strong = strong ;
    
    ref->obj = obj2 ;
    
    ref->class_of_origin = cls ;
    
    if ( (ref->strong) && (obj1 != obj2) ) add_ref_count(ref->obj) ;

    RKStore_AddItem(obj1->private_refs, ref, name) ;
    
    return obj2 ;
}

AnyClass fudge_store_object_protected( AnyClass obj1, AnyClass obj2, const char* name, int strong, fudge_class cls ) {
    
    if (obj1->class_of_object != cls) return obj2 ;
    
    if ( name == NULL ) return obj2 ;
        
        if ( RKStore_ItemExists(obj1->public_refs, name) ) {
            
            fudge_ref ref = RKStore_GetItem(obj1->protected_refs, name) ;
            
            if ( (ref->strong) && (ref->obj != obj1) && (ref->obj != obj2) ) sub_ref_count(ref->obj) ;
            
            free(ref) ;
        }
    
    fudge_ref ref = RKMem_NewMemOfType(struct fudge_ref_s) ;
    
    ref->base_obj = obj1 ;
    
    ref->strong = strong ;
    
    ref->obj = obj2 ;
    
    ref->class_of_origin = NULL ;
    
    if ( (ref->strong) && (obj1 != obj2) ) add_ref_count(ref->obj) ;
    
    RKStore_AddItem(obj1->protected_refs, ref, name) ;
    
    return obj2 ;
}

AnyClass fudge_get_object_public( AnyClass obj, const char* name ) {
    
    fudge_ref ref = RKStore_GetItem(obj->public_refs, name) ;
    
    if ( ref == NULL ) return NULL ;
    
    return ref->obj ;
}

AnyClass fudge_get_object_private( AnyClass obj, const char* name, fudge_class access_class ) {
    
    fudge_ref ref = RKStore_GetItem(obj->private_refs, name) ;
    
    if ( ref == NULL ) return NULL ;
    
    if ( ref->class_of_origin != access_class ) return NULL ;
    
    return ref->obj ;
}

AnyClass fudge_get_object_protected( AnyClass obj, const char* name, fudge_class access_class ) {
    
    if ( obj->class_of_object != access_class ) return NULL ;
    
    fudge_ref ref = RKStore_GetItem(obj->protected_refs, name) ;
    
    if ( ref == NULL ) return NULL ;
    
    return ref->obj ;
}


void fudge_add_ref_count( AnyClass obj ) {
    
    if ( obj != NULL ) obj->refcount++ ;
}

void fudge_sub_ref_count( AnyClass obj ) {
    
    if ( obj != NULL ) obj->refcount-- ;
    
    if ( obj != NULL ) if ( obj->refcount <= 0 ) free_object(obj) ;
}

fudge_class fudge_class_alloc( fudge_classdef the_classdef, const char* classname ) {
    
    fudge_class cls = RKMem_NewMemOfType(struct fudge_class_s) ;
    
    cls->verify = 12345423 ;
    
    cls->base_type = RKArgs_Dynamic_type ;
    
    cls->type_array = RKMem_CArray(2, void*) ;
    
    const char* fudgeclass = "fudge_class" ;
    
    RKLong size = strlen(fudgeclass) ;
    
    char* the_string = RKMem_CArray(size+1, char) ;
    
    strcpy(the_string, fudgeclass) ;
    
    ((void**)cls->type_array)[0] = the_string ;
    
    ((void**)cls->type_array)[1] = NULL ;
    
    cls->fudge_type_array = RKMem_CArray(3, void*) ;
    
    RKLong size2 = strlen(classname) ;
    
    char* the_string2 = RKMem_CArray(size2+1, char) ;
    
    strcpy(the_string2, classname) ;
    
    const char* anyclass = "AnyClass" ;
    
    RKLong size3 = strlen(anyclass) ;
    
    char* the_string3 = RKMem_CArray(size3+1, char) ;
    
    strcpy(the_string3, anyclass) ;
    
    ((void**)cls->fudge_type_array)[0] = the_string2 ;
    
    ((void**)cls->fudge_type_array)[1] = the_string3 ;
    
    ((void**)cls->fudge_type_array)[2] = NULL ;
    
    cls->final_init = fudge_default_func ;
    
    cls->final_deinit = fudge_default_func ;
    
    cls->init = fudge_default_func ;
    
    cls->deinit = fudge_default_func ;
    
    cls->classinit = fudge_default_class_init_and_deinit ;
    
    cls->classdeinit = fudge_default_class_init_and_deinit ;
    
    cls->final_class_methods = RKStore_NewStore() ;
    
    cls->class_methods = RKStore_NewStore() ;
    
    cls->final_methods = RKStore_NewStore() ;
    
    cls->methods = RKStore_NewStore() ;
    
    cls->data = RKStore_NewStore() ;
    
    cls->the_classdef = the_classdef ;
    
    cls->superclass_refs = RKList_NewList() ;
    
    return cls ;
}

static void fudge_dealloc_method_refs( void* data ) {
    
    fudge_method_ref ref = data ;
    
    free(ref) ;
}

static void fudge_dealloc_superclass_refs( void* data ) {
    
    fudge_class cls = (fudge_class)data ;
    
    cls->the_classdef(-1, NULL) ;
}

void fudge_class_dealloc( fudge_class cls ) {
    
    cls->classdeinit(cls) ;
    
    int i = 0 ;
    
    while (((void**)cls->type_array)[i] != NULL) {
        
        free(((void**)cls->type_array)[i]) ;
        
        i++ ;
    }
    
    free(cls->type_array) ;
    
    i = 0 ;
    
    while (((void**)cls->fudge_type_array)[i] != NULL) {
        
        free(((void**)cls->fudge_type_array)[i]) ;
        
        i++ ;
    }
    
    free(cls->fudge_type_array) ;
    
    RKStore_DestroyStore(cls->final_class_methods) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_method_refs, cls->class_methods) ;
    
    RKStore_DestroyStore(cls->class_methods) ;
    
    RKStore_DestroyStore(cls->final_methods) ;
    
    RKStore_IterateStoreWith(fudge_dealloc_method_refs, cls->methods) ;
    
    RKStore_DestroyStore(cls->methods) ;
    
    RKStore_DestroyStore(cls->data) ;
    
    RKList_IterateListWith(fudge_dealloc_superclass_refs, cls->superclass_refs) ;
    
    RKList_DeleteList(cls->superclass_refs) ;
    
    free(cls) ;
}

void fudge_add_class_ref_to_subclass( fudge_class cls, fudge_class subclass ) {
    
    RKList_AddToList(subclass->superclass_refs, cls) ;
}

void fudge_invoke_classinit_method( fudge_class cls ) {
    
    cls->classinit(cls) ;
}

void fudge_add_classinit_method( fudge_class_init_and_deinit_type classinit, fudge_class cls ) {
    
    if ( classinit == NULL ) classinit = fudge_default_class_init_and_deinit ;
    
    cls->classinit = classinit ;
}

void fudge_add_classdeinit_method( fudge_class_init_and_deinit_type classdeinit, fudge_class cls ) {
    
    if ( classdeinit == NULL ) classdeinit = fudge_default_class_init_and_deinit ;
    
    cls->classdeinit = classdeinit ;
}

void fudge_add_init_method( fudge_method method, fudge_class cls ) {
    
    if ( cls->final_init == get_null_method ) cls->init = method ;
}

void fudge_add_deinit_method( fudge_method method, fudge_class cls ) {
    
    if ( cls->final_deinit == get_null_method ) cls->deinit = method ;
}

void fudge_add_final_init_method( fudge_method method, fudge_class cls ) {
    
    if ( cls->final_init == get_null_method ) cls->final_init = method ;
}

void fudge_add_final_deinit_method( fudge_method method, fudge_class cls ) {
    
    if ( cls->final_deinit == get_null_method ) cls->final_deinit = method ;
}

void fudge_add_required_method( const char* name, fudge_class cls, int mode ) {
    
    fudge_method method = NULL ;
    
    if ( !(RKStore_ItemExists(cls->final_methods, name)) )  {
         
      fudge_method_ref ref = RKMem_NewMemOfType(struct fudge_method_ref_s) ;
    
      ref->class_of_origin = cls ;
     
      ref->method = method ;
    
      ref->mode = mode ;
    
      if ( (RKStore_ItemExists(cls->methods, name)) ) free(RKStore_GetItem(cls->methods, name)) ;
    
      RKStore_AddItem(cls->methods, ref, name) ;
        
    }
}

void fudge_add_method( fudge_method method, const char* name, fudge_class cls, int mode ) {
    
     if ( method == NULL ) return ;
    
     if ( !(RKStore_ItemExists(cls->final_methods, name)) )  {
         
      fudge_method_ref ref = RKMem_NewMemOfType(struct fudge_method_ref_s) ;
    
      ref->class_of_origin = cls ;
     
      ref->method = method ;
    
      ref->mode = mode ;
    
      if ( (RKStore_ItemExists(cls->methods, name)) ) free(RKStore_GetItem(cls->methods, name)) ;
    
      RKStore_AddItem(cls->methods, ref, name) ;
        
    }
}

fudge_method fudge_get_method( AnyClass obj, const char* name, fudge_class access_class) {
    
    fudge_method_ref ref = NULL ;
    
    ref = RKStore_GetItem(obj->class_of_object->methods, name) ;
    
    if ( ref == NULL ) return fudge_default_func ;
    
    switch (ref->mode) {
            
        case 0:
            
            return ref->method ;
            
        break;
            
        case 1:
            
            if ( ref->class_of_origin == access_class ) return ref->method ;
            
        break;

            
        case 2:
            
            if ( obj->class_of_object == access_class ) return ref->method ;
            
        break;

            
        default:
            break;
    }
    
    
    return fudge_default_func ;
}

void fudge_add_final_method( const char* name, fudge_class cls ) {
    
    RKStore_AddItem(cls->final_methods, NULL, name) ;
}

void fudge_add_required_class_method( const char* name, fudge_class cls, int mode ) {
    
    fudge_method method = NULL ;
    
    if ( !(RKStore_ItemExists(cls->final_class_methods, name)) )  {
        
        fudge_method_ref ref = RKMem_NewMemOfType(struct fudge_method_ref_s) ;
        
        ref->class_of_origin = cls ;
        
        ref->method = method ;
        
        ref->mode = mode ;
        
        if ( (RKStore_ItemExists(cls->class_methods, name)) ) free(RKStore_GetItem(cls->class_methods, name)) ;
        
        RKStore_AddItem(cls->class_methods, ref, name) ;
        
    }
}

void fudge_add_class_method( fudge_method method, const char* name, fudge_class cls, int mode ) {
    
    if ( method == NULL ) return ;
    
    if ( !(RKStore_ItemExists(cls->final_class_methods, name)) )  {
        
        fudge_method_ref ref = RKMem_NewMemOfType(struct fudge_method_ref_s) ;
        
        ref->class_of_origin = cls ;
        
        ref->method = method ;
        
        ref->mode = mode ;
        
        if ( (RKStore_ItemExists(cls->class_methods, name)) ) free(RKStore_GetItem(cls->class_methods, name)) ;
        
        RKStore_AddItem(cls->class_methods, ref, name) ;
        
    }

}

int fudge_is_class_superclass_of_class( fudge_class superclass, fudge_class subclass ) {
    
    RKList_node node = RKList_GetFirstNode(subclass->superclass_refs) ;
    
    while ( node != NULL ) {
        
        if ( RKList_GetData(node) == superclass ) return 1 ;
        
        node = RKList_GetNextNode(node) ;
    }
    
    return 0 ;
}

fudge_method fudge_get_class_method( fudge_class cls, fudge_class access_class, const char* name ) {
    
    fudge_method_ref ref = NULL ;
    
    ref = RKStore_GetItem(cls->class_methods, name) ;
    
    if ( ref == NULL ) return fudge_default_func ;
    
    switch (ref->mode) {
            
        case 0:
            
            return ref->method ;
            
            break;
            
        case 1:
            
            if ( ref->class_of_origin == access_class ) return ref->method ;
            
            break;
            
            
        case 2:
            
            if ( (fudge_is_class_superclass_of_class(ref->class_of_origin, access_class)) || (ref->class_of_origin == access_class) ) return ref->method ;
            
            break;
            
            
        default:
            break;
    }
    
    
    return fudge_default_func ;

}

void fudge_add_final_class_method( const char* name, fudge_class cls ) {
    
    RKStore_AddItem(cls->final_class_methods, NULL, name) ;
}

fudge_classdef obj_get_classdef_from_class( fudge_class cls ) {
    
    return cls->the_classdef ;
}

int fudge_verify_object_is_of_class( AnyClass obj, fudge_class cls ) {
    
    if ( obj->class_of_object == cls ) return 1 ;
    
    return 0 ;
}

void* fudge_get_data_from_object( const char* protocol_name, AnyClass obj, fudge_class cls, fudge_get_set_cls_type get_set_cls, int mode ) {
    
    fudge_data_ref data_ref = NULL ;
    
    switch (mode) {
            
        case 0:
            
                data_ref = RKStore_GetItem(obj->public_data, protocol_name) ;
            
            break;
            
        case 1:
                data_ref = RKStore_GetItem(obj->private_data, protocol_name) ;
            
                if (data_ref != NULL) if (data_ref->class_of_origin != get_set_cls(NULL)) data_ref = NULL ;
            
            break;
            
            
        case 2:
            
               if (obj->class_of_object == cls) data_ref = RKStore_GetItem(obj->protected_data, protocol_name) ;
    
            break;
            
        default:
            break;
    }
    
    if ( data_ref == NULL ) return NULL ;
    
    return data_ref->data ;
}

void fudge_set_data_for_object( const char* protocol_name, void* ref, AnyClass obj, fudge_class cls, fudge_get_set_cls_type get_set_cls, int mode ) {
    
    fudge_data_ref data_ref = RKMem_NewMemOfType(struct fudge_data_ref_s) ;
    
    data_ref->class_of_origin = NULL ;
    
    if (get_set_cls != NULL) data_ref->class_of_origin = get_set_cls(NULL) ;
    
    data_ref->data = ref ;
    
    switch (mode) {
            
        case 0:
            
            if ( !RKStore_ItemExists(obj->public_data, protocol_name) ) {
            
                RKStore_AddItem(obj->public_data, data_ref, protocol_name) ;
                
                return ;
            }
            
        break;
            
        case 1:
            
            if ( !RKStore_ItemExists(obj->private_data, protocol_name) ) {
                
                RKStore_AddItem(obj->private_data, data_ref, protocol_name) ;
                
                return ;
            }
            
        break;
            
            
        case 2:
            
            if ( !RKStore_ItemExists(obj->protected_data, protocol_name) ) {
                
                RKStore_AddItem(obj->protected_data, data_ref, protocol_name) ;
                
                return ;
            }
            
        break;
            
        default:
            break;
    }
    
    free(ref) ;
    
    free(data_ref) ;
}

void* fudge_get_data_from_class( const char* protocol_name, fudge_class cls ) {
    
    void* data_ref = NULL ;
    
    data_ref = RKStore_GetItem(cls->data, protocol_name) ;
    
    if ( data_ref == NULL ) return NULL ;
    
    return data_ref ;
}

void fudge_set_data_for_class( const char* protocol_name, void* ref, fudge_class cls ) {
    
    if ( !RKStore_ItemExists(cls->data, protocol_name) ) {
                
        RKStore_AddItem(cls->data, ref, protocol_name) ;
    }
}

RKLong fudge_call_method( fudge_method method, RKArgs external_arglist, const AnyClass obj, RKArgs method_arglist ) {
    
    fudge_class iclass = ( obj != NULL ) ? obj->class_of_object : NULL ;
    
    start_arglist(method_arglist) ;
    
    RKLong retval = method(external_arglist,obj,iclass,method_arglist) ;
    
    end_arglist(method_arglist) ;
    
    return retval ;
}

RKLong fudge_call_class_method( fudge_classmethod method, RKArgs external_arglist, fudge_class cls, RKArgs method_arglist ) {
    
    return method(external_arglist,cls,method_arglist) ;
}

RKLong fudge_call_class_method_with_cls( fudge_classmethod method, RKArgs external_arglist, fudge_class cls, RKArgs method_arglist ) {
    
    start_arglist(method_arglist) ;
    
    RKLong retval = method(external_arglist,cls,method_arglist) ;
    
    end_arglist(method_arglist) ;
    
    return retval ;

}

void fudge_add_typestring_to_class( fudge_class cls, const char* typestring ) {
    
    int i = 0 ;
    
    char** type_array = cls->fudge_type_array ;
    
    while ( type_array[i] != NULL ) {
        
        i++ ;
    }
    
    int size = i + 1 ;
    
    cls->fudge_type_array = RKMem_Realloc(cls->fudge_type_array, size+1, size, char*, 1) ;
    
    RKLong size2 = strlen(typestring) ;
    
    char* the_string = RKMem_CArray(size2+1, char) ;
    
    strcpy(the_string, typestring) ;
    
    type_array = cls->fudge_type_array ;
    
    type_array[size-1] = the_string ;
    
    type_array[size] = NULL ;
}