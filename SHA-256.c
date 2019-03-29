//Uamhan Mac Fheaghusa
//Secure Hash Algorithm, 256 bit version

//link to Secure Hash Algorithm Standard
//https://www.nist.gov/publications/secure-hash-standard

//standard input/output header file
#include <stdio.h>
//fixed bit integer header file
#include <stdint.h>

#include <string.h>
#define BLOCK_SIZE 64
#define TOTAL_LEN_LEN 8

// buffer state struct that holds information regarding the message to be hashed during
struct buffer_state {
    //pointer to the input to be hashed
	const uint8_t * p;
	//length of the current block
    size_t len;
    //total length of the input
	size_t total_len;
    //checks for padding method
	int single_one_delivered; 
	int total_len_delivered; 
};

//union structure message block
union msgblock {
    uint8_t e[64];
    uint32_t t[16];
    uint64_t s[8];
};
//enum controling state of file during padding
enum status {READ,PAD0,PAD1,FINISH};

//sha256 method definition.
void sha256(FILE *msgf);

//little to big endianan method definition
unsigned int LToBEndian(uint32_t x);
//added method to initilse the bufferstate structure
static void initBufferState(struct buffer_state * state, const void * input, size_t len);

//next message block method defination handles the padding of each message block.
int nextmsgblock(uint8_t block[BLOCK_SIZE], struct buffer_state * state);

//functions used in hashing section
//see section 4.1.2  for definitions
uint32_t sig0(uint32_t x);
uint32_t sig1(uint32_t x);

uint32_t SIG0(uint32_t x);
uint32_t SIG1(uint32_t x);

uint32_t Ch(uint32_t x,uint32_t y,uint32_t z);
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z);

//see Section 3.2 for definitions
uint32_t rotr(uint32_t n, uint32_t X);
uint32_t shr(uint32_t n, uint32_t X);



int main(int argc, char *argv[]){
    //user input file
    FILE* msgf;

    //TODO implement error checking
    //openfile given as first commandline argument
    msgf = fopen(argv[1],"r");
    //executes the sha256 algorithm on the file
    sha256(msgf);
    //close file
    fclose(msgf);
    

    return 0;
}

int nextmsgblock(uint8_t block[BLOCK_SIZE], struct buffer_state * state){

    size_t spaceInBlock;

    //if we have finished all message blocks we return 0 letting the sha algorithm know we have finished padding
	if (state->total_len_delivered) {
		return 0;
	}
    //if the length of the current block is greater than or equal to BLOCK_SIZE which is 64.
	if (state->len >= BLOCK_SIZE) {
        //copys the BLOCK_SIZE() amount of characters to for state->p to block (memcpy copys from the memory location of the inputs)
		memcpy(block, state->p, BLOCK_SIZE);
        //adds the BLOCK_SIZE to state->p
		state->p += BLOCK_SIZE;
        //takes BLOCK_SIZE away from state ->legnth
		state->len -= BLOCK_SIZE;
        //return one back to sha256 method letting it know to continue looping
		return 1;
	}
    //copys state->len characters from state->p to to block (memcpy copys from the memory location of the inputs)
	memcpy(block, state->p, state->len);
    //adds state->len to block
	block += state->len;
    //sets the varible spaceINBlock to equal Block_SIZE - state->len
    spaceInBlock = BLOCK_SIZE - state->len;
    //adds state -> len to state->p
	state->p += state->len;
    //sets state->len to 0
	state->len = 0;

	// If we reach this point, spaceInBlock is a one at minimum. 
    //if state->single_one_delivered dose not equal one we add the one to the end of the message block as per the sha256 standard padding section
	if (!state->single_one_delivered) {
        //add a one to the end of the block
		*block++ = 0x80;
        //reduce the space in the block by 1
	    spaceInBlock -= 1;
        //set state-> single_one_delivered to 1
		state->single_one_delivered = 1;
	}

	
	//At This point their is either enough space left for the total length left to parse and we can finish
    //or there isint enough space left and we need to padd the rest of this block with zeros
    //in this case there will be enough space in this block to finish next time this method is run.
    //if Space in block is greater than the the total length left in the message
	if  spaceInBlock >= TOTAL_LEN_LEN) {
        //take total length left away from spaceInBlock
		const size_t left = spaceInBlock - TOTAL_LEN_LEN;
        //len is set to the total length left in the block
		size_t len = state->total_len;

		
        //we set the space left in the block to 0's
		memset(block, 0x00, left);
        //we add left to the block
		block += left;

		//we append the length of the message to the end of the block
		block[7] = (uint8_t) (len << 3);
        //shift the length by 5
		len >>= 5;
        //for loop that that makes the last digits in the block big endian
		for (int i = 6; i >= 0; i--) {
			block[i] = (uint8_t) len;
			len >>= 8;
		}
        //sets the total length delivered varible in state to done
		state->total_len_delivered = 1;
	} else {
        //otherwise fill the current block with 0's
		memset(block, 0x00, spaceInBlock);
	}

	return 1;

}

//sha256 method implementation
void sha256(FILE *msgf){

    uint64_t nobits = 0;

    //current status of the file
    enum status S = READ;
    //the message block
    union msgblock M;

    // K Constants. defined section 4.2.2.
    uint32_t K[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };


    
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
    
    //forloop interator
    int i,t;

    while(nextmsgblock(msgf,&M,&S,&nobits)){

        //Message schedule (section 6.2)
        uint32_t W[64];

        // from page 22 of standard, W[t] = M[t] for 0 <= t <= 15.
        for (t = 0; t < 16; t++)
            W[t] = LToBEndian(M.t[t]);
    
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
            T1 = h + SIG1(e) + Ch(e,f,g) + K[t] + W[t];
            T2 = SIG0(a) + Maj(a,b,c);
            h = g;
            g = f;
            f = e;
            e = d+T1;
            d = c;
            c = b;
            b = a;
            a = T1+T2;
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
    //coverts from little to big endian
    
    //prints results to console
    printf("%08x %08x %08x %08x %08x %08x %08x %08x \n",H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
    //for (i = 0; i <32; i++ ){
    //    printf("%x ",hash[i]);
    //}
}

static void initBufferState(struct buffer_state * state, char input[], size_t len){
	state->p = input;
	state->len = len;
	state->total_len = len;
	state->single_one_delivered = 0;
	state->total_len_delivered = 0;
}

//method converts from little endian to big endian 
uint32_t LToBEndian(uint32_t x)
{
    //swaps the order of the bytes
	return (((x>>24) & 0x000000ff) | ((x>>8) & 0x0000ff00) | ((x<<8) & 0x00ff0000) | ((x<<24) & 0xff000000));
}

//see section 3.2 for definitions
uint32_t rotr(uint32_t n, uint32_t X){
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

uint32_t SIG0(uint32_t x){
    return (rotr(2,x) ^ rotr(13,x) ^ rotr(22,x));
}
uint32_t SIG1(uint32_t x){
    return (rotr(6,x) ^ rotr(11,x) ^ rotr(25,x));
}

uint32_t Ch(uint32_t x,uint32_t y,uint32_t z){
    return ((x & y) ^ ((!x) & z));
}
uint32_t Maj(uint32_t x,uint32_t y,uint32_t z){
    return ((x & y) ^ (x & z) ^ (y & z));
}


