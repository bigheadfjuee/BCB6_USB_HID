//---------------------------------------------------------------------------
#ifndef UnitUsbProcH
#define UnitUsbProcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Math.h>

#include "USB_HID_IOc.h"

#define CODE_LEN 8   //指令長度
#define CMD_STATE  0xCB  //取得狀態(on/off)

extern byte WriteBuffer[USB_BUF_SIZE];
extern byte ReadBuffer[USB_BUF_SIZE];
//---------------------------------------------------------------------------
class TUsbProc : public TThread
{
  private:
      void __fastcall DoWork();
      DWORD mResult;
      DWORD mSleepTime;
      bool  mChangeBuf;
      byte  mSN_lo;
      byte  mSN_hi;
      TMemo *mMemo;
  protected:
      void __fastcall Execute();
  public:
      bool DevConnected;                 //true = 裝置已連接
      bool Send;
      bool CheckChange;
      bool NotMain;

      __fastcall TUsbProc(bool CreateSuspended);

      void SetSleepTime(int value);
      int GetSleepTime();
      void SendCmd(Byte cmd);

      void SetMemoLog(TMemo *memo);
      void MyLog(String str);
};
//---------------------------------------------------------------------------
#endif
