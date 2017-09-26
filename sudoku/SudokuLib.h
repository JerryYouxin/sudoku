#pragma once
#include "time.h"
#include "windows.h"
#include "stdio.h"
#include <iostream>

typedef struct HashNode_st {
	short *d;
	struct HashNode_st* next;
}HashNode;

// Sudoku class with batch
class Sudoku
{
	public:
	Sudoku(int N);
	Sudoku(char* filename);
	~Sudoku();

	void generate();
	void solve();
	inline short* mutable_cpu_data() { return data; }
	inline int getSize() { return N; }
	bool check();
	void print();

	private:
	short *data;
	bool *hash_calced;
	HashNode hash[100000] ={ 0 };
	int N;
};

Sudoku::Sudoku(char* filename) {
	int n=0;
	FILE *fp;
	fopen_s(&fp,filename,"r");
	if(fp==NULL) {
		printf("ERROR: File %s open failed!\n",filename);
		exit(1);
	}
	this->data = new short[81000000];
	short* cdata = this->data;
	printf("reading file %s...\n",filename);
	int r=1;
	char x;
	while(r>0) {
		r = fread(cdata,1,162,fp);
		r = fread(&x,1,1,fp);
		if(r<=0) break;
		++n;
		cdata += 81;
	}
	printf("%d sudoku puzzle read\n",n);
	this->N = n;
	this->hash_calced = 0;
}

// generate puzzle from answers.
void generate_sudoku(short* data, int N) {
	for(int n=0;n<N;++n) {
		int nBlank = (rand()%31)+30;
		short* cdata = data+n*81;
		for(int i=0;i<81;++i) {
			char* x = (char*)(cdata+i);
			if(81-i == nBlank) {
				nBlank--;
				x[0] = '0';
			} else if(rand()/(float)RAND_MAX > 0.6) {
				nBlank--;
				x[0] = '0';
			}
		}
	}
}

Sudoku::Sudoku(int N)
{
	this->data	= new short[81*N];
	this->hash_calced = new bool[N];
	this->N		= N;
	memset(hash_calced,0,sizeof(bool)*N);
	// set rand seed
	srand((unsigned int)time(0));
}

Sudoku::~Sudoku()
{
	if(hash_calced!=0) delete this->hash_calced;
	delete this->data;
}

void Sudoku::print() {
	char* buff = new char[172*N+1];
	FILE* fp;
	//printf("now writing...\n");
	fopen_s(&fp,"sudoku.txt","w");
	if(fp==0) {
		printf("ERROR: File sudoku.txt open failed.\n");
		exit(1);
	}
	char *pbuff = buff;
	short* dp = this->data;
	for(int n=0; n<N; ++n) {
		fwrite((char*)dp,162,1,fp);
		fwrite("\n",1,1,fp);
		dp += 81;
	}
	//printf("finish\n");
	fclose(fp);

}

int hash_code(short *data) {
	//hash_code(A) = multiply all(sum of each block's i-th value)
	int hash[9] ={ 0 };
	for(int r=0;r<9;++r)
		for(int c=0;c<9;++c) {
			int b_th = 3*(r%3)+(c%3);
			hash[b_th] += ((char*)(data+r*9+c))[0]-'0';
		}
	for(int i=1;i<9;++i) {
		hash[0] = (hash[0] * hash[i]) % 100000;
	}
	return hash[0];
}

bool Sudoku::check() {
	// hash check: hash_code(A) = multiply all (sum of each block's i-th value)
	// if hash_code is same, check one by one to varify if the two sudoku is same
	//for(int i=0;i<100000;++i) {
	//	hash[i].d = 0;
	//	hash[i].next = 0;
	//}
	short *cdata;
	int h;
	for(int i=0;i<N;++i) {
		if(!hash_calced[i]) {
			cdata = data+i*81;
			h = hash_code(cdata);
			if(hash[h].d==0) {
				hash[h].d = cdata;
			}
			else {
				// check seriously
				//printf("has same hash code %d(%d). checking seriously\n",h,i);
				HashNode *src = hash+h;
				while(src!=0) {
					int j;
					for(j=0;j<81;++j) {
						if(((char*)((src->d)+j))[0]!=((char*)(cdata+j))[0])
							break;
					}
					if(j>=81) {
						printf("Has same sudoku!\n");
						return false;
					}
					if(src->next==0) break;
					src = src->next;
				}
				src->next = new HashNode();
				src->next->d = cdata;
				src->next->next = 0;
			}
		}
	}
	return true;
}

void generate_rand_sudoku(short *data) {
	memset(data,0,81*sizeof(short));
	bool tried_value[81][9] ={ 0 };
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	char* chdata = (char*)data;
	chdata[0] = '5'; // magic number required for the project.
	chdata[1] = ' ';
				 // 4 = 5-1
	empty_value[4][0][0] = true; // 1st row
	empty_value[4][0][1] = true; // 1st col
	empty_value[4][0][2] = true; // 1st block
	tried_value[0][4] = true;
	// block = 3*(row/3) + col/3
	int count = 0;
		for(int i=1;i<81;++i) {
			if(i<1) return;
			int r = i/9;
			int c = i%9;
			int b = 3*(r/3) + (c/3);
			short v = rand()%9;
			int voff;
			for(voff=0;voff<9;++voff) {
				count++;
				if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2]||tried_value[i][v])) {
					char *x = (char*)(data+i);
					x[0] = (v+1)+'0';
					if(c==8) x[1] = '\n';
					else     x[1] = ' ';
					empty_value[v][r][0] = true;
					empty_value[v][c][1] = true;
					empty_value[v][b][2] = true;
					tried_value[i][v] = true;
					break;
				}
				v = (v + 1)%9;
			} 
			if(voff==9) {
				int tr = (i-1)/9;
				int tc = (i-1)%9;
				int tb = 3*(tr/3)+(tc/3);
				int tv = ((char*)(data+i-1))[0]-'0'-1;
				empty_value[tv][tr][0] = false;
				empty_value[tv][tc][1] = false;
				empty_value[tv][tb][2] = false;
				for(int j=0;j<9;++j) tried_value[i][j] = false;
				i-=2;
			}
		}
		//printf("Count = %d\n",count);
}

void generate_rand_sudoku_batch(short *data,int N) {
	short __data[81] ={ 0 };
	bool tried_value[81][9] ={ 0 };
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	char* chdata = (char*)__data;
	chdata[0] = '5'; // magic number required for the project.
	chdata[1] = ' ';
				 // 4 = 5-1
	empty_value[4][0][0] = true; // 1st row
	empty_value[4][0][1] = true; // 1st col
	empty_value[4][0][2] = true; // 1st block
	tried_value[0][4] = true;
	// block = 3*(row/3) + col/3
	int count = 0;
	int tcount = 0;
	int i = 1;
	//printf("N=%d\n",N);
	for(int n=0;n<N;++n) {
		while(i<81) {
			if(i<1) return;
			int r = i/9;
			int c = i%9;
			int b = 3*(r/3) + (c/3);
			short v = rand()%9;
			int voff;
			for(voff=0;voff<9;++voff) {
				count++;
				if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2]||tried_value[i][v])) {
					char *x = (char*)(__data+i);
					x[0] = (v+1)+'0';
					if(c==8) x[1] = '\n';
					else     x[1] = ' ';
					//__data[i] = v+1;
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
				int tv = ((char*)(__data+i-1))[0]-'0'-1;
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
		memcpy(data+n*81,__data,sizeof(short)*81);
		int tr = (i-1)/9;
		int tc = (i-1)%9;
		int tb = 3*(tr/3)+(tc/3);
		int tv = ((char*)(__data+i-1))[0]-'0'-1;
		empty_value[tv][tr][0] = false;
		empty_value[tv][tc][1] = false;
		empty_value[tv][tb][2] = false;
		--i;
	}
	//printf("Count = %d\n",tcount);
}

void Sudoku::generate() {
	short *cdata = this->data;
	short *odata = this->data;
	const int N = this->N;
	//int n;
	printf("gen rand batch\n");
	generate_rand_sudoku_batch(cdata,N);
	printf("fin");
}

void Sudoku::solve() {
	// naive algorithm
	#pragma omp parallel for
	for(int n=0;n<N;++n) {
		//printf("Solving %d...\n",n);
		//int t0 = clock();
		int fill[82]; // log if i-th grid should be filled in
		memset(fill,-1,sizeof(int)*82);
		bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
		short trying_value[81] ={ 0 };
		short *cdata = this->data + n*81;
		int ik=0;
		for(int i=0;i<81;++i) {
			int r = i/9;
			int c = i%9;
			int b = 3*(r/3) + (c/3);
			char x = ((char*)(cdata+i))[0]-'0';
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
			//printf("Now searching value for %d(%d)\n",k,ik);
				int r = k/9;
				int c = k%9;
				int b = 3*(r/3) + (c/3);
				int v;
				for(v=trying_value[k];v<9;++v) {
					if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2])) {
						//printf("%d: %d ==> %d %d\n",k,v+1,r,c);
						char *tc = (char*)(cdata+k);
						trying_value[k] = v+1;
						tc[0] = v+1+'0';
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
						printf("%d No answer!\n",n);
						break;
					}
					--ik;
					int tr = fill[ik]/9;
					int tc = fill[ik]%9;
					int tb = 3*(tr/3)+(tc/3);
					int tv = ((char*)(cdata+fill[ik]))[0]-'1';
					empty_value[tv][tr][0] = false;
					empty_value[tv][tc][1] = false;
					empty_value[tv][tb][2] = false;
					//printf("%d: Not found. Reset to %d\n",k,fill[ik]);
				}
				else {
					++ik;
				}
		}
		
		//int t1 = clock();
		//printf("\ttime:%d\n",t1-t0);
	}
}