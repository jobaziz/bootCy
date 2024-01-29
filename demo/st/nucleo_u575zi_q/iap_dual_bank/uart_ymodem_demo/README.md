# Getting started

This CycloneBOOT project demonstrates a secure firmware update mechanism over UART using In-Application Programming (IAP). IAP is a method of updating firmware using an update library stored in the user application.

This demo makes use of the MCU's dual bank flash functionality. One bank holds the current application firmware while the other bank will serve to store the new application firmware. If the update process is successful, then a swap between the two flash banks will be performed to boot to the new firmware.

You will find in this demo project a Y-Modem client (from STMicroelectronics) bundled with our CycloneBOOT Update Library: 

- Y-Modem client will receive an update image from a host PC through a serial terminal software (ex. Tera Term) 
- The CycloneBOOT Update Library will process and install the update image in next available bank
- If the update process was a success, the MCU will reboot and start the updated firmware

For the following steps, you can choose either the provided **"Helper Script"** using Python and batch files (on Windows), or the **"Manual Method"** using commands through a console.

## Step #0 : Tool Prerequisites 

### STM32CubeProgrammer

- Subsequent steps will use the STM32CubeProg utility provided by STMicroelectronics to program the evaluation board. 
  - Download and install STM32CubeProgrammer: https://www.st.com/en/development-tools/stm32cubeprog.html

### CycloneBOOT ImageBuilder

Before loading the new firmware onto your device, it must be “packaged” to a CycloneBOOT-compatible format. We provide a command-line interface (CLI) tool named ImageBuilder located in `utils/ImageBuilder/bin/` directory. This utility can encrypt the new firmware (if required) and compute either an integrity tag, an authentication tag or a signature. You will find more information about using this utility in the subsequent steps.

### Python 

- For convenience of evaluation of our demonstration project, we provide several scripts to automate build/flash process. If you plan to use the **"Helper Script"**, some of these scripts require Python to be installed.
  - Download and install Python (v3.0+): https://www.python.org/downloads/ 

### Windows Console

If you choose the **"Manual Method"** in the subsequent steps, all manual commands must be executed in a in Windows Command Line (cmd) console: 

1. Open Windows Command Line (cmd) console from `iap_dual_bank/uart_ymodem_demo/` directory
2. Set the STM32CubeProgrammer executable path: `set "PATH=%PATH%;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\"`
3. Set the ImageBuilder executable path: `set "PATH=%PATH%;..\..\..\..\..\utils\ImageBuilder\bin\"`

> Warning: do not close this Windows console, otherwise you will loose the path configuration!

## Step #1 : Board Configuration

In this step, we will configure the MCU to run in dual bank mode (dual bank flash).

**Helper Script**:

1. Connect the evaluation board to your computer
2. Navigate to `scripts/` directory
3. [Optional] Open `configure_dual_bank_mode.bat` and ensure that the STM32CubeProgrammer executable path is valid
4. Run `configure_dual_bank_mode.bat` 
5. Your board is now ready with dual bank mode activated

**Manual Method** 

1. Connect the evaluation board to your computer
2. Run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -ob DBANK=1 SWAP_BANK=0 NSBOOTADD0=0x100000 NSBOOTADD1=0x100200`
3. Your board is now ready with dual bank mode activated

## Step #2 : Generate Initial Firmware Binary (v1.0.0)

1. Open the demo project with your IDE
2. Check the macros in the `version.h` file located in the `src/` directory. The default application version is set to `1.0.0`.

```c
//Version string
#define APP_VERSION_STRING "1.0.0"
//Major version
#define APP_MAJOR_VERSION 1
//Minor version
#define APP_MINOR_VERSION 0
//Revision number
#define APP_REV_NUMBER 0
```

2. Build the project
3. As we use a post-compilation routine which adds the version number to the filename and save the resulting .bin file to the root of the demo folder, you should see the generated binary file `uart_ymodem_demo_1_0_0.bin` in `iap_dual_bank/uart_ymodem_demo/` directory

## Step #3 : Load Initial Firmware Binary (v1.0.0) 

Once the application firmware generated, load it into the first flash bank at address `0x08000000`.

**Helper Script**:

1. Navigate to `scripts/` directory
2. [Optional] Open `load_initial_firmware.bat` and ensure that the STM32CubeProgrammer executable path is valid
3. Run `load_initial_firmware.bat`
4. The Windows console will provide feedback on whether this process succeeded or not, along with any relevant log messages

**Manual Method**

- Use your IDE to load the firmware generated in *Step #2*
- Or run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -d uart_ymodem_demo_1_0_0.bin 0x08000000 -halt -rst`

## Step #4 : Generate New Firmware Binary (v2.0.0)

1. Open the demo project with your IDE
2. Modify the macros in the `version.h` file located in the `src/` directory by updating the version to `v2.0.0`.

```c
//Version string
#define APP_VERSION_STRING "2.0.0"
//Major version
#define APP_MAJOR_VERSION 2
//Minor version
#define APP_MINOR_VERSION 0
//Revision number
#define APP_REV_NUMBER 0
```

2. Rebuild the project
3. You should see the generated binary file `uart_ymodem_demo_2_0_0.bin` in `iap_dual_bank/uart_ymodem_demo/` directory

## Step #5 : Generate Firmware Update Image (related to v2.0.0) using ImageBuilder

For demo purposes, we will generate an update image with encrypted firmware (using AES-CBC) and a signature (using RSA-SHA256). The CycloneBOOT Update Library embedded in the UART/Y-Modem demo will expect an update image with this specific configuration.

The next stage involves the use of ImageBuilder CLI utility for transforming the binary file (.bin) generated in the previous step into an image file (.img) compatible with CycloneBOOT Update Library:

**Helper Script**

1. Navigate to `scripts/` directory
2. [Optional] Open `generate_update_image.bat` and ensure that the ImageBuilder executable path is valid
3. Run `generate_update_image.bat` in `scripts/` directory
4. You should see the generated image file `uart_ymodem_demo_2_0_0.img` in `iap_dual_bank/uart_ymodem_demo/` directory

**Manual Method**

1. Run the following command in the Windows console: `image_builder_windows.exe --input uart_ymodem_demo_2_0_0.bin --output uart_ymodem_demo_2_0_0.img --enc-algo aes-cbc --enc-key aa3ff7d43cc015682c7dfd00de9379e7 --sign-algo rsa-sha256 --sign-key .\resources\keys\rsa_private_key.pem`
2. You should see the generated image file `uart_ymodem_demo_2_0_0.img` in `iap_dual_bank/uart_ymodem_demo/` directory

## Step #6 : Upload Firmware Update Image (related to v2.0.0) to the board

> Note: UART/Y-Modem demo uses two separate U(S)ART interfaces for the trace outputs and for the update process. The following steps will therefore utilize the two distinct UARTs : 
>
> - USART1 : Trace (using Virtual COM / ST-Link) on pins PA10 (Rx) and PA9 (Tx).
> - USART2 : Firmware update (using a UART-USB converter) on pins PD6 (Rx) and PD5 (Tx).

#### Setup Trace UART (USART1)

1. Open your favourite serial console (115200 bauds)
2. Connect to the board
3. Click on the reset button to restart the demo
4. You should obtain a greeting message with the current app version (v1.0.0).

#### Setup Update UART (USART2)

1. Open Tera Term (or any other Y-Modem compatible serial terminal client with 115200 baud)
2. Connect to the board
3. Click on the reset button to restart the demo
4. You should obtain a simple command interpreter, with two options : 
   1. Press '1' to upload a new firmware image
   2. Press '2' to obtain the current firmware version
5. Press '1' to upload the firmware image generated in step #5 (uart_ymodem_demo_2_0_0.img).
   1. If using Tera Term, select `File > Transfer > YMODEM > Send ...` and select the update image to be sent to the board
   2. A progress bar should appear (alongside other information such as speed) on Tera Term client. The progress bar will reach 100% and should automatically disappear after few seconds
   3. Once the update process is complete, the MCU should restart automatically and you should obtain the greeting message reflecting the new app version (v2.0.0)

Now, you can proceed to replicate *Step #4* to *Step #6* to generate new update image files with different versions and additional application features.



## Troubleshooting

> Q: My debugger cannot connect to my board after an update. 

**A**: One of the reasons for this could be that MCU has activated the "swap bank" feature following an update, meaning the current running app might not be running from the bank which debugger expects by default. If this is the case, we recommend using STM32CubeProgrammer to re-initialize Option Bytes (Option Bytes -> User Configuration -> uncheck Swap Banks checkbox). 
