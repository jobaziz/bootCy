@echo off

REM Update_Image_Builder
REM   0. CycloneBOOT ImageBuilder executable is located in "<root_path>Middlewares/Oryx/utils/ImageBuilder/bin/"
REM   1. Make sure to set the CycloneBOOT ImageBuilder executable path in the variable bellow according your installation path

set "PATH=%PATH%;..\..\..\..\..\..\utils\ImageBuilder\bin"

setlocal enabledelayedexpansion

:: The Python script to run
set "script=.\misc\get_version.py"

:: The file to extract version from
set "file=..\src\version.h"

:: Run the Python script and capture its output
for /f "delims=" %%i in ('python "%script%" "%file%"') do (
    set "version=%%i"
)

:: Change version format : replace '.' with '_'
set "version=%version:.=_%"

:: Generate an update image with a ciphered firmware (using AES-CBC) and a signature (using RSA-SHA256)
image_builder_windows.exe --input ..\iap_single_bank_http_server_demo_%version%.bin --output ../iap_single_bank_http_server_demo_%version%.img --enc-algo aes-cbc --enc-key aa3ff7d43cc015682c7dfd00de9379e7 --sign-algo rsa-sha256 --sign-key ../resources/keys/rsa_private_key.pem --vtor-align

endlocal

pause
