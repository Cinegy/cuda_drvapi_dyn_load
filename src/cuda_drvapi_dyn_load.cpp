//
// (!) This code was implemented based on CUDA SDK ver. 11.1.1.456.81
//

#if defined(_WIN32)
#include <windows.h> // for HMODULE, LoadLibrary/GetProcAddress
#endif

#include <vector>
#include <string>
#include <mutex>

#define _CUDA_DRVAPI_FUNCS_DEFINITION_SECTION_
#define DECLARE_FUNC_CUDA_DRVAPI(f) decltype(f) *p##f = 0;
#include "cuda_drvapi_dyn_load.h"
#undef  _CUDA_DRVAPI_FUNCS_DEFINITION_SECTION_

#if defined(_WIN32)
//#define CUDA_DRVAPI_FILENAME "cuda.dll"
static const std::vector<std::string> cuda_drvapi_paths = {
    "nvcuda.dll" // Default
};
#else
#include <dlfcn.h>
#define LoadLibraryA(name) dlopen(name, RTLD_LAZY)
#define FreeLibrary(lib) dlclose(lib)
#define GetProcAddress(lib, func) dlsym(lib, func)
typedef void* FARPROC;
typedef void* HMODULE;
//#define CUDA_DRVAPI_FILENAME "cuda.so"
static const std::vector<std::string> cuda_drvapi_paths = {
    "libcuda.so" // Default
};
#endif
static std::string str_cuda_drvapi_path = "";

static std::mutex g_mutex_hCudaDrvAPI;
HMODULE g_hCudaDrvAPI = nullptr;

#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)

#define LOAD_CUDA_DRVAPI_FUNC(function) \
	FUNC_CUDA_DRVAPI(p##function) = (decltype(function)*)GetProcAddress(g_hCudaDrvAPI, STRINGIZE(function)); CHECK_FUNC_CUDA_DRVAPI(p##function)

int LoadCudaDrvApiLib()
{
	std::lock_guard<std::mutex> guard(g_mutex_hCudaDrvAPI);

	if (g_hCudaDrvAPI)
		return 0;

	for (size_t i = 0; i < cuda_drvapi_paths.size(); i++)
	{
		std::string str_cuda_drvapi_lib_path = cuda_drvapi_paths[i];
		g_hCudaDrvAPI = LoadLibraryA(str_cuda_drvapi_lib_path.c_str());
		if (g_hCudaDrvAPI)
		{
			str_cuda_drvapi_path = str_cuda_drvapi_lib_path;
			break;
		}
	}

	if (g_hCudaDrvAPI)
	{
#ifdef CUDAD3D11_H
		LOAD_CUDA_DRVAPI_FUNC(cuD3D11GetDevice)
		LOAD_CUDA_DRVAPI_FUNC(cuD3D11GetDevices)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsD3D11RegisterResource)
		LOAD_CUDA_DRVAPI_FUNC(cuD3D11CtxCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuD3D11CtxCreateOnDevice)
		LOAD_CUDA_DRVAPI_FUNC(cuD3D11GetDirect3DDevice)
#endif
		LOAD_CUDA_DRVAPI_FUNC(cuGetErrorString)
		LOAD_CUDA_DRVAPI_FUNC(cuGetErrorName)
		LOAD_CUDA_DRVAPI_FUNC(cuInit)
		LOAD_CUDA_DRVAPI_FUNC(cuDriverGetVersion)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGet)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetCount)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetName)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetUuid)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetLuid)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceTotalMem)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetTexture1DLinearMaxWidth)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetAttribute)
		//LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetNvSciSyncAttributes) // not load (?)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetProperties)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceComputeCapability)
		LOAD_CUDA_DRVAPI_FUNC(cuDevicePrimaryCtxRetain)
		LOAD_CUDA_DRVAPI_FUNC(cuDevicePrimaryCtxRelease)
		LOAD_CUDA_DRVAPI_FUNC(cuDevicePrimaryCtxSetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuDevicePrimaryCtxGetState)
		LOAD_CUDA_DRVAPI_FUNC(cuDevicePrimaryCtxReset)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxPushCurrent)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxPopCurrent)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxSetCurrent)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetCurrent)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetDevice)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxSynchronize)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxSetLimit)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetLimit)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetCacheConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxSetCacheConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetSharedMemConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxSetSharedMemConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetApiVersion)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxGetStreamPriorityRange)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxResetPersistingL2Cache)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxAttach)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxDetach)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleLoad)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleLoadData)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleLoadDataEx)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleLoadFatBinary)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleUnload)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleGetFunction)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleGetGlobal)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleGetTexRef)
		LOAD_CUDA_DRVAPI_FUNC(cuModuleGetSurfRef)
		LOAD_CUDA_DRVAPI_FUNC(cuLinkCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuLinkAddData)
		LOAD_CUDA_DRVAPI_FUNC(cuLinkAddFile)
		LOAD_CUDA_DRVAPI_FUNC(cuLinkComplete)
		LOAD_CUDA_DRVAPI_FUNC(cuLinkDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuMemGetInfo)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAlloc)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAllocPitch)
		LOAD_CUDA_DRVAPI_FUNC(cuMemFree)
		LOAD_CUDA_DRVAPI_FUNC(cuMemGetAddressRange)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAllocHost)
		LOAD_CUDA_DRVAPI_FUNC(cuMemFreeHost)
		LOAD_CUDA_DRVAPI_FUNC(cuMemHostAlloc)
		LOAD_CUDA_DRVAPI_FUNC(cuMemHostGetDevicePointer)
		LOAD_CUDA_DRVAPI_FUNC(cuMemHostGetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAllocManaged)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetByPCIBusId)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetPCIBusId)
		LOAD_CUDA_DRVAPI_FUNC(cuIpcGetEventHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuIpcOpenEventHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuIpcGetMemHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuIpcOpenMemHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuIpcCloseMemHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuMemHostRegister)
		LOAD_CUDA_DRVAPI_FUNC(cuMemHostUnregister)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyPeer)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyHtoD)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyDtoH)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyDtoD)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyDtoA)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyAtoD)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyHtoA)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyAtoH)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyAtoA)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy2D)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy2DUnaligned)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy3D)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy3DPeer)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyPeerAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyHtoDAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyDtoHAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyDtoDAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyHtoAAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpyAtoHAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy2DAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy3DAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemcpy3DPeerAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD8)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD16)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD32)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D8)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D16)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D32)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD8Async)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD16Async)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD32Async)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D8Async)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D16Async)
		LOAD_CUDA_DRVAPI_FUNC(cuMemsetD2D32Async)
		LOAD_CUDA_DRVAPI_FUNC(cuArrayCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuArrayGetDescriptor)
		LOAD_CUDA_DRVAPI_FUNC(cuArrayGetSparseProperties)
		LOAD_CUDA_DRVAPI_FUNC(cuMipmappedArrayGetSparseProperties)
		LOAD_CUDA_DRVAPI_FUNC(cuArrayDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuArray3DCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuArray3DGetDescriptor)
		LOAD_CUDA_DRVAPI_FUNC(cuMipmappedArrayCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuMipmappedArrayGetLevel)
		LOAD_CUDA_DRVAPI_FUNC(cuMipmappedArrayDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAddressReserve)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAddressFree)
		LOAD_CUDA_DRVAPI_FUNC(cuMemCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuMemRelease)
		LOAD_CUDA_DRVAPI_FUNC(cuMemMap)
		LOAD_CUDA_DRVAPI_FUNC(cuMemMapArrayAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemUnmap)
		LOAD_CUDA_DRVAPI_FUNC(cuMemSetAccess)
		LOAD_CUDA_DRVAPI_FUNC(cuMemGetAccess)
		LOAD_CUDA_DRVAPI_FUNC(cuMemExportToShareableHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuMemImportFromShareableHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuMemGetAllocationGranularity)
		LOAD_CUDA_DRVAPI_FUNC(cuMemGetAllocationPropertiesFromHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuMemRetainAllocationHandle)
		LOAD_CUDA_DRVAPI_FUNC(cuPointerGetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuMemPrefetchAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuMemAdvise)
		LOAD_CUDA_DRVAPI_FUNC(cuMemRangeGetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuMemRangeGetAttributes)
		LOAD_CUDA_DRVAPI_FUNC(cuPointerSetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuPointerGetAttributes)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamCreateWithPriority)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamGetPriority)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamGetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamGetCtx)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamWaitEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamAddCallback)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamBeginCapture)
		LOAD_CUDA_DRVAPI_FUNC(cuThreadExchangeStreamCaptureMode)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamEndCapture)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamIsCapturing)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamGetCaptureInfo)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamAttachMemAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamQuery)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamSynchronize)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamCopyAttributes)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamGetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamSetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuEventCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuEventRecord)
		LOAD_CUDA_DRVAPI_FUNC(cuEventRecordWithFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuEventQuery)
		LOAD_CUDA_DRVAPI_FUNC(cuEventSynchronize)
		LOAD_CUDA_DRVAPI_FUNC(cuEventDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuEventElapsedTime)
		LOAD_CUDA_DRVAPI_FUNC(cuImportExternalMemory)
		LOAD_CUDA_DRVAPI_FUNC(cuExternalMemoryGetMappedBuffer)
		LOAD_CUDA_DRVAPI_FUNC(cuExternalMemoryGetMappedMipmappedArray)
		LOAD_CUDA_DRVAPI_FUNC(cuDestroyExternalMemory)
		LOAD_CUDA_DRVAPI_FUNC(cuImportExternalSemaphore)
		LOAD_CUDA_DRVAPI_FUNC(cuSignalExternalSemaphoresAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuWaitExternalSemaphoresAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuDestroyExternalSemaphore)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamWaitValue32)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamWaitValue64)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamWriteValue32)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamWriteValue64)
		LOAD_CUDA_DRVAPI_FUNC(cuStreamBatchMemOp)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncGetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncSetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncSetCacheConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncSetSharedMemConfig)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchKernel)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchCooperativeKernel)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchCooperativeKernelMultiDevice)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchHostFunc)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncSetBlockShape)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncSetSharedSize)
		LOAD_CUDA_DRVAPI_FUNC(cuParamSetSize)
		LOAD_CUDA_DRVAPI_FUNC(cuParamSeti)
		LOAD_CUDA_DRVAPI_FUNC(cuParamSetf)
		LOAD_CUDA_DRVAPI_FUNC(cuParamSetv)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunch)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchGrid)
		LOAD_CUDA_DRVAPI_FUNC(cuLaunchGridAsync)
		LOAD_CUDA_DRVAPI_FUNC(cuParamSetTexRef)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddKernelNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphKernelNodeGetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphKernelNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddMemcpyNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphMemcpyNodeGetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphMemcpyNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddMemsetNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphMemsetNodeGetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphMemsetNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddHostNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphHostNodeGetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphHostNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddChildGraphNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphChildGraphNodeGetGraph)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddEmptyNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddEventRecordNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphEventRecordNodeGetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphEventRecordNodeSetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddEventWaitNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphEventWaitNodeGetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphEventWaitNodeSetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphClone)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphNodeFindInClone)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphNodeGetType)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphGetNodes)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphGetRootNodes)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphGetEdges)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphNodeGetDependencies)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphNodeGetDependentNodes)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphAddDependencies)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphRemoveDependencies)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphDestroyNode)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphInstantiate)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecKernelNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecMemcpyNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecMemsetNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecHostNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecChildGraphNodeSetParams)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecEventRecordNodeSetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecEventWaitNodeSetEvent)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphUpload)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphLaunch)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphExecUpdate)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphKernelNodeCopyAttributes)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphKernelNodeGetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphKernelNodeSetAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuOccupancyMaxActiveBlocksPerMultiprocessor)
		LOAD_CUDA_DRVAPI_FUNC(cuOccupancyMaxActiveBlocksPerMultiprocessorWithFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuOccupancyMaxPotentialBlockSize)
		LOAD_CUDA_DRVAPI_FUNC(cuOccupancyMaxPotentialBlockSizeWithFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuOccupancyAvailableDynamicSMemPerBlock)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetArray)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetMipmappedArray)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetAddress)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetAddress2D)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetFormat)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetAddressMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetFilterMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetMipmapFilterMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetMipmapLevelBias)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetMipmapLevelClamp)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetMaxAnisotropy)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetBorderColor)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefSetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetAddress)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetArray)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetMipmappedArray)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetAddressMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetFilterMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetFormat)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetMipmapFilterMode)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetMipmapLevelBias)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetMipmapLevelClamp)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetMaxAnisotropy)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetBorderColor)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefGetFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuTexRefDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuSurfRefSetArray)
		LOAD_CUDA_DRVAPI_FUNC(cuSurfRefGetArray)
		LOAD_CUDA_DRVAPI_FUNC(cuTexObjectCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuTexObjectDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuTexObjectGetResourceDesc)
		LOAD_CUDA_DRVAPI_FUNC(cuTexObjectGetTextureDesc)
		LOAD_CUDA_DRVAPI_FUNC(cuTexObjectGetResourceViewDesc)
		LOAD_CUDA_DRVAPI_FUNC(cuSurfObjectCreate)
		LOAD_CUDA_DRVAPI_FUNC(cuSurfObjectDestroy)
		LOAD_CUDA_DRVAPI_FUNC(cuSurfObjectGetResourceDesc)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceCanAccessPeer)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxEnablePeerAccess)
		LOAD_CUDA_DRVAPI_FUNC(cuCtxDisablePeerAccess)
		LOAD_CUDA_DRVAPI_FUNC(cuDeviceGetP2PAttribute)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsUnregisterResource)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsSubResourceGetMappedArray)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsResourceGetMappedMipmappedArray)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsResourceGetMappedPointer)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsResourceSetMapFlags)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsMapResources)
		LOAD_CUDA_DRVAPI_FUNC(cuGraphicsUnmapResources)
		LOAD_CUDA_DRVAPI_FUNC(cuGetExportTable)
		LOAD_CUDA_DRVAPI_FUNC(cuFuncGetModule)
	}
	else
		return fprintf(stderr, "CUDA Driver API load error: failed to load!\n"), -1;

	return 0;
}

int UnLoadCudaDrvApiLib()
{
	std::lock_guard<std::mutex> guard(g_mutex_hCudaDrvAPI);

	if (g_hCudaDrvAPI)
	{
		FreeLibrary(g_hCudaDrvAPI);
	}

	g_hCudaDrvAPI = nullptr;

	return 0;
}

class CRunLoadCudaDrvApiLib
{
public:
	CRunLoadCudaDrvApiLib()
	{
		LoadCudaDrvApiLib();
	}
	~CRunLoadCudaDrvApiLib()
	{
		UnLoadCudaDrvApiLib();
	}
};

//CRunLoadCudaDrvApiLib g_runCudaDrvApi;

//---------------------------------------------------------------
const char* GetCudaDrvApiErrorText(CUresult cures)
//---------------------------------------------------------------
{
	const char* pStr = nullptr;

	if (pcuGetErrorString)
		pcuGetErrorString(cures, &pStr);
	else
		return "CUDA Driver API not load!";

	return pStr;
}