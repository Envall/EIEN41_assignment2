@echo off
set MATLAB=C:\Program Files\MATLAB\R2007b
"%MATLAB%\bin\win32\gmake" -f Parallel.mk  GENERATE_REPORT=0
