@echo off

REM Update_Image_Builder
REM   0. CycloneBOOT ImageBuilder executable is located in "<root_path>Middlewares/Oryx/utils/ImageBuilder/bin/"
REM   1. Make sure to set the CycloneBOOT ImageBuilder executable path in the variable bellow according your installation path

set "PATH=%PATH%;..\..\..\..\..\..\utils\ImageBuilder\bin"

:: Generate a bootable image (with plaintext binary and simple crc32)
image_builder_windows.exe --input ..\iap_single_bank_http_server_demo_1_0_0.bin --output ../iap_single_bank_http_server_demo_1_0_0_bootable.bin --integrity-algo crc32 --vtor-align

pause
