# Simple-Logistics

The programm reads .csv file with 4 data columns.
1st column should be the name of the city (or any name chosen to call a set of coordinates).
2nd column is some demand variable, populaton of a city is used in the example.
3rd column is latitude and 4th is longitude.
The program uses Haversine formula to calculate the distance between two points on Earth.
Also computes the sum of all distances from a point to all the other points.
The programm can be used to find the best place to put a delivery hub excluding or including 
the demand. If you want to only calculate the shortest distance without the demand, leave the 2nd column 
empty, however it needs to be there
The .csv file must also have it's first row filled with column titles
