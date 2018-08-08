//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UnitUsbProc.h"
#include "uMain.h"

#define BXB_PID 0xFF // �v��



extern byte WriteBuffer[USB_BUF_SIZE] = {0};
extern byte ReadBuffer[USB_BUF_SIZE]  = {0};

byte old1=0,old2=0,new1=0,new2=0; //Debug�ΨӬݦ��쪺��ƥ�
//---------------------------------------------------------------------------
__fastcall TUsbProc::TUsbProc(bool CreateSuspended)
        : TThread(CreateSuspended)
{
  mResult = 0;
  mSleepTime = 100;  //�j��100ms�|����P��GUI�������ܺC�A�p��40�S�|�ɭP�Y�Ƕǿ�|loss

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
    Synchronize(DoWork); // �� GUI �n�� Synchronize
    Sleep(mSleepTime);
  }
}
//---------------------------------------------------------------------------
//�]��Overlapped IO�ݭn���ݵ��G�A�Y�g�bfmMain���ܡA�����|�d�d
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
      WriteBuffer[4] = CMD_STATE;     //CMD  : �d�ߪ��A
      WriteBuffer[5] = 0x00;          //PARAM1
      WriteBuffer[6] = 0x00;          //PARAM2

      byte lrc = 0x00;
      for(int l=0; l<7; ++l)
         lrc ^= WriteBuffer[l];

      WriteBuffer[7] = lrc;      //LRC


      mChangeBuf = false; //���ݦA����WriteBuffer����
    }

    // �ݭn�@���ݡAUSB�~�|�^��
    mResult = USB.WriteReport(WriteBuffer,CODE_LEN);
    if(mResult!=0) {
      String temp = "WriteReport Result: ";
      temp += mResult;

      if(mResult==1)  // �w�O WriteReport => WAIT_TIMEOUT => CancelIo �����G
      { temp += ", USB WAIT_TIMEOUT"; NotMain=true; }
      else if(mResult==1637608)
        temp += ", USB���A���ܤ�";
      else if(mResult==1636680) // Win7 x64
        temp += ", USB�i��Q�ް�";

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

    //���ܤƮ�,�~show�X�ӡA�H�KDebug
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
