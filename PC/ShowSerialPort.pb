If OpenSerialPort(1,"/dev/ttyUSB0",115200,#PB_SerialPort_NoParity,8,1,#PB_SerialPort_NoHandshake,4096,1024)
  string$=Space(1000)
  Repeat
    Delay(3)
    chars=AvailableSerialPortInput(1)
    If chars
      input=1
      ReadSerialPortData(1,@string$+pos,chars)
      pos+chars
    ElseIf input=1
      input=0
      Debug Left(string$,pos)
      If Left(string$,3)="abc"
        WriteSerialPortString(1,"def")
      EndIf
      pos=0
    EndIf 
  ForEver
EndIf

; IDE Options = PureBasic 5.31 (Linux - x64)
; CursorPosition = 3
; EnableXP