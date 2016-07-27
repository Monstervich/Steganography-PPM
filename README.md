# Stenography-PPM
Stenography PPM

<b>Program Outline</b> 

I have created a steganography software using C to be used with “ppm” format files using a 2D array. 
This C program consists of three files in order to better maintain the program.

The first file is the header file named “ppm.h” which consists of headers and function declarations.
The second file, called “ppmfun.c”, holds the implementations to the declared functions within the header file.
The third file, called “steg.c” holds the ‘main’ functionality of the program. 

Within the program, I have used the linked list data structure to be used with the 2D array to store each character in the array within the red fields of the modified image. 

<b>Usage instructions</b>
NOTE: Do not include the quotation marks. (i.e. "")

To encode: ./steg e "file1.ppm"

steg prompts for a message and shows  file1.ppm with the message encoded within it. 
Here, the output is redirected to file2.ppm.

To decode: ./steg d "file1.ppm" "file2.ppm"

steg compares file1.ppm with file2.ppm to show the hidden message.
