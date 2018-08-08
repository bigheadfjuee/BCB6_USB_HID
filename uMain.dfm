object FormMain: TFormMain
  Left = 425
  Top = 225
  Width = 473
  Height = 343
  Caption = 'ICP-5011Test'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  OnClose = FormClose
  PixelsPerInch = 120
  TextHeight = 16
  object Memo1: TMemo
    Left = 8
    Top = 8
    Width = 401
    Height = 177
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Lines.Strings = (
      'Memo1')
    ParentFont = False
    TabOrder = 0
  end
  object btnSend: TButton
    Left = 8
    Top = 200
    Width = 81
    Height = 33
    Caption = 'Send'
    TabOrder = 1
    OnClick = btnSendClick
  end
end
