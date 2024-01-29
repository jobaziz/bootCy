# Getting started

This CycloneBOOT project demonstrates a secure firmware update mechanism over Ethernet, using In-Application Programming (IAP). IAP is a method of updating firmware using an update library stored in the user application.

This demo makes use of the MCU's dual bank flash functionality. One bank holds the current application firmware while the other bank will serve to store the new application firmware. If the update process is successful, then a swap between the two flash banks will be performed to boot to the new firmware.

You will find in this demo project a HTTP server (from CycloneTCP stack) bundled with our CycloneBOOT Update Library: 
- The HTTP server will fetch the update image from the network interface
- The CycloneBOOT Update Library will process and install the update image in next available bank
- If the update process was a success, the MCU will reboot and start the updated firmware

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

In this step, we will configure the MCU to run in dual bank mode (dual bank flash).

**Helper Script**:
1. Connect the evaluation board to your computer
2. Navigate to `scripts/` directory
3. [Optional] Open `configure_dual_bank_mode.bat` and ensure that the STM32CubeProgrammer executable path is valid
4. Run `configure_dual_bank_mode.bat` 
5. Your board is now ready with dual bank mode activated

**Manual Method**: 
1. Connect the evaluation board to your computer
2. Run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -ob BFB2=0`
3. Your board is now ready with dual bank mode activated

## Step #2 : Generate Initial Firmware Binary (v1.0.0)

1. Open the demo project with your IDE
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

2. Build the project
3. As we use a post-compilation routine which adds the version number to the filename and save the resulting .bin file to the root of the demo folder, you should see the generated binary file `iap_dual_bank_http_sever_demo_1_0_0.bin` in `iap_dual_bank/http_server_demo/` directory

## Step #3 : Load Initial Firmware Binary (v1.0.0) 

Once the application firmware generated, load it into the first flash bank at address `0x08000000`.

**Helper Script**:
1. Navigate to `scripts/` directory
2. [Optional] Open `load_initial_firmware.bat` and ensure that the STM32CubeProgrammer executable path is valid
3. Run `load_initial_firmware.bat`
4. The Windows console will provide feedback on whether this process succeeded or not, along with any relevant log messages

**Manual Method**:
- Use your IDE to load the firmware generated in *Step #2*
- Or run the following command in the Windows console: `STM32_Programmer_CLI.exe -c port=SWD index=0 -d iap_dual_bank_http_server_demo_1_0_0.bin 0x08000000 -halt -rst`

## Step #4 : Generate New Firmware Binary (v2.0.0)

1. Open the demo project with your IDE
2. Modify the macros in the `version.h` file located in the `src/` directory by updating the version to `v2.0.0` and setting the background colour to green (`#00FF00`)

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

## Step #5 : Generate Firmware Update Image (for v2.0.0) using ImageBuilder

For demo purposes, we will generate an update image with encrypted firmware (using AES-CBC) and a signature (using RSA-SHA256). The CycloneBOOT Update Library embedded in the HTTP server demo will expect an update image with this specific configuration.

The next stage involves the use of ImageBuilder CLI utility for transforming the binary file (.bin) generated in the previous step into an image file (.img) compatible with CycloneBOOT Update Library:

**Helper Script**:
1. Navigate to `scripts/` directory
2. [Optional] Open `generate_update_image.bat` and ensure that the ImageBuilder executable path is valid
1. Run `generate_update_image.bat` in `scripts/` directory
2. You should see the generated image file `iap_dual_bank_http_server_demo_2_0_0.img` in `iap_dual_bank/http_server_demo/` directory

**Manual Method**:
1. Run the following command in the Windows console: `image_builder_windows.exe --input iap_dual_bank_http_server_demo_2_0_0.bin --output iap_dual_bank_http_server_demo_2_0_0.img --enc-algo aes-cbc --enc-key aa3ff7d43cc015682c7dfd00de9379e7 --sign-algo rsa-sha256 --sign-key .\resources\keys\rsa_private_key.pem`
2. You should see the generated image file `iap_dual_bank_http_server_demo_2_0_0.img` in `iap_dual_bank/http_server_demo/` directory

## Step #6 : Upload Firmware Update Image (for v2.0.0) through HTTP server

1. Open your favorite serial console (115200 bauds)
2. Click on the reset button to restart the demo
3. Get the IP address of the board from the serial console output
4. Open a web browser and then navigate to the IP address of the board
5. You should be able to see a web page with a yellow background (initial application v1.0.0)
6. Click on `Browse` and select `iap_dual_bank_http_server_demo_2_0_0.img` file
7. Click on `Upload` for fetching the update image onto the board. If the update process is OK then the board will automatically reboot.
8. Refresh your web browser, and you should see a new web page with a green background, indicating the updated application to version 2.0.0.

Now, you can proceed to replicate **Step #4** (Generate New Firmware Binary) to **Step #6** (Upload Firmware Update Image) to generate new update image files with different versions and additional application features.


## Troubleshooting

> **Q**: Why can't my debugger connect to the board after an update? 
>
> **A**: One of the reasons for this could be that MCU has activated the "swap bank" feature following an update, meaning the current running app might not be running from the bank which debugger expects by default. If this is the case, we recommend using STM32CubeProgrammer to re-initialize Option Bytes (Option Bytes -> User Configuration -> uncheck Swap Banks checkbox). 
