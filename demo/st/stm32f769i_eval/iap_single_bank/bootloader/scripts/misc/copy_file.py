import sys
import re
import os
import shutil

# Check if the correct number of command line arguments is provided
if len(sys.argv) != 3:
    print("Usage: python copy_file.py <input_binary_path> <output_binary_path>")
else:
    # Get the file path from the command line arguments
    input_binary_path = sys.argv[1]
    # Get the file path from the command line arguments
    output_binary_path = sys.argv[2]


# Path to the version.h file
#version_file_path = "../src/version.h"
version_file_path = output_binary_path + "/src/version.h"
app_version = ""

# Check if the file exists
if os.path.isfile(version_file_path):
    print("Version header file found.")
    # Open the version.h file (if exists) and extract current app version.
    try:
        with open(version_file_path, "r") as file:
            # Read the file line by line
            for line in file:
                # Use regular expression to find APP_VERSION_STRING and extract its value
                match = re.search(
                    r'#define\s+APP_VERSION_STRING\s+"(.*?)"', line)
                if match:
                    app_version = match.group(1)
                    break  # Stop searching after finding the first occurrence
    except FileNotFoundError:
        print(f"File '{version_file_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")
else:
    # No version present
    print("No version header file found. Using default binary name.")


# Split the string based on '/'
parts = input_binary_path.split('/')
# Extract the last part as the binary name
binary_name = parts[-1]

if app_version != "":
    # Add a specific character between "demo" and ".bin"
    app_version_replaced = app_version.replace(".", "_")
    modified_binary_name = binary_name.replace(
        "demo.bin", f"demo_{app_version_replaced}.bin")
else:
    modified_binary_name = binary_name

# Destination file path
destination_file = f"{output_binary_path}{modified_binary_name}"

try:
    # Copy the file from source to destination
    shutil.copy(input_binary_path, destination_file)
    print(
        f"File copied successfully from '{input_binary_path}' to '{destination_file}'.")
except FileNotFoundError:
    print("Source file not found.")
except PermissionError:
    print("Permission error: Unable to copy the file.")
except Exception as e:
    print(f"An error occurred: {e}")
