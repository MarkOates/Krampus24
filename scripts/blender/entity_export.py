
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













# Store initial hidden/visibility states and list hidden collections
initial_states = {col.name: col.hide_viewport for col in bpy.context.view_layer.layer_collection.children}
hidden_collections = [col.name for col in bpy.context.view_layer.layer_collection.children if col.hide_viewport]

# Output the collection's visibility states for debugging
print("Collection's initial visibility states:")
for col in bpy.context.view_layer.layer_collection.children:
    print(f"- {col.name} - {col.hide_viewport}")


# bpy.data.collections["visual"].hide_viewport = False
# bpy.context.view_layer.layer_collection.children['visual'].hide_viewport = False
for col in bpy.context.view_layer.layer_collection.children:
    print(f"- {col.name} - {col.hide_viewport}")



puzzle_name_and_version = basename_without_extension


def toggle_collection_visibility(temp_state):
    """
    Toggles visibility of collections based on the given state.
    :param temp_state: Boolean value to set hide_viewport.
    """
    for collection in bpy.context.view_layer.layer_collection.children:
        collection.hide_viewport = temp_state








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
export_collection_as_obj("left_door");
export_collection_as_obj("right_door");






# Restore the original visibility state
print("Restoring collection's initial visibility states:")
for col_name, was_hidden in initial_states.items():
    bpy.context.view_layer.layer_collection.children[col_name].hide_viewport = was_hidden



