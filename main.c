#include <stdlib.h>
#include <stdio.h>
int main() {
    FILE *arqw = fopen("record.txt", "w");
    fscanf(arqw, "%d", 0);

    fclose(arqw);
    return 0;
}