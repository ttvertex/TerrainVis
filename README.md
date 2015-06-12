# TerrainVis
This is a heightmap visualization project. Using the modern OpenGL pipeline (GLSL 4.4)

I'ts an experimental project for glsl 4.x practice.

What it does:
-loads a grayscale image using freeimage
-from the image, triangulate a mesh and calculate normals for each vertex
-uses VBO/VAO to sent the geometry to the GPU
-uses shaders for
--Geometry shader: calculates the intersection plane
--Fragment shader: highlights the level curve and implements a phong lighting

+info:
--GLSL Cookbook
--GLSL 4.4 manual
