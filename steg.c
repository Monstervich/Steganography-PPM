#include "ppm.h"

int main (int argc, char const *argv[]){

	//check argv
	if(argc < 3){
		printf("steg: Improper use of steg\n");
		printf("Use either:  ./steg e <file1.ppm>\n");
		printf("             ./steg d <file1.ppm> <file2.ppm>\n");
		exit(0);
	}

	if(argv[1][0] == 'e'){
		encodeFile(argc, argv);
	}
	else if(argv[1][0] == 'd'){
		if(argc != 4){
			printf("steg: Improper use of steg\n");
                	printf("Use either:  ./steg e <file1.ppm>\n");
                	printf("             ./steg d <file1.ppm> <file2.ppm>\n");
                	exit(0);
		}
		decodeFile(argc, argv);
	}
	else{
		printf("steg: Improper use of steg\n");
                printf("Use either:  ./steg e <file1.ppm>\n");
                printf("             ./steg d <file1.ppm> <file2.ppm>\n");
                exit(0);
	}

	return 0;
}
