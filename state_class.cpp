#include <iostream>
#include "state_class.h"
using namespace std;

StateClass::StateClass(int p, int r) { //Just allocates required memory for a system state
	np = p;
	nr = r;
	resources = new int[r]();		//Resources in the system will be initialized to 0
	available = new int[r]();		//Available resources will also be initialized to 0
	claim = new int*[p];
	alloc = new int*[p];
	for(int i = 0; i < p; i++) {
		claim[i] = new int[r]();		//Claimed resources by all the processes will be initialized to 0
		alloc[i] = new int[r]();		//Allocated resources to all the processes will be initialized to 0
	}
}

void StateClass::inputStateFromUser() {
	//Input the total resources from stdin
	for(int i = 0; i < nr; i++) {
		cin >> resources[i];
		available[i] = resources[i];	//Since initially available resources will be equal to total resources in the system.
	}

	for(int i = 0; i < np; i++)
		for(int j = 0; j < nr; j++)
			cin >> claim[i][j];		//Input claim for each process for each resource from stdin
}

void StateClass::displayState() {
	cout << "Claim matrix is:\n";
	for(int i = 0; i < np; i++) {
		for(int j = 0; j < nr; j++) {
			cout << claim[i][j] << "\t";
		}
		cout << endl;
	}
	cout << "Allocation matrix is:\n";
	for(int i = 0; i < np; i++) {
		for(int j = 0; j < nr; j++) {
			cout << alloc[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
	cout << "Resources array is:\n";
	for(int i = 0; i < nr; i++) {
		cout << resources[i] << "\t";
	}
	cout << endl;
	cout << "Available resources in the system are:\n";
	for(int i = 0; i < nr; i++) {
		cout << available[i] << "\t";
	}
	cout << endl;
}