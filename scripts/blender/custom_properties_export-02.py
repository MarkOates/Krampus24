import bpy
from datetime import datetime

# Specify the file path to save the output
# output_map_folder = "/Users/markoates/Repos/Krampus24/tests/fixtures/maps/"
output_folder = "/Users/markoates/Repos/Krampus24/tmp/"
file_path = f"{output_folder}output_file-02.custom_properties"

timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")[:-3]  # Trims microseconds to milliseconds


# Get the active object
obj = bpy.context.object

with open(file_path, 'w') as file:
    # Iterate over all custom properties of the object
    for prop_name, prop_value in obj.items():
        if prop_name not in obj.bl_rna.properties:  # Ensures it's a custom property
            # Get the type
            prop_type = type(prop_value).__name__
            
            # Print details
            print(f"Property Name: {prop_name}")
            print(f"Object Name: {obj.name}")
            print(f"Type: {prop_type}")
            print(f"Value: {prop_value}")
            print("-" * 30)
            file.write(f"{obj.name}, {prop_name}, {prop_type}, {prop_value}\n")

    file.write(f"# written at {timestamp}\n")
