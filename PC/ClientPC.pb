InitNetwork()
; 192.168.16.49
id = OpenNetworkConnection("10.42.0.57",9999)

If Not id
  Debug "error, could not connect, will quit"
  End
EndIf


; Read from network till a byte with 255 occurs and return the data
Procedure.s NetworkReadLine(id)
  *buffer=AllocateMemory(1)
  starttime=ElapsedMilliseconds()
  Repeat
    result=ReceiveNetworkData(id,*buffer, 1)
    If result >0
      out$+PeekS(*buffer,1)
    Else
      Debug "Transmission terminated before newline"
      ProcedureReturn ""
    EndIf
  Until Right(out$,1)=Chr($ff) Or ElapsedMilliseconds()>starttime+1000
  If ElapsedMilliseconds()>starttime+1000
    Debug "Reading timeout, received so far:" +out$
    ProcedureReturn 
  EndIf
  
  If out$=Chr($ff)
    Debug "Newline without Data!"
    ProcedureReturn ""
  EndIf
  ProcedureReturn Left(out$,Len(out$)-1)
EndProcedure

; Send a command over the network. 
; Conid=Id of the Network connection
; command = id of command, 1 for speed, 2 for heading
; *daten= pointer to data
; lendaten = length of data
Procedure SendNetworkCommand(conid,command,*daten,lendaten)
  *buffer=AllocateMemory(lendaten+2)
  PokeB(*buffer,command)
  For a=0 To lendaten-1
    PokeB(*buffer+1+a,PeekB(*daten+a))
  Next
  PokeB(*buffer+lendaten+1,$ff)
  
  SendNetworkData(conid,*buffer,lendaten+2)
EndProcedure

; Dummy for now.
Procedure ParseMessage(*message,length)
EndProcedure


OpenWindow(1,0,0,800,600,"Autonomous Car Control")

; Constant which make GUI (Graphical User Interface) elements more easyly recognizable
Enumeration 
  #Speed =1
  #Heading
EndEnumeration

Enumeration 
  #Speedshow=100
  #headingShow
  #Messageshow
EndEnumeration

; Creating GUI elements
TrackBarGadget(#Speed,5,5,30,560,0,200,#PB_TrackBar_Vertical)
TrackBarGadget(#Heading,5,570,600,25,0,200)
SetGadgetState(#Speed,100)
SetGadgetState(#Heading,100)
TextGadget(#Speedshow,50,15,100,25,"Speed: 0")
TextGadget(#headingShow,50,45,100,25,"Heading: 0")
ListViewGadget(#Messageshow,400,5,390,565)

; Main loop
Repeat
  nevent=NetworkClientEvent(id)
  event=WindowEvent()
  Delay(0)
  
  tmptime=ElapsedMilliseconds()
  If tmptime-oldtmptime>1000
    *tmpdata=AllocateMemory(3)
    PokeB(*tmpdata,1)
    PokeB(*tmpdata+1,100)
    SendNetworkCommand(id,01,*tmpdata,2)
    oldtmptime=tmptime
  EndIf
  
  ; check if somethign happened on the network
  Select nevent
    Case #PB_NetworkEvent_Disconnect ; disconnect from server
      If disconnected=0
        disconnected=1
        SetWindowTitle(1,GetWindowTitle(1)+" ---- Disconnected, restart server and program!")
      EndIf
      ; disable user interface in an ugly way 
      For a=0 To 9999
        If IsGadget(a) 
          DisableGadget(a,1)
        EndIf
      Next
    Case #PB_NetworkEvent_Data       ; We have received Data
      message$=NetworkReadLine(id)
      If message$
        ParseMessage(@message$,Len(message$))
        AddGadgetItem(#Messageshow,-1,Str(ElapsedMilliseconds())+Chr(9)+message$ )
        If CountGadgetItems(#Messageshow)>20
          RemoveGadgetItem(#Messageshow,0)
        EndIf
      EndIf
  EndSelect
  
  
  ; Check if something happened on the window
  Select event
      ; somebody touched/changed/clicked a button/trackbar...
    Case #PB_Event_Gadget
      ; get the GUI element which produced the event
      Select EventGadget()
        Case #Speed
          speed.a=GetGadgetState(#Speed)
          ; some code to send 100 once if there is a change but the value is between 75 and 107
          If speed>75 And speed<107 
            If lastspeedsetting<>speed
              lastspeddsetting=speed
              oldspeed=0
            EndIf
            speed=100
          EndIf
          If speed<>oldspeed 
            oldspeed=speed
            SendNetworkCommand(id,#Speed,@speed,1)
            SetGadgetText(#Speedshow,"Speed: "+Str(speed-100))
          EndIf
        Case #Heading
          heading.a=GetGadgetState(#Heading)
          SendNetworkCommand(id,#Heading,@heading,1)
          SetGadgetText(#headingShow,"Heading: "+Str(GetGadgetState(#Heading)-100))
      EndSelect
  EndSelect
  
  ; a little break of two ms so we do not use up all the cpu power
  Delay(2)
Until event=#PB_Event_CloseWindow
; IDE Options = PureBasic 5.31 (Linux - x64)
; CursorPosition = 101
; FirstLine = 72
; Folding = +
; EnableXP
; Executable = /home/macros/ram/SerialTest