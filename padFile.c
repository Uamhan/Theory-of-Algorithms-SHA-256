#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]){
    
    FILE* f;

    char c;
    //TODO implement error checking
    f = fopen(argv[1],"r");

    printf("%c\n", fread(&c,1,1,f));

    fclose(f);
    
    return 0;
}