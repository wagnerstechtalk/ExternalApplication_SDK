@echo off

REM Check if the Docker image already exists
docker image inspect arm-a53-toolchain:latest >nul 2>&1
if not %errorlevel%==0 (
  docker build -t arm-a53-toolchain:latest .
)

if not "%~1"=="" if exist "media\%~1" (
  docker run -it -v "%cd%\media:/workspace" arm-a53-toolchain:latest /bin/bash -c "./build.sh \"%~1\""
) else (
  docker run -it -v "%cd%\media:/workspace" arm-a53-toolchain:latest /bin/bash
)