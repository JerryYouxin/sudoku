#include "pch.h"
#include "SudokuLib.h"

#include "string.h"
#include "stdlib.h"
#include "time.h"

using namespace std;

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

}

int hash_code(short *data) {
	//hash_code(A) = multiply all(sum of each block's i-th value)
	//if()
	return 0;
}

bool Sudoku::check() {
	// hash check: hash_code(A) = multiply all (sum of each block's i-th value)
	// if hash_code is same, check one by one to varify if the two sudoku is same
	return true;
}

void generate_rand_sudoku(short *data) {
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	data[0] = 5; // magic number required for the project.
				 // 4 = 5-1
	empty_value[4][0][0] = true; // 1st row
	empty_value[4][0][1] = true; // 1st col
	empty_value[4][0][2] = true; // 1st block
								 // block = 3*(row/3) + col/3
	for(int r=0; r<9; ++r) {
		for(int c=0; c<9; ++c) {
			if(r==0&&c==0) continue;
			int b = 3*(r/3) + (c/3);
			int v = rand();
			int i = r*9+c;
			for(int voff=0;voff<9;++voff) {
				v = (v + voff)%9;
				if(!(empty_value[v][r][0]||empty_value[v][c][1]||empty_value[v][b][3])) {
					data[i] = v+1;
					empty_value[v][r][0] = true;
					empty_value[v][c][0] = true;
					empty_value[v][b][0] = true;
					break;
				}
			}
		}
	}
}

void Sudoku::generate() {
	// Step 0: generate a random Sudoku.
	short *cdata = this->data;
	short *odata = this->data;
	const int N = this->N;
	int n;
	generate_rand_sudoku(cdata);
	cdata += 81;
	n = 1;

	const int x[3][2] ={ { 0,1 },{ 0,2 },{ 1,2 } };

	while(n<N) {
		for(int i=0;i<9;++i) {
			// Step 1: col/row transformation
			memcpy(cdata,odata,4096*81);
			for(int j1=-1;j1<3;++j1) { // row grid 1
				for(int k1=-1;k1<3;++k1) { // col grid 1
					for(int j2=-1;j2<3;++j2) { // row grid 2
						for(int k2=-1;k2<3;++k2) { // col grid 2
							for(int j3=-1;j3<3;++j3) { // row grid 3
								for(int k3=-1;k3<3;++k3) { // col grid 3
									if(j1>=0) {
										memcpy(&cdata[9*(x[j1][1])],&odata[9*(x[j1][0])],9);
										memcpy(&cdata[9*(x[j1][0])],&odata[9*(x[j1][1])],9);
									}
									if(j2>=0) {
										memcpy(&cdata[9*(3+x[j1][1])],&odata[9*(3+x[j1][0])],9);
										memcpy(&cdata[9*(3+x[j1][0])],&odata[9*(3+x[j1][1])],9);
									}
									if(j3>=0) {
										memcpy(&cdata[9*(6+x[j1][1])],&odata[9*(6+x[j1][0])],9);
										memcpy(&cdata[9*(6+x[j1][0])],&odata[9*(6+x[j1][1])],9);
									}
									if(k1>=0) {
										for(int ii=0;ii<9;++ii) {
											cdata[ii*9+x[k1][0]] = odata[ii*9+x[k1][1]];
											cdata[ii*9+x[k1][1]] = odata[ii*9+x[k1][0]];
										}
									}
									if(k2>=0) {
										for(int ii=0;ii<9;++ii) {
											cdata[ii*9+x[k2][0]] = odata[ii*9+x[k2][1]];
											cdata[ii*9+x[k2][1]] = odata[ii*9+x[k2][0]];
										}
									}
									if(k3>=0) {
										for(int ii=0;ii<9;++ii) {
											cdata[ii*9+x[k3][0]] = odata[ii*9+x[k3][1]];
											cdata[ii*9+x[k3][1]] = odata[ii*9+x[k3][0]];
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
			for(int j=0;j<81;++j) {
				cdata[j] = odata[i]+1;
				if(cdata[j]==9) cdata[j]=1;
			}
			cdata += 81;
			// Step 3: loop back to Step 1.
			odata = cdata;
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

Sudoku* Sudoku::solve() {
	return NULL;
}
