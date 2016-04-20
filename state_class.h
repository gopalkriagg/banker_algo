#ifndef STATE_CLASS_H
#define STATE_CLASS_H 

class StateClass{
private:
	int np, nr;
public:
	int * resources;
	int * available;
	int ** claim;
	int ** alloc;

	StateClass(int p, int r);
	void inputStateFromUser();
	void displayState();
};

#endif