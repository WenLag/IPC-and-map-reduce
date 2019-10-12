
//  Copyright 2019 Nguyen vu Nguyen
#include <sstream>
#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <vector>
using namespace std;




int main(void){
  string file_name = "test.txt";
  string given_word = "volunteer";
  int sv[2]; /* the pair of socket descriptors */
  static const int parentsocket = 0;
  static const int childsocket = 1;
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        exit(1);
  }

  int pid = fork();

  if (pid == 0){ //child
    char buf;
    int i;
    string file_as_string = "";
    vector<string> words_vector;
    //If  read() is interrupted by a signal after successfully reading some data, it returns the number of bytes read.
    while ((i = read(sv[parentsocket], &buf, 1)) > 0) {
      file_as_string.push_back(buf);
    }
    cout << "reading from parent" << endl;
    istringstream iss(file_as_string);
    string word;

    while(iss >> word) { // while loop to split up words til the end of text file
    words_vector.push_back(word);
    }
    for (unsigned int i = 0; i < words_vector.size(); i++) {
      if (given_word == words_vector[i]) {
        write(sv[parentsocket], &i, 4);
      }
    }
  }

  else { // parent
    int buf;
    vector<string> file;
    ifstream fileHandler;

    fileHandler.open(file_name.c_str()); // open text file


    for (string word; getline(fileHandler,word);) { // go through every line in the textfile.
      file.push_back(word);
    }
    string passablefile = "";

    for (unsigned int i = 0; i < file.size(); i++) {
      passablefile = passablefile + file.at(i);
    }

    int fileSize = passablefile.size();

    write(sv[childsocket], passablefile.c_str(), fileSize+1); // pass content to child

    cout << "sending to child" << endl;
    shutdown(sv[childsocket], SHUT_WR);
    wait(NULL);
    read(sv[childsocket], &buf, 1); // read the line number

    vector<string> words_vector; // problem: can't use previous vector to print out word. solution: make new vector
    istringstream iss(passablefile);
    string word;
    while(iss >> word) {
    words_vector.push_back(word);
    }
    cout << words_vector[buf] << endl; // print out the word with the corresponding read number
  }


  return 0;
}
