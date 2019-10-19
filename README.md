# IPC-and-map-reduce

# subProject1

Sub Project #1 is a c++ program that has 2 processes, a parent and a child process.
Given a text file and desired word in the terminal, e.g: "./sub_project1 big.txt word",
the program searches for "word" in "big.txt".
The program starts by letting the parent process open and read the text file,
go through each line and put each line in a vector. Once done, the parent adds a char to the end of each line.
The char will indicate that the line has ended.
The parent than compiles the vector into a string variable and
passes the string containing the file to the child process using the UNIX domain socket write().
The child will than read() and parse the content to find the lines.
Using the char as an end of line indicator, the child is able to save the line numbers in a vector.
After every line is parsed, the child sends the vector to the parent process using multiple write().
After, the parent process removes the char and outputs the lines in their alphabetical order that contains the desired word.

_SubProject1 Issues_

The main problem that often occurred was the child process kept getting blocked and the program would forever be locked.
The solution was shutdown(), which gave the child process an end of file indication and shuts down part of the connection.
One big problem of how I designed my program would be how I chose to indicate the end of a line, which is char.
If a file were to contain a char, it would count as the end of a line in a file and could result in inaccurate parsing.

_HOWTO_

To get a copy of the program on your local linux environment,
"git clone https://github.com/WenLag/IPC-and-map-reduce.git" in your terminal.
Simply start the program by typing "make" in your program. You should now have an executable file called sub_project1.
Run the program by typing "./sub_project1 [textfile] [desired word]" in your terminal.
