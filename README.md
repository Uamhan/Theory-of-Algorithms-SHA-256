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

