Partners: Dylan Banh, Marisa Liu

Summary/Solution Approach: 
  Server:
    Everytime a new client joins it creates a new thread that calls the function chat
    The chat function will first read in the username and print that a client joined of the client and store it in an array along with the ip in a parallel array
    Then it will keep looping and reading the incoming text, parsing it and sending it to the correct user or it will implement the special commands accordingly
  Client
    Initially sends it's username to the server
    Creates a new thread that keeps looping to read and print incoming messages
    The main thread will keep looping to send the strings from stdin to the server
    

Errors we encountered:
  The first time the client sends a message to the server it sends an additional junk character at the beginning. To solve this issue, for the first received message we parse the string starting at the second character.
  Encountered a buffer overflow in server if we sent too long of a message. This error was fixed once we realized that we were malloc-ing only enough for the username portion and forgot to change that to include the entire message being sent.
  

Note: we coded a limit of 100 characters per message and only allow 50 users to join a single chat room.  
    

