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
  //�d�I�t�ΰT���A��w��˸m���ܮɡA���s����USB
  if(Message.Msg == WM_DEVICECHANGE)
  {
    DetectUSB();
  }
  //�N�T���ٵ� Form ��ӳB�z�{��
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
    Memo1->Lines->Add("=== DetectUSB() �w������ ===");
  }
  else
  {
    EnableBtn(false);
    UsbProc->DevConnected = false;
    Memo1->Lines->Add("=== DetectUSB() �������� ===");
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

