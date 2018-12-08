Partners: Dylan Banh, Marisa Liu

Summary: 
  Server:
    Everytime a new client joins it creates a new thread that calls the function chat
    The chat function will first read in the username and print that a client joined of the client and store it in an array along with the ip in a parallel array
    Then it will keep looping and reading the incoming text, parsing it and sending it to the correct user
  Client
    Initially sends it's username to the server
    Creates a new thread that keeps looping to read and print incoming messages
    The main thread will keep looping to send the strings from stdin to the server
    

