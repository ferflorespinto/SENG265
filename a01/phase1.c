/*
SENG265
Summer Session 2017
Name: Jorge Fernando Flores Pinto
ID: V00880059

This file phase1.c is the first phase towards data compression. It takes
a text file and divides it into blocks of size 21 (or less in case the size
of the remaining string is less that 21), to perform transforms on the files.
phase1.c performs forward and backward transforms, based on the user's
input in the command line. The forward transform turns .txt files into .ph1
files and prints. The backward transform turns .ph1 into .txt files.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

/* printProgress prints a progress bar of the transform. I found this at:
https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf */
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 50

void printProgress (double percentage) {
	int val = (int) (percentage * 100);
	int lpad = (int) (percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
	fflush (stdout);
}

/*This function rows in 2D arrays lexicographically. It is inefficient
compared to qsort, but in my implementation I preferred to have greater control
over my arrays to track them step by step.*/
void sorting(char array[22][22], int size) {
	int l = 0;
	l = size;
	char temp[l];
	int a, b;
	int m, n, g, swapped;

	for (a = 1; a < l; a++) {
		m = array[a - 1][0];
		n = array[a][0]; 
		if(m > n) {
			for(b = 0; b < l; b++) {
				temp[b] = array[a][b];

			}
			for(b = 0; b < l; b++) {
				array[a][b] = array[a - 1][b];
				array[a - 1][b] = temp[b];
			}
			a = 0;
		}

	}
	/*If there was a swap, the loop starts over again from the first
	element of the array*/
	for (a = 1; a < l; a++) {
		swapped = 0;
		m = array[a - 1][0];
		n = array[a][0]; 

		if (m == n) {
			for(b = 0; b < l; b++) {
				m = array[a - 1][b];
				n = array[a][b]; 
				if (m > n) {
					for (g = 0; g < l; g++) {
						temp[g] = array[a][g];
					}
					for (g = 0; g < l; g++) {
						array[a][g] = array[a - 1][g];
						array[a - 1][g] = temp[g];
					}
					b = 0;
					swapped = 1;
				}
				else if (m < n) {
					break;
				}
			}

		}
		if(swapped) {
			a = 0;
		}
	}
}
/*Performs the forward transform*/
void forward(FILE *fp, char original[], char *infile, int numChars) {
	char substring[21];
	char array[22][22];
	char result[22];
	int l;

	
	fp = fopen(infile, "r");
	char start[numChars + 1];
	char addChar = 0;
	int index = 0;
	while(addChar != EOF) {
		addChar = getc(fp);
		start[index] = addChar;
		index++;
	}
	
	start[index - 1] = 0;
	int a = strlen(start);
	fclose(fp);

	int s = 0;
	int m = 0;
	int c = strlen(start);

	char final[numChars + 3];
	
	int looper = 1;
	while (m < c) {
		m += s;
		printProgress(m/c);
		if (c - m >= 20) {
			s = 20;
		}
		else if (c - m > 0) {
			s = c - m;
		}
		else {
			break;
		}

		strncpy(substring, start + m, 20);
		substring[20] = 0;

		l = strlen(substring);

		char substring_copy[22];
		strncpy(substring_copy, substring, l);
		
		/* Last character of string has to end with special character ASCII 3 */
		substring_copy[l] = 3; 
		l++;
		substring_copy[l] = 0;

		char copy[22];
		strncpy(copy, substring_copy, l);
		int a, b;

		for(a = 0; a < l; a++) {
			/*The following two statements will "rotate" string into copy 
			to match specification.*/
			strncpy(copy, substring_copy + a, l - a);
			strncpy(copy + l - a, substring_copy, a);

			/*This will copy the modified string into our 2D array*/
			for(b = 0; b < l; b++) {
				array[a][b] = copy[b];
			}

		}
				
		/*End of creating 2D array*/

		/*Sorting our 2D array by lexicographic order*/
		sorting(array, l);

		/* End of sorting*/

		/*Now, we need to copy the last column of the 2D array into a new array*/
		for(a = 0; a < l; a++) {
			result[a] = array[a][l - 1];

		}
		result[l] = 0;
		
		int m_copy;
		if (m >= 20) {
			m_copy = m + looper;
			looper++;

		}
		else {
			m_copy = m;
		}
		for(a = m_copy, b = 0; a < m_copy + 22; a++, b++) {
			final[a] = result[b];
		}
		
	}
	for(a = 0; a < numChars + looper; a++) {
		original[a] = final[a];
	}
	original[a] = 0;
}
/*Performs the backward transform*/
void backward(FILE *fp, char original[], char *infile, char final[]) {
	fp = fopen(infile, "r");
	char addChar = 0;
	int index = 0;
	int positions = 8;
	while(addChar != EOF) {
		addChar = getc(fp);
		if (positions > 0) {
			positions--;
			continue;
		}
		original[index] = addChar;
		index++;
	}
	fclose(fp);
	original[index - 1] = 0;
	
	char substring[22];

	unsigned int s = 0;
	unsigned int m = 0;
	int c = (int) strlen(original);
	int looper = 1;

	while (m < c) {
		if (c - m >= 21) {
			s = 21;
		}
		else if (c - m > 0){
			s = c - m;
		}
		else {
			break;
		}
		char result_back[s + 1];
		
		strncpy(substring, original + m, s);
		substring[s] = 0;
		int length = 0;
		length = (int) strlen(substring);
		char array_back[22][22];
		char array_back_copy[22][22];
		int i, j; /*helpers for loops*/

		/*First, we copy ph1 string into our array_back (only first column)
		and array_back_copy (only first column)*/
		for(i = 0; i < length; i++) {
			for(j = 0; j < length; j++) {
				array_back[i][j] = 0;
				array_back_copy[i][j] = 0;
			}
		}
		for (i = 0; i < length; i++) {
			array_back[i][0] = substring[i];
			array_back_copy[i][0] = substring[i];
		}

		/*Now the sorting part.*/

		/*First we sort our first column*/
		sorting(array_back_copy, length);

		/*Now the copying.*/
		int y;
		char temp[s];
		for(j = 1; j < length; j++) {
			for(i = 0; i < s; i++) {
				/*Copying the first column of original array into
				last column of copy*/
				array_back_copy[i][j] = array_back[i][0];
				array_back[i][j] = array_back_copy[i][j - 1];

				/* Moving the column we just copied into array_back_copy
				to the first column. */
				for (y = j; y > 0; y--) {
					temp[i] = array_back_copy[i][y - 1];
					array_back_copy[i][y - 1] = array_back_copy[i][y];
					array_back_copy[i][y] = temp[i];
				}
			}
			sorting(array_back_copy, length);

		}
		
		for(i = 0; i < length; i++) {
			if (array_back[i][length - 1] == 3) {
					for(j = 0; j < length; j++) {
					result_back[j] = array_back[i][j];
				}
				break;
			}
		}

		if(result_back[length - 1] == 3) {
			result_back[length - 1] = 0;
		}

		int m_copy;
		if (m >= 20) {
			m_copy = m - looper;
			looper++;

		}
		else {
			m_copy = m;
		}

		for(i = m_copy, j = 0; i < m_copy + s; i++, j++) {
			final[i] = result_back[j];
		}
		final[i] = 0;
		m += s;
		printProgress(m/c);

	}
}
int main(int argc, char* argv[]) {
	/*Loading the text file and loading instructions*/
	FILE *fp;

	int g;
	char* infile;
	char* outfile;
	extern char *optarg;
	extern int optind, optopt;

	/* For the following getopt_long code, I used this source for guidance:
	https://stackoverflow.com/questions/17877368/getopt-passing-string-parameter-for-argument*/
	int fflag = 0;
	int bflag = 0;
	const char *short_opt = "fbio:";
	struct option long_opt[] = 
	{
		{"forward",		no_argument,	NULL,	'f'},
		{"backward",	no_argument,	NULL,	'b'},
		{"infile",		required_argument, NULL,	'i'},
		{"outfile", 	required_argument, NULL, 	'o'}
	};

	while((g = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
		switch(g) {
			case -1:
			case 0:
			break;

			case 'f':
				fflag = 1;
				printf("You requested a forward transform.\n");
				break;
			case 'b':
				bflag = 1;
				printf("You requested a backward transform\n");
				break;
			case 'i':
				printf("Input file for transform: %s\n", optarg);
				infile = optarg;
				break;
			case 'o':
				printf("Output destination to: %s\n", optarg);
				outfile = optarg;
				break;
		}
	}

	fp = fopen(infile, "r");
	char nextChar = getc(fp);
	int numChars = 0;
	/*I found this useful while loop at:
	https://stackoverflow.com/questions/2645598/counting-unknown-numbers-of-chars-from-a-file-in-c*/
	while (nextChar != EOF) {
		numChars++;
		nextChar = getc(fp);
	}
	fclose(fp);

	if(fflag == 1) {
		int repetitions = (int) numChars / 20;
		char string[numChars + repetitions + 2];
		forward(fp, string, infile, numChars);
		fp = fopen(outfile, "wb");
		fputc(0xab, fp);
		fputc(0xba, fp);
		fputc(0xbe, fp);
		fputc(0xef, fp);
		fputc(0x14, fp);
		fputc(0x00, fp);
		fputc(0x00, fp);
		fputc(0x00, fp);

		fputs(string, fp);
		fclose(fp);

		printf("\nDone!\n");
	}
	else if(bflag == 1) {
		int repetitions = (int) numChars / 21;
		char original_back[numChars + 1];
		char final[numChars - repetitions + 1];
		backward(fp, original_back, infile, final);
		fp = fopen(outfile, "wb");
		fputs(final, fp);
		fclose(fp);

		printf("\nDone!\n");

	}
	
	exit(0);
}