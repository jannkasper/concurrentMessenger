1. COMPILATION INSTRUCTIONS
	- Makefile starts in the terminal

2. START-UP INSTRUCTIONS
	- 3 terminals needed to check the operation of the program [./s ./k ./k]
	- to run the server, issue the command: ./s
	- to run the client, issue the command: ./k

3. CONTENT OF THE FILE Jan.Kasper.114885_s.c
	- the server is not subject to user service
	- the server works on received information from clients
	- the server displays the result of performed operations
	- we can finish the work of the server by calling ENETER


   CONTENTS OF THE FILE Jan.Kasper.114885_k.c
     * login
	- login using ID [int] and LOGIN [char [10] - only alphabet letters]
	- login block after 3 error attempts in ID and LOGIN
	- start asynchronous receiving messages with '1' [otherwise '0']
	- calling any user function by selecting [int] from the 1-7 range
     	* function [1] [6]
	- subscription in the form of a single character [char]
     * function [2]
	- subscribers [char] are defined when sending messages
	- when sending a message, priority is given on a 0-10 scale [int]
	- message sent has limited capacity [char [100]]
     * function [3]
	- displaying the MENU again
     * function [4]
	- blocking the user by entering his exact LOGIN
     * function [5]
	- displaying synchronous messages according to the priority of sending
     * function [7]
	- running processes and the user's pipe removed when shutting down the system
