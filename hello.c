#include <stdio.h>

void greet(const char *name);   // Function declaration

int main()
{
    greet("Ada");

    printf("Hello World!\n");

    return 0;
}

void greet(const char *name)
{
    printf("Hello, %s! Welcome to your GitHub portfolio.\n", name);
}