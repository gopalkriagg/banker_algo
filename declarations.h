/****************************************************
 * 	Variables: Global Variables
 *		p 			-	Number of process
 * 		r 			- 	number of (type of) resources
 *****************************************************/
extern int p, r;

/****************************************************
 * 	Variables: Global Variables
 *		terminated 	-	This array would eventually contain list of terminated process
 * 		state 		- 	State of the current system
 *****************************************************/
extern bool * terminated;
typedef StateClass *  State;
extern State state;


bool allTerminated();
void buildResourceRequest(int * request, int index);
bool safe(State tempState);
void terminateFullyAllocatedProcesees();
bool isRequestGreaterThanAvailable(int * request);
void add(int * x, int * y, int * result, int size);
void subtract(int * x, int * y, int * result, int size);
bool isEqual(int * x, int * y, int size);
void buildTempState(State tempState, int * request, int index);
void allocate(int * request, int index);