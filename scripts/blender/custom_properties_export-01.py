import bpy
from datetime import datetime

# Specify the file path to save the output
# output_map_folder = "/Users/markoates/Repos/Krampus24/tests/fixtures/maps/"
output_folder = "/Users/markoates/Repos/Krampus24/tmp/"
file_path = f"{output_folder}custom_properties_output_file.txt"

timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]  # Trims microseconds to milliseconds


blacklist_keys = {"_RNA_UI", "cycles"}

# import bpy

# Specify the file path to save the output
# file_path = "/path/to/your/output_file.txt"

# Open the file in write mode
with open(file_path, 'w') as file:
    # Iterate through all objects in the scene
    for obj in bpy.data.objects:
        # Check if the object has custom properties
        if obj.keys():  # obj.keys() gives all the properties
            for prop in obj.keys():
                # Ignore properties that aren't custom (e.g., `_RNA_UI`)
                if prop not in blacklist_keys:  # Skip blacklisted keys
                    # Write the object name, property, and its value to the file
                    file.write(f"{obj.name}, {prop}, {obj[prop]}\n")

    file.write(f"# written at {timestamp}\n")

print(f"Custom properties exported to {file_path}")
