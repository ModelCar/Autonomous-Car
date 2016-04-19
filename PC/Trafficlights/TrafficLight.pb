
res$=InputRequester("enter resolution","in form 1300x730","1300x730")
w=Val(StringField(res$,1,"x"))
h=Val(StringField(res$,2,"x"))
If w=0 Or h=0
  MessageRequester("error","invalid input, quitting")
  End
EndIf

time$=InputRequester("Timing","Time between switching in seconds","2")
waittime.d=ValD(time$)*1000
If waittime=0
  waittime=2000
EndIf

CreateImage(1,w,h)

Procedure drawLights(image,lightbitmask)
  StartDrawing(ImageOutput(image))
  w=ImageWidth(image)
  h=ImageHeight(image)
  Box(0,0,w,h,0)
  
  For a=0 To 2
    Select a
      Case 0
        color=$FF
      Case 1
        color=$FFFF
      Case 2
        color=$FF00
    EndSelect
    
    
    FrontColor(color)
    BackColor(RGB((Red(color)+$ff)/2,(Green(color)+$ff)/2,(Blue(color)+$ff)/2))
    ; make an offset because we specify circle _center_ and radius
    x=50+(a+0.5)*w/3.2
    y=h/2
    r=w/7
    
    If lightbitmask & 1<<a
      ; make active lights shiny
      DrawingMode(#PB_2DDrawing_Gradient)  
      CircularGradient(x,y,r)
      Circle(x,y,r)
    Else
      DrawingMode(#PB_2DDrawing_Default) 
      darkcolor=RGB(Red(color)/8,Green(color)/8,Blue(color)/8)
      Circle(x,y,r,darkcolor)
    EndIf
    
  Next
  
  StopDrawing()
EndProcedure


OpenWindow(1, 0,0,ImageWidth(1),ImageHeight(1),"Traffic lights",#PB_Window_ScreenCentered|#PB_Window_SystemMenu)
ImageGadget(1,0,0,ImageWidth(1),ImageHeight(1),ImageID(1))

switchtime=ElapsedMilliseconds()

Repeat
  event=WaitWindowEvent(10)
  
  If switchtime<ElapsedMilliseconds()
    switchtime+waittime
    counter+1
    Select counter%4
      Case 0
        drawLights(1,%1)
      Case 1
        drawLights(1,%11)
      Case 2
        drawLights(1,%100)
      Case 3
         drawLights(1,%10)
    EndSelect
    SetGadgetState(1,ImageID(1))
  EndIf
  
  
Until event=#PB_Event_CloseWindow




; IDE Options = PureBasic 5.41 LTS (Linux - x64)
; CursorPosition = 62
; FirstLine = 41
; Folding = -
; EnableXP
; Executable = /home/macros/ram/TrafficLights