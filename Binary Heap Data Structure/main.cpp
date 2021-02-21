#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <sstream>

const  double hotel_long=33.40819;  //hotel longitude
const  double hotel_lati=39.19001;  //hotel latitude
using namespace std;

void swap(double *x,double *y){  //swap function
	double temp=*x;
	*x=*y;
	*y=temp;	
}


void min_heapify (vector<double> &distances , int i, int N)  //min_heapify function, Vector array, i value, Vector array size
{
	double left  = 2*i+1;  //left child
	double right = 2*i+2;  //right child
	double min;  
	if(left < N && distances[left] < distances[i] ) min = left;  //if left is smaller than N and left element is smaller than ith elements , min is equal left child
	else min = i;  //else min is equal i;
	
	if(right < N && distances[right] < distances[min] ) min = right; //if right is smaller than N and right element value is smaller than min value
	    
	    
	if(min != i)  //if min is not equal to ith index
	{
	    swap (&distances[i],& distances[min]);  //swap parent and child
	    min_heapify (distances, min,N);  //rebuild again binary heap tree
	} 
}

void decrease_distance(vector<double> &distances,int i,double new_distance) {  //decrease_distance function , ith elements, new_distance is new value
	if(new_distance > distances[i]) {  //if new _distance is bigger current_distance
		cout << "new distance is bigger than current distance" << endl;
		return;
	}
	distances[i] = new_distance;  //new_distance -> distances[i]
	if(distances[i]<0) distances[i]=0;  //set to zero if distance is negative
	while (i != 0 && distances[ (i-1) / 2] > distances[i] ){   //compare ith elements value and its parent until the root
		swap(&distances[(i-1)/2],&distances[i]);  //swap ith element and its parent
		i = (i-1)/2;  //new i value is i's parent
	}
}

double extract_min(vector<double> &distances,int N){  //function to delete root 
	if (N == 0) { //if array is empty
		cout << " heap underflow " << endl;
		return -1;  //exit function
	}
	double min=distances[0];  //distances[0] -> min
	distances[0] = distances[N-1];  //last element -> distance[0]
	distances.pop_back(); //delete last element
	min_heapify(distances,0,distances.size());  //rebuild binary heap tree
	return min;  //and return min
}
void insert(vector<double> &distances,double key,int N){  //insert function
	distances.push_back(key);  //first add the last of array
	decrease_distance(distances,N,key);  //then update the tree
}


int main(int argc, char **argv){
	
	
	ifstream file;
	file.open("locations.txt");
	
	if (!file){  //if file cannot be opened
		cerr << "File cannot be opened!";
		exit(1);
	}
	stringstream ss(argv[1]);  //first parameter is char so must convert int
	int m;
	ss >> m;  
	string line; //for first line (longitude,latitude)
	
	long double longitude;
	long double latitude;
	
	getline(file,line);  //get first line (longitude,latitude)
	
	stringstream probability(argv[2]);
	float p;
	probability >> p; //second parameter convert float
	
	vector<double> distances;  //to store distances
	vector<double> delete_distances;  //to store delete_distances
	srand (time(NULL));  //for rand number
	int delete_number=m/100; 
	int update_counter=0;  //to count update operations 
	int insert_counter=0;  //to count insert operations
	int what_operation;  //operation type
	int update_random;  //random index of vector to update
	
	clock_t t;
	t = (double)clock();

	
	for (int i=0;i<m;i++){
		file >> longitude;  //get longitude
		file >> latitude;   //get latitude
		double euclid=pow( pow(longitude-hotel_long,2) + pow(latitude-hotel_lati,2 ) , 0.5);  //apply euclid
		what_operation=(rand()%100) < p*100;  //and determine the type if what_operation = 0, operation is insert, else update
		if(what_operation==0) {  //insert operation
			insert(distances,euclid,distances.size());  //insert to array 
			insert_counter+=1;  //increment insert_counter
		}
		else{
			if(distances.size() == 0) continue;  // if array is empty , start loop again
			update_random=rand() % distances.size();  //select random index to update
			decrease_distance(distances,update_random,distances[update_random]-0.01);  //call decrease_distance , distances[update_random]-0.01 : Random element minus 0.01
			update_counter += 1;  //increment update_counter
		} 
		if((i+1)%100 == 0  && i != 0){  //if i is 100x , and i is not equal to zero
			double extract_value=extract_min(distances,distances.size());
			if (extract_value != -1){
		    	delete_distances.push_back(extract_value);  //add returns of extract_min function to delete_distances array
			}
		}
	}
	
	t = (double)clock() - (double)t;
	for (int j=0;j<delete_distances.size();j++) cout << "The nearest Taxi  :  " << delete_distances[j] << endl;  //print nearest_distances
//	cout << "Delete Taxi  " << delete_distances.size() << endl;
	cout << "The number of taxi additions :  " << insert_counter << "   The number of distance updates  :  " << update_counter << endl;  //print update and insert numbers
	
	cout << fixed <<"Execution Time   : " << (double)t / CLOCKS_PER_SEC << endl;
	return 0;

}
