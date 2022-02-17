#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *word;
  int n;
} word_count;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("You need to pass the name of a file to this program:\n./task1.exe [FILENAME]");
    return (1);
  }
  int N = 25;
  char buf[255];
  word_count **map;
  map = calloc(sizeof(word_count *), N);
  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("Error while reading a file... Maybe typo in the name of file?");
    return (1);
  }
  char chunk;
  short i = 0;
  int n = 0;
  short done = 0;
  char *word;
  short wordOK = 1;
  char *bannedWords[] = {" ", "in", "the", "for", "and", "or", "of"};
  int bannedWordsCount = sizeof(bannedWords) / sizeof(char *);
read_word:
  chunk = fgetc(fp);
  if (!(chunk >= 97 && chunk <= 122) && !(chunk >= 65 && chunk <= 90) && chunk != '\'')
  {
    if (chunk == EOF)
      done = 1;
    goto read_word_end;
  }

  buf[i] = chunk;
  i++;
  goto read_word;
read_word_end:
  word = calloc(i, sizeof(char));
  int j = 0;
word_load:
  if ((buf[j] >= 97 && buf[j] <= 122) || (buf[j] >= 65 && buf[j] <= 90) || buf[j] == '\'')
  {
    word[j] = (buf[j] >= 65 && buf[j] <= 90) ? buf[j] + 32 : buf[j];
  }
  else
  {
    i--;
    if (i < 1) {
      goto next_loop;
    }
    word = realloc(word, sizeof(char) * i);
  }
  buf[j] = EOF;
  j++;
  if (j < i)
    goto word_load;
  word = realloc(word, sizeof(char) * (i + 1));
  word[i] = '\0';
  j = 0;
  i = 0;

  int g = 0;
checkExisting:
  if (g < n && strcmp(word, map[g]->word) == 0)
  {
    map[g]->n++;
    goto next_loop;
  }
  g++;
  if (g < n)
  {
    goto checkExisting;
  }

  wordOK = 1;
  int k = 0;
validate_word:
  if (strcmp(word, bannedWords[k]) == 0)
    wordOK = 0;
  k++;
  if (k < bannedWordsCount && wordOK == 1)
  {
    goto validate_word;
  }

  if (wordOK == 1)
  {
    word_count *w;
    w = malloc(sizeof(word_count));
    w->word = word;
    w->n = 1;
    map[n++] = w;
    if (n >= N)
    {
      map = realloc(map, sizeof(word_count *) * (n + 1));
    }
  }
  if (n < 2 || wordOK == 0)
    goto next_loop;
  int h = 0;
outer_sort:
  h = 0;
  short sorted = 1;
inner_sort:
  if (map[h]->n < map[h + 1]->n)
  {
    word_count *buf;
    buf = map[h];
    map[h] = map[h + 1];
    map[h + 1] = buf;
    sorted = 0;
  }
  h++;
  if (h < n - 1)
    goto inner_sort;
  if (sorted == 0)
    goto outer_sort;
next_loop:
  if (done == 0)
    goto read_word;
  h = 0;
read_map:
  printf("%s - %d\n", map[h]->word, map[h]->n);
  h++;
  if (h < (n < N ? n : N))
  {
    goto read_map;
  }
  fclose(fp);
  return 0;
}