import json
import bpy
from mathutils import Vector

cam = bpy.context.scene.camera

# No idea how this works tbh
lookingAt = cam.matrix_world @ Vector((0, 0, -1))

cam_dict = {
    "position": {
        "x": cam.location.x,
        "y": cam.location.y,
        "z": cam.location.z
    },
    # TODO
    "up": {
        "x": 0,
        "y": -1,
        "z": 0
    },
    "lookingAt": {
        "x": lookingAt.x,
        "y": lookingAt.y,
        "z": lookingAt.z
    },
    # Is this it??
    "fov": {
        "x": cam.data.angle_x,
        "y": cam.data.angle_y
    }
}

out = {
    "title": "Blender scene",
    "model": "scene.obj",
    "width": bpy.context.scene.render.resolution_x,
    "height": bpy.context.scene.render.resolution_y,
    "frames": [{
        "frame": 0,
        "camera": cam_dict
    }],
    "output": "image.png"
}

with open("scene.json", "w") as f:
    json.dump(out, f)

bpy.ops.export_scene.obj(filepath="scene.obj")

print("DONE")