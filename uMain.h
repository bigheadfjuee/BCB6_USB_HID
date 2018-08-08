//---------------------------------------------------------------------------

#ifndef uMainH
#define uMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "UnitUsbProc.h"
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
  TMemo *Memo1;
  TButton *btnSend;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall btnSendClick(TObject *Sender);

private:	// User declarations
  TUsbProc *UsbProc;
  void __fastcall MyWndProc(Messages::TMessage &Message);
  void DetectUSB();

public:		// User declarations
  __fastcall TFormMain(TComponent* Owner);
  void EnableBtn(bool active);

};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
