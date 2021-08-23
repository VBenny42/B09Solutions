#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int sad = 0;
    int *hj = &sad;
    printf("%d\n", *(hj));
}