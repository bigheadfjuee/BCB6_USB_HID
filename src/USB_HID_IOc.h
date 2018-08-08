//---------------------------------------------------------------------------

#ifndef USB_HID_IOcH
#define USB_HID_IOcH
//---------------------------------------------------------------------------

#include <wtypes.h>
#include <initguid.h>

extern "C" {
#include "hidsdi.h"
#include <setupapi.h>
}
//---------------------------------------------------------------------------
//最早用 Capabilities.InputReportByteLength
//但WinXP下會失敗？
//Tony: Win7 x64下，得到的值是0
#define USB_BUF_SIZE 65  //64+1

class CUsbhidioc //: public CDialog
{
private:
    USHORT       VendorID;
    USHORT       ProductID;
    DWORD        mTimeOut;

    PSP_DEVICE_INTERFACE_DETAIL_DATA	detailData;
    HANDLE      DeviceHandle;
    HIDP_CAPS   Capabilities; //  08Mar2005 - Capabilities call fails on WinXP
    DWORD			  dwError;
    HANDLE			hEventObject;
    HANDLE			hDevInfo;
    GUID		  	HidGuid;
    OVERLAPPED 	HIDOverlapped;
    char			  InputReport[USB_BUF_SIZE];
    ULONG			  Length;
    DWORD			  NumberOfBytesRead;
    HANDLE			overHandle;
    ULONG			  Required;

public:
    __fastcall CUsbhidioc();
    DWORD __fastcall FindTheHID();
    DWORD __fastcall ReadReport (char *);
    DWORD __fastcall WriteReport(char *, unsigned int);
    void __fastcall CloseReport ();
    bool DeviceDetected;
    int  DevCount;
protected:
    void __fastcall GetDeviceCapabilities();
    void __fastcall PrepareForOverlappedTransfer();
};
//---------------------------------------------------------------------------
extern CUsbhidioc  USB;
//---------------------------------------------------------------------------
#endif
