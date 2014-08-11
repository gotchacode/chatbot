#include <stdio.h>
#include <string.h>
#include "chatbot.h"

int main(void) {
  char line[LINELENGTH];
  char *word;

  printf("\n Chatbot v0.0.1!\n");

  while(1) {
    fgets(line, LINELENGTH, stdin);
    if (strlen(line) <= 1) break; /*exit program*/
    word = strtok(line, SEPCHARS); /*Find first word */
    while (word != NULL) {
      // Some responses based on the keywords
      if (strncmp(word, "Hi", 50) == 0) {
        printf("> %s\n","Hello" );
      }
      else if (strncmp(word, "coding", 50) == 0) {
        printf("> %s\n","Yes, I love programming!" );
      }
      else if (strncmp(word, "hear", 50) == 0) {
        printf("> %s\n","What you heard is right" );
      }
      else if (strncmp(word, "python", 50) == 0) {
        printf("> %s\n","Yo, I love Python" );
      }
      else if (strncmp(word, "?", 50) == 0) {
        printf("> %s\n","Is that a question ?" );
      }
      else if  (strncmp(word, "love", 50) == 0) {
        printf("> %s\n","Yes I love it" );
      }
      else if  (strncmp(word, "How", 50) == 0) {
        printf("> %s\n","Oh, now you don't know lair!" );
      }
      else if  (strncmp(word, "What", 50) == 0) {
        printf("> %s\n","It is clear, ain't it?" );
      }
      else if (strncmp(word, "loveyou", 50) == 0) {
        printf("> %s\n","I love you too" );
      }
       else if (strncmp(word, "sorry", 50) == 0) {
        printf("> %s\n","Are you really Sorry?" );
      }
       else if (strncmp(word, "doing", 50) == 0) {
        printf("> %s\n", "How can you do that?" );
      }
       else if (strncmp(word, "light", 50) == 0) {
         printf("> %s\n", "Yes, I like light");
       }
      else {
        printf("> %s\n", "Sorry, I don't know what to say about that" );
      }
      // printf("\"%s\"\n", word);
      word = strtok(NULL, SEPCHARS);
    }
  }
}
