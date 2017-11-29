#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <process.h> // for _beginthreadex
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <sstream>


//unsigned long long
int SharedCount = 0;
//int iterations = 0;
HANDLE ghMutex;
bool enableMutex = false; // Command line parameter
int count = 0; // Actual count after update by threads
unsigned long numberIter = 0; // Command line parameter

using namespace std;


unsigned _stdcall threadWork(void *threadNo) {

	if (enableMutex == true) {
		if (ghMutex == NULL) {
			printf("CreateMutex error: %d\n", GetLastError());
			return 1;
		}
		
		  // Create a CPU load by burning some cycles. Hope to cause contention
		for (unsigned int i = 0; i < numberIter; i++) {
			WaitForSingleObject(ghMutex, INFINITE);
			// now the mutax insures that others wont touch
			for (int j = 0; j < 10; j++) {
				SharedCount = SharedCount + abs(*(int *)threadNo); // Update the count
				//cout << SharedCount << endl; 
			}
			// Release Mutex		
			ReleaseMutex(ghMutex);
		}
	}
	// End of Mutex
	else {
		for (unsigned int i = 0; i < numberIter; i++) {
			for (int j = 0; j < 10; j++) {
				SharedCount = SharedCount + abs(*(int*)threadNo); // Update the count
				//cout << SharedCount << endl;
			}
		}
	}
	// notify main this thread is done
	return 0;
}




// Get the command line parameters
int main(int argc, char * argv[]) {

	//argc should be 4
	int temp = 0;
	int  threadCount;
	if (argc != 4)
	{
		printf("Invalid Parameters \n");
		printf("Default Values: Iter = 100000, threads = 98, mutex = 1 \n");
		numberIter = 1000000; // Default 1000000 	
		temp = 98; // Default 98
		 threadCount = temp;
		enableMutex = 1;
	}
	else {
		//int exponent = atoi(argv[2]); // use atoi with integer
		cout << "Input For Number Of Threads : " << atoi(argv[1]) << endl; //and argv[1] should be numberOfThreads, 
		cout << "Input For Iterations        : " << argv[2] << endl;	// argv[2] is number of Iterations,

		if ((atoi(argv[2]) > 0) && (atoi(argv[2]) < 10000000))
			numberIter = atoi(argv[2]);
		else {
			cout << "Invalid Iteration value, Not in Range: Default  10000" << endl;
			numberIter = 10000;
		}

		// handle itertion input 
		
		temp = atoi(argv[1]);
		if ((temp <100) && (temp > 0))
			threadCount = atoi(argv[1]);
		else {
			threadCount = 98;
			cout << "Invalid Thread value, Default value used " << threadCount << endl;
			}
		// Handle Mutex if not 1 or 0 default
		enableMutex = atoi(argv[3]);
		if ((enableMutex == 1) || (enableMutex == 0)) {
			cout << "Mutex value is: " << enableMutex << endl;
		}
		else {
			cout << "Invalid Mutex value, Defaulted to Mutex on: " << enableMutex << endl;
			enableMutex = 1;
		}

	}


	//int i = 1;
	// const int threadCount = set_arg2(temp);
	//int  threadCount = set_arg2();
	//const int threadCount = 89;

	//cin >> threadCount;
	unsigned int* array = new unsigned int[threadCount];

	unsigned  int* ThreadID = new unsigned int [threadCount];
	uintptr_t*  ThreadHandle = new uintptr_t [threadCount];

	//uintptr_t ThreadHandle[threadCount];
	int Param;


	// Create a mutex with no initial owner
	ghMutex = CreateMutex(NULL, FALSE, NULL);// default security attributes// initially not owned  // unnamed mutex
	

	

	for (int i = 0; i < threadCount; i++) {
		// ThreadHandle[i] = CreateThread(NULL, 0, Summation, &Param, 0, &ThreadID[i]);
		// ThreadHandle[i] = _beginthreadex(NULL, 0, threadWork, &ThreadID[i], 0, &ThreadID[i]);
		ThreadHandle[i] =_beginthreadex(NULL, 0, threadWork, &ThreadID[i], 0, &ThreadID[i]);
		cout <<"Thread Created: " << ThreadHandle[i]
			<< " ThreadID: " << &ThreadID[i] << endl;
	}

	Sleep(1000);
	for (int i = 0; i < threadCount; i++) {
	//	WaitForMultipleObjects(threadCount, (HANDLE *)ThreadHandle, TRUE, INFINITE);
		WaitForSingleObject((HANDLE *)ThreadHandle[i], INFINITE);
		if (ThreadHandle[i] != NULL) {
			CloseHandle((HANDLE)ThreadHandle[i]);
			cout << "Thread Closed " << &ThreadID[i] << " Thread Handdle "
				<< ThreadHandle[i] << endl;
		}
	}
	// Calculate the theoratical value
	int theoretical = 0;
	for (unsigned int i = 0; i < numberIter; i++) {
		for (int i = 0; i < threadCount; i++) {
			theoretical += (10 * ThreadID[i]);
		}
	}

	cout << endl << "Actual Sum    : " << SharedCount << endl;
	cout << "Predicted Sum : " << theoretical << endl << endl;

	// Check if Value is Correct
	if (SharedCount != (theoretical)) {
		cout << endl << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "Error: Race Condition detected!!!!" << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	}

	cout << "Mutex : " << enableMutex << endl << "numberIter : "   << numberIter << endl << "Thread Count : " << threadCount;
	cout << endl;
	system("pause");
	return 0;
}