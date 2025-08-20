#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *ptr_1 = NULL;
    char *ptr_2 = NULL;
    char *ptr_3 = NULL;

    ptr_1 = malloc(sizeof(char) * 2);
    ptr_2 = malloc(sizeof(char) * 2);

    /* NULL pointer dereference in the application code */
    printf("%c", ptr_3[1]);

    free(ptr_1);
    free(ptr_2);

    return 0;
}
