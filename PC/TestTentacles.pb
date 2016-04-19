; Tentacles inspired by 
; Driving With Tentacles: Integral Structures For Sensing And Motion
; by Felix von Hundelshausen, Michael Himmelsbach, Falk Hecker, Andre Mueller, And Hans-Joachim Wuensche

; First tests for Algorithms on PC



#Tentacle_resolution=200
#tentaclesperside=20
#vehicle_width=10
#CheckpointsPerTentacleStep=9 ; has to be odd
#checkpointsforcrash=2
#Max_Steering_Change=30
#Steering_Max_Angle=17
#Steering_Amplifier=15 
#image_width=750
#image_heigth=750
; maximum reached angle= #Steering_Amplifier*#Steering_Max_Angle

Structure Point
  x.i
  y.i
EndStructure

Structure S_Checkpoint Extends Point
  checkpoints.Point[#CheckpointsPerTentacleStep]
EndStructure

Structure S_tentacle
  Coordinate.S_Checkpoint[#Tentacle_resolution]
  crashsegment.i
  crashdistance.i
EndStructure

; Structure S_Path
;   Coordinate_l.Point[#Tentacle_resolution]
;   Coordinate_r.Point[#Tentacle_resolution]
; EndStructure

Structure S_Obstacle
  centerx.i
  centery.i
  List Vertex.Point()
EndStructure
  
Global Dim Tentacle.S_tentacle(#tentaclesperside*2)
; Global Dim Tentacle_Path.S_Path(#tentaclesperside*2)

Procedure generateTentacles(speed.d)
  speed*10/#Tentacle_resolution
  cursteering.d
  For steering=-#tentaclesperside To #tentaclesperside
    tentacleid=steering+#tentaclesperside
    targetsteering.d=steering*#Steering_Max_Angle/#tentaclesperside
    heading.d=0
    cursteering=0
    x.d=#image_width/2
    y.d=#image_heigth
    Tentacle(tentacleid)\Coordinate[0]\x=x
    Tentacle(tentacleid)\Coordinate[0]\y=y
    
    For t=1 To #Tentacle_resolution-1
      diff.d=targetsteering-cursteering

      If diff>#Max_Steering_Change/#Tentacle_resolution
        diff=#Max_Steering_Change/#Tentacle_resolution
      ElseIf diff<-#Max_Steering_Change/#Tentacle_resolution
        diff=-#Max_Steering_Change/#Tentacle_resolution
      EndIf

      
      cursteering+diff
      
      heading+cursteering/#Tentacle_resolution*#Steering_Amplifier
        
      x=x+speed*Sin(heading*#PI/180)
      y=y-speed*Cos(heading*#PI/180)
      With Tentacle(tentacleid)\Coordinate[t]
        \x=x
        \y=y
        
        checkhalf=Round(#CheckpointsPerTentacleStep/2,#PB_Round_Down)
        part_x.d=Cos(heading*#PI/180)
        part_y.d=Sin(heading*#PI/180)
        For checkpoint=0 To #CheckpointsPerTentacleStep-1
          factor.d=#vehicle_width/2*(checkpoint-checkhalf)/checkhalf
          check_x=x-part_x*factor ; -sin(heading+90)
          check_y=y-part_y*factor
          \checkpoints[checkpoint]\x=check_x
          \checkpoints[checkpoint]\y=check_y
        Next
      EndWith
      
    Next

  Next
  
;   For a=0 To 20-1
;     For t=1 To #Tentacle_resolution-1
;       x0=Tentacle(a)\Coordinate[t-1]
;       y0=Tentacle(a)\Coordinate[t-1]\y
;       x1=Tentacle(a)\Coordinate[t]\x
;       y1=Tentacle(a)\Coordinate[t]\y
;       
      ;- fillme!
;       Tentacle_Path(a)\Coordinate_l[t]\x=0
;       Tentacle_Path(a)\Coordinate_l[t]\y=0
;       Tentacle_Path(a)\Coordinate_r[t]\y=0
;       Tentacle_Path(a)\Coordinate_r[t]\y=0
;     Next
;   Next
EndProcedure

Procedure H_RandomObstacle(*targetobstacle.S_Obstacle)
  
  For a=0 To 5
    rad.d=#PI/3.0*a
    radius=Random(35)+25
    AddElement(*targetobstacle\Vertex())
    *targetobstacle\Vertex()\x=*targetobstacle\centerx+radius*Cos(rad)
    *targetobstacle\Vertex()\y=*targetobstacle\centery+radius*Sin(rad)
  Next
  
  
EndProcedure

Global NewList obstacles.S_Obstacle()

Procedure CheckTentacles()
  StartDrawing(ImageOutput(1))
  If DrawingBufferPixelFormat()<> #PB_PixelFormat_24Bits_RGB
    MessageRequester("Error", "Wrong pixel format ")
  EndIf
  For a=0 To #tentaclesperside*2
    matches=0
    For t=0 To #Tentacle_resolution-1
      matches=0
      For check=0 To #CheckpointsPerTentacleStep-1
        
        If Tentacle(a)\Coordinate[t]\checkpoints[check]\x>0 And Tentacle(a)\Coordinate[t]\checkpoints[check]\x<#image_width And Tentacle(a)\Coordinate[t]\checkpoints[check]\y<#image_heigth And Tentacle(a)\Coordinate[t]\checkpoints[check]\y>0
          
          If Point(Tentacle(a)\Coordinate[t]\checkpoints[check]\x,Tentacle(a)\Coordinate[t]\checkpoints[check]\y)=$ff
            matches+1
          EndIf
        EndIf
      Next
      
      If matches>=#checkpointsforcrash
        Tentacle(a)\crashdistance=t
        Break 
      EndIf

    Next
    If matches<#checkpointsforcrash
      Tentacle(a)\crashdistance=10000
    EndIf
  Next

  StopDrawing()
EndProcedure

Procedure RefreshOutput(speedtest=0)
  drawhiskers=GetGadgetState(3)
  StartVectorDrawing(ImageVectorOutput(1))
  VectorSourceColor(RGBA(0,0,0,255))
  FillVectorOutput()
  
  VectorSourceColor(RGBA(255,255,255,255))
  MovePathCursor(0,0)
  DrawVectorText("0\0")
  
   ; Obstacles
  ForEach obstacles()
    FirstElement(obstacles()\Vertex())
    MovePathCursor(obstacles()\Vertex()\x,obstacles()\Vertex()\y)
    a=0
    While NextElement(obstacles()\Vertex())
      a+1
      AddPathLine(obstacles()\Vertex()\x,obstacles()\Vertex()\y)
      SaveVectorState()
      VectorSourceColor(RGBA(0, 255, 255, 255))

      DrawVectorText(Str(a))
      RestoreVectorState()
    Wend
    FirstElement(obstacles()\Vertex())
    AddPathLine(obstacles()\Vertex()\x,obstacles()\Vertex()\y)
    VectorSourceColor(RGBA(255, 0, 0, 255))
    StrokePath(5, #PB_Path_DiagonalCorner)
  Next
  StopVectorDrawing()
  CheckTentacles()
  If speedtest=0
    StartVectorDrawing(ImageVectorOutput(1))

    ; ########Tentacles
    ;  Whiskers
    If drawhiskers
      For a=0 To #tentaclesperside*2
        MovePathCursor(#image_width/2,#image_heigth-1)
        For t=0 To #Tentacle_resolution-1
          For check=0 To #CheckpointsPerTentacleStep-1
            cx=Tentacle(a)\Coordinate[t]\checkpoints[check]\x
            cy=Tentacle(a)\Coordinate[t]\checkpoints[check]\y
            MovePathCursor(cx,cy)
            AddPathLine(1,0,#PB_Path_Relative)
            VectorSourceColor(RGBA(255, 255, 0, 255))
            StrokePath(1)
            ResetPath()
          Next
        Next
      Next
    EndIf
    ; Tentacles themselves
    
    
    For a=0 To #tentaclesperside*2
      
      VectorSourceColor(RGBA(0, 255, 0, 255))
      MovePathCursor(#image_width/2,#image_heigth-1)
      
      For t=0 To #Tentacle_resolution-1
        x=Tentacle(a)\Coordinate[t]\x
        y=Tentacle(a)\Coordinate[t]\y
        AddPathLine(x,y)
        If Tentacle(a)\crashdistance=t
          StrokePath(4)
          MovePathCursor(x,y)
          VectorSourceColor(RGBA(255, 120, 0, 255))
        EndIf
      Next
      StrokePath(4)
    Next
    
    
    
    StopVectorDrawing()
    StartVectorDrawing(ImageVectorOutput(2))
    DrawVectorImage(ImageID(1))
    StopVectorDrawing()
    
    
    SetGadgetState(0,ImageID(2))
  EndIf
EndProcedure

Procedure RandomObstacles(number)
  ForEach obstacles()
    ClearStructure(@obstacles(),S_Obstacle)
  Next
  ClearList(obstacles())

  For a=1 To number
    AddElement(obstacles())
    InitializeStructure(obstacles(),S_Obstacle)
    obstacles()\centerx=Random(#image_width-200)+100
    obstacles()\centery=Random(#image_heigth-200)+100
    H_RandomObstacle(@obstacles())
  Next
EndProcedure




OpenWindow(1,0,0,1000,810,"Tentacles  Collision Test",#PB_Window_SystemMenu|#PB_Window_ScreenCentered)
ButtonGadget(1,5,5,300,25,"Generate new random obstacles")
TextGadget(99,310,5,150,25,"Vehicle Speed 0",#PB_Text_Center)
TrackBarGadget(2,470,5,200,25,0,100)
SetGadgetState(2,30)
CheckBoxGadget(3,700,5,90,5,"Draw whiskers")
SetGadgetState(3,1)
ImageGadget(0,5,40,#image_width, #image_heigth,0)
CreateImage(1,#image_width,#image_heigth)
CreateImage(2,#image_width,#image_heigth)
AddKeyboardShortcut(1,#PB_Shortcut_F5,1)
AddKeyboardShortcut(1,#PB_Shortcut_F6,2)

RandomObstacles(5)
generateTentacles(30)

Repeat
  event=WaitWindowEvent(10)
  a+1
  
  
  If recalctime<ElapsedMilliseconds()
    RefreshOutput()

    
    If speed <> oldspeed
      SetGadgetText(99, "Vehicle Speed "+Str(speed))
      generateTentacles(speed)
      oldspeed=speed
    EndIf
    recalctime=ElapsedMilliseconds()+2
  EndIf
  
  Select event
    Case #PB_Event_CloseWindow
      End
    Case #PB_Event_Gadget
      Select EventGadget()
        Case 1
          RandomObstacles(5)
          obstacleschanged=1
        Case 2
          speed=GetGadgetState(2)
         
      EndSelect
    Case #PB_Event_Menu
      Select EventMenu()
        Case 1
          RandomObstacles(5)
        Case 2
          starttime=ElapsedMilliseconds()
          tmp=0
          Repeat
            tmp+1
            RandomObstacles(5)
            RefreshOutput(1)
          Until ElapsedMilliseconds()-starttime>1000
          MessageRequester("Info","Performed "+Str(tmp)+" Tentacle checks in a Second")
      EndSelect
  EndSelect
  
Until quit=1
; IDE Options = PureBasic 5.41 LTS (Linux - x64)
; CursorPosition = 81
; FirstLine = 75
; Folding = h
; EnableXP
; Executable = /home/macros/ram/TentacleTest2
; DisableDebugger
; EnableCompileCount = 2
; EnableBuildCount = 0