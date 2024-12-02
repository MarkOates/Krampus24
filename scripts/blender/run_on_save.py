import bpy
import importlib.util
import sys
import os

def run_script_on_save(dummy):
    print("Blender file has been saved.")
    export_custom_level()

def export_custom_level():
    #script_path = "/path/to/scripts/my_level_export.py"  # Update this to the correct path
    script_path = "/Users/markoates/Repos/TileFPS/scripts/blender/blocking_export4.py"

    # Load and execute the external script
    spec = importlib.util.spec_from_file_location("my_level_export", script_path)
    module = importlib.util.module_from_spec(spec)
    sys.modules["my_level_export"] = module
    spec.loader.exec_module(module)

    # Call the function from the external script
    if hasattr(module, 'export_level'):
        module.export_level()
    else:
        print("export_level() function not found in the external script.")

# Add the callback to the save_post handler
if run_script_on_save not in bpy.app.handlers.save_post:
    bpy.app.handlers.save_post.append(run_script_on_save)

print("Handler registered to run script on save.")

