#pragma once

//
// (!) This code was implemented based on CUDA SDK ver. 11.1.1.456.81
//

#ifdef _WIN32
#include <d3d11.h>
#endif

#include <cuda.h>

#ifdef _WIN32
  #ifdef __has_include
    #if __has_include(<cudaD3D11.h>)
      #include <cudaD3D11.h>
    #endif
  #endif

  #ifndef CUDAD3D11_H
    #pragma message("Warning! <cudaD3D11.h> is not loaded ==> D3D11-related CUDA functions are disabled!")
  #endif
#endif

#define FUNC_CUDA_DRVAPI(function) function

#define CHECK_FUNC_CUDA_DRVAPI(function) \
	if (!FUNC_CUDA_DRVAPI(function)) { \
		fprintf(stderr, "CUDA Driver API init error: failed to find required functions (%s) (File: %s Line %d)\n", #function, __FILE__, __LINE__); \
	}

#if defined(_WIN32)
#include <windows.h>
#else
typedef void* HMODULE;
#endif

extern HMODULE g_hCudaDrvAPI;

int LoadCudaDrvApiLib();
int UnLoadCudaDrvApiLib();

const char* GetCudaDrvApiErrorText(CUresult cures);

#include "cuda_drvapi_dyn_load_declare.h"
