

# CycloneBOOT Image Builder

```
 █████                                             ███████████              ███  ████      █████                   
░░███                                             ░░███░░░░░███            ░░░  ░░███     ░░███                    
 ░███  █████████████    ██████    ███████  ██████  ░███    ░███ █████ ████ ████  ░███   ███████   ██████  ████████ 
 ░███ ░░███░░███░░███  ░░░░░███  ███░░███ ███░░███ ░██████████ ░░███ ░███ ░░███  ░███  ███░░███  ███░░███░░███░░███
 ░███  ░███ ░███ ░███   ███████ ░███ ░███░███████  ░███░░░░░███ ░███ ░███  ░███  ░███ ░███ ░███ ░███████  ░███ ░░░ 
 ░███  ░███ ░███ ░███  ███░░███ ░███ ░███░███░░░   ░███    ░███ ░███ ░███  ░███  ░███ ░███ ░███ ░███░░░   ░███     
 █████ █████░███ █████░░████████░░███████░░██████  ███████████  ░░████████ █████ █████░░████████░░██████  █████    
░░░░░ ░░░░░ ░░░ ░░░░░  ░░░░░░░░  ░░░░░███ ░░░░░░  ░░░░░░░░░░░    ░░░░░░░░ ░░░░░ ░░░░░  ░░░░░░░░  ░░░░░░  ░░░░░     
                                 ███ ░███                                                                          
                                ░░██████                                                                           
                                 ░░░░░░                                                                                                                                                                                                                                                                   
```
---

# Getting started

Along with your download of CycloneBOOT package, we already provide a pre-built binaries of UpdateImageBuilder tool (Windows/Linux) inside 'utils/UpdateImageBuilder/bin' directory. 
You are only required to build this project if those pre-built binaries are not compatible with your target system or you really like tinkering with systems.

## Building on Windows

### Prerequisites

- CMake
- Visual Studio Code (Editor/IDE)
- Visual Studio Community Edition (MSVC compiler)
- CMake and C/C++ extensions for Visual Studio Code

### Using Visual Studio Code
1. Open the current folder in Visual Studio Code
2. CMake and C/C++ extensions will recognize the CMakeLists.txt file in the root and begin to configure the project.
 - 2b. You might want to change the CMake Generator in Settings -> CMake to "NMake Makefiles", in most cases.
 - 2c. Visual Studio code might ask to choose a 'Kit' to build the project. If you are on Windows, use MSVC compiler (installed alongside Visual Studio Community Edition).
3. Using the 'build' button (cog wheel icon) build the project.
4. The resulting executable will be placed in /build directory inside the current folder.

### Using Terminal/Powershell and equivalents

- Make sure the prerequisites are installed (except VSCode extensions) and are in PATH variable.
- Create a directory called build in the current folder.
- From within this directory (dir build), execute following commands: 
    - `cmake ..`
    - `cmake --build . --config Release`
- After the compilation process, image_builder.exe executable will be created in the current directory.

### More Information
Here are some sources to setup your development environment in Windows using CMake

- https://computingonplains.wordpress.com/building-c-applications-with-cmake-and-visual-studio-code/
- https://devblogs.microsoft.com/cppblog/cmake-tools-extension-for-visual-studio-code/ 

## Building on an Unix-like OS

### Prerequisites 
Same as the 'Building on Windows' section, except for the compiler. This tool is built/tested using GCC compiler.

### Using Visual Studio Code
Same as the 'Building on Windows' section.

### Using the Terminal

From within the current directory, execute the following commands: 
- `$ mkdir build`
- `$ cd build`
- `$ cmake ..`
- `$ cmake --build . --config Release`

- After the compilation process, the executable image_builder will be created in the current directory. 

## Dependencies

This tool uses 2 libraries to generate images. [cargs](https://github.com/likle/cargs) to parse CLI and [CycloneCRYPTO](https://oryx-embedded.com/products/CycloneCRYPTO.html) for Cryptographic operations. All the source code for the dependencies are supplied for user convenience. 
