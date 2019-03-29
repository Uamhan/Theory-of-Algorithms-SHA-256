# Theory of Algorithms -  SHA256 Implementation.

### Installation

First we need to install a C compiler if you do not already have one. A C compiler known as GCC can be found at
https://gcc.gnu.org/

You can check if you already have it installed by entering the following in command prompt.


```sh
gcc -v
```


The next step is to clone this repository to your machine.


```sh
git clone https://github.com/Uamhan/Theory-of-Algorithms-SHA-256
```


Once the repository has been downloaded to your machine, navigate to it's location via the comandline and enter the following command.


```c
gcc -o SHA-256 SHA-256.c
```


You will now have a compiled executable file in the cloned directory. This program can be run via the comand line
with the following command.


```bash
//replace test.txt with the file name you wish to execute the program on.
./SHA-256.exe test.txt
```

### Execution

Once Compiled the cloned directory will contain a file named SHA-256.exe. The directory contains a txt file named test.txt
By default it contains the string "abc". to execute the program on this file you can use the following comand 


```bash

./SHA-256.exe test.txt

```

if you wish to test a difrent input you can change the contents of test.txt or replace test.txt in the comand above with the location of the fill you wish to execute the program on.

### Testing

With the sha 256 algorithm given the same input the output will be the same. with this in mind we can test this program by using test values from other sources. for testing this implementation i used the test values found at.

https://www.di-mgt.com.au/sha_testvectors.html

The output from this program for the test string abc is 

```

ba7816bf 8f01cfea 414140de 5dae2223 b00361a3 96177a9c b410ff61 f20015ad

```

which matches the expected value from https://www.di-mgt.com.au/sha_testvectors.html other tested values include

```
//input of "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"
248d6a61 d20638b8 e5c02693 0c3e6039 a33ce459 64ff2167 f6ecedd4 19db06c1

```


```
//input of "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmnopjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu"
cf5b16a7 78af8380 036ce59e 7b049237 0b249b11 e8f07a51 afac4503 7afee9d1

```

### References


Profesor Ianmcloughlin (G.M.I.T) lecture notes, lectures/tutorials and implementation.

the Secure hash algorithm standard documentation (specificly the sections on SHA256
https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

The wiki Page on the sha-2 standard
https://en.wikipedia.org/wiki/SHA-2





