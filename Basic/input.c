#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define NUMDATA 99999999

int main(){
	//./input>./input.txt 해서 사용!
	int *flag = (int *)malloc(sizeof(int)*NUMDATA);
	int i;
	srand(time(NULL));
	for(i=0; i<NUMDATA; i++){
		flag[i] = 0;
	}

	for(i=0; i<10000000; i++){
		unsigned long tmpkey =rand()%NUMDATA;
		while(flag[tmpkey]==1)
			tmpkey =rand()%NUMDATA;
		flag[tmpkey]=1;
		printf("%lu\n", tmpkey);
	}

}
