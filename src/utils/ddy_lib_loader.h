#pragma once

#include "ddefines.h"

typedef void* ddy_lib_loader_t; 

ddy_lib_loader_t ddy_lib_loader_create(void);
bool             ddy_lib_loader_load_lib(ddy_lib_loader_t loader, const char * library_name);
bool             ddy_lib_loader_load_sym(ddy_lib_loader_t loader, const char * sym_name, void** out_sym);
bool             ddy_lib_loader_unload_lib(ddy_lib_loader_t loader);
bool             ddy_lib_loader_load_copy_lib(ddy_lib_loader_t loader, const char * exist_lib_name, const char* new_lib_name);
bool             ddy_lib_loader_reload_copy_lib(ddy_lib_loader_t loader, const char * exist_lib_name, const char* new_lib_name);
void             ddy_lib_loader_destroy (ddy_lib_loader_t loader);


#if defined(DDY_LIB_LOADER_IMPLEMENTATION)

#include <windows.h>
#include <malloc.h>

typedef HANDLE ddy_lib_loader_t; 

ddy_lib_loader_t ddy_lib_loader_create(void)
{
    ddy_lib_loader_t loader = NULL;
    return loader;
}

bool ddy_lib_loader_load_lib(ddy_lib_loader_t loader, const char * library_name)
{

    if (loader) ddy_lib_loader_unload_lib(loader);
    HANDLE lib_handle = LoadLibraryA(library_name);
    if (!lib_handle) return false;
    
    loader = lib_handle;
    return true;
}

bool ddy_lib_loader_load_sym(ddy_lib_loader_t loader, const char * sym_name, void** out_sym)
{
    if (!loader) return false;    
    void* sym = (void*)GetProcAddress(loader, sym_name);
    if (!sym) return false;
    *out_sym = sym;
    return true;
}

bool ddy_lib_loader_unload_lib(ddy_lib_loader_t loader)
{
    if (!loader) return false;
    return FreeLibrary(loader);
}

bool ddy_lib_loader_load_copy_lib(ddy_lib_loader_t loader, const char *exist_library_name, const char *new_library_name)
{
    if (!CopyFile(exist_library_name, new_library_name, false)) return false;

    return ddy_lib_loader_load_lib(loader, new_library_name);
}

bool ddy_lib_loader_reload_copy_lib(ddy_lib_loader_t loader, const char *exist_lib_name, const char *new_lib_name)
{
    // TODO: Add check for valid dll before copy
    Sleep(1000);
    // ^ should go..
    if (!loader) return false;
    FreeLibrary(loader);
    return ddy_lib_loader_load_copy_lib(loader, exist_lib_name, new_lib_name);
}

void ddy_lib_loader_destroy (ddy_lib_loader_t loader)
{
    if (loader)
    {
        FreeLibrary(loader);

    }
}

#endif