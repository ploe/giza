#! /bin/bash

# Gets and builds the repos needed to build giza


# murmur3 hashing algorithm
git clone https://github.com/PeterScott/murmur3.git
cd murmur3
make
cd -
pwd

# murp JSON lexer ;)
git clone https://github.com/ploe/murp.git
cd murp
cd -
