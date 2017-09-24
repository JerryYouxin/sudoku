#pragma once
#include "time.h"
#include "stdio.h"
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
	int N;
};

Sudoku::Sudoku(char* filename) {
	int n=0;
	FILE *fp;
	fopen_s(&fp,filename,"r");
	this->data = new short[81000000];
	short* cdata = this->data;
	printf("reading file %s...\n",filename);
	while(1) {
		int r;
		for(int i=0;i<81;++i) {
			char x;
			r = fscanf_s(fp,"%c",&x);
			if(r<=0) break;
			while(x<'0' || x>'9') {
				r = fscanf_s(fp,"%c",&x);
				if(r<=0) break;
			}
			if(r<=0) break;
			cdata[i] = x-'0';
			/*
			if(n>49100) {
				if(i%9==0) printf("\n");
				printf("%d ",cdata[i]);
				if(cdata[i]<0) { printf("ERROR %d %d %d\n",i,n,x); exit(-1); }
			}
			*/
		}
		//if(n%100==0)printf("%d finish\n",n);
		if(r<=0) break;
		++n;
		cdata += 81;
	}
	printf("%d sudoku puzzle read\n",n);
	this->N = n;
}

Sudoku::Sudoku(int N)
{
	this->data	= new short[81*N];
	this->N		= N;
	// set rand seed
	srand((unsigned int)time(0));
}

Sudoku::~Sudoku()
{
	delete this->data;
}

void Sudoku::print() {
	FILE* fp;
	printf("now writing...\n");
	fopen_s(&fp,"sudoku.txt","w");
	short* dp = this->data;
	for(int n=0; n<N; ++n) {
		for(int i=0; i<9; ++i) {
			for(int j=0; j<9; ++j) {
				fprintf(fp,"%d ",dp[i*9+j]);
				//printf("%d ",dp[i*9+j]);
			}
			//printf("\n");
			fprintf(fp,"\n");
		}
		//printf("\n");
		fprintf(fp,"\n");
		dp += 81;
	}
	printf("finish\n");
	fclose(fp);

}

int hash_code(short *data) {
	//hash_code(A) = multiply all(sum of each block's i-th value)
	int hash[9] ={ 0 };
	for(int r=0;r<9;++r)
		for(int c=0;c<9;++c) {
			int b_th = 3*(r%3)+(c%3);
			hash[b_th] += data[r*9+c];
		}
	for(int i=1;i<9;++i) {
		hash[0] = (hash[0] * hash[i]) % 100000;
	}
	return hash[0];
}

typedef struct HashNode_st {
	short *d;
	struct HashNode_st* next;
}HashNode;

bool Sudoku::check() {
	// hash check: hash_code(A) = multiply all (sum of each block's i-th value)
	// if hash_code is same, check one by one to varify if the two sudoku is same
	HashNode hash[100000] ={ 0 };
	for(int i=0;i<100000;++i) {
		hash[i].d = 0;
		hash[i].next = 0;
	}
	short *cdata;
	for(int i=0;i<N;++i) {
		cdata = data+i*81;
		int h = hash_code(cdata);
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
					if(src->d[j]!=cdata[j])
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
	return true;
}

void generate_rand_sudoku(short *data) {
	memset(data,0,81*sizeof(short));
	bool tried_value[81][9] ={ 0 };
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	data[0] = 5; // magic number required for the project.
				 // 4 = 5-1
	empty_value[4][0][0] = true; // 1st row
	empty_value[4][0][1] = true; // 1st col
	empty_value[4][0][2] = true; // 1st block
	tried_value[81][4] = true;
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
					data[i] = v+1;
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
				empty_value[data[i-1]-1][tr][0] = false;
				empty_value[data[i-1]-1][tc][1] = false;
				empty_value[data[i-1]-1][tb][2] = false;
				for(int j=0;j<9;++j) tried_value[i][j] = false;
				i-=2;
			}
		}
		printf("Count = %d\n",count);
}

void generate_rand_sudoku_batch(short *data,int N) {
	short __data[81] ={ 0 };
	bool tried_value[81][9] ={ 0 };
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	__data[0] = 5; // magic number required for the project.
				 // 4 = 5-1
	empty_value[4][0][0] = true; // 1st row
	empty_value[4][0][1] = true; // 1st col
	empty_value[4][0][2] = true; // 1st block
	tried_value[81][4] = true;
	// block = 3*(row/3) + col/3
	int count = 0;
	int tcount = 0;
	int i = 1;
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
				/*for(int i=0;i<9;++i) {
				for(int j=0;j<9;++j) {
				printf("%d ",data[i*9+j]);
				}
				printf("\n");
				}*/
				int tr = (i-1)/9;
				int tc = (i-1)%9;
				int tb = 3*(tr/3)+(tc/3);
				empty_value[__data[i-1]-1][tr][0] = false;
				empty_value[__data[i-1]-1][tc][1] = false;
				empty_value[__data[i-1]-1][tb][2] = false;
				for(int j=0;j<9;++j) tried_value[i][j] = false;
				--i;
			}
			else ++i;
		}
		tcount+=count;
		count = 0;
		memcpy(data+n*81,__data,sizeof(short)*81);
		int tr = (i-1)/9;
		int tc = (i-1)%9;
		int tb = 3*(tr/3)+(tc/3);
		empty_value[__data[i-1]-1][tr][0] = false;
		empty_value[__data[i-1]-1][tc][1] = false;
		empty_value[__data[i-1]-1][tb][2] = false;
		--i;
	}
	printf("Count = %d\n",tcount);
}

void Sudoku::generate() {
	// Step 0: generate a random Sudoku.
	short *cdata = this->data;
	short *odata = this->data;
	const int N = this->N;
	int n;
//#ifdef GEN_WITH_RANDBATCH
	if(N<=1000) {
		printf("gen rand batch\n");
		generate_rand_sudoku_batch(data,N);
		printf("fin");
	}
//#else
	else {
		generate_rand_sudoku(cdata);
		cdata += 81;
		n = 1;

		const int x[3][2] ={ { 1,2 }, { 0,1 },{ 0,2 } };
		int trans[8] ={ 1,2,3,4,6,7,8,9 };

		for(int i=0;i<9;++i) {
			for(int j=0;j<9;++j) {
				printf("%d ",data[i*9+j]);
			}
			printf("\n");
		}

		printf("begin generate with transformation\n");
		while(n<N) {
			for(int i=0;i<8;++i) {
				// Step 1: col/row transformation
				for(int j1=-1;j1<1;++j1) { // row grid 1
					// first row could not change
					for(int k1=-1;k1<1;++k1) { // col grid 1
						// first col could not change
						for(int j2=-1;j2<3;++j2) { // row grid 2
							for(int k2=-1;k2<3;++k2) { // col grid 2
								for(int j3=-1;j3<3;++j3) { // row grid 3
									for(int k3=-1;k3<3;++k3) { // col grid 3
										if(j1<0&&k1<0&&j2<0&&k2<0&&j3<0&&k3<0) continue;
										memcpy(cdata,odata,sizeof(short)*81);
										if(j1>=0) {
											memcpy(&cdata[9*(x[j1][1])],&odata[9*(x[j1][0])],sizeof(short)*9);
											memcpy(&cdata[9*(x[j1][0])],&odata[9*(x[j1][1])],sizeof(short)*9);
										}
										if(j2>=0) {
											memcpy(&cdata[9*(3+x[j2][1])],&odata[9*(3+x[j2][0])],sizeof(short)*9);
											memcpy(&cdata[9*(3+x[j2][0])],&odata[9*(3+x[j2][1])],sizeof(short)*9);
										}
										if(j3>=0) {
											memcpy(&cdata[9*(6+x[j3][1])],&odata[9*(6+x[j3][0])],sizeof(short)*9);
											memcpy(&cdata[9*(6+x[j3][0])],&odata[9*(6+x[j3][1])],sizeof(short)*9);
										}
										if(k1>=0) {
											for(int ii=0;ii<9;++ii) {
												cdata[ii*9+x[k1][0]] = odata[ii*9+x[k1][1]];
												cdata[ii*9+x[k1][1]] = odata[ii*9+x[k1][0]];
											}
										}
										if(k2>=0) {
											for(int ii=0;ii<9;++ii) {
												cdata[ii*9+3+x[k2][0]] = odata[ii*9+3+x[k2][1]];
												cdata[ii*9+3+x[k2][1]] = odata[ii*9+3+x[k2][0]];
											}
										}
										if(k3>=0) {
											for(int ii=0;ii<9;++ii) {
												cdata[ii*9+6+x[k3][0]] = odata[ii*9+6+x[k3][1]];
												cdata[ii*9+6+x[k3][1]] = odata[ii*9+6+x[k3][0]];
											}
										}
										cdata += 81;
										++n;
										if(n>=N) return;
									}
								}
							}
						}
					}
				}
				// Step 2: non-linear transformation
				for(int j=0;j<8;++j) {
					trans[j] = (trans[j]+1)%9;
					if(trans[j]>=5) trans[j] = (trans[j]+1)%9;
				}
				for(int j=0;j<81;++j) {
					if(odata[j]<4)
						cdata[j] = odata[j]+1;
					else if(odata[j]==4)
						cdata[j] = odata[j]+2;
					else if(odata[j]==5)
						cdata[j] = odata[j];
					else
						cdata[j] = (odata[j]+1)%9;
				}
				odata = cdata;
				cdata += 81;
				// Step 3: loop back to Step 1.
				++n;
			}
			// Step 4: if already create all equal sudoku, generate new one and loop back to Step 1.
			if(n<N) {
				generate_rand_sudoku(cdata);
				cdata += 81;
				++n;
			}
		}
	}
}

void Sudoku::solve() {
	// naive algorithm
	//#pragma omp parallel
	for(int n=0;n<N;++n) {
		printf("Solving %d...\n",n);
		bool fill[81] ={ 0 }; // log if i-th grid should be filled in
		bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
		short trying_value[81] ={ 0 };
		short *cdata = this->data + n*81;
		for(int i=0;i<81;++i) {
			int r = i/9;
			int c = i%9;
			int b = 3*(r/3) + (c/3);
			if(cdata[i]!=0) {
				int v = cdata[i]-1;
				empty_value[v][r][0] = true;
				empty_value[v][c][1] = true;
				empty_value[v][b][2] = true;
			}
			else fill[i] = true;	
		}
		// begin search
		int k=0;
		while(k<81) {
			if(fill[k]) {
				int r = k/9;
				int c = k%9;
				int b = 3*(r/3) + (c/3);
				int v;
				for(v=trying_value[k];v<9;++v) {
					if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][2])) {
						//printf("%d: %d ==> %d %d\n",k,v+1,r,c);
						trying_value[k] = v+1;
						cdata[k] = v+1;
						empty_value[v][r][0] = true;
						empty_value[v][c][1] = true;
						empty_value[v][b][2] = true;
						break;
					}
				}
				if(v>=9) {
					trying_value[k] = 0;
					--k;
					int tr = k/9;
					int tc = k%9;
					int tb = 3*(tr/3)+(tc/3);
					empty_value[cdata[k]-1][tr][0] = false;
					empty_value[cdata[k]-1][tc][1] = false;
					empty_value[cdata[k]-1][tb][2] = false;
					//printf("Not found. Reset to %d\n",k);
				}
				else {
					++k;
				}
			}
			else ++k;
		}

	}
}


