#include "ppm.h"
/*Implementations for declared functions*/

/* readLine used from Lab 3 */
char * readLine(FILE * fin){

	char buffer[BMAX];
	int j, k;
	char ch;
	char * line;

	j = 0;
	ch = getc(fin);
	if (ch == EOF)
		return NULL;
	while (ch!='\n' && j < (BMAX -1)){
		buffer[j] = ch;
		j++;
		ch = getc(fin);
	}

	if (ch != '\n')
		while (ch != '\n')
			ch = getc(fin);
	buffer[j] = '\0';
	line = malloc((j+1) * sizeof(char));

	for (k = 0; k <= j; k++)
		line[j] = buffer[j];
		line[j] = '\0';

	return line;
	free(buffer);
}


/* showPPM prints information stored in PPM struct
 * in the given structural format
 */
void showPPM (struct PPM * i){

        //print type
        printf("%s\n", i->type);

        //print text
        int j;
        struct text * t = i->text;
        printf("%s\n", i->text->text);
        for(j = 1; j<i->cn; j++){
                t = t->next;
                printf("%s\n", t->text);
        }

        //print general info
        printf("%d\n", i->w);
        printf("%d\n", i->h);
        printf("%d\n", i->max);

        //print pixels
        int k,l;
        for(k=0; k<i->h; k++){
		for(l=0; l<i->w; l++){
                        struct pixel p = i->p[k][l];
                        printf("%i %i %i\n", p.r, p.g, p.b);
                }
        }
}


/* Utilizes the above readLine function to find the comments */
void getText(FILE * fin, struct PPM * i){
	char ch = getc(fin);
	while(ch == '#') //lines beginning with "#" character
	{
		ungetc(ch, fin);

		//get comment
		char * line = readLine(fin);

		//add comment to linked list
		if(i->cn == 0)
		{

			//no previous comments
			i->text = (struct text * )malloc(sizeof(struct text));
			i->text->text = line;
			i->cn = 1;
		}
		else
		{
			//previous comments
			int j;
			struct text * current = i->text;

			for(j=1;j<i->cn;j++)
				current = current->next;
			current->next = (struct text *)malloc(sizeof(struct text));

			current->next->text = line;
			i->cn = i->cn + 1;
		}

		//move to next line
		ch = getc(fin);
	}

	//Restore to starting position
	ungetc(ch, fin);
}

void getPixels(FILE * fin, struct PPM * i){

	//allocate rows
	i->p = (struct pixel ** ) malloc(sizeof(struct pixel *) * i->h);

	int j,k;
	for(j=0;j<i->h;j++)
	{
		//allocate columns
		i->p[j] = (struct pixel * ) malloc(sizeof(struct pixel) * i->w);
		for (k=0;k<i->w;k++)
		{
			//get pixels for each
			fscanf (fin, "%d", &(i->p[j][k].r)); //get memory address
			fscanf (fin, "%d", &(i->p[j][k].g));
			fscanf (fin, "%d", &(i->p[j][k].b));
		}
	}
}


/* Return PPM image file from fin */
struct PPM * getPPM(FILE * fin){
        struct PPM * i = malloc(sizeof(struct PPM));

        //Type
        i->type = malloc(2*sizeof(char));
        i->type = readLine(fin);

        i->cn = 0;
        getText(fin, i);

        //Size
        fscanf (fin, "%d", &i->w);
        fscanf (fin, "%d", &i->h);
        fscanf (fin, "%d", &i->max);

        //Pixels
        getPixels(fin, i);

        return i;
}

/* Return PPM image i with message (hidden within r field) */
struct PPM * encode (char * txt, struct PPM * i){
        srand(time(NULL)); //Randomize seed. Seed set to current time.
        int j, len, sum, w, random;

        w = i->w;
        sum = 0;
        len = strlen(txt);

        //Check for compatability
        if((len * 75) < (w * i->h)){
                printf("steg: File is appropriate for encoding. \n");
        }
        else{
                printf("steg: Error- Image size not acceptable. \n");
                exit(0);
        }

        //for each char
        for (j=0;j<len;){
                random = (rand() % 100);
                sum = sum + random;

                //calc the pixel in terms of row and col
		int row, column;
                row = sum / (w);
                column = sum - (row * w);

                //get pixel
                struct pixel * p = &(i->p[row][column]); //Use memory address

                //check red pixel val != asci of letter encoded
                if(txt[j] != p->r){

                        p->r = txt[j];

                        j++;
                }
                else{
                        fprintf(stderr, "char [%j] - [%c] same. Retrying. \n", j, txt[j]);
		}
        }

        return i;
}

void writePPM(FILE * fin, struct PPM * i, const char *fileName){
	FILE *pfile = NULL;
	int x,y;

	pfile = fopen(fileName, "w");
	//fprintf(pfile, "%d\n %s\n %d %d\n %d\n", i->type, i->text, i->w, i->h, i->max);
	fprintf(pfile, "P3\n%d %d\n255\n", i->w, i->h);

	for(x=0; x<i->h; x++) {
		for(y=0; y<i->w;y++){
			struct pixel p = i->p[x][y];
			fprintf(pfile, "%i %i %i \n", p.r, p.g, p.b);		}
	}
	fclose(pfile);
}


char * decode(struct PPM * i1, struct PPM * i2){
	char * buffer = malloc(sizeof(char) * PPM_MAXMESS);

	//confirm that the files are the 'same'
	if(!(i1->h == i2->h && i1->w == i2->w)){
		printf("steg: Error - PPM files do not match. \n");
		exit(0);
	}

	int j, k, l, m, n;
	l = 0;

	//for each row
	for(j=0;j<i2->h;j++){
		//for each column
		for(k=0;k<i2->w;k++){
			//check if red pixels are not equal
			if(i2->p[j][k].r != i1->p[j][k].r)
				//get encoded char
				buffer[l] = i2->p[j][k].r;
				printf("%c", buffer[l]);
				l = l+1;
		}
	}

	//return txt
	m = strlen(buffer) - 1;
	if (buffer[m] == '\n')
		buffer[m] = '\0';

	char * str = malloc(sizeof(char) * (m+1));

	for(n=0; n<(m+1); n++)
		str[n] = buffer[n];

	free(buffer);
	return str;

}


void encodeFile(int argc, char const ** argv){
	char txt[PPM_MAXMESS];
	int j;
	//FILE * outFin;

	//outFin = fopen("modified.ppm", "w");

	FILE * fin = fopen(argv[2], "r");
	if(fin == NULL){
		fprintf(stderr," steg: Error - Unable to open the file '%s' .\n", argv[2]);
		exit(0);
	}

	struct PPM * i = getPPM(fin);

	//get txt
	fprintf(stderr, "Message to Encode-> ");
	fgets(txt, PPM_MAXMESS, stdin);

	j = strlen(txt) - 1;
	if(txt[j] == '\n')
		txt[j] = '\0';

	fprintf(stderr, "encoding in progress...\n");

	//encode ppm
	encode(txt, i);

	//output ppm
	//fputc(showPPM, outFin);
	writePPM(fin, i, "modified.ppm");
	showPPM(i);
}

void decodeFile(int argc, char const ** argv){

	FILE * i1 = fopen(argv[2], "r");
	FILE * i2 = fopen(argv[3], "r");

	if(i1 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[2]);
		exit(0);
	}

	if(i2 == NULL){
		printf("steg: Error - File '%s' could not be opened. \n", argv[3]);
		exit(0);
	}

	//get files
	struct PPM * oPPM = getPPM(i1);
	struct PPM * ePPM = getPPM(i2);

	//get encoded txt
	char * str = decode(oPPM, ePPM);

	//print txt
	printf("%s\n", str);
}

