# CSIS-434-Multi-Threading-Mutex-Example
Windows Threads.
Created by Sam Arnold on 11/28/17


Example of Usage: 
./WindowsThreads.exe 20 300 s

good test to generate compare: 
./WindowsThreads.exe 98 100000 1
./WindowsThreads.exe 98 100000 0



Paramater 1 - Number of threads, Accepted Values Range: 1-100
This dedicates how many threads are created in the project

Paramater 2 - Number of Iterations, Accepted Values Range: 1-1000000
This dedicates how much work is done on each thread. More Work more likely for race condition if mutex is not enabled

Paramater 3 - Mutex lock, , Accepted Values Range: 1 or 0
This detemines whether there is a mutex lock that controls the relationship between the threads and variable
if the value is 1 race conditions should not happen if value is 0, Race conditions might occure
