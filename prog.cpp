/*******************************************************
 * Author		:		Gopal Krishan Aggarwal
 * Roll Number	:		B13121
 * Date			:		10 April, 2016
 * Description	:		Implementing banker's algorithm
 ******************************************************/
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <limits>
#include "state_class.h"
#include "declarations.h"
#define dbg(x) cout << "Value of var is: " << x << endl;
using namespace std;

// Extern variable definition
int p, r;
bool * terminated;
State state;

int main() {
	cin >> p >> r;
	state = new StateClass(p, r);
	State tempState = new StateClass(p, r);
	terminated = new bool[p];	//Stores whether each process is terminated
	int * request = new int[r];			//To indicate the request a process is making for more resources.

	state->inputStateFromUser();

	//In the beginning no process is terminated
	for(int i = 0; i < p; i++) {
		terminated[i] = false;
	}
	
	int decision;	//To store the decision whether this process will run this second or not
	
	//Printing the process headers for the oupute table
	for(int i = 0; i < p; i++) {
		cout << "p" << i+1 << "\t";
	}
	cout << endl;
	while(!allTerminated()) { //Until all processes are terminated
		for(int i = 0; i < p; i++) {	//Loop through the processes in round robin way.

			decision = 0;	// if at the end of for loop decision is still 0 it means i-th process is terminated already
			if(!terminated[i]) {	//If this ith process is not terminated...
				
				//Each second, each process asks for some random amount of more resources such that total resources 
				//allocated to it are less than what it claimed for.
				buildResourceRequest(request, i); //Build some random resources request for ith process.
				

				if(isRequestGreaterThanAvailable(request)) {
					decision = -1;
					cout << "-\t" << flush;	//A hyphen indicates the process is suspended for this second
					continue; //And continue checking for next process
				}
				

				//If the program reaches this point it means the request can
				//be fulfilled if the system is left in a safe state after
				//fulfilling the request
				
				buildTempState(tempState, request, i); //Define new temporary state assuming the request is fulfilled by the i-th process


				if(safe(tempState)) {
					allocate(request, i);	//Allocate request amount of resources to i-th process.
					decision = 1;
				}
				else
					decision = -1;
			}
			switch(decision) {
				case  0: cout << "\t" << flush;		//Indicating the process is already terminated.
						break;
				case -1: cout << "-\t" << flush;	//A hyphen indicates the process is suspended for this second
						break;
				case  1: cout << "|\t" << flush;	//Show that the process is executing this second
						break;
			}
		}

		sleep(1);	//Sleep for 1 second.
		//If some process has been allocated resources == to its claim then return the allocated resources and terminate the process.
		terminateFullyAllocatedProcesees();
		cout << endl;
	}
	

	return 0;
}