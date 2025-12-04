# AVL Tree Dictionary

Academic project developed for the *Algorithms and Data Structures II* course.

This project implements a high-performance dictionary using an *AVL Tree (Self-Balancing Binary Search Tree)* written in C. It ensures logarithmic time complexity for search, insertion, and deletion operations.

## Features

- Self-balancing structure (AVL) with automatic rotations.
- Core operations: Insert, Remove, Search, and Print.
- Performance analysis module with bulk insertion from files.

## How to Compile and Run

Compile the code using GCC:

gcc main.c -o dictionary

Run the executable:

./dictionary

## Usage Commands

The program interacts via numerical commands in the standard input.

1 
Creates a root.

2 [word]
Removes a word.

3 [word] [meaning]
Inserts a word into the dictionary.

4 [word]
Searches for a word.

5
Prints all words in alphabetical order.

6 [filename]
Loads words from a text file for performance testing.

0
Exits the program.
