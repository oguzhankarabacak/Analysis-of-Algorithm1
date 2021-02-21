#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <sstream>
#include <iomanip>
using namespace std;

class Sale {  //this class to keep the data in sales.txt files
private:
	string country;  //contry_name
	string item_type;  //item_type
	string order_id; //order_id
	int units_sold; //units_sold
	double total_profit;   //total_profit
public:
	Sale() {}
	Sale(string country, string item_type, string order_id, int units_sold, double total_profit) :country(country), item_type(item_type), order_id(order_id), units_sold(units_sold), total_profit(total_profit) {}  //constructor for Sale class
	string getCountry() { return this->country; }   //get function for Country
	double getTotalProfit() { return this->total_profit; }  //get function for total_profit
	string getItemType() {return this->item_type;	}  //get function for item_type
	string getOrderId() {return this->order_id;	} //get function for order_id
	int getUnitsSold() {return this->units_sold;	}  //get function for units_sold
	
};

void swap(Sale* s1, Sale* s2) {  //this is swap function
	Sale t = *s1;
	*s1 = *s2;
	*s2 = t;
}
int Partition(Sale sales[], int low, int high) {  //this is partition function
	Sale pivot = sales[high];  //first assign last element of array to pivot
	int i = (low - 1);  //assign low-1 to i

	for (int j = low; j <= high-1; j++) {  //j increment until the j==high
		if (sales[j].getCountry() < pivot.getCountry()) {  //we compare country of j element of sales and pivot country
		
			i++;    //increment index of small element
			swap(&sales[i], &sales[j]);  //we swap sales[i] and sales[j]
		}
		if (sales[j].getCountry() == pivot.getCountry()) {   //if country_name is equal 
			if (sales[j].getTotalProfit() > pivot.getTotalProfit()) {   // we compare total_profit 
				i++;
				swap(&sales[i], &sales[j]);  //if sales[j] profit is bigger than pivot profit,we swap them
			}
		}
	}
	swap(&sales[i+1], &sales[high]);  //the end we swap pivot and sales[i+1]
	return (i + 1);  //and return i+1

}
void Quicksort(Sale sales[], int low, int high) {  //this is quick sort function
	if (low < high) {  //if all elements is not sorted
		int p = Partition(sales, low, high);  
		Quicksort(sales, low, p - 1);  //for first partition of subproblems
		Quicksort(sales, p + 1, high);  //for second partition of subproblems

	}
}


int main(int argc, char **argv) {

	ifstream file;
	file.open("sales.txt");

	if (!file) {
		cerr << "File cannot be opened!";
		exit(1);
	}

	stringstream ss(argv[1]);
	int N ; //you should read value of N from command line
	ss >> N;
	string line; 
	string line1; //country
	string line2;   //item_type
	string line3;   //order id 
	int line4; //units sold
	double line5; //total_profit
	string line6;

	getline(file, line); //this is the header line
	Sale* sales=new Sale[N];  // first we allocate N array size


	for (int i = 0; i < N; i++) {

		getline(file, line1, '\t'); //country (string)
		getline(file, line2, '\t'); //item type (string)
		getline(file, line3, '\t'); //order id (string)
		file >> line4; //units sold (integer)
		file >> line5; //total profit (float)
		Sale s(line1, line2, line3, line4, line5);  // we create Sale object
		sales[i]=s;   // and add sale object to array
		getline(file, line6, '\n'); //this is for reading the \n character into dummy variable.
	}

	
	clock_t t;  //for runtime
	t = (double)clock(); //we take begin time 
	Quicksort(sales, 0, N-1);
	t =clock()-(double)t;  //end-begin time 
	ofstream filep; //file object
  	filep.open("sorted.txt"); //open sorted.txt
  	filep << "Country"<<"\t"<<"item_type\torder_id\tunits_sold\ttotal_profit\n" ;

  	for (int j=0;j<N;j++){  //write array to sorted.txt
  		filep << fixed <<setprecision(2) <<sales[j].getCountry() << "\t" << sales[j].getItemType() << "\t" << sales[j].getOrderId() << "\t" << sales[j].getUnitsSold() << "\t" << sales[j].getTotalProfit() << "\n";  
	}
  	
  	filep.close(); //close file
  	
	

	cout <<  fixed << "Execution time: " << (double)t/CLOCKS_PER_SEC << endl;
	


	return 0;
}
