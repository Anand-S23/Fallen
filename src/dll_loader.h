#ifndef DLL_LOADER_H
#define DLL_LOADER_H

// TODO: Move this with external libs after this is completed

void *dl_open(const char *dll_file_path);
void *dl_load_function(void *dll_handle, const char *func_name);
int dl_close(void *dll_handle);

#if (defined _WIN32 || defined _WIN64)
    // TODO: Fix windows functions
    #include <windows.h>

    void *dl_open(const char *dll_file_path)
    {
        return (void *)LoadLibraryA(dll_file_path);
    }

    void *dl_load_function(void *dll_handle, const char *func_name)
    {
        return GetProcAddressA(dll_handle, func_name);
    }

    int dl_close(void *dll_handle)
    {
        // TODO: See if this right
        return (FreeLibraryA((HMODULE)dll_handle) == 0);
    }
#elif (defined linux || defined _linux || defined __linux__)
    #include <dlfcn.h>

    void *dl_open(const char *dll_file_path)
    {
        return dlopen(dll_file_path, RTLD_NOW);
    }

    void *dl_load_function(void *dll_handle, const char *func_name)
    {
        return dlsym(dll_handle, func_name);
    }

    int dl_close(void *dll_handle)
    {
        return (dlclose(dll_handle) == 0);
    }
#endif

#endif
