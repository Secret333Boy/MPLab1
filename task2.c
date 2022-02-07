#include <stdio.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("You need to pass the name of a file to this program:\n./task2.exe [FILENAME]");
    return (1);
  }
  return 0;
}