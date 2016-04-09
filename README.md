# SAMGenerator
a Simple Anisotropic Mesh Generator

##about
This is part of a research project I carried out at Télécom Paristech, under the supervision of Tamy Boubekeur and Pooran Memari.
**The point was to find a way to generate meshes from a set of points given an anisotropic metric, in 2D.**

##solution
After some research, the solution I imagined was to use Voronoï diagrams to generate such meshes. To set the anisotropic, I use here Voronoï diagrams of ellipsoïds, allowing us to approximate anisotropy by setting an ellipse at each vertex (could be procedurally generated).

To compute Voronoï diagrams of ellipsoïds, the algorithm discretises each ellipse in a set of vertices, thus we come back to a simple Voronoï diagram that we can easily compute.

To find the corresponding Mesh, one has to triangulate (Delaunay) over the Voronoï diagrams of ellipsoïds.

##implementation

This repository is a demonstrator that implements the Voronoï part, with **Qt** and **CGAL**. 


*NB : However the Delaunay triangulation is not functional yet, ran into some troubles to compute it, but it should theoretically work!
