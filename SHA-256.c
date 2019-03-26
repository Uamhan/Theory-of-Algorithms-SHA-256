//Uamhan Mac Fheaghusa
//Secure Hash Algorithm, 256 bit version

//link to Secure Hash Algorithm Standard
//https://www.nist.gov/publications/secure-hash-standard

//standard input/output header file
#include <stdio.h>
//fixed bit integer header file
#include <stdint.h>

//sha256 method definition.
void sha256();

int main(int argc, char *argv[]){

    sha256();

    return 0;
}

//sha256 method implementation
void sha256(){

    //Message schedule (section 6.2)
    uint32_t w[64];
    //Working Variable (section 6.2)
    uint32_t a,b,c,d,e,f,g,h;
    //Two temporary variables (section 6.2)s
    uint32_t T1,T2;

    //The hash Value (section 6.2)
    //Values come from Section 5.3.3
    uint32_t H[8] = {
        0x6a09e667,
        0xbb67ae85,
        0x3c6ef372,
        0xa54ff53a,
        0x510e527f,
        0x9b05688c,
        0x1f83d9ab,
        0x5be0cd19
    };


}