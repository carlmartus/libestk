#!/usr/bin/python

# Execute: blender -b <filename.blend> -P extra/blender_export.py

import bpy, bmesh

def fl2Hex(fl): return '0x%.2x' % int(fl*255.0)

def export():
    bm = bmesh.new()
    bm.from_mesh(bpy.context.selected_objects[0].data)
    layerColor = bm.loops.layers.color.active
    for loops in bm.calc_tessface():
        for loop in loops:
            loc = loop.vert.co
            col = loop[layerColor]
            line = '{ %.1f, %.1f, %.1f, %s, %s, %s }' % (
                    loc.x, loc.y, loc.z,
                    fl2Hex(col.r), fl2Hex(col.g), fl2Hex(col.b))
            print(line)
    bm.free()

if __name__ == "__main__":
    export()

