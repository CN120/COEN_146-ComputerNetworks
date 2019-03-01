/*
provided command line info
- Router ID
- number of nodes
- table with costs NxN passed as filename
- table with machine names, IP's, and prt #'s passed as filename
 */

 /*
Program Threads
Thread 1:
- loops forever
- recieves messages from other nodes
- updates cost table with new cost "c" for x⟶y and y⟶x

Thread 2:
- reads new change from keyboard every 10 seconds
- updaes cost table
- sends messages using UDP
- finishes 30 seconds after executing 2 changes

Thread 3:
- loops forever
- sleeps for random number of seconds (10-20)
- runs algorithm to update least costs
  */


  
