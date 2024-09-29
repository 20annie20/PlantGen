""" main function
Takes set of transformation rules per given species, user parameters and an initial word
produces a plant skeleton using L-systems in a bounded area
"""

import json
import os
from pathlib import Path

import l_system_simulator
from l_system_simulator.l_system_operator import Parser3D

from l_system_simulator.l_system import LSystemSimulator
from l_system_simulator.skeleton_output_builder import SkeletonBuilder

import unreal
from typing import Tuple
# from tensorflow.python.distribute.strategy_combinations import strategies_minus_tpu

import importlib
importlib.reload(l_system_simulator.l_system)
importlib.reload(l_system_simulator.skeleton_output_builder)

def main(load_from_file, file_path, initial_tree, rules_str,
         iterations, ignore_chars, angle, length, asset_path, asset_name ):
    os.chdir(Path(__file__).parents[0])

    if load_from_file:
        path = str(file_path)
        if os.path.isfile(path):
            with open(os.path.normpath(path), "r", encoding="utf-8") as file_handle:
                rules_content = json.load(file_handle)
                initial_tree = rules_content["axiom"]
                iterations = rules_content["iterations"]
                rules = rules_content["rules"]

                if "ignore" in rules_content.keys():
                    ignore_chars = rules_content["ignore"]
                    do_ignore = True
                else:
                    do_ignore = False
                    ignore_chars = []
        else:
            raise FileNotFoundError("Rules file not found.")
    else:
        initial_tree = str(initial_tree)
        iterations = int(iterations)

        ignore_chars = str(ignore_chars)
        if len(ignore_chars) > 0:
            do_ignore = True
        else:
            do_ignore = False
        rules = json.loads(str(rules_str))
    word = LSystemSimulator.produce_word(do_ignore, ignore_chars, initial_tree, rules, iterations)

    blueprint = create_base_blueprint(package_path=asset_path, asset_name=asset_name)


    spline_component = assign_spline_component(blueprint)
    spline_component.clear_spline_points(True)
    for point in range(iterations):
        spline_component.add_spline_point(unreal.Vector(0, 25*point, 100*point*length), unreal.SplineCoordinateSpace.LOCAL)

        location, tangent = spline_component.get_location_and_tangent_at_spline_point(point, unreal.SplineCoordinateSpace.LOCAL)
        static_component = assign_static_component(blueprint)
        static_component.set_editor_property("relative_location", location + unreal.Vector(0.0, 0.0, 50*length))
        static_component.set_editor_property("relative_rotation", tangent.rotator_from_axis_and_angle(90))
        static_component.set_editor_property("relative_scale3d", unreal.Vector(length, length, length))

    print(word)

def assign_static_component(blueprint):
    so_subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    sub_handle, static_component = add_subobject(
        subsystem=so_subsystem,
        blueprint=blueprint,
        new_class=unreal.StaticMeshComponent,
        name='StaticMesh'
    )
    assert isinstance(static_component, unreal.StaticMeshComponent)
    unreal.log('new_sub_object:{}'.format(static_component))

    cube_mesh: unreal.StaticMesh = load_static_mesh(path='/Engine/BasicShapes/Cylinder')
    static_component.set_static_mesh(new_mesh=cube_mesh)
    return static_component

def assign_spline_component(blueprint):
    so_subsystem = unreal.get_engine_subsystem(unreal.SubobjectDataSubsystem)
    sub_handle, spline_component = add_subobject(
        subsystem=so_subsystem,
        blueprint=blueprint,
        new_class=unreal.SplineComponent,
        name='Spline'
    )
    assert isinstance(spline_component, unreal.SplineComponent)
    unreal.log('new_sub_object:{}'.format(spline_component))
    return spline_component

def create_base_blueprint(package_path: str, asset_name: str) -> unreal.Blueprint:
    factory = unreal.BlueprintFactory()
    # this works, the saved blueprint is derived from Actor
    factory.set_editor_property(name='parent_class', value=unreal.GeneratedDynamicMeshActor)

    # make the blueprint
    asset_tools: unreal.AssetTools = unreal.AssetToolsHelpers.get_asset_tools()
    asset: object = asset_tools.create_asset(
        asset_name=asset_name,
        package_path=package_path,
        asset_class=None,
        factory=factory
    )
    if not isinstance(asset, unreal.Blueprint):
        raise Exception('Failed to create blueprint asset')
    blueprint: unreal.Blueprint = asset  # noqa

    return blueprint

def add_subobject(subsystem: unreal.SubobjectDataSubsystem,
                  blueprint: unreal.Blueprint,
                  new_class,
                  name: str) -> Tuple[unreal.SubobjectDataHandle, unreal.Object]:

    root_data_handle: unreal.SubobjectDataHandle = \
        subsystem.k2_gather_subobject_data_for_blueprint(context=blueprint)[0]

    sub_handle, fail_reason = subsystem.add_new_subobject(
        params=unreal.AddNewSubobjectParams(
            parent_handle=root_data_handle,
            new_class=new_class,
            blueprint_context=blueprint))
    if not fail_reason.is_empty():
        raise Exception("ERROR from sub_object_subsystem.add_new_subobject: {fail_reason}")

    subsystem.rename_subobject(handle=sub_handle, new_name=unreal.Text(name))
    # subsystem.attach_subobject(owner_handle=root_data_handle, child_to_add_handle=sub_handle)

    BFL = unreal.SubobjectDataBlueprintFunctionLibrary
    obj: unreal.Object = BFL.get_object(BFL.get_data(sub_handle))
    return sub_handle, obj

def load_static_mesh(path: str) -> unreal.StaticMesh:
    eal = unreal.EditorAssetLibrary
    asset: object = eal.load_asset(path)
    if not isinstance(asset, unreal.StaticMesh):
        raise Exception("Failed to load StaticMesh from {path}")
    return asset

def generate_skeleton(static_mesh_actor, word, angle, length):
    ANGLE = float(angle)
    LENGTH = float(length)

    meshBuilder = SkeletonBuilder(ANGLE, LENGTH, True)
    operator_parser = Parser3D(meshBuilder)

    spline_coords = [[tuple(meshBuilder.get_curr_pos())]]  # list of lists of 3D points

    branch_idx = 0
    for char in word:
        operation = operator_parser.map_symbol(char)
        operation()
        if char == "[" and len(spline_coords[branch_idx]):
            branch_idx += 1
            spline_coords.append([])
        elif char == "]":
            branch_idx += 1
            spline_coords.append([])
        else:
            spline_coords[branch_idx].append(tuple(meshBuilder.get_curr_pos()))

    merged = merge_lists(spline_coords)
    return merged


def merge_lists(lists):
    merged = []

    while lists:
        # Pop the first list from the list of lists
        curr_list = lists.pop(0)

        # Try to find a list where we can merge with the current list
        i = 0
        while i < len(lists):
            # If the last tuple of the current list matches the first tuple of another list
            if curr_list[-1] == lists[i][0]:
                # Merge the two lists
                curr_list += lists.pop(i)[1:]
                i = 0  # Start over as we've changed the list structure
            else:
                i += 1

        # Append the merged or standalone list to the result
        merged.append(curr_list)

    return merged