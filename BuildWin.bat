@echo off
title Building TraceR - Release
cls

set "TARGET_ACTION=Build"
if /i "%~1"=="--rebuild" (
    set "TARGET_ACTION=Rebuild"
)

set "MSBUILD_PATH="
set "SOLUTION="

for /f "usebackq delims=" %%i in (`
"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" ^
-latest ^
-products * ^
-requires Microsoft.Component.MSBuild ^
-find MSBuild\**\Bin\MSBuild.exe
`) do (
    set "MSBUILD_PATH=%%i"
)

if not exist "%MSBUILD_PATH%" (
    color 0C
    echo [ERROR] MSBuild could not be found.
    goto end
)

if exist "build\TraceR.slnx" (
    set "SOLUTION=build\TraceR.slnx"
) else if exist "build\TraceR.sln" (
    set "SOLUTION=build\TraceR.sln"
) else (
    color 0C
    echo [ERROR] Could not find TraceR.sln or TraceR.slnx in the build directory.
    goto end
)

echo [INFO] Found MSBuild at: "%MSBUILD_PATH%"
echo [INFO] Using Solution: "%SOLUTION%"
echo [INFO] Target Action: %TARGET_ACTION%
echo -----------------------------------------------------------------

"%MSBUILD_PATH%" "%SOLUTION%" ^
    /t:%TARGET_ACTION% ^
    /p:Configuration=Release ^
    /p:Platform=x64 ^
    /m

if %ERRORLEVEL% NEQ 0 (
    color 0C
    echo -----------------------------------------------------------------
    echo [ERROR] Release %TARGET_ACTION% failed! Check the errors listed above.
) else (
    color 0A
    echo -----------------------------------------------------------------
    echo [SUCCESS] Release %TARGET_ACTION% completed successfully!

    if exist ".\build\bin\Release\SandboxApp.exe" (
        .\build\bin\Release\SandboxApp.exe
    ) else (
        echo [WARNING] SandboxApp.exe was not found.
    )
)

:end
pause