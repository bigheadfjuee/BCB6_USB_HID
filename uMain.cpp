//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "uMain.h"
#include "UnitUsbProc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
  : TForm(Owner)
{
  UsbProc = new TUsbProc(true);
  UsbProc->FreeOnTerminate = true;
  UsbProc->SetMemoLog(Memo1);
  UsbProc->Resume();

  WindowProc = MyWndProc;
  DetectUSB();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
  UsbProc->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::MyWndProc(Messages::TMessage &Message)
{
  //攔截系統訊息，當硬體裝置改變時，重新偵測USB
  if(Message.Msg == WM_DEVICECHANGE)
  {
    DetectUSB();
  }
  //將訊息還給 Form 原來處理程序
  WndProc(Message);
}
//---------------------------------------------------------------------------
void TFormMain::DetectUSB()
{

  USB.FindTheHID();
  if(USB.DeviceDetected)
  {
    EnableBtn(true);
    UsbProc->DevConnected = true;
    Memo1->Lines->Add("=== DetectUSB() 已偵測到 ===");
  }
  else
  {
    EnableBtn(false);
    UsbProc->DevConnected = false;
    Memo1->Lines->Add("=== DetectUSB() 未偵測到 ===");
  }
}
//---------------------------------------------------------------------------
void TFormMain::EnableBtn(bool active)
{
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::btnSendClick(TObject *Sender)
{
  Memo1->Lines->Add("UsbProc->SendCmd");
  UsbProc->SendCmd(0x01);
  UsbProc->CheckChange = true;
}
//---------------------------------------------------------------------------

