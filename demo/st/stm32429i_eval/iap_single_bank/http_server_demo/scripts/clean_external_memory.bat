@echo off

REM STM32CubeProgrammer
REM   1. STM32CubeProgrammer must be installed first : https://www.st.com/en/development-tools/stm32cubeprog.html
REM   2. Set the STM32CubeProgrammer CLI executable path in the variable bellow according your installation path

:: STM32CubeProgrammer CLI executable absolute path
set "PATH=%PATH%;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin"

:: Locate STM32CubeProgrammer CLI executable
where STM32_Programmer_CLI.exe > tmpFile
SET /p STM32_CUBE_PROGRAMMER_PATH= < tmpFile
del tmpFile

:: Select STM32CubeProgrammer external flash loader (M29W128GL)
set EXTERNAL_FLASH_LOADER_NAME=M29W128GL_STM324x9I-EVAL.stldr
:: Format STM32CubeProgrammer external flash loader path
set EXTERNAL_FLASH_LOADER_PATH="%STM32_CUBE_PROGRAMMER_PATH%\..\ExternalLoader\%EXTERNAL_FLASH_LOADER_NAME%"

:: Clean external memory content
STM32_Programmer_CLI.exe -c port=SWD index=0 -halt -rst -el %EXTERNAL_FLASH_LOADER_PATH% -e all

echo Done.
pause
