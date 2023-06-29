# Rovers Server
----------------------------------------------------------------
This project is part of a course of networking. The task is to implement the server side of the application that coordinates the robots in the maze.

The work was based on the use of TCP/IP communication via sockets, using multiple threads for simultaneous control of multiple robots (clients).

The language chosen for implementation was python. 

The task can be divided into several key aspects:
* Communicating with the robot using specific commands
* Authentication using keys
* Ensuring correct operation in case of message delays or interruptions
* The logic part of the server that controls the coordination of the robot
