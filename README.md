# MiniBash
COMP 8567- Advanced Systems Programming<br>
Instructors: Dr. Boufama and Dr. Ranga<br>
Project Work<br>
Title: Distributed processing of grep with client server programming. 

Write two programs server.c (server) and dgrep.c (client) in C to implement a simple
command to search whole words in two files using distributed processing.
<br>The server and the client are required to run on two different machines.
<br>The communication between the client and the server will be through sockets
<br>The server must start running before the client, and wait for a connection from the
client.
<br>The server must run in an infinite loop.
The client’s name is dgrep and must accept the following (three) command line
arguments
<br>argv[1]=pattern (pattern to be searched in whole words)
<br>argv[2]= file1(Relative or absolute path of the first file)
<br>argv[3]= file2 (Relative or absolute path of the second file)
<br>Ex: $ dgrep hello sample.txt test.txt
<br>Both file2 and file2 are initially stored on the client machine.
<br>if either file1 or file2 cannot be found by the client, the client program is
terminated with a suitable message.
<br>If the client is unable to connect to the server for any reason, the client program
is aborted and a suitable message is displayed.
<br>If the client successfully connects to the server:
<br>The client sends file2 to the server (file1 remains on the client machine)
<br>the client runs grep -w pattern file1 locally on the client machine
<br>The server runs grep -w pattern file2 and returns the result to the client. (use
the system() library function to execute commands)
<br>The communication between the server and the client must be through sockets

<br>The client should finally display the combined results (from both the client and
the server – in that order) on the standard output.
<br>Each line of the display must be prefixed by the name of the respective
file followed by ‘:’ Ex: the following should be the output of
$ dgrep hello text1.txt text2.txt assuming text1.txt has 4 lines of “Hello
and Welcome to COMP8567” and text2.txt has three lines of “Hello and
Welcome”
<br>text1.txt:Hello and Welcome to COMP8567
<br>text1.txt:Hello and Welcome to COMP8567
<br>text1.txt:Hello and Welcome to COMP8567
<br>text1.txt:Hello and Welcome to COMP8567
<br>text2.txt:Hello and Welcome
<br>text2.txt:Hello and Welcome
<br>text2.txt:Hello and Welcome
<br>text2.txt:Hello and Welcome
<br>The pattern should be highlighted in red
<br>The client can terminate its connection to the server and also terminate its
execution after the displaying the results as mentioned in the previous step.
<br>Since the server runs continuously and is waiting for the client, the client
program can run anytime with the required parameters. 
