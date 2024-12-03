import bpy
import os
import math


# ABOUT
# This script will export the following collections:
#  - "visual" layer (as obj)
#  - "collision_mesh" layer (as obj)
#  - the "blocking" layer (as obj, for debugging)
#  - the "blocking" layer as a ".blocking" file, which can be used in TileFPS to build a height map
#  - the "entities" layer


# HOW TO USE
#
# Option 1: Run in blender
# 1) Start blender in terminal mode (makes debugging this script easier)
#    /Applications/Blender.app/Contents/MacOS/blender
# 2) In the "Scripting" pane in blender, open this file
# 3) Press the "run" icon (looks like the play button)
# 4) Done.
#
# Option 2: Run script from terminal
# 1) Note to run scripts in the currently running instnace of blender:
#    /Applications/Blender.app/Contents/MacOS/blender -b -P /Users/markoates/Repos/TileFPS/scripts/blender/blocking_export3.py
# 2) Done.




output_map_folder = "/Users/markoates/Repos/Krampus24/tests/fixtures/maps/"
output_model_folder = "/Users/markoates/Repos/Krampus24/tests/fixtures/models/"


# Get the current Blender project file name, and extract the basename
blend_filepath = bpy.data.filepath
blend_basename = os.path.basename(blend_filepath)
basename_without_extension = os.path.splitext(blend_basename)[0]


puzzle_name_and_version = basename_without_extension

def find_min_max_scale_coords(obj):
    # Ensure the object has mesh data
    if obj.type != 'MESH':
        return None

    # Get the mesh data
    mesh = obj.data

    # Initialize min and max coordinates with the coordinates of the first vertex
    min_coords = list(obj.matrix_world @ mesh.vertices[0].co)
    max_coords = list(obj.matrix_world @ mesh.vertices[0].co)

    # Iterate through all vertices
    for vertex in mesh.vertices:
        vertex_world = obj.matrix_world @ vertex.co
        # scale = obj.matrix_world.to_scale()
        for i in range(3):  # 0 is x, 1 is y, 2 is z
            if vertex_world[i] < min_coords[i]:
                min_coords[i] = vertex_world[i]
            if vertex_world[i] > max_coords[i]:
                max_coords[i] = vertex_world[i]

    # flip_y = true
    # if flip_y == true:
    # min_coords[1] = min_coords[1] * -1.0
    scale = obj.scale

    return min_coords, max_coords, scale



def format_coords(coords, flip_y_coord=False):
    if flip_y_coord:
        coords[1] = -coords[1]
    result = ' '.join(f'{coord:.6f}' for coord in coords)
    if flip_y_coord:
        coords[1] = -coords[1]
    return result


def collection_exists(collection_name):
    collection = bpy.data.collections.get(collection_name)
    if collection:
        return True
    else:
        return False


# Location to the output the ".blocking" file
output_file_path = os.path.expanduser(f"{output_map_folder}{puzzle_name_and_version}.blocking")

print(f"\033[94mWriting \".blocking\" file to {output_file_path}\033[0m")

# Open the file for writing
with open(output_file_path, 'w') as file:
    print("Writing blocks...")
    collection_name = "blocking"
    collection = bpy.data.collections.get(collection_name)

    # Iterate through all objects in the scene
    # for obj in bpy.context.scene.objects:
    if collection:
        for obj in collection.objects:
            if obj.type == 'MESH':
                min_coords, max_coords, scale = find_min_max_scale_coords(obj)
                #min_coords[1] = -min_coords[1];
                if min_coords and max_coords:
                    file.write(f"block, {obj.name}, {format_coords(min_coords, True)}, {format_coords(max_coords, True)}, {format_coords(scale)}\n")
                    # file.write("\n")
                    # file.write(f"- name: {obj.name}\n")
                    # file.write(f"  min: {min_coords}\n")
                    # file.write(f"  max: {max_coords}\n")
                    # file.write("\n")
        print("...done.")
    else:
        #raise RuntimeError(f"\033[31mERROR: Expecting a collection named \"{collection_name}\" but it does not exist\033[0m")
        print(f"\033[93mWARNING: Expecting a collection named \"{collection_name}\" but it does not exist\033[0m")
        
    print("Writing entities...")
    collection_name = "entities"
    collection = bpy.data.collections.get(collection_name)

    # Iterate through all objects in the scene
    # for obj in bpy.context.scene.objects:
    if collection:
        for obj in collection.objects:
            if obj.type == 'MESH':
                obj_name = obj.name
                obj_location = obj.location
                obj_rotation_euler = obj.rotation_euler
                obj_rotation_x = math.degrees(obj_rotation_euler.x) - 90.0 # Blender coordinates add 90.0 here
                obj_rotation_y = math.degrees(obj_rotation_euler.y)
                obj_rotation_z = math.degrees(obj_rotation_euler.z)
                obj_scale = obj.scale    
                # min_coords, max_coords, scale = find_min_max_scale_coords(obj)
                #min_coords[1] = -min_coords[1];
                #if min_coords and max_coords:
                file.write(f"entity, {obj_name}, {format_coords(obj_location, True)}, {obj_rotation_x} {obj_rotation_y} {obj_rotation_z}, {format_coords(obj_scale)}\n")
                    # file.write("\n")
                    # file.write(f"- name: {obj.name}\n")
                    # file.write(f"  min: {min_coords}\n")
                    # file.write(f"  max: {max_coords}\n")
                    # file.write("\n")
        print("...done.")
    else:
        #raise RuntimeError(f"\033[31mERROR: Expecting a collection named \"{collection_name}\" but it does not exist\033[0m")
        print(f"\033[93mWARNING: Expecting a collection named \"{collection_name}\" but it does not exist\033[0m")


# print(f"\033[94mResults have been written to {output_file_path}\033[0m")
print(f"\033[94m    ...\"{output_file_path}\" written successfully.\033[0m")



def export_collection_as_obj(collection_name):
   """
   Exports all objects in a specified Blender collection as an OBJ file.

   Args:
      collection_name (str): The name of the collection to export.
      output_path (str): The file path where the OBJ file should be saved.
   """
   output_path = f"{output_model_folder}{puzzle_name_and_version}-{collection_name}.obj"

   print(f"\033[94mWriting collection \"{collection_name}\" as .obj to \"{output_path}\"...\033[0m")

   # Deselect all objects
   bpy.ops.object.select_all(action='DESELECT')

   # Get the collection
   collection = bpy.data.collections.get(collection_name)

   if collection:
      # Select all objects in the collection
      for obj in collection.objects:
         obj.select_set(True)

      # Export the selected objects as an OBJ file
      bpy.ops.export_scene.obj(filepath=output_path, use_selection=True, use_materials=False, use_triangles=True)
   else:
      raise RuntimeError(f"\033[31mERROR: Expecting a collection named \"{collection_name}\" but it does not exist\033[0m")

   print(f"\033[94m    ...\"{output_path}\" written successfully.\033[0m")


## Output an OBJ of the "blocking" blocks. The purpose of this is for debugging.

# Specify the path where you want to save the OBJ file
if collection_exists("blocking"):
    export_collection_as_obj("blocking");
export_collection_as_obj("visual");
export_collection_as_obj("collision_mesh");


print(f"\033[32mElements written successfully.\033[0m")
