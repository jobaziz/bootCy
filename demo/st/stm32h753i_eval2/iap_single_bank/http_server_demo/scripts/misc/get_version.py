import re
import sys

# The file to extract version from is given as a command-line argument
file_path = sys.argv[1]

# Open the file
with open(file_path, 'r') as file:
    # Read the file line by line
    for line in file:
        # Check if the line contains the version string
        if '#define APP_VERSION_STRING' in line:
            # Extract the version number using regex
            version = re.search(r'"(.*?)"', line).group(1)
            break

print(version)
