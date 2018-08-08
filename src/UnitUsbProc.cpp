//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitUsbProc.h"
#include "uMain.h"

#define BXB_PID 0xFF // 治具



extern byte WriteBuffer[USB_BUF_SIZE] = {0};
extern byte ReadBuffer[USB_BUF_SIZE]  = {0};

byte old1=0,old2=0,new1=0,new2=0; //Debug用來看收到的資料用
//---------------------------------------------------------------------------
__fastcall TUsbProc::TUsbProc(bool CreateSuspended)
        : TThread(CreateSuspended)
{
  mResult = 0;
  mSleepTime = 100;  //大於100ms會明顯感到GUI的反應變慢，小於40又會導致某些傳輸會loss

  mChangeBuf = true;
  DevConnected=false;
  Send=false;
  CheckChange=false;
  NotMain=false;
}
//---------------------------------------------------------------------------
void __fastcall TUsbProc::Execute()
{
  //---- Place thread code here ----
  while(!Terminated)
  {
    Synchronize(DoWork); // 有 GUI 要用 Synchronize
    Sleep(mSleepTime);
  }
}
//---------------------------------------------------------------------------
//因為Overlapped IO需要等待結果，若寫在fmMain的話，反應會卡卡
void __fastcall TUsbProc::DoWork()
{
  int CheckCoutn;

  if(DevConnected)
  {
    //fmMain->edtSend->Text = BoolToStr(Send);
    if(Send)
    {
      Send = false;
      mChangeBuf=true;
    }
    else if(mChangeBuf)
    {

      WriteBuffer[0] = BXB_PID;      //PID
      WriteBuffer[1] = 0x00;          //MID
      WriteBuffer[2] = 0x00;          //DID2
      WriteBuffer[3] = 0x00;          //DID1
      WriteBuffer[4] = CMD_STATE;     //CMD  : 查詢狀態
      WriteBuffer[5] = 0x00;          //PARAM1
      WriteBuffer[6] = 0x00;          //PARAM2

      byte lrc = 0x00;
      for(int l=0; l<7; ++l)
         lrc ^= WriteBuffer[l];

      WriteBuffer[7] = lrc;      //LRC


      mChangeBuf = false; //不需再改變WriteBuffer的值
    }

    // 需要一直問，USB才會回傳
    mResult = USB.WriteReport(WriteBuffer,CODE_LEN);
    if(mResult!=0) {
      String temp = "WriteReport Result: ";
      temp += mResult;

      if(mResult==1)  // 已是 WriteReport => WAIT_TIMEOUT => CancelIo 的結果
      { temp += ", USB WAIT_TIMEOUT"; NotMain=true; }
      else if(mResult==1637608)
        temp += ", USB狀態改變中";
      else if(mResult==1636680) // Win7 x64
        temp += ", USB可能被拔除";

      MyLog(temp);
      return;
    }
    else
    {
      //fmMain->DisplayWriteBuffer();
      NotMain=false;
    }

    mResult = USB.ReadReport(ReadBuffer);
    if(mResult!=0) {
      String temp = "ReadReport Result:";
      MyLog(temp+mResult);
      return;
    }
    //else fmMain->DisplayReadBuffer();

    //有變化時,才show出來，以便Debug
    if(CheckChange) {
      new1=ReadBuffer[4]; //CMD
      new2=ReadBuffer[5]; //P1
      if(old1!=new1 || old2!=new2)
      {
        old1=new1; old2=new2;
        String s1 = IntToHex(old1,2);
        String s2 = IntToHex(old2,2);

        MyLog("[4][5] Changed: " + s1 + ", " + s2);

      }
    }

  } // end of if(DevConnected)

}
//---------------------------------------------------------------------------
void TUsbProc::SetSleepTime(int value)
{
  mSleepTime = value;
}
//---------------------------------------------------------------------------
int TUsbProc::GetSleepTime()
{
  return mSleepTime;
}
//---------------------------------------------------------------------------
void TUsbProc::SendCmd(Byte cmd)
{
  WriteBuffer[0] = BXB_PID; //PID
  WriteBuffer[1] = 0x00;           //MID
  WriteBuffer[2] = 0x00;          //DID2
  WriteBuffer[3] = 0x00;          //DID1
  WriteBuffer[4] = cmd;           //CMD
  WriteBuffer[5] = 0x00;          //PARM1
  WriteBuffer[6] = 0x00;          //PARM2

  byte lrc = 0x00;
  for(int l=0; l<7; ++l)
     lrc ^= WriteBuffer[l];
  WriteBuffer[7] = lrc;      //LRC

  this->Send = true;
}
//---------------------------------------------------------------------------
void TUsbProc::SetMemoLog(TMemo *memo)
{
  mMemo = memo;
}
//---------------------------------------------------------------------------
void TUsbProc::MyLog(String str)
{
  if(mMemo != NULL)
    mMemo->Lines->Add(str);
}
//---------------------------------------------------------------------------
