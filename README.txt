The output obj after iteration are stored in the Data folder with the names bunny_iteration_1 and bunny_iteration_2, and teapot_iteration_1 and teapot_iteration_2.




To run the Subdivision.cpp code, navigate to Subdivision folder and use the following commands:

g++ -o Subdivision  -I../Core Subdivision.cpp ../Core/Edge.cpp ../Core/Face.cpp ../Core/FException.cpp ../Core/HalfEdge.cpp ../Core/OBJFileReader.cpp ../Core/Parser.cpp ../Core/Point.cpp ../Core/Solid.cpp ../Core/SolidDelegate.cpp ../Core/StringTokenizer.cpp ../Core/TopologyException.cpp ../Core/Trait.cpp ../Core/Vertex.cpp


Subdivision ../Data/input.obj ../Data/output.obj


The output will be in the Data Folder.