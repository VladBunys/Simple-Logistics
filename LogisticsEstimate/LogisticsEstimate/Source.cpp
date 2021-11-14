// Vladislovas Bunys

/*
			Logistics Calculator
			--------------------
The programm reads .csv file with 4 data columns.
1st column should be the name of the city (or any name chosen to call a set of coordinates).
2nd column is some demand variable, populaton of a city is used in the example.
3rd column is latitude and 4th is longitude.
The program uses Haversine formula to calculate the distance between two points on Earth.
Also computes the sum of all distances from a point to all the other points.
The programm can be used to find the best place to put a delivery hub excluding or including 
the demand. 
The .csv file must also have it's first row filled with column titles

*/
/*            Assumptions
			  -----------

1) The lengh of the roads is equal to the great circle distance
2) Each Place has a direct road to all other places
3) All places have equal demand for deliveries, alternatively can include demand 

*/

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<cmath>
#include <ctime>
// The following libraries are needed for finding the minimum
#include<stdio.h> 
#include<limits.h>
// Define the relevant constants 
#define pi 3.14159265358979323846
#define earthRkm 6371.0
using namespace std;

// declare the number of cities (places) 
const int Np = 100;

// Check the time it take to run this program
clock_t tStart = clock();

// This is the function which finds the positions of all the commas in the file
vector<int> findLocation(string sample, char findIt)
{
	vector<int> characterLocations;
	//unlike int, size_t is unsigned and i doesn't have to be negative. this way more iterations can be made
	for (size_t i = 0; i < sample.size(); i++)
		// find the commas
		if (sample[i] == findIt)
			// add to the vector
			characterLocations.push_back(i);

	return characterLocations;
}
// Convert degrees to radiants 
double deg2rad(double deg) {
	return (deg * pi / 180);
}

// The Haversine formula, finds the distance for given the latitudes and longitudes
double Haversine(double latI, double latF, double longI, double longF) {
	double dLat = latF - latI;
	double dLong = longF - longI;
	double a = (sin(dLat / 2) * sin(dLat / 2)) + (cos(latI) * cos(latF) * sin(dLong / 2) * sin(dLong));
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return  earthRkm * c;

}
// This function will find the smallest sum of distances according to assumption #2
int Small_dist = 0;
int Small_dist_dem = 0;
void printSmallest(double arr[], int arr_size, int k)
{
	int i, first, second;

	/* There should be atleast two elements */


	first = second = INT_MAX;
	for (i = 0; i < arr_size; i++)
	{
		//If current element is smaller than first then update both first and second  
		if (arr[i] < first)
		{
			second = first;
			first = arr[i];
		}

		// If arr[i] is in between first and second then update second
		else if (arr[i] < second && arr[i] != first)
			second = arr[i];
	}
	// Output the result on the screen
	if ( k == 0);
	{
		Small_dist = first;
	}
	if ( k == 1 );
	{
		Small_dist_dem = first;
	}
}

int main() {

	// Declare vectors to strore the data
	vector <double> latitude, longitude, demand;
	vector <string> place;
	
	

	ifstream  my_file("file_name.csv");
	// Check if the file is open
	if (my_file.is_open()) {
		
		cout << " The file is open \n";
		// The following will skip the first line
		string str;
		getline(my_file, str);
		// declare the vector for comma locations
		vector<int> charLocs;
		// while not end of file execute the following
		while (!my_file.eof()) {

			string read_line;
			// this will feed the lines from the file to read_line
			getline(my_file, read_line);
			charLocs = findLocation(read_line, ',');
			charLocs.push_back(read_line.size());

			// So the values added to the vector the following way: substr(start from, length)
			place.push_back(read_line.substr(0, charLocs[0]));

			// Declare the temprorary vector to store string type values
			vector<string> x;
			vector<string> y;
			
			
			for (size_t i = 0; i < charLocs.size()-1; i++) {
				x.push_back(read_line.substr(charLocs[i] + 1, charLocs[i + 1] - charLocs[i] - 1));
				y.push_back(read_line.substr(charLocs[i]+1, charLocs[i]-charLocs[i]-1));
			}
			// Now convert strings to doubles where nessesary
			for (size_t i = 2; i < charLocs.size() - 1; i++) {
				longitude.push_back(atof(x[2].c_str()));
				latitude.push_back(atof(x[1].c_str()));
				demand.push_back(atof(y[0].c_str()));
			}
			
		}
		
		// The following code is the algorithm for finding the place with minimum total distance, hence the best place 
		// to put the delivery hub in.
		// First, all the distances need to be assigned to an array
		
		
		double d[Np][Np];

		for (size_t i = 0; i < Np ; i++) {
			for (size_t j = 0; j < Np; j++) {
				// The function with the formula and conversion is called here
				d[i][j] = Haversine(deg2rad(latitude[i]), deg2rad(latitude[j]), deg2rad(longitude[i]), deg2rad(longitude[j]));

			}

		}
		cout << d[2][2] << endl;
		// Now declare an array to store all the sums of distances.
		// So sumDistances[i] will correspond to a place[i], the sum of all distances for that patricular place
		double sumDistance[Np];
		double sumDemDist[Np];

		for (size_t i = 0; i < Np ; i++) {
			double sum = 0;
			double sumDem = 0;

			for (size_t j = 0; j < Np ; j++) {
				// For each place[i] sum over all d[j]
				sum += d[i][j];
				// need to multiply by a big number so the really small numbers are not counted as zeros
				sumDem += (1000000*(d[i][j]) / demand[j]);
				if (j == i) {
					sumDem += 1000000 / demand[i];
				}
			}
			// Add the resultant sum to the array of sums
			sumDistance[i] = sum;
			sumDemDist[i] = sumDem;

		}


		int n = sizeof(sumDistance) / sizeof(sumDistance[0]);
		// Call the function wich will find the smallest total distance.
		// It will also output the value to command line
		printSmallest(sumDistance, n, 0);
		
		
		// Now, given the smallest total distance, find the index under which that distance lies
		int k = 0;
		for (size_t i = 0; i < Np ; i++) {

			if (sumDistance[i] > Small_dist+1) {
				k++;
			}
			else {
				break;
			}
		}

		
		// Output the results to command line
		cout << "The smallest total distance is " << sumDistance[k] << " km" << endl;
		cout << "The best place to put the delivery hub in will be " << place[k] << endl;

		printSmallest(sumDemDist, n, 1);
				int p = 0;
				for (size_t i = 0; i < Np; i++) {

					if (sumDemDist[i] > Small_dist_dem + 1) {
						p++;
					}
					else {
						break;
					}
				}

		
		cout << "If we count the demand then the best place to put the delivery hub in will be " << place[p] << endl;
		cout << "with total distance of " << sumDistance[p] << " km" << endl;

		
		// Now output the time it took
		printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
		// Don't forget to close the file
		my_file.close();
	}
	// Incase the file is not open
	else {
		cout << "Unable to open the file \n";

	}


	return 0;


}
