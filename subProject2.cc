
//  Copyright 2019 Nguyen vu Nguyen
#include <p1.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

/* UNFINISHED */
int main(int argc, char** argv) {
  string file_path = argv[1];
  string given_word = argv[2];

int shm_open (const char *name, int oflag, mode_t mode);


  int pid = fork();

  if (pid == 0) { /* child */
    char buf;
    int i;
    bool found = false;
    string words;
    string file_as_string = "";
    vector<string> words_vector;



    int counter = 0;
    char c;
    while (file_as_string[counter]) {
      c = file_as_string[counter];
      file_as_string[counter] = tolower(c);
      counter++;
    }
    for (int i = 0, len = file_as_string.size(); i < len; i++) {
        // check whether parsing character is punctuation or not
        if (ispunct(file_as_string[i]) && file_as_string[i] != '`') {
            file_as_string.erase(i--, 1);
            len = file_as_string.size();
        }
    }
    istringstream iss(file_as_string);
    string word;
    /* while loop to split up words til the end of text file */
    while (iss >> word) {
      words_vector.push_back(word);
    }
    vector<int> numb;
    int b = 0;
    unsigned int j = 0;
    unsigned int k = 0;

    for (unsigned int i = 0; i < words_vector.size(); i++) { /* parsing string file */
      if (words_vector[i] == "`") {
        j++;
        found = true;
      }
      while (words_vector[k] != given_word && found == true) {
        if (k < i) {
          k++;
        } else {
          found = false;
        }
      }

      if (words_vector[k] == given_word && i > k && found == true) {
        b = j - 1;
        found = false;
        k = i;
        numb.push_back(b);
      }

    }
  } else { /* parent */
    vector<string> file;
    vector<int> size;
    ifstream fileHandler;

    fileHandler.open(file_path.c_str()); /* open text file */

    /* go through every line in the textfile and put each line in the vector*/
    for (string word; getline(fileHandler, word);) {
      file.push_back(word);
    }

    for (unsigned int i = 0; i < file.size() ; i++) {
      file[i] = file[i] + " ` ";
    }

    string passablefile = "";

    for (unsigned int i = 0; i < file.size(); i++) {
      passablefile = passablefile + file.at(i);
    }

    int fileSize = passablefile.size();


    for (unsigned int i = 0; i < file.size(); i++) { /* remove all the ` that was indicatng end of line */
      file[i].erase(std::remove(file[i].begin(),
                    file[i].end(), '`'),
                    file[i].end());
    }
    int j;
    int i;


  }
  return 0;
}
