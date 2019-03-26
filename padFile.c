#include <stdio.h>
#include <stdint.h>

union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};

enum status {READ,PAD0,PAD1,FINISH};

int main(int argc, char *argv[]){
    
    FILE* f;

    uint64_t nobytes;
    uint64_t nobits = 0;

    enum status S = READ;

    union msgblock M;
    //iterator for forloop
    int i;

    //TODO implement error checking
    f = fopen(argv[1],"r");
    
    while(S == READ){
        nobytes = fread(M.e,1,64,f);
        nobits = nobits + (nobytes * 8);
        if(nobytes < 56){
            printf("block with less than 55 bytes\n");
            M.e[nobytes]=0x80;
            while(nobytes <56){
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
            M.s[7] = nobits;
            S=FINISH;
        }else if(nobytes < 64){
            S = PAD0;
            M.e[nobytes]=0x80;
            while(nobytes <64){
                nobytes = nobytes + 1;
                M.e[nobytes] = 0x00;
            }
        }else if(feof(f)){
            S=PAD1;
        }
    }

    if(S==PAD0 || S==PAD1){
        for(i = 0; i < 56; i++)
            M.e[i] = 0x00;
        M.s[7] = nobits;
    }
    if(S==PAD1)
        M.e[0] = 0x80;

    fclose(f);

    for(i=0;i<64;i++)
        printf("%x",M.e[i]);
    printf("\n");
    
    return 0;
}