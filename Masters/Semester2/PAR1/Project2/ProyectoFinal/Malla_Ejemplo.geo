//EJEMPLO CUADRADO

tmE  = 0.75;        // Tamaño de malla exterior (Ejemplo A)
//tmE  = 0.10;        // Tamaño de malla exterior (Ejemplo B)
//tmE  = 0.05;        // Tamaño de malla exterior (Ejemplo C)
//tmE  = 0.01;        // Tamaño de malla exterior (Ejemplo D)

//-------------------- NIVEL 1, GEOMETRIA
// Puntos
//----------Rectangulo exterior
Point(1) = { 0.0, 0.0, 0.0, tmE};
Point(2) = { 1.5, 0.0, 0.0, tmE};
Point(3) = { 1.5, 1.5, 0.0, tmE};
Point(4) = { 0.0, 1.5, 0.0, tmE};

// Lineas
Line(1) = {1,2};
Line(2) = {2,3};
Line(3) = {3,4};
Line(4) = {4,1};

//-------------------- NIVEL 2, DOMINIO COMPUTACIONAL 

Curve Loop(1) = {1,2,3,4};  // Cerrando el rectangulo

Plane Surface(1) = {1};   // Definiendo el dominio

//-------------------- NIVEL 3, ETIQUETADO
Physical Line('Oeste') = {4}; 
Physical Line('Este')  = {2};
Physical Line('Norte') = {3};
Physical Line('Sur')   = {1};

Physical Surface("Dominio") = {1};

//-------------------- NIVEL 4, GENERAR MALLA

Mesh 2;

Mesh.SurfaceFaces = 1;
Mesh.Points       = 1;

Save "Malla_Ejemplo.vtk";