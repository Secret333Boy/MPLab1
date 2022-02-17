#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
  char *word;
  int count;
  int *pages;
} dict_item;

short PAGE_LINES = 45;
short IGNORE_COUNT = 100;

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("You need to pass the name of a file to this program:\n./task2.exe [FILENAME]");
    return (1);
  }

  FILE *fp;
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
    printf("Error while reading a file... Maybe typo in the name of file?");
    return (1);
  }
  dict_item **index;
  long items_count = 0;
  index = malloc(0);
  char chunk;
  char *page;
  page = calloc(sizeof(char), 1);
  page[0] = ' ';
  long page_count = 0;
  short done = 0;
  int i = 0;
  int n = 0;
read_page:
  chunk = fgetc(fp);
  if (chunk == EOF)
  {
    done = 1;
    goto read_page_end;
  }
  if (chunk == '\n')
    n++;
  page = realloc(page, sizeof(char) * (i + 1));
  page[i++] = chunk;
  if (n != PAGE_LINES)
    goto read_page;
read_page_end:
  page = realloc(page, sizeof(char) * (i + 1));
  page[i++] = '\0';
  page_count++;
  int chars_on_page = i + 1;
  i = 0;
  short j = 0;
  char *word;
  word = malloc(0);
read_words:
  if (!(page[i] >= 97 && page[i] <= 122) && !(page[i] >= 65 && page[i] <= 90) && page[i] != '\'')
  {
    word = realloc(word, sizeof(char) * (j + 1));
    word[j++] = '\0';
    if (word[0] != '\0')
    {
      int *pages_list;
      dict_item *item;
      n = 0;
      short found = 0;
    find_in_index:
      if (strcmp(index[n]->word, word) == 0)
      {
        if (page_count > index[n]->pages[index[n]->count - 1])
        {
          index[n]->pages = realloc(index[n]->pages, sizeof(int) * (index[n]->count + 1));
          index[n]->pages[index[n]->count] = page_count;
          index[n]->count++;
        }
        found = 1;
        goto find_in_index_end;
      }

      n++;
      if (n < items_count)
        goto find_in_index;
    find_in_index_end:
      if (found == 0)
      {
        pages_list = calloc(sizeof(int), 1);
        pages_list[0] = page_count;
        item = malloc(sizeof(dict_item));
        item->word = word;
        item->pages = pages_list;
        item->count = 1;
        index = realloc(index, sizeof(dict_item *) * (items_count + 1));
        index[items_count++] = item;
        short sorted;
        if (items_count > 1)
        {
        sort:
          sorted = 1;
          int k = items_count - 2;
        bubble:
        {
          if (strcmp(index[k]->word, index[k + 1]->word) > 0)
          {
            dict_item *buf;
            buf = index[k];
            index[k] = index[k + 1];
            index[k + 1] = buf;
            sorted = 0;
          }
        }
          k--;
          if (k >= 0)
            goto bubble;
          if (sorted == 0)
            goto sort;
        }
      }
    }
    j = 0;
    word = malloc(0);
    i++;
    goto read_words;
  }
  if ((page[i] >= 97 && page[i] <= 122) || (page[i] >= 65 && page[i] <= 90) || (page[i] == '\'' && j != 0))
  {
    word = realloc(word, sizeof(char) * (j + 1));
    word[j++] = (page[i] >= 65 && page[i] <= 90) ? page[i] + 32 : page[i];
  }
  i++;
  if (i < chars_on_page)
    goto read_words;
next_loop:
  i = 0;
  n = 0;
  j = 0;
  if (done == 0)
  {
    goto read_page;
  }
read_word_from_index:
  if (index[i]->count <= IGNORE_COUNT)
  {
    printf("%s - ", index[i]->word);
  read_pages:
    printf("%d", index[i]->pages[j]);
    if (j != index[i]->count - 1)
      printf(", ");
    j++;
    if (j < index[i]->count)
      goto read_pages;
    printf("\n");
    j = 0;
  }
  i++;
  if (i < items_count)
    goto read_word_from_index;
  return 0;
}