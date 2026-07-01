@REM This is here because cmake is trash

@echo off
title Building TraceR.sln - Release
cls

:: Check for the --rebuild argument
set "TARGET_ACTION=Build"
if /i "%~1"=="--rebuild" (
    set "TARGET_ACTION=Rebuild"
)

:: Locate Visual Studio 2019 MSBuild using vswhere
set "MSBUILD_PATH="
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -version [16.0^,17.0^) -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
    set "MSBUILD_PATH=%%i\MSBuild\Current\Bin\MSBuild.exe"
)

:: Fallback check if vswhere didn't find it
if not exist "%MSBUILD_PATH%" (
    if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
    ) else if exist "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe" (
        set "MSBUILD_PATH=%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Enterprise\MSBuild\Current\Bin\MSBuild.exe"
    )
)

:: Verify MSBuild exists before running
if not exist "%MSBUILD_PATH%" (
    color 0C
    echo [ERROR] Visual Studio 2019 MSBuild could not be found!
    echo Please ensure Visual Studio 2019 is installed correctly.
    goto end
)

echo [INFO] Found MSBuild at: "%MSBUILD_PATH%"
echo [INFO] Target Action: %TARGET_ACTION%
echo [INFO] Starting Release build for TraceR.sln...
echo -----------------------------------------------------------------

:: Run MSBuild with the dynamic target action
"%MSBUILD_PATH%" build/TraceR.sln /p:Configuration=Release /p:Platform="x64" /m

if %ERRORLEVEL% NEQ 0 (
    color 0C
    echo -----------------------------------------------------------------
    echo [ERROR] Release %TARGET_ACTION% failed! Check the errors listed above.
) else (
    color 0A
    echo -----------------------------------------------------------------
    echo [SUCCESS] Release %TARGET_ACTION% completed successfully!
)

:end
pause

.\build\bin\Release\SandboxApp.exe