
//  Copyright 2019 Nguyen vu Nguyen
#include <p1.h>




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
    cout << "Reading from parent..." << endl;
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
    for (unsigned int i = 0; i < words_vector.size(); i++) {
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
        printf("output: %i", j);
        printf("\n");
        numb.push_back(b);
      }
      if (i == words_vector.size()-1) {
        ssize_t total_bytes_written = 0;
        while (total_bytes_written != 1024) {
          assert(total_bytes_written < 1024);
          ssize_t bytes_written = write(sv[parentsocket],
                                  &numb[total_bytes_written],
                                  1024 - total_bytes_written);

          if (bytes_written == -1) {   /* Report failure and exit. */
            break;
          }
        total_bytes_written += bytes_written;
        }
      shutdown(sv[parentsocket], SHUT_WR);
      }
    }
  }
  else { /* parent */
    vector<string> file;
    vector<int> size;
    ifstream fileHandler;
    fileHandler.open(file_path.c_str()); /* open text file */
    for (string word; getline(fileHandler,word);) { /* go through every line in the textfile */
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


    write(sv[childsocket], passablefile.c_str(), (fileSize)); // pass content to child
    shutdown(sv[childsocket], SHUT_WR);
    wait(NULL);
    cout << "sending to child" << endl;
    for (unsigned int i = 0; i < file.size(); i++) {
      file[i].erase(std::remove(file[i].begin(),
                    file[i].end(), '`'),
                    file[i].end());
    }
    int j;
    int i;
    while ((i = read(sv[childsocket], &j, sizeof(j))) > 0) {

      cout << file[j] << endl;

    }

  }


  return 0;
}
