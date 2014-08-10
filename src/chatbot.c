#include <stdio.h>
#include <string.h>
#define LINELENGTH 80
#define SEPCHARS "* .,\"\n"

int main(void) {
  char line[LINELENGTH];
  char *word;

  printf("\n Chatbot v0.0.1!\n");

  while(1) {
    fgets(line, LINELENGTH, stdin);
    if (strlen(line) <= 1) break; /*exit program*/
    word = strtok(line, SEPCHARS); /*Find first word */
    while (word != NULL) {
      if (strncmp(word, "Hi", 4) == 0) {
        printf("> %s\n","Hello" );
      }
      else if (strncmp(word, "coding", 4) == 0) {
        printf("> %s\n","Yes, I love programming!" );
      }
      else if (strncmp(word, "?", 4) == 0) {
        printf("> %s\n","Is that a question" );
      }
      else if  (strncmp(word, "love", 4) == 0) {
        printf("> %s\n","Yes I love it" );
      }
      else if  (strncmp(word, "How", 4) == 0) {
        printf("> %s\n","Oh, now you don't know lair!" );
      }
      else if  (strncmp(word, "What", 4) == 0) {
        printf("> %s\n","It is clear, ain't it?" );
      }
      else {
        printf("> %s\n", "Sorry, I don't know what to say about that" );
      }
      // printf("\"%s\"\n", word);
      word = strtok(NULL, SEPCHARS);
    }
  }
}