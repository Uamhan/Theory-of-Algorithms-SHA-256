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
#define MAX_INPUT_SIZE 4096

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
void sha256(uint8_t hash[32], char input[], size_t len);

//added method to initilse the bufferstate structure
static void initBufferState(struct buffer_state * state, char input[], size_t len);

//next message block method defination handles the padding of each message block.
int nextmsgblock(uint8_t block[BLOCK_SIZE], struct buffer_state * state);

//see Section 3.2 for definitions
static inline uint32_t rotr(uint32_t value, unsigned int count);

// K Constants. defined section 4.2.2.
static const uint32_t k[] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

int main(int argc, char *argv[]){

    //aray that will hold the hashed message
    uint8_t hash[32];
    //char array that will hold the input message for the hashing algorithm
    char input[MAX_INPUT_SIZE];
    //user file that will message to be hashed will be read from
    FILE* file;
    //opens user file.
    file = fopen(argv[1],"r");
    //reads string from file into input and prints the input to console.

    printf("Input from file = ");
    while (fgets(input, MAX_INPUT_SIZE, file) != NULL){
        printf("%s", input);
    }
    
    printf("\n\n");
    //closes file.
    fclose(file);    
    //hash algorithm that fills the hash array with the hashed message
    sha256(hash, input, strlen(input));
    //prints the hashed message
    for(int i = 0; i <32;i++)
        printf("%x",hash[i]);
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
	if(spaceInBlock >= TOTAL_LEN_LEN) {
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
	}else {
        //otherwise fill the current block with 0's
		memset(block, 0x00, spaceInBlock);
	}

	return 1;

}

//sha256 method implementation
void sha256(uint8_t hash[32], char input[], size_t len){

    //The initial hash Values (section 6.2)
    //Values come from Section 5.3.3
    uint32_t h[] = { 
        0x6a09e667,
        0xbb67ae85, 
        0x3c6ef372, 
        0xa54ff53a, 
        0x510e527f, 
        0x9b05688c, 
        0x1f83d9ab, 
        0x5be0cd19 };
	
    int i, j;

	// block that will be hashed by the algorithm
	uint8_t block[64];
    //state struct that holds the state of the message to be hashed
	struct buffer_state state;
    //initilises the state struct.
	initBufferState(&state, input, len);

    //loop that iterates through the message to be hashed block by block calling nextmsgblock each iteration to padd the block.
	while (nextmsgblock(block, &state)) {
		uint32_t workingSet[8];
		
        //array that holds the message that will be hashed by the algorithm
		uint32_t w[64];
        //current padded block for the iteration
		const uint8_t *p = block;
        //fills the w array with 0's to initilise it before we use it.
		memset(w, 0x00, sizeof w);
        //from page 22 of standard, W[t] = M[t] for 0 <= t <= 15 with big endian coversion.
		for (i = 0; i < 16; i++) {
			w[i] = (uint32_t) p[0] << 24 | (uint32_t) p[1] << 16 | (uint32_t) p[2] << 8 | (uint32_t) p[3];
			p += 4;
		}

		// from page 22, w[t] = ...
		for (i = 16; i < 64; i++) {
			const uint32_t s0 = rotr(w[i - 15], 7) ^ rotr(w[i - 15], 18) ^ (w[i - 15] >> 3);
			const uint32_t s1 = rotr(w[i - 2], 17) ^ rotr(w[i - 2], 19) ^ (w[i - 2] >> 10);
			w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		}
		
		//initilise working set a-h as per step 2 page 19 for this implemtation they will be stored in the array workingSet. workingSet[0] == a workingset[1] == b ect...
		for (i = 0; i < 8; i++)
			workingSet[i] = h[i];

		//step 3 creating new temp values for working varibales defined above.
		for (i = 0; i < 64; i++) {
			const uint32_t s1 = rotr(workingSet[4], 6) ^ rotr(workingSet[4], 11) ^ rotr(workingSet[4], 25);
			const uint32_t ch = (workingSet[4] & workingSet[5]) ^ (~workingSet[4] & workingSet[6]);
			const uint32_t temp1 = workingSet[7] + s1 + ch + k[i] + w[i];
			const uint32_t s0 = rotr(workingSet[0], 2) ^ rotr(workingSet[0], 13) ^ rotr(workingSet[0], 22);
			const uint32_t maj = (workingSet[0] & workingSet[1]) ^ (workingSet[0] & workingSet[2]) ^ (workingSet[1] & workingSet[2]);
			const uint32_t temp2 = s0 + maj;

			workingSet[7] = workingSet[6];
			workingSet[6] = workingSet[5];
			workingSet[5] = workingSet[4];
			workingSet[4] = workingSet[3] + temp1;
			workingSet[3] = workingSet[2];
			workingSet[2] = workingSet[1];
			workingSet[1] = workingSet[0];
			workingSet[0] = temp1 + temp2;
		}
        // add the new hashed block to the current hash values
		for (i = 0; i < 8; i++)
			h[i] += workingSet[i];
	}

	//adds fiinished hash set to the hash array as big endian.
	for (i = 0, j = 0; i < 8; i++)
	{
		hash[j++] = (uint8_t) (h[i] >> 24);
		hash[j++] = (uint8_t) (h[i] >> 16);
		hash[j++] = (uint8_t) (h[i] >> 8);
		hash[j++] = (uint8_t) h[i];
	}
}

static void initBufferState(struct buffer_state * state, char input[], size_t len){
	state->p = input;
	state->len = len;
	state->total_len = len;
	state->single_one_delivered = 0;
	state->total_len_delivered = 0;
}

//see section 3.2 for definitions
static inline uint32_t rotr(uint32_t value, unsigned int count){
	return value >> count | value << (32 - count);
}



