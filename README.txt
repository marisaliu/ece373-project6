The provided code is the concurrent echo client and server from class. You can use this code to build your chat client and server.

Provide a summary of your code, your solution approach, and any problems you encountered and how you addressed them.  You should also state any problems with the code, and if its usage differs in anyway from the specification above.  Also, if you worked with a partner, make sure to state your partner. Note that everyone must turn in their code. 

-client first connects, send username to server
-server needs to accept and store the username and connect it to the clientfd
-server stores usernames in array
-server prints when users join and leave
-when client sends message, parse first token starting with @ and ending with space to figure out where to redirect the message
-special commands list-users and quit
-when quit, server must clean up any state from quit user to prevent memory leaks
