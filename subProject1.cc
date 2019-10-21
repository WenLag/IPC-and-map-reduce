
//  Copyright 2019 Nguyen vu Nguyen
#include <p1.h>

/* FINSHED */
int main(int argc, char** argv) {
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

  if (pid == 0) { /* child */
    char buf;
    int i;
    bool found = false;
    string words;
    string file_as_string = "";
    vector<string> words_vector;
    /* If  read() is interrupted by a signal after successfully reading some data,
      it returns the number of bytes read */
    while ((i = read(sv[parentsocket], &buf, sizeof(buf))) > 0) {
      file_as_string.push_back(buf);
    }


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


      if (i == words_vector.size()-1) { /* sending file to parent once it has finished parsing */
        ssize_t total_bytes_written = 0;
        while (total_bytes_written != (numb.size()*4)) {
          ssize_t bytes_written = write(sv[parentsocket],
                                  &numb[total_bytes_written],
                                  (numb.size()*4) - total_bytes_written);

          if (bytes_written == -1) {   /* Report failure and exit. */
            break;
          }
        total_bytes_written += bytes_written;
        }
      shutdown(sv[parentsocket], SHUT_WR); /* end of file indication */
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

    /* pass content to child */
    write(sv[childsocket], passablefile.c_str(), (fileSize));
    shutdown(sv[childsocket], SHUT_WR); /* end of file indication to prevent blocking of child's read */
    wait(NULL);

    for (unsigned int i = 0; i < file.size(); i++) { /* remove all the ` that was indicatng end of line */
      file[i].erase(std::remove(file[i].begin(),
                    file[i].end(), '`'),
                    file[i].end());
    }

    int j;
    int i;
    while ((i = read(sv[childsocket], &j, sizeof(j))) > 0) {
      cout <<  file[j] << "\n";
    }
  }
  return 0;
}
