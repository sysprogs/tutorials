@echo off
REM Run this file to build the project outside of the IDE.
REM WARNING: if using a different machine, copy the .rsp files together with this script.
echo pch.h
C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe @"VisualGDB/Debug/PCH/pch.h-CXX.gcc.rsp" || exit 1
echo PrecompiledHeaderDemo.cpp
C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe @"VisualGDB/Debug/PrecompiledHeaderDemo.gcc.rsp" || exit 1
echo Source.cpp
C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe @"VisualGDB/Debug/Source.gcc.rsp" || exit 1
echo Linking VisualGDB/Debug/PrecompiledHeaderDemo...
C:\SysGCC\raspberry\bin\arm-linux-gnueabihf-g++.exe @VisualGDB/Debug/PrecompiledHeaderDemo.link.rsp || exit 1
