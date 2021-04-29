CFS Simulator
==============
1 virtual tick = 1 real tick
Tasks are read from file specified into linked list. 
Then simulation loop starts. Tasks whose appearance time is suitable are added to task queue. Vruntime for new tasks is current time.
If there is burst time left, task is readded to task queue with vruntime adjusted with actual runtime and priority.
Priority is calculated by dividing process priority value by total of priority values of all active processes.


Tests
==============
rbt_test.c contains functions used to test red black tree implementation. Also it has function to dump tree in graphwiz format. Visualization showed that tree is red black tree indeed.

Known flaws
==============
Program is probably not well designed and CFS related computations are messier than I want them to be.
I guess that is normal for C programs.

Build instruction
==============
I've used clang and cmake. However `gcc main.c` or `clang main.c` should work fine.

