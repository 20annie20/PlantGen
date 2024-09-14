# import unreal

from l_system_simulator.l_system_operator import Parser3D
from l_system_simulator.skeleton_output_builder import SkeletonBuilder


def generate_skeleton(static_mesh_actor, word, angle, length):
    ANGLE = float(angle)
    LENGTH = float(length)

    meshBuilder = SkeletonBuilder(ANGLE, LENGTH, True)
    operator_parser = Parser3D(meshBuilder)

    spline_coords = []  # list of lists of 3D points
    # 1. start at some 0,0 coord
    # 2. create list of edges per branch, save last visited coord in branch to go back to
    # meshBuilder = SkeletonBuilder(ANGLE, LENGTH, True)
    #
    #

    # transformer.apply_operators()
    # return list of edges
    print(type(static_mesh_actor))
    print(word)
    """
    ResultMesh = unreal.DynamicMesh()
    StaticMeshComponent = static_mesh_actor.static_mesh_component
    StaticMesh = static_mesh_actor.static_mesh
    # final mesh will be here

    # Define vertices for a simple cube
    vertices = [
        unreal.Vector(-50, -50,  50),  # Front bottom left
        unreal.Vector( 50, -50,  50),  # Front bottom right
        unreal.Vector( 50,  50,  50),  # Front top right
        unreal.Vector(-50,  50,  50),  # Front top left
        unreal.Vector(-50, -50, -50),  # Back bottom left
        unreal.Vector( 50, -50, -50),  # Back bottom right
        unreal.Vector( 50,  50, -50),  # Back top right
        unreal.Vector(-50,  50, -50)   # Back top left
    ]

    # Define triangles (indices)
    triangles = [
        0, 1, 2,  0, 2, 3,  # Front face
        4, 6, 5,  4, 7, 6,  # Back face
        0, 3, 7,  0, 7, 4,  # Left face
        1, 5, 6,  1, 6, 2,  # Right face
        3, 2, 6,  3, 6, 7,  # Top face
        0, 4, 5,  0, 5, 1   # Bottom face
    ]

    # Define UVs (Optional)
    uvs = [
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1),  # Front
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1),  # Back
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1),  # Left
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1),  # Right
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1),  # Top
        unreal.Vector2D(0, 0), unreal.Vector2D(1, 0), unreal.Vector2D(1, 1), unreal.Vector2D(0, 1)   # Bottom
    ]

    # Access the mesh's LOD (Level of Detail) to define the geometry
    lod_index = 0
    mesh_lod = StaticMesh.get_lod(lod_index)

    # Create a new section (mesh part) for the LOD
    section_index = 0
    mesh_section = mesh_lod.create_section(section_index)

    # Set vertices, triangles, and UVs
    mesh_section.vertices = vertices
    mesh_section.triangles = triangles
    mesh_section.uvs = uvs

    StaticMesh.buid()

    # Save the newly created mesh asset to disk
    unreal.EditorAssetLibrary.save_loaded_asset(StaticMesh)
    """