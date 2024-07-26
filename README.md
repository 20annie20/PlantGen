# Foliage 3D mesh reconstruction & generation pipeline

This project aims to create an entire procedure for reconstructing visually correct representations of trees and other types of foliage from input images.
## L-System Stage
L-systems are a grammar based descriptions for procedural generation rules perfect for the organic structures. 
This stage takes a set of rules to reconstruct a model of a given species, classified from an image. Skeleton is constructed within boundaries determined from the input image.
The skeleton is represented in a form of list of vertices and edges in 3D space, which will be a base for mesh construction in the next stage of the pipeline.
