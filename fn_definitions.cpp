#ifndef FN_DEFINITIONS_CPP
#define FN_DEFINITIONS_CPP
#include "state_class.h"
#include "declarations.h"
#include <cstdlib>
#include <time.h>
#include <vector>
#include <iostream>
using namespace std;

//Returns if all processes are terminated
bool allTerminated() {
	int yes = 1;	//Assume all are terminated
	for(int i = 0; i < p; i++) {
		if(terminated[i] == false) {
			yes = 0;
			break; 		//No need to check further
		}
	}
	return yes == 1;
}

/*****************************************************
 *	Function: buildResourceRequest
 *
 *	Builds a random resource request for ith process
 *
 *	Parameters:
 *
 *		request 	- 	Pointer to request matrix which would be built
 *		index		-	Index of the process of whose request matrix is to be built
 *
 *	Global Variables used:	r, state
 *****************************************************/
void buildResourceRequest(int * request, int index) {
	srand (time(NULL));	//Seed the random number generator
	for(int j = 0; j < r; j++) { 
		if(state->claim[index][j] == state->alloc[index][j]) // But alloc of some resource is already equal to its claim
			request[j] = 0;	//Then more request for that resource will be 0
		else
			request[j] = rand() % (state->claim[index][j] - state->alloc[index][j] + 1);	//Else randomly request some more resource s.t. its request is less than or equal claim - alloc
	}
}

//Check if the given state is a safe state 
bool safe(State tempState) {
	//Loop through all non-terminated processes and see if any of them can be taken to completion
	vector <int> rest;	//At any time rest contains all the processes that have to run to completion
	for(int i = 0; i < p; i++) {
		if(terminated[i] == false)
			rest.push_back(i);
	}

/*
	cout << "The processes which have yet not run to completion in simulation:\n";
	for(int i = 0; i < rest.size(); i++) {
		cout << rest[i];
	}
	cout << endl;*/


	int flag;
	//Check now if there is any path through which rest of the remaining processes can run to completion
	bool possible = true; //Assume there is such a path
	while(possible && rest.size() > 0) {
		//Check out of all processes if any can run to completion
		//That is find a process whose claim - alloc <= available resources.
		for(int i = 0; i < rest.size(); i++) {
			flag = 1; //assume rest[i]th process can actually go to completion
			for(int j = 0; j < r; j++) {
				if(tempState->claim[rest[i]][j] - tempState->alloc[rest[i]][j] > tempState->available[j]) {
					//In this case rest[i] th process cannot go to completion
					flag = 0;
					break;
				}
			}
			//If found:
			if(flag == 1) {//If flag = 1 that implies rest[i]th process can run to completion
				//cout << "This process can run to completion: " << rest[i] << endl;
				//Give all the allocated resources to available pool
				for(int j = 0; j < r; j++) {
					tempState->available[j] += tempState->alloc[rest[i]][j];
				}
				//Delete this process from rest of the remaining processes taking that it will run to completion
				rest.erase(rest.begin() + i);
				possible = true;	
				break;	//break with possible true. Runs next iteration of while loop
			}
			else possible = false;	//This is reached whenever currently iterated process can't run to completion
		}	
	}
	return rest.size() == 0; //If rest size is 0 it means all processes could run to completion
}

//To terminate the processes which has been allocated max resources they claimed for
void terminateFullyAllocatedProcesees() {
	for(int i = 0; i < p; i++) {	//iterate through all the processes
		if(isEqual(state->alloc[i], state->claim[i], r)) { //That is only when alloc[i][*] = claim[i][*]
			//Return all the resources...
			add(state->available, state->alloc[i], state->available, r);	//Add the resources taken by this process to the pool of available resources.
			subtract(state->alloc[i], state->alloc[i], state->alloc[i], r); //Since the process can't withhold the resources it must give them back setting its allocated resources = 0
			
			// ...and terminate the process
			terminated[i] = true;	//Finally terminate this process
		}
	}
}


/**************************************************************
 *	Function:	requestGreaterThenAvailable
 *
 *	Determines if request made by process is greater than available resources.
 *
 *	Parameters:
 *
 *		request 	- 	The request array of resources.
 *
 *	Returns:
 *		TRUE if request cannot be made.
 *		FALSE if request can be made.
 **************************************************************/
bool isRequestGreaterThanAvailable(int * request) {
	//Check if this resource request can be made
	int flagP = 1; //Flag to determine possiblity whether request can potentialy be given or not.
	//Assume request for resource can really be given by letting flagP = 1
	for(int k = 0; k < r; k++) { //Suspend this process for this second if request greater than available resources
		if(request[k] > state->available[k]) {
			flagP = 0;
			break;
		}
	}
	return flagP == 0;
}

void buildTempState(State tempState, int * request, int index) {
	//The new temp state will have same amount of total resources.
	for(int i = 0; i < r; i++) {
		tempState->resources[i] = state->resources[i];
		tempState->available[i] = state->available[i];
	}

	//Dbg Line: tempState->displayState();
	//The new temp state will have same claimed and allocated resources...
	for(int i = 0; i < p; i++)
		for(int j = 0; j < r; j++) {
			tempState->alloc[i][j] = state->alloc[i][j];
			tempState->claim[i][j] = state->claim[i][j];
		}
	//Dbg Line: tempState->displayState();
	// ...except that process with index 'index' will have request amount of more resources...
	add(tempState->alloc[index], request, tempState->alloc[index], r);
	//Dbg Line: tempState->displayState();
	// ...and availble resources will be reduced in this tempState by request amount
	subtract(state->available, request, tempState->available, r);
	//Dbg Line: tempState->displayState();
}


//Allocate the resources to process with index 'index' with amount = request
void allocate(int * request, int index) {
	add(state->alloc[index], request, state->alloc[index], r);	//Increase the allocated resources of the i-th process by request amount.
	subtract(state->available, request, state->available, r); //Decrease the available resources by request amount.
}


//Adds x and y arrays each of size 'size' and stores the result in result array
void add(int * x, int * y, int * result, int size) {
	for(int i = 0; i < size; i++) {
		result[i] = x[i] + y[i];
	}
}

//Subtracts x and y arrays each of size 'size' and stores the result in result array
void subtract(int * x, int * y, int * result, int size) {
	for(int i = 0; i < size; i++) {
		result[i] = x[i] - y[i];
	}
}

//Returns whether two arrays are equal
bool isEqual(int * x, int * y, int size) {
	bool flag = true;	//Assume they are equal
	for(int i = 0; i < size; i++) {
		if(x[i] != y[i]) {
			flag = false;
			break;
		}
	}
	return flag;
}
#endif