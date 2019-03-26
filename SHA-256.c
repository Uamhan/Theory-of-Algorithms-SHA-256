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

//see section 4.1.2  for definitions
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

unint32_t SIG0(uint32_t x);
unint32_t SIG1(uint32_t x);

uint32_t Ch(unint32_t x,unint32_t y,unint32_t z);
uint32_t Maj(unint32_t x,unint32_t y,unint32_t z);

//see Section 3.2 for definitions
unint32_t rotr(uint32_t n, uint32_t X);
uint32_t shr(uint32_t n, uint32_t X);

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

    //current message block
    uint32_t M[16];
    
    //forloop interator
    int t;

    // from page 22 of standard, W[t] = M[t] for 0 <= t <= 15.
    for (t = 0; i < 16; i++)
        W[i] = M[i];
    
    // from page 22, W[t] = ...
    for (t=16; t<64; t++)
      W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
        
    // Initialise a,b,c,d,e,f,g and h as per step 2 page 19
    a = H[0];
    b = H[1];
    c = H[2];
    d = H[3];
    e = H[4];
    f = H[5];
    g = H[6];
    h = H[7];

    //step 3 creating new temp values for working varibales defined above.
    for (t = 0; t<64; t++){
        T1 = h + SIG_1(e) + CH(e,f,g)+K[t] + W[t];
        T2 = SIG_0(a) + Maj(a,b,c);
        h=g;
        g=f;
        f=e;
        e=d+T1;
        d=c;
        c=b;
        b=a;
        a=T1+T2;
    }

    //step 4
    H[0] = a + H[0];
    H[1] = b + H[1];
    H[2] = c + H[2];
    H[3] = d + H[3];
    H[4] = e + H[4];
    H[5] = f + H[5];
    H[6] = g + H[6];
    H[7] = h + H[7];


}
//see section 3.2 for definitions
unint32_t rotr(uint32_t n, uint32_t X){
    return (X >> n) | (X << (32-n));
}
//see section 3.2 for definitions
uint32_t shr(uint32_t n, uint32_t X){
    return (X >> n);
}

uint32_t sig0(uint32_t x){
    // see section 3.2 & 4.1.2 for definitions
    return(rotr(7,x) ^ rotr(18,x) ^ shr(3,x));
}
uint32_t sig1(uint32_t x){
    // see section 3.2 & 4.1.2 for definitions
    return(rotr(17,x) ^ rotr(19,x) ^ shr(10,x));
}

unint32_t SIG0(uint32_t x){

}
unint32_t SIG1(uint32_t x){

}

uint32_t Ch(unint32_t x,unint32_t y,unint32_t z){

}
uint32_t Maj(unint32_t x,unint32_t y,unint32_t z){
    
}