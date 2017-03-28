# Microsoft Developer Studio Project File - Name="wxRaytracer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=wxRaytracer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxRaytracer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxRaytracer.mak" CFG="wxRaytracer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxRaytracer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "wxRaytracer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D WXDEBUG=1 /D "__WIN32__" /D "__WXMSW__" /D WINVER=0x0400 /D "STRICT" /Fo"$(IntDir)/" /Fd"$(IntDir)/vc70.pdb" /I /I /I /TP /GZ "" wxWidgets\include"" "" wxWidgets\mswd"" "" wxWidgets\include"" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "wxwidgets\msw" /I "wxwidgets\include" /I "raytracer\BRDFs" /I "raytracer\BTDFs" /I "raytracer\build" /I "raytracer\Cameras" /I "raytracer\GeometricObjects" /I "raytracer\Lights" /I "raytracer\Mappings" /I "raytracer\Materials" /I "raytracer\Noises" /I "raytracer\Samplers" /I "raytracer\Textures" /I "raytracer\Tracers" /I "raytracer\UserInterface" /I "raytracer\Utilities" /I "raytracer\World" /D "WIN32" /D "NDEBUG" /D "__WIN32__" /D "__WXMSW__" /D WINVER=0x0400 /D "STRICT" /Fo"Debug\wxRaytracer/" /Fd"Debug\wxRaytracer.pdb" /c
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d WXDEBUG=1 /d "WIN32" /d "__WIN32__" /d "__WXMSW__" /d WINVER=0x0400 /d "STRICT" wxWidgets\include""
# ADD RSC /l 0x409 /i "wxWidgets\msw" /i "wxWidgets\include" /i "." /i "wxWidgets" /d "_DEBUG" /d "__WXDEBUG__" /d WXDEBUG=1 /d "WIN32" /d "__WIN32__" /d "__WXMSW__" /d WINVER=0x0400 /d "STRICT" wxWidgets\include""
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wxbase28d_net.lib wxmsw28d_core.lib wxmsw28d_adv.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib winmm.lib comctl32.lib rpcrt4.lib wsock32.lib oleacc.lib odbc32.lib kernel32.lib user32.lib gdi32.lib ole32.lib oleaut32.lib comdlg32.lib uuid.lib advapi32.lib shell32.lib "wxWidgets\lib" " /nologo /subsystem:windows /debug /machine:IX86 /nodefaultlib:"MSVCRT.lib" /nodefaultlib:"LIBCD.lib" /nodefaultlib:"LIBCMTD.lib" /nodefaultlib:"LIBC.lib" /nodefaultlib:"LIBCMT.lib" /pdbtype:sept /libpath:""wxWidgets\lib""
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib winmm.lib comctl32.lib rpcrt4.lib wsock32.lib oleacc.lib odbc32.lib kernel32.lib user32.lib gdi32.lib ole32.lib oleaut32.lib comdlg32.lib uuid.lib advapi32.lib shell32.lib /nologo /subsystem:windows /debug /machine:IX86 /nodefaultlib:"MSVCRT.lib" /nodefaultlib:"LIBCD.lib" /nodefaultlib:"LIBCMTD.lib" /nodefaultlib:"LIBC.lib" /nodefaultlib:"LIBCMT.lib" /pdbtype:sept /libpath:"wxwidgets\lib" /libpath:"wxWidgets\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc_msw"
# PROP BASE Intermediate_Dir "vc_msw\wxRaytracer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I "C:\wxWidgets-2.8.0\lib\vc_lib\msw" /I "C:\wxWidgets-2.8.0\include" /I "." /I "C:\wxWidgets-2.8.0\samples" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /Fd"vc_msw\wxRaytracer.pdb" /FD /EHsc /c
# ADD CPP /nologo /MD /W4 /GR /Zi /O2 /I "wxwidgets\msw" /I "wxwidgets\include" /I "raytracer\BRDFs" /I "raytracer\BTDFs" /I "raytracer\build" /I "raytracer\Cameras" /I "raytracer\GeometricObjects" /I "raytracer\Lights" /I "raytracer\Mappings" /I "raytracer\Materials" /I "raytracer\Noises" /I "raytracer\Samplers" /I "raytracer\Textures" /I "raytracer\Tracers" /I "raytracer\UserInterface" /I "raytracer\Utilities" /I "raytracer\World" /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /Fo"Release\wxRaytracer/" /Fd"Release\wxRaytracer.pdb" /FD /EHsc /c
# ADD BASE MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "WIN32" /D "__WXMSW__" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i "C:\wxWidgets-2.8.0\lib\vc_lib\msw" /i "C:\wxWidgets-2.8.0\include" /i "." /i "C:\wxWidgets-2.8.0\samples" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "wxWidgets\msw" /i "wxWidgets\include" /i "." /i "wxWidgets" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:"wxWidgets\lib"
# ADD LINK32 wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:"wxwidgets\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "wxRaytracer - Win32 Debug"
# Name "wxRaytracer - Win32 Release"
# Begin Group "BRDFs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\BRDFs\BRDF.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\BRDFs\BRDF.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\BRDFs\Lambertian.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\BRDFs\Lambertian.h
# End Source File
# End Group
# Begin Group "BTDFs"

# PROP Default_Filter ""
# End Group
# Begin Group "Cameras"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\Cameras\Camera.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Cameras\Camera.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Cameras\Pinhole.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Cameras\Pinhole.h
# End Source File
# End Group
# Begin Group "Geometric Objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\GeometricObject.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\GeometricObject.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\Plane.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\Plane.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\Sphere.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\GeometricObjects\Sphere.h
# End Source File
# End Group
# Begin Group "Lights"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\Lights\Ambient.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Lights\Ambient.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Lights\Directional.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Lights\Directional.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Lights\Light.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Lights\Light.h
# End Source File
# End Group
# Begin Group "Mappings"

# PROP Default_Filter ""
# End Group
# Begin Group "Materials"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\Materials\Material.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Materials\Material.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Materials\Matte.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Materials\Matte.h
# End Source File
# End Group
# Begin Group "Noises"

# PROP Default_Filter ""
# End Group
# Begin Group "Samplers"

# PROP Default_Filter ""
# End Group
# Begin Group "Textures"

# PROP Default_Filter ""
# End Group
# Begin Group "Tracers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\Tracers\MultipleObjects.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\MultipleObjects.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\RayCast.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\RayCast.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\SingleSphere.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\SingleSphere.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\Tracer.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Tracers\Tracer.h
# End Source File
# End Group
# Begin Group "User Interface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\UserInterface\wxraytracer.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\UserInterface\wxraytracer.h
# End Source File
# End Group
# Begin Group "Utilities"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\Utilities\Constants.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Maths.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Matrix.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Normal.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Normal.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Point2D.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Point2D.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Point3D.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Point3D.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Ray.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Ray.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\RGBColor.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\RGBColor.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\ShadeRec.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\ShadeRec.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Vector3D.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\Utilities\Vector3D.h
# End Source File
# End Group
# Begin Group "World"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\raytracer\World\ViewPlane.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\World\ViewPlane.h
# End Source File
# Begin Source File

SOURCE=.\raytracer\World\World.cpp

!IF  "$(CFG)" == "wxRaytracer - Win32 Debug"

!ELSEIF  "$(CFG)" == "wxRaytracer - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\raytracer\World\World.h
# End Source File
# End Group
# End Target
# End Project
