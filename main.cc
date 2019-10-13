
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




int main(int argc, char** argv){
  string file_name = argv[1];
  string given_word = argv[2];
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
    int temp;
    vector<int> size;
    bool found = false;
    string words;
    string file_as_string = "";
    vector<string> words_vector;
    //If  read() is interrupted by a signal after successfully reading some data, it returns the number of bytes read.
    while ((i = read(sv[parentsocket], &buf, sizeof(buf))) > 0) {
      file_as_string.push_back(buf);
    }
    cout << "reading from parent" << endl;


    istringstream iss(file_as_string);
    string word;
    while(iss >> word) { // while loop to split up words til the end of text file
      words_vector.push_back(word);
    }
    int x = 0;
    int b = 0;
    int j = 0;
    unsigned int k = 0;
    for (unsigned int i = 0; i < words_vector.size(); i++) {
      if (i == words_vector.size()-1) {
        for (unsigned int i = 0; i < size.size(); i++)
          write(sv[parentsocket],&size[i], size.size()+1);
      }
      while (words_vector[k] != given_word && found == true) {
        k++;

      }
      if (words_vector[i] == "*") {
        j++;
        found = true;
      }

      if (words_vector[k] == given_word && i > k && words_vector[i] == "*" && found == true) {
        x++;
        b = j - 1;
        size.push_back(b);
        found = false;
        k = i;

        cout << given_word << " is in " << j << endl;
      }

    }

  }
  else { // parent
    int x;
    int buf;
    vector<string> file;
    vector<int> size;
    ifstream fileHandler;

    fileHandler.open(file_name.c_str()); // open text file

    for (string word; getline(fileHandler,word);) { // go through every line in the textfile.
      file.push_back(word);
    }
    for (unsigned int i = 0; i < file.size() ; i++) {
      file[i] = file[i] + " * ";
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
    unsigned int k;
    read(sv[childsocket],&buf, sizeof(buf));
    cout << file[buf] << endl;







  }


  return 0;
}
