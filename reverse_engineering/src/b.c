#include <stdio.h>
#include <stdlib.h>

int main() {
    char *ptr_1 = (char *)malloc(1);
    char *ptr_2 = NULL;
    *ptr_2 = 'a';

    return 0;
}
