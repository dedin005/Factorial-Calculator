# This is for when I come back to this project after a while:

1. I create 20 pthreads, and increment 1 to 100mil with i += 20

2. Then multiply the 20 parts together

3. Then export the result to a mmap file

4. The mmap file will need to be through a socket if using the cse lab machine because filesizes are capped at 512, and the output.txt file is 3.1 GB

    * There are client/server socket examples in this folder

    * The cse lab machine will be the client, and the main.c file will need to implement the client code

    * The home computer will be the server, and will run a socket listener program with a mmap file to write to

5. The mmap file will be on a remote server, probably the one at home or a rpi that I have