
//  Copyright 2019 Nguyen vu Nguyen

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
  string file_name = "big.txt";
  int sv[2]; /* the pair of socket descriptors */
  static const int parentsocket = 0;
  static const int childsocket = 1;
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        exit(1);
  }

  int pid = fork();

  if (pid == 0){ //child
    cout << "child runs" << endl;
    string passablefile = "";
    char buf;



    read(sv[parentsocket], &buf, sizeof(buf));
    cout << "reading" << endl;
    cout << "buf: " << buf << endl;


  }

  else { // parent
    vector<string> file;
    cout << "parent runs" << endl;

    ifstream fileHandler; 
    string word;
    fileHandler.open(file_name.c_str());

    std::getline(fileHandler,word);
    file.push_back(word);




    string passablefile = "";
    int fileSize = passablefile.size();
    for (unsigned int i = 0; i < file.size(); i++) {
      passablefile = passablefile + file.at(i);
      cout << passablefile[i] << endl;
    }
    cout << "sending7" << endl;
    write(sv[childsocket], passablefile.c_str(), fileSize+1);
    cout << "sending8" << endl;
    wait(NULL);
  }


  return 0;
}
