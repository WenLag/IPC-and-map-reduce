
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

using namespace std;




int main(void){
  int sv[2]; /* the pair of socket descriptors */
  char buf;
  string word;
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        exit(1);
  }

  int pid = fork();
  ifstream file_("big.txt");
  std::string line_;
  if (pid > 0) { //parent
  //   while(file_ >> word) {
  //   getline(file_,line_);
  //   cout << line_ << endl;
  // }

    cout << "parent runs" << endl;
    write(sv[0], "big.txt", 1);
    printf("parent: sent 'b'\n");
    read(sv[0], &buf, 1);
    printf("parent: read '%c'\n", buf);
    wait(NULL); /* wait for child to die */
  }

  if (pid == 0){ //child
    cout << "child runs" << endl;
    read(sv[1], &buf, 1);
    printf("child: read '%c'\n", buf);
    buf = toupper(buf);  /* make it uppercase */
    write(sv[1], &buf, 1);
    printf("child: sent '%c'\n", buf);
  }
  return 0;
}
