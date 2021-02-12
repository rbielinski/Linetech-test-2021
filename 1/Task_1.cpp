/* Write a program, in C++ language, that launches two separate threads. The
first thread will have to return the arithmetic average of the dataset, and the
second one should return the median value. Threads should be running in parallel.
When return values from threads are collected, the main thread has to compare
them and write to standard output which of them is bigger.
Input data is in “1/data.txt”.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <algorithm>
using namespace std;

double avg;
double med;

// define functions for threads 1 and 2
void average(vector<int> numbers) {
	int total = 0;
	for (size_t i = 0; i < numbers.size(); i++) {
		total += numbers[i];
	}
	avg = double(total) / (numbers.size());
}

void median(vector<int> numbers) {
	// if dataset is even, the median is the mean of the two middle values:
	if (numbers.size() % 2 == 0) {
		const auto mid_iter1 = numbers.begin() + numbers.size() / 2;
		const auto mid_iter2 = numbers.begin() + numbers.size() / 2 - 1;
		nth_element(numbers.begin(), mid_iter1, numbers.end());
		const auto mid1 = *mid_iter1;
		nth_element(numbers.begin(), mid_iter2, numbers.end());
		const auto mid2 = *mid_iter2;
		med = (double(mid1) + double(mid2)) / 2;
	}
	else {
		const auto mid_iter = numbers.begin() + numbers.size() / 2;
		nth_element(numbers.begin(), mid_iter, numbers.end());
		med = *mid_iter;
	}
}

int main() {
	vector <int> dataset;
	dataset.reserve(10000);

	// open the file and read the data into the dataset vector
	ifstream input("data.txt");
	if (input.is_open()) {
		int number;
		while (input >> number) {
			dataset.push_back(number);
		}
	}
	else {
		cout << "Could not open file. Please make sure it is saved in the same folder as 'data.txt'" << endl;
		return 0;
	}
	input.close();

	// copy the dataset to avoid errors (as one of the threads is going to manipulate it)
	vector <int> datasetCopy(dataset);

	// launch threads 1 and 2 and wait for both to finish
	thread th1(average, dataset);
	thread th2(median, datasetCopy);
	th1.join();
	th2.join();
	
	// compare the returned values and output the result
	cout << "Median: " << med << endl;
	cout << "Average: " << avg << endl;
	if (avg > med) {
		cout << "The arithmetic average is bigger than the median." << endl;
	}
	else if (med > avg) {
		cout << "The median is bigger than the arithmetic average." << endl;
	}
	else {
		cout << "The arithmetic average and the median are equal." << endl;
	}
	return 0;
}
