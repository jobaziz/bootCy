# Getting started

This CycloneBOOT project demonstrates a secure firmware update mechanism over Ethernet, using In-Application Programming (IAP). IAP is a method of updating firmware using an update library stored in the user application.

This demo makes use of the MCU's internal flash and an external memory. The internal flash memory holds the current application firmware and also a static bootloader that is required to complete the update process. The external memory will be used to store the new application firmware. If the update process is successful with a valid update image, the system is rebooted and the static bootloader completes the update process by transferring the new application firmware from the external memory to the internal flash.

This type of architecture, also known as "Single Bank", is especially useful for MCUs without "Dual Bank" feature, in order to perform IAP-style firmware updates.

You will find in this demo project a HTTP server (from CycloneTCP stack) bundled with our CycloneBOOT Update Library: 
- The HTTP server will fetch the update image from the network interface
- The CycloneBOOT Update Library will process and store the update image in external memory
- If the update process was a success and the update image is valid, the MCU will reboot and the static bootloader will install the new firmware over the existing one in the internal flash

For the following steps, you can choose either the provided **"Helper Script"** using Python and batch files (on Windows), or the **"Manual Method"** using commands through a console.

## Step #0 : Tool Prerequisites 

### STM32CubeProgrammer
Subsequent steps will use the STM32CubeProg utility provided by STMicroelectronics to program the evaluation board. 
- Download and install STM32CubeProgrammer: https://www.st.com/en/development-tools/stm32cubeprog.html

### CycloneBOOT ImageBuilder
Before loading the new firmware onto your device, it must be “packaged” to a CycloneBOOT-compatible format. We provide a command-line interface (CLI) tool named ImageBuilder located in `utils/ImageBuilder/bin/` directory. This utility can encrypt the new firmware (if required) and compute either an integrity tag, an authentication tag or a signature. You will find more information about using this utility in the subsequent steps.

### Python 
For convenience of evaluation of our demonstration project, we provide several scripts to automate build/flash process. If you plan to use the **"Helper Script"**, some of these scripts require Python to be installed.
- Download and install Python (v3.0+): https://www.python.org/downloads/ 

### Windows Console
If you choose the **"Manual Method"** in the subsequent steps, all manual commands must be executed in a in Windows Command Line (cmd) console: 
1. Open Windows Command Line (cmd) console from `iap_dual_bank/http_server_demo/` directory
2. Set the STM32CubeProgrammer executable path: `set "PATH=%PATH%;C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\"`
3. Set the ImageBuilder executable path: `set "PATH=%PATH%;..\..\..\..\..\utils\ImageBuilder\bin\"`

> Warning: do not close this Windows console, otherwise you will loose the path configuration!

## Step #1 : Board Configuration

In this step, we'll ensure the MCU is operating in single bank mode, utilizing the entire internal flash without dividing it into separate banks.

**Helper Script**:
1. Connect the evaluation board to your computer
2. Navigate to `scripts/` directory
3. [Optional] Open `configure_single_bank_mode.bat` and ensure that the STM32CubeProgrammer executable path is valid
4. Run `configure_single_bank_mode.bat` 
5. Your board is now ready with single bank mode activated

**Manual Method**: 
1. Connect the evaluation board to your computer
2. Run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -ob nDBANK=1 nDBOOT=1 BOOT_ADD0=0x2000 BOOT_ADD1=0x2040`
3. Your board is now ready with single bank mode activated

## Step #2 : Generate Static Bootloader

When operating the MCU in single bank mode, a static bootloader is necessary. It has the following responsibilities:
- Check the validity of the current firmware in internal flash and boot it if it is the up-to-date version
- Check the availability of a potential new update image in external flash and, if available, copy it to the internal flash and then boot it

1. Open `iap_single_bank/bootloader/` project with your IDE
2. Build the project
3. You should see the generated binary file `iap_single_bank_bootloader.bin` in `iap_single_bank/bootloader/` directory

## Step #3 : Load Static Bootloader into Internal Flash

The bootloader **must** be programmed at the beginning of the flash (`0x08000000`).

**Helper Script**:
1. Navigate to `iap_single_bank/bootloader/scripts/` directory
2. [Optional] Open `load_bootloader.bat` and ensure that the STM32CubeProgrammer executable path is valid
3. Run `load_bootloader.bat`
4. The Windows console will provide feedback on whether this process succeeded or not, along with any relevant log messages

**Manual Method**: 
- Use your IDE to load the bootloader firmware generated in *Step #2*
- Or run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -d ../bootloader/iap_single_bank_bootloader.bin 0x08000000 -halt -rst` 

## Step #4 : Generate Initial Firmware Binary (v1.0.0)

1. Open `iap_single_bank/http_server_demo/` project with your IDE
2. Check the macros in the `version.h` file located in the `src/` directory. The default application version is set to `1.0.0`, and the web server page's background colour is configured as yellow (`#FFE082`).

```c
//Version string
#define APP_VERSION_STRING "1.0.0"
//Major version
#define APP_MAJOR_VERSION 1
//Minor version
#define APP_MINOR_VERSION 0
//Revision number
#define APP_REV_NUMBER 0
//Version color
#define APP_VERSION_COLOR "#FFE082" //YELLOW
```

3. Build the project
4. As we use a post-compilation routine which adds the version number to the filename and save the resulting .bin file to the root of the demo folder, you should see the generated binary file `iap_single_bank_http_sever_demo_1_0_0.bin` in `iap_single_bank/http_server_demo/` directory

> Note: As a static bootloader is placed at the beginning of the flash, the application firmware **must** be compiled to start with an offset. In our case, the application is configured to start at `0x08020400`. More information about this offset can be found in the next step.

## Step #5 : Generate Bootable Version of Initial Firmware Binary (v1.0.0) using ImageBuilder

We need to generate a **bootable version** of initial firmware binary before loading it into internal flash.

- Indeed, as a static bootloader is placed at the beginning of the flash, the application firmware **must** be written into internal flash with a specific offset that combines the **bootloader size** and a **VTOR offset** according to the MCU family.
- Moreover, the static bootloader expects some metadata (like integrity check data) to be packaged with the firmware binary before executing it.

For achieving this, we will use ImageBuilder CLI utility to tranform the initial firmware binary into a **bootable version** including the correct offset and the necessary metadata.

**Helper Script**:
1. Navigate to `scripts/` directory 
2. [Optional] Open `generate_bootable_initial_firmware.bat` and ensure that the ImageBuilder executable path is valid
3. Run `generate_bootable_initial_firmware.bat`
4. You should see the generated image (binary) file `iap_single_bank_http_server_demo_1_0_0_bootable.bin` in `iap_single_bank/http_server_demo/` directory

**Manual Method**:
1. Run the following command in the Windows console: `image_builder_windows.exe --input iap_single_bank_http_server_demo_1_0_0.bin --output iap_single_bank_http_server_demo_1_0_0_bootable.bin --integrity-algo crc32 --vtor-align`
2. You should see the generated image (binary) file `iap_single_bank_http_server_demo_1_0_0_bootable.bin` in `iap_single_bank/http_server_demo/` directory

## Step #6 : Load Bootable Version of Initial Firmware Binary (v1.0.0) into Internal Flash

**Helper Script**:
1. Navigate to `scripts/` directory 
2. [Optional] Open `load_bootable_initial_firmware.bat` and ensure that the STM32CubeProgrammer executable path is valid
3. Run: `load_bootable_initial_firmware.bat`

**Manual Method**:
- Run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -d iap_single_bank_http_server_demo_1_0_0_bootable.bin 0x08020000 -halt -rst`

## Step #7 : Generate New Firmware Binary (v2.0.0)

1. Open the demo project for your chosen IDE
2. Modify the macros in the `version.h` file within the `src/` directory. Change the version to `v2.0.0` and the background color to green (`#00FF00`).

```c
//Version string
#define APP_VERSION_STRING "2.0.0"
//Major version
#define APP_MAJOR_VERSION 2
//Minor version
#define APP_MINOR_VERSION 0
//Revision number
#define APP_REV_NUMBER 0
//Version color
#define APP_VERSION_COLOR "#00FF00" //GREEN
```

2. Rebuild the project
3. You should see the generated binary file `iap_dual_bank_http_sever_demo_2_0_0.bin` in `iap_dual_bank/http_server_demo/` directory

## Step #8 : Generate Firmware Update Image (for v2.0.0) using ImageBuilder

For demo purposes, we will generate an update image with an encrypted firmware (using AES-CBC) and with a signature (using RSA-SHA256). The CycloneBOOT Update Library embedded in the HTTP server demo will expect an update image with this specific configuration.

The next stage involves the use of ImageBuilder CLI utility for transforming the binary file (.bin) generated in the previous step into an image file (.img) compatible with CycloneBOOT Update Library:

**Helper Script**:
1. Navigate to `scripts/` directory
2. [Optional] Open `generate_update_image.bat` and ensure that the ImageBuilder executable path is valid
3. Run `generate_update_image.bat` in `scripts/` directory
4. You should see the generated image file `iap_single_bank_http_server_demo_2_0_0.img` in `iap_single_bank/http_server_demo/` directory

**Manual Method**:
1. Run the following command in the Windows console: `image_builder_windows.exe --input iap_single_bank_http_server_demo_2_0_0.bin --output iap_single_bank_http_server_demo_2_0_0.img --enc-algo aes-cbc --enc-key aa3ff7d43cc015682c7dfd00de9379e7 --sign-algo rsa-sha256 --sign-key .\resources\keys\rsa_private_key.pem --vtor-align`
2. You should see the generated update image file `iap_single_bank_http_server_demo_2_0_0_bootable.img` in `iap_single_bank/http_server_demo/` directory

## Step #9 : Upload Firmware Update Image (for v2.0.0) through HTTP server

1. Open your favorite serial console (115200 bauds)
2. Click on the reset button to restart the demo
3. Get the IP address of the board from the serial console output
4. Open a web browser and then navigate to the IP address of the board
5. You should be able to see a web page with a yellow background (initial application v1.0.0)
6. Click on `Browse` and select `iap_single_bank_http_server_demo_2_0_0.img` file 
7. Click on `Upload` for fetching the update image onto the board. If the update process is OK then the board will automatically reboot.
8. Refresh your web browser and you should get an new web page with a green background (new application v2.0.0)

Now, you can proceed to replicate **Step #7** (Generate New Firmware Binary) to **Step #9** (Upload Firmware Update Image) to generate new update image files with different versions, colours, and additional application features.


## Troubleshooting
In case you want to restart this tutorial from scratch and reload an initial firmware into internal flash (*Step #4* to *Step #6*), be aware that a previous update image remains in external flash that will trigger an update as soon as the static bootloader is launched. To avoid this problem, you will have to reset the external flash. Depending the evaluation board used, you may find a ready-to-use **external loader** to perform read/write/erase through a debug probe. Bellow you will find an example to erase the external flash using STM32CubeProg utility on STM32 evaluation boards. 

**Helper Script**:
1. Navigate to `scripts/` directory
2. [Optional] Open `clean_external_flash.bat` and ensure that the STM32CubeProgrammer executable path is valid
3. Run `clean_external_flash.bat`
4. The external flash is now erased

**Manual Method**:
- Run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -halt -rst -el "C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader\MT25QL512A_STM32769I-EVAL.stldr" -e all`
