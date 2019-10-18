
//  Copyright 2019 Nguyen vu Nguyen
#include <sstream>
#include<iostream>
#include<fstream>
#include<string>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <vector>
#include <ctype.h>
#include <algorithm>
using namespace std;




int main(int argc, char** argv){
  string file_path = argv[1];
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
    int counter = 0;
    char c;
    while (file_as_string[counter]) {
      c = file_as_string[counter];
      file_as_string[counter] = tolower(c);
      counter++;
    }
    for (int i = 0, len = file_as_string.size(); i < len; i++)
    {
        // check whether parsing character is punctuation or not
        if (ispunct(file_as_string[i]) && file_as_string[i] != '~')
        {
            file_as_string.erase(i--, 1);
            len = file_as_string.size();
        }
    }

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

      if (words_vector[i] == "~" ) {
        j++;
        found = true;
      }

      while (words_vector[k] != given_word && found == true) {
        if (k == words_vector.size()-1) {
          break;
        }
        if (k < i) {
          k++;
        }
        else {
          found = false;
        }
      }
      cout << "k" << k << endl;
      cout << "I:" << i << endl;
      cout << "VS:" << words_vector.size() << endl;
      if (words_vector[k] == given_word && i > k && found == true) {
        x++;
        b = j - 1;
        found = false;
        k = i;
        printf("output: %i", j );
        printf("\n");

        write(sv[parentsocket],&b, sizeof(b));
      }
      if (i == words_vector.size()-1) {
        shutdown(sv[parentsocket],SHUT_WR);
      }

    }

  }
  else { // parent


    vector<string> file;
    vector<int> size;
    ifstream fileHandler;

    fileHandler.open(file_path.c_str()); // open text file

    for (string word; getline(fileHandler,word);) { // go through every line in the textfile.
      file.push_back(word);
    }
    for (unsigned int i = 0; i < file.size() ; i++) {
      file[i] = file[i] + " ~ ";
    }
    string passablefile = "";

    for (unsigned int i = 0; i < file.size(); i++) {
      passablefile = passablefile + file.at(i);
    }

    int fileSize = passablefile.size();


    write(sv[childsocket], passablefile.c_str(), (fileSize)); // pass content to child

    cout << "sending to child" << endl;

    for (unsigned int i = 0; i < file.size(); i++) {
      file[i].erase(std::remove(file[i].begin(), file[i].end(), '~'), file[i].end());
    }

    shutdown(sv[childsocket], SHUT_WR);
    wait(NULL);
    //   for (unsigned int i = 0; i < file.size(); i++) {
    //   read(sv[childsocket],&i, sizeof(i));
    //   cout << file[i] << endl;
    // }
    int i;
    int buf;
    while ((i = read(sv[childsocket], &buf, sizeof(buf))) > 0) {
      cout << file[buf] << endl;

    }






  }


  return 0;
}
