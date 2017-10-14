#pragma once


namespace Slg3DScanner
{
    template<class DirectXReturn, class ... Args>
    void DXTry(const DirectXReturn& returnFromFunc, const Args& ... args)
    {
        DXTry(returnFromFunc);
        DXTry(args...);
    }

    template<class DirectXReturn>
    void DXTry(const DirectXReturn& returnFromFunc)
    {
        if(returnFromFunc != S_OK)
        {
            throw returnFromFunc;
        }
    }

    template<class DirectXReturn, class ErrorCode>
    void DXTryWithSpecificErrorCode(const DirectXReturn& returnFromFunc, const ErrorCode& error)
    {
        if(returnFromFunc != DI_OK)
        {
            throw error;
        }
    }

    template<class Error, class DXPtr, class ... Args>
    void DXPointerValidate(const Error& error, const DXPtr& dxPtr, const Args& ... args)
    {
        DXTry(error, dxPtr);
        DXTry(error, args...);
    }

    template<class Error, class DXPtr>
    void DXPointerValidate(const Error& error, const DXPtr& dxPtr)
    {
        if(dxPtr == nullptr)
        {
            throw error;
        }
    }

    template<class DXPtr, class ... Args>
    void DXRelease(DXPtr& dxPtr, Args& ... args)
    {
        DXRelease(dxPtr);
        DXRelease(args...);
    }

    template<class DXPtr>
    void DXRelease(DXPtr& dxPtr)
    {
        if(dxPtr != nullptr)
        {
            dxPtr->Release();
            dxPtr = nullptr;
        }
    }

    template<class DXDevicePtr, class ... Args>
    void DXAcquire(DXDevicePtr& dxDevicePtr, Args& ... args)
    {
        DXAcquire(dxDevicePtr);
        DXAcquire(args...);
    }

    template<class DXDevicePtr>
    void DXAcquire(DXDevicePtr& dxDevicePtr)
    {
        if(dxDevicePtr != nullptr)
        {
            dxDevicePtr->Acquire();
        }
    }

    template<class DXDevicePtr, class ... Args>
    void DXUnacquire(DXDevicePtr& dxDevicePtr, Args& ... args)
    {
        DXUnacquire(dxDevicePtr);
        DXUnacquire(args...);
    }

    template<class DXDevicePtr>
    void DXUnacquire(DXDevicePtr& dxDevicePtr)
    {
        if(dxDevicePtr != nullptr)
        {
            dxDevicePtr->Unacquire();
        }
    }

    template<class DXStruct, class ... Args>
    void DXZeroMemory(DXStruct& dxDevicePtr, Args& ... args)
    {
        DXZeroMemory(dxDevicePtr);
        DXZeroMemory(args...);
    }

    template<class DXStruct>
    void DXZeroMemory(DXStruct& dxDevicePtr)
    {
        memset(&dxDevicePtr, 0, sizeof(DXStruct));
    }
}
