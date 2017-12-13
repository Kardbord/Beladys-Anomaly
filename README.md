# Assignment 6

### Tanner Kvarfordt

### Preface

This program was written for CS3100 (OS and concurrency). 

The description below is taken directly from Dr. Mathias's canvas page for this assignment and modified
to be a markdown file.

### Introduction

The book describes an anomaly discovered with paging when using the FIFO page replacement scheme, also presented in class.  A bunch of years ago I was part of the team interviewing a candidate to be hired within our dept. I took him to breakfast and during breakfast I asked him about a paper he'd written regarding Belady's Anomaly.  He explained it to me and I didn't believe his results, I then asked him for his paper.  He later sent the paper to me and the paper confirmed my thinking, he did it wrong, but thinks he did it right. You are going to get the chance to create a small experiment that will establish the likelihood of this anomaly and prove, once and for all, whether or not Mr. Job Candidate was mistaken.

### Assignment

Develop a software experiment that uses FIFO page replacement on a randomly generated page usage sequence, counting the number of page faults generated when testing the page usage sequence.  You will do this over a range of memory sizes.  The program will then examine the number of page faults at each memory size to determine if and how often Belady's anomaly occurs.

* Physical memory frame sizes from 1 frame to 100 frames, stepping by 1 frame.
* Page usage sequence size of 1000, selecting pages from 1 to 250.
* Generate 100 different random page sequences and test each of these sequences on the range of frame sizes. This will give you a two dimensional array of results. For each frame size (1 to 100) there will be 100 different results.
* Output should look almost exactly like that shown towards the end of this page
    * Show the number of sequences tested
    * Show the length of the memory reference string
    * Show the number of frames of physical memory
    * A report each time an anomaly is detected
* Remember to use the -O3 optimization flag, this helps a LOT.

Write a program that tests the above set of parameters using FIFO page replacement, tracking how many page faults take place for each frame size and page usage sequence. Then, given this information, determine how many times Belady's Anomaly has occurred.

### Notes
* Write a CMakeLists.txt file the grader can use to build your program.
* Make smart programming choices, this is not a difficult program to write. For example, don't write your own FIFO queue, use the C++ STL library for your queue data structure (assuming you use a queue).
* When compiling, use the -O3 option to tell the compiler to optimize your code.  That is "capital oh three", not "zero three".  This makes a big, big difference in how fast your program executes!
