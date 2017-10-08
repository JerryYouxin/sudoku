#pragma once
#include "time.h"
#include "windows.h"
#include "stdio.h"
#include <iostream>

class Core
{
	public:
	void generate(int number,int mode,int *result);
	void generate(int number,int lower,int upper,bool unique,int *result);
	void generate(int number,int *result); // generate final states
	bool solve(int *puzzle,int *solution);
	void solve(int number,int *puzzle,int *solution);
	int read_sudoku(int **puzzle,const char* filename);
	int write_sudoku(int number,int *puzzle,const char* filename);
	const int FILE_OPEN_ERROR = -2;
	const int FILE_READ_ERROR = -1;
	const int FILE_WRITE_ERROR= -1;
	const int SUDOKU_NUM_ERROR= -1;
};

int Core::read_sudoku(int **puzzle,const char* filename) {
	int n = 0;
	int size = 0;
	int r = 0;
	int i = 0;
	int * data = 0;
	char* rdata= 0;
	FILE *fp;
	fopen_s(&fp,filename,"rb");
	if(fp==NULL) {
		//printf("ERROR: File %s open failed!\n",filename);
		return FILE_OPEN_ERROR;
	}
	// get file size
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	rewind(fp);
	// allocate memory
	//printf("allocate data: %d\n",size*sizeof(int));
	data = new int[size];
	//printf("allocate rdata: %d\n",size*sizeof(char));
	rdata= new char[size];
	//printf("reading file %s...\n",filename);
	r = fread(rdata,1,size,fp);
	if(r<0 || r!=size) {
		delete rdata;
		delete data;
		return FILE_READ_ERROR; // Error
	}
	for(i=0;i<size;++i) {
		if(rdata[i]>='0'&&rdata[i]<='9') {
			data[n] = rdata[i]-'0';
			++n;
		}
	}
	if(n%81!=0) return SUDOKU_NUM_ERROR;
	n = n/81;
	//printf("%d sudoku puzzle read\n",n);
	*puzzle = data;
	delete rdata;
	return n;
}

int Core::write_sudoku(int number,int *puzzle,const char* filename) {
	char* buff = new char[163*number+1];
	char* pbuff= 0;
	int * pdata= 0;
	FILE* fp;
	//printf("now writing...\n");
	for(int n=0; n<number; ++n) {
		pbuff = buff+n*163; // 163 = 81 number+ 81 space or \n + 1 \n
		pdata = puzzle+n*81;
		for(int i=0;i<81;++i) {
			pbuff[2*i] = pdata[i]+'0';
			if(i%9==8) pbuff[2*i+1] = '\n';
			else pbuff[2*i+1] = ' ';
		}
		pbuff[162] = '\n';
	}
	fopen_s(&fp,filename,"w");
	if(fp==0) {
		//printf("ERROR: File sudoku.txt open failed.\n");
		return FILE_OPEN_ERROR;
	}
	fwrite(buff,163,number,fp);
	//printf("finish\n");
	fclose(fp);
	delete buff;
	return 0;
}

void Core::generate(int number,int mode,int *result) {
	// TODO
	printf("Not Implemented\n");
}
void Core::generate(int number,int lower,int upper,bool unique,int *result) {
	// TODO
	printf("Not Implemented\n");
}
void Core::generate(int number,int *result) {
	int __data[81] ={ 0 };
	bool tried_value[81][9] ={ 0 };
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	// block = 3*(row/3) + col/3
	int count = 0;
	int tcount = 0;
	int i = 0;
	//printf("N=%d\n",N);
	for(int n=0;n<number;++n) {
		while(i<81) {
			if(i<0) return;
			int r = i/9;
			int c = i%9;
			int b = 3*(r/3) + (c/3);
			int v = rand()%9;
			int voff;
			for(voff=0;voff<9;++voff) {
				count++;
				if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2]||tried_value[i][v])) {
					__data[i] = v+1;
					empty_value[v][r][0] = true;
					empty_value[v][c][1] = true;
					empty_value[v][b][2] = true;
					tried_value[i][v] = true;
					break;
				}
				//else printf("%d %d %d -> %d failed: %d %d %d\n",r,c,b,v+1,empty_value[v][r][0],empty_value[v][c][1],empty_value[v][b][2]);
				v = (v + 1)%9;
			}
			if(voff==9) {
			int tr = (i-1)/9;
				int tc = (i-1)%9;
				int tb = 3*(tr/3)+(tc/3);
				int tv = __data[i-1]-1;
				empty_value[tv][tr][0] = false;
				empty_value[tv][tc][1] = false;
				empty_value[tv][tb][2] = false;
				for(int j=0;j<9;++j) tried_value[i][j] = false;
				--i;
			}
			else ++i;
		}
		tcount+=count;
		count = 0;
		//if(n%10000==0)
		//	printf("n=%d\n",n);
		memcpy(result+n*81,__data,sizeof(int)*81);
		int tr = (i-1)/9;
		int tc = (i-1)%9;
		int tb = 3*(tr/3)+(tc/3);
		int tv = __data[i-1]-1;
		empty_value[tv][tr][0] = false;
		empty_value[tv][tc][1] = false;
		empty_value[tv][tb][2] = false;
		--i;
	}
}
bool Core::solve(int *puzzle,int *solution) {
	int fill[82]; // log if i-th grid should be filled in
	memset(fill,-1,sizeof(int)*82);
	if(puzzle!=solution)
		memcpy(solution,puzzle,81*sizeof(int));
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	int trying_value[81] ={ 0 };
	int ik=0;
	for(int i=0;i<81;++i) {
		int r = i/9;
		int c = i%9;
		int b = 3*(r/3) + (c/3);
		int x = puzzle[i];
		if(x!=0) {
			int v = x-1;
			empty_value[v][r][0] = true;
			empty_value[v][c][1] = true;
			empty_value[v][b][2] = true;
		}
		else {
			fill[ik] = i;
			++ik;
		}
	}
	// begin search
	int k=0;
	int last=0;
	ik = 0;
	while((k=fill[ik])>=0) {
		int r = k/9;
		int c = k%9;
		int b = 3*(r/3) + (c/3);
		int v;
		for(v=trying_value[k];v<9;++v) {
			if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2])) {
				//char *tc = (char*)(cdata+k);
				trying_value[k] = v+1;
				solution[k] = v+1;
				empty_value[v][r][0] = true;
				empty_value[v][c][1] = true;
				empty_value[v][b][2] = true;
				break;
			}
		}
		if(v>=9) {
			trying_value[k] = 0;
			if(ik==0) {
				// no number can fill in the first empty block
				// so no answer could be found
				//printf("No answer!\n");
				return false;
			}
			--ik;
			int tr = fill[ik]/9;
			int tc = fill[ik]%9;
			int tb = 3*(tr/3)+(tc/3);
			int tv = solution[fill[ik]]-1;
			empty_value[tv][tr][0] = false;
			empty_value[tv][tc][1] = false;
			empty_value[tv][tb][2] = false;
		}
		else {
			++ik;
		}
	}
	return true;
}
void Core::solve(int number,int *puzzle,int *solution) {
	#pragma omp parallel for
	for(int i=0;i<number;++i) {
		//printf("Solving %d...\n",i);
		solve(puzzle+i*81,solution+i*81);
	}
}