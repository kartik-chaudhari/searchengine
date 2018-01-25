#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int maxpages = 50;

typedef struct word { 
	char word[32]; //word
	int *pages[50]; //index of page is the pointer index while positions are stored at ptrs.
} word;

int lines (char filename[64]) {
	char x = 'a';
	int count = 0;
	FILE *fp = fopen(filename, "r");
	while (x != EOF) {
		x = fgetc(fp);
		if (x == '\n')
			count++;
	}
	printf("%d\n", count);
	fclose(fp);
	return count;
}

int length (int *arr) {
	int len = 0;
	while (arr[len] != 0)
		len++;
	return len;
}

int freq (word *arr, int npage, int nkey) {
	int i, j, k, temp;
	int count = 1, freq = 0;
	for (i = 0; i < nkey; i++)
		if (arr[i].pages[npage])
			for (j = 0; j < length(arr[i].pages[npage]); j++)
				arr[i].pages[npage][j] -= i;
				
	if (arr[0].pages[npage]) {		
		for (j = 0; j < length(arr[0].pages[npage]); j++) {
			temp = arr[0].pages[npage][j];
			for (i = 1; i < nkey; i++)
				if (arr[i].pages[npage]) 
					for (k = 0; arr[i].pages[npage][k] <= temp, k < length(arr[i].pages[npage]); k++)
						if (arr[i].pages[npage][k] == temp)
							count++;
			if (count == nkey)
				freq++;
		}
	}
	
	return (freq * nkey); 
	
}

int sort(float *scores, int *ranked_id) {
	int i, j, n = 0;
	float scores2[50], temp;
	for (i = 0; i < 50; i++)
		scores2[i] = scores[i];
	
	for (i = 0; i < 50; i++)
        	for (j = i + 1; j < 50; j++)
           		if (scores2[i] < scores2[j]) {
                		temp = scores2[i];
                		scores2[i] = scores2[j];
                		scores2[j] = temp;
            		}
            		
        for (n = 0; n < 50; n++)
        	if (scores2[n] == 0)
        		break;

        		
	for (i = 0; i < n; i++)
		for (j = 0; j < 50; j++)
			if (scores[j] == scores2[i]) {
				ranked_id[i] = j + 1;
				scores[j] = 0;
				break;
			} 
	return n;      
        	
}
				
	
	
int main (int argc, char *argv[]) {
	FILE *fp;
	word arr[argc - 2];
	char tempword[32], occur[1024], extra, *currnum;
	int wc = lines(argv[1]), i = 0, j, k = 0, p, r, q;
	float scores[50];
	int ranked_id[50], n;
	fp = fopen(argv[1], "r");
	
	for (k = 2; k < argc; k++)
		for (q = 0; q < 50; q++)
			arr[k-2].pages[q] = NULL;
	
	while (i < wc) {
		if (i > 0)
			fgetc(fp);
		for (j = 0; j < 32; j++) {
			tempword[j] = fgetc(fp);
			if (tempword[j] == '|')
				break;
		}
		tempword[j] = '\0';
		printf("%s\n", tempword);
		for (k = 2; k < argc; k++) {
			if (strcmp(tempword, argv[k]) == 0) {
				strcpy(arr[k-2].word, tempword);
				
				while(1) {
					extra = fgetc(fp);
					r = atoi(&extra);
					if (r == 0)
						break;
					printf("%d\n", r);
					
					fgetc(fp);
					p = 0;
					do {
						occur[p++] = fgetc(fp);
					} while (occur[p - 1] != ';');						
					
					occur[p - 1] = '\0';
					printf("%s\n", occur);
					p = 0;
					
					/* putting positions into array */
					while ((currnum = strtok(p ? NULL : occur, ",")) != NULL) {
						arr[k-2].pages[r-1] = (int *)realloc(arr[k-2].pages[r-1], sizeof(int) * (p+1));
        					arr[k-2].pages[r-1][p++] = atoi(currnum);
        				}
					/*printf("len %d %d %d\n", k-2, r-1, length(arr[k-2].pages[r-1]));
					for (q = 0; q < length(arr[k-2].pages[r-1]); q++)
						printf("%d ", arr[k-2].pages[r-1][q]);
					printf("\n");*/
				}
				break;					
			}
		}
		i++;
	}
	fclose(fp);
	/* data entered into respective structures */
	/*
	for (j = 0; j < argc - 2; j++) {
		printf("%d: ", j);
		for (i = 0; i < 50; i++)
			if (arr[j].pages[i] != NULL)
				printf("%d ", i);
		printf("\n");
	}
			
	for (i = 0; i < 10; i++) {
		printf("%d: ", i+1);
		for (j = 0; j < argc - 2; j++) {
			if (arr[j].pages[i] != NULL) 
				for (k = 0; k < length(arr[j].pages[i]); k++)
					printf("%d ", arr[j].pages[i][k]);
			printf("; ");
		}
		printf("\n");
	}*/
	
	/* scores */
	for (i = 0; i < 50; i++) {
		scores[i] = 0;
		/* key word frequencies */
		for (j = 0; j < (argc-2); j++)
			if (arr[j].pages[i])
				scores[i] += ((float)length(arr[j].pages[i])/(argc-2));
		
		/* number of keywords the page has */
		for (j = 0; j < (argc-2); j++)
			if (arr[j].pages[i])
				scores[i] += 1.0;
		
		/* consecutive key words */
		scores[i] += (float)freq(arr, i, argc-2);
		if (scores[i])
			printf("Score of page %d is %f\n", i+1, scores[i]);
	}
	
	n = sort(scores, ranked_id);
	
	fp = fopen("id_rank.txt", "w");
	
	
	for (i = 0; i < n; i++)
		fprintf(fp, "%d ", ranked_id[i]);
	
	fclose(fp);		
	
	return 0;
}
