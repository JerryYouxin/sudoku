#pragma once
#include "time.h"
#include "windows.h"
#include "stdio.h"
#include <iostream>
#include <map>
#include <string>
#include "time.h"

class Core
{
	public:
	// sudoku generator
	void generate(int number,int mode,int result[][81]);
	void generate(int number,int lower,int upper,bool unique,int result[][81]);
	void generate(int number,int result[][81]); // generate final states
	// sudoku solver
	bool solve(int puzzle[],int solution[]);
	void solve(int number,int *puzzle,int *solution);
	// I/O functions
	int read_sudoku(int **puzzle,const char* filename);
	int write_sudoku(int number,int *puzzle,const char* filename);
	// check functions
	bool check_valid(int *solution);
	int  check_valid(int number,int *solution);
	bool check_same(int number,int *solution);
	//some tool functions
	int get_free_degree_plus(int betsudoku[], int index);

	// Error codes
	const int FILE_OPEN_ERROR = -2;
	const int FILE_READ_ERROR = -1;
	const int FILE_WRITE_ERROR= -1;
	const int SUDOKU_NUM_ERROR= -1;
};

typedef std::map<std::string, int> hashMap;

bool Core::check_same(int number,int *solution) {
	if(solution==0||number<=0) return false;
	hashMap hmap;
	long long hash[6];
	for(int i=0;i<number;++i) {
		int* ptr = solution+i*81;
		memset(hash,0,6*sizeof(long long));
		// convert to 6 long long
		char* ph = (char*)hash;
		for(int j=0;j<40;++j) {
			ph[j] = ptr[2*j]|(ptr[2*j+1]<<4);
		}
		ph[40] = ptr[80];
		ph[41] = '\0';
		std::string x(ph);
		// search if the number is already exist
		hashMap::iterator it= hmap.find(x);
		if(it == hmap.end()) {
			hmap[x] = 1;
		}
		else {
			return false;
		}
	}
	return true;
}

bool Core::check_valid(int *solution) {
	if(solution==0) return false;
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	for(int r=0;r<9;++r) {
		for(int c=0;c<9;++c) {
			int b = 3*(r/3)+(c/3);
			int i = r*9+c;
			int v = solution[i]-1;
			if(v>9||v<1) return false;
			if(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2]) {
				return false;
			}
			empty_value[v][r][0] = true;
			empty_value[v][c][1] = true;
			empty_value[v][b][2] = true;
		}
	}
	return true;
}

int Core::check_valid(int number,int *solution) {
	if(solution==0) return -1;
	for(int i=0;i<number;++i) {
		if(!check_valid(solution+i*81)) {
			return i;
		}
	}
	return 0;
}

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

void Core::generate(int number,int mode,int result[][81]) {
	// TODO
	int i = 0;
	int free_degree = 0;
	int sudoku_num = 0;
	int num = 0;
	int the_time = 0;
	int hollow_num = 0;
	for (sudoku_num == 0; sudoku_num < number; sudoku_num++)
	{
		free_degree = 0;
		num = 0;
		if (mode == 1)
		{
			srand((unsigned)time(NULL));
			hollow_num=rand() % 9 + 40;
			if (the_time == 0)
			{
				for (i = 0;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 700)
					{
						the_time = 1;
						continue;
					}
				}


			}
			else
			{
				for (i = 1;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 700)
					{
						the_time = 0;
						continue;
					}
				}
			}
		}
		if (mode == 2)
		{
			srand((unsigned)time(NULL));
			hollow_num = rand() % 9 + 49;
			if (the_time == 0)
			{
				for (i = 0; i < 81; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 900)
					{
						the_time = 1;
						continue;
					}
				}
				for (i = 1;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 900)
					{
						the_time = 1;
						continue;
					}
				}
			}
			else
			{
				for (i = 1; i < 81; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 900)
					{
						the_time = 0;
						continue;
					}
				}
				for (i = 0;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 900)
					{
						the_time = 0;
						continue;
					}
				}
			}
		}
		if (mode == 3)
		{
			srand((unsigned)time(NULL));
			hollow_num = rand() % 9 + 58;
			if (the_time == 0)
			{
				for (i = 0; i < 81; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 1100)
					{
						the_time = 1;
						continue;
					}
				}
				for (i = 1;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 1100)
					{
						the_time = 1;
						continue;
					}
				}
			}
			else
			{
				for (i = 0; i < 81; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 1100)
					{
						the_time = 0;
						continue;
					}
				}
				for (i = 1;; i = i + 2)
				{
					result[sudoku_num][i] = 0;
					num++;
					free_degree += get_free_degree_plus(result[num], i);
					if (num >= hollow_num || free_degree >= 1100)
					{
						the_time = 0;
						continue;
					}
				}
			}
		}
	}
	printf("Not Implemented\n");
}



void Core::generate(int number,int lower,int upper,bool unique,int result[][81]) {
	if(unique) {
		// TODO
		printf("Not Implemented\n");
	} else {
		generate(number,result);
		srand((unsigned int)time(0));
		for(int n=0;n<number;++n) {
			int num = rand()%(upper-lower+1)+lower;
			for(int i=0;i<81;++i) {
				if(81-i+1<=num) {
					result[n][i] = 0;
					--num;
				}
				else {
					double r = rand()/(double)RAND_MAX;
					if(r<0.5) {
						result[n][i] = 0;
						--num;
					}
				}
			}
		}
	}
}
void Core::generate(int number,int result[][81]) {
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
		memcpy(result[n],__data,sizeof(int)*81);
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
bool Core::solve(int puzzle[],int solution[]) {
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
int Core::get_free_degree_plus(int betsudoku[], int index)
{
	int i = index / 9;
	int j = index % 9;
	/* x为n所在的小九宫格左顶点竖坐标 */
	int x = index / 9 / 3 * 3;
	/* y为n所在的小九宫格左顶点横坐标 */
	int y = index % 9 / 3 * 3;

	int line_free_degree = 0;
	int column_free_degree = 0;
	int area_free_degree = 0;
	int free_degree = 0;
	// 循环变量
	int p = 0;
	int q = 0;
	//区域自由度
	for (p = x; p < x + 3; p++)
	{
		for (q = y; q < y + 3; q++)
		{
			if ((betsudoku[9 * p + q] == 0) && ((9 * p + q) != index) && (i != p) && (j != q))
			{
				area_free_degree++;
			}
			else
			{
				continue;
			}
		}
	}
	//行自由度
	for (p = 9 * i; p < 9 * i + 9; p++)
	{
		if (betsudoku[p] == 0)
		{
			line_free_degree++;
		}
	}


	//列自由度
	for (q = j; q < 81; q = q + 9)
	{
		if (betsudoku[q] == 0)
		{
			column_free_degree++;
		}
	}

	free_degree = area_free_degree + line_free_degree + column_free_degree;
	return free_degree;
}