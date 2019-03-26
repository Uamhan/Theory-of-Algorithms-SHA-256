#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
}

int main(int argc, char *argv[]){
    
    FILE* f;

    uint64_t nobytes;

    union msgblock M;

    //TODO implement error checking
    f = fopen(argv[1],"r");
    //TODO error check
    while(!feof(f)){

        nobytes = fread(M.e,1,64,f);
        if(nobytes < 56){
            printf("block with less than 55 bytes\n");
            M.e[nobytes]=0x80;
            while(nobytes <56){
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
        }

        
    }
    fclose(f);
    
    return 0;
}