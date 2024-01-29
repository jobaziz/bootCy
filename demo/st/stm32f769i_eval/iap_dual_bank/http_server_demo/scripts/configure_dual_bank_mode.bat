@echo off

REM STM32CubeProgrammer
REM   1. STM32CubeProgrammer must be installed first : https://www.st.com/en/development-tools/stm32cubeprog.html
REM   2. Set the STM32CubeProgrammer CLI executable path in the variable bellow according your installation path

:: STM32CubeProgrammer CLI executable absolute path
set "PATH=%PATH%;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"

:: Configure MCU in dual bank mode
STM32_Programmer_CLI.exe -c port=SWD index=0 -ob nDBANK=0 nDBOOT=0 BOOT_ADD0=0x2000 BOOT_ADD1=0x2040

echo Done.
pause
