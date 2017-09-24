// sudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SudokuLib.h"

int string2int(char* str,int *value) {
	int v = 0;
	for(int i=0; str[i]!='\0'; ++i) {
		if(str[i]<'0'&&str[i]>'9') {
			return -1;
		}
		v = v*10+str[i]-'0';
	}
	*value = v;
	return 0;
}

int parse(int argc,char* argv[],int* N,char* path) {
	bool parsed[26]={ 0 };
	for(int i=1;i<argc;++i) {
		if(strcmp(argv[i],"-c")==0) {
			/*if(parsed['c']) {
				printf("Error: %d argument -c already parsed\n.",i);
				return -4;
			}*/
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -s\n");
				return -1;
			}
			int r = string2int(argv[i],N);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				return -2;
			}
			printf("N = %d\n",*N);
			parsed['c'] = true;
		}
		else if(strcmp(argv[i],"-s")==0) {
			if(parsed['s']) {
				printf("Error: argument -s already parsed\n.");
				return -4;
			}
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -s\n");
				return -1;
			}
			path = argv[i];
			parsed['s'] = true;
		}
		else {
			printf("Error: unknown argument: %s\n",argv[i]);
			return -3;
		}
	}
	return 0;
}

int main(int argc,char* argv[])
{
	int N=0;
	char* path=NULL;
	parse(argc,argv,&N,path);
	Sudoku *sudoku;
	//N = 1000000;
	if(N>0) {
		sudoku = new Sudoku(N);
		sudoku->generate();
		sudoku->print();
		sudoku->check();
		delete sudoku;
	}
	else if(path!=NULL) {
		//sudoku = new Sudoku(path);
	}
	return 0;
}

