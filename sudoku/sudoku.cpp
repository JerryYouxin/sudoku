// sudoku.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "SudokuLib.h"

class Parser {
	public:
	Parser(int argc,char* argv[]);
	int N=0;
	char* path=0;
	int mode=0;
	int lower=0;
	int upper=0;
	bool unique=false;
	int code_type=0;
	int err=0;
	// constants
	const int _C = 1;
	const int _N = 2;
	const int _M = 3;
	const int _R = 4;
	const int _S = 5;

	private:
	int string2int(char* str,int *value);
	const int RMIN = 20;
	const int RMAX = 55;
	const int EASY = 1;
	const int NORM = 2;
	const int HARD = 3;
	const int NMIN = 1;
	const int NMAX = 10000;
	const int CMAX = 1000000;
};
int Parser::string2int(char* str,int *value) {
	int v = 0;
	int i = 0;
	int signal = 1;
	if(str[i]=='-') {
		signal = -1;
		++i;
	}
	else if(str[i]=='+') {
		++i;
	}
	for(; str[i]!='\0'; ++i) {
		if(str[i]<'0'||str[i]>'9') {
			return -1;
		}
		else if(i>9) {
			return -2;
		}
		v = v*10+str[i]-'0';
	}
	*value = signal*v;
	return 0;
}

Parser::Parser(int argc,char* argv[]) {
	for(int i=1;i<argc;++i) {
		if(strcmp(argv[i],"-c")==0) {
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -c\n");
				err = -1;
				return;
			}
			int r = string2int(argv[i],&N);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				err = -2;
				return;
			}
			if(N<NMIN||N>CMAX) {
				printf("Error: input value is out of range: %d (valid range:%d~%d)\n",N,NMIN,CMAX);
				err = -3;
				return;
			}
			printf("N = %d\n",N);
			code_type = _C;
		}
		else if(strcmp(argv[i],"-s")==0) {
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -s\n");
				err = -1;
				return;
			}
			path = argv[i];
			printf("path = %s\n",path);
			code_type = _S;
		}
		else if(strcmp(argv[i],"-n")==0) {
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -n\n");
				err = -1;
				return;
			}
			int r = string2int(argv[i],&N);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				err = -2;
				return;
			}
			if(N<NMIN||N>NMAX) {
				printf("Error: input value is out of range: %d (valid range:%d~%d)\n",N,NMIN,NMAX);
				err = -3;
				return;
			}
			printf("N = %d\n",N);
			if(code_type==0) code_type = _N;
		}
		else if(strcmp(argv[i],"-m")==0) {
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -n\n");
				err = -1;
				return;
			}
			int r = string2int(argv[i],&mode);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				err = -2;
				return;
			}
			if(mode!=EASY&&mode!=NORM&&mode!=HARD) {
				printf("Error: Unknown mode input, valid mode choice: %d %d %d (from easy to hard)\n",EASY,NORM,HARD);
				err = -3;
				return;
			}
			printf("mode = %d\n",mode);
			code_type = _M;
		}
		else if(strcmp(argv[i],"-r")==0) {
			++i;
			if(i>=argc) {
				printf("Error: no input for argument -n\n");
				err = -1;
				return;
			}
			char* buf2=0;
			char* buf3=0;
			char* buf = strtok_s(argv[i],"~",&buf2);
			int r = string2int(buf,&lower);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				err = -2;
				return;
			}
			buf2 = strtok_s(buf2,"~",&buf3);
			r = string2int(buf2,&upper);
			if(r<0) {
				printf("Error: could not parse input value: %s (error code %d)\n",argv[i],r);
				err = -2;
				return;
			}
			if(lower>upper) {
				printf("Error: lower %d is larger than upper %d.\n",lower,upper);
				err = -3;
				return;
			}
			if(lower<RMIN||lower>RMAX||upper<RMIN||upper>RMAX) {
				printf("Error: input range is out of range: %d~%d(valid range: %d~%d)\n",lower,upper,RMIN,RMAX);
				err = -3;
				return;
			}
			printf("lower = %d, upper = %d\n",lower,upper);
			code_type = _R;
		}
		else if(strcmp(argv[i],"-u")==0) {
			unique = true;
			printf("unique = %d\n",unique);
		}
		else {
			printf("Error: unknown argument: %s\n",argv[i]);
			err = -3;
			return;
		}
	}
	err = 0;
}

int main(int argc,char* argv[])
{
	int t0 = clock();
	int N=0;
	char* path=NULL;
	Parser parser(argc,argv);
	Core core;
	N = parser.N;
	path = parser.path;
	//N = 1000000;
	if(parser.code_type==parser._C) {
		int *result = new int[N*81];
		int(*hh)[81] = (int(*)[81])result;
		core.generate(N,hh);
		core.write_sudoku(N,result,"sudoku.txt");
		delete[] result;
	}
	else if(parser.code_type==parser._S) {
		int *puzzle = 0;
		int n;
		n = core.read_sudoku(&puzzle,path);
		if(n<0) {
			switch(n) {
				case Core::FILE_OPEN_ERROR:
					printf("Error: file %s could not open\n",path);
					goto error;
					break;
				case Core::FILE_READ_ERROR:
					printf("Error: file %s write error\n",path);
					goto error;
					break;
				case Core::MEMORY_ALLOC_ERROR:
					printf("Error: not enough memory!\n");
					goto error;
					break;
				default:
					printf("Unknown error detected.\n");
					goto error;
			}
		}
		int *solution = new int[n*81];
		core.solve(n,puzzle,solution);
		int r = core.write_sudoku(n,solution,"sudoku.txt");
		if(r<0) {
			switch(r) {
				case Core::FILE_OPEN_ERROR:
					printf("Error: file %s could not open\n","sudoku.txt");
					goto error;
					break;
				case Core::FILE_WRITE_ERROR:
					printf("Error: file %s write error\n","sudoku.txt");
					goto error;
					break;
				case Core::MEMORY_ALLOC_ERROR:
					printf("Error: not enough memory!\n");
					goto error;
					break;
				default:
					printf("Unknown error detected.\n");
					goto error;
			}
		}
		delete puzzle;
		delete[] solution;
	}
	else if(parser.code_type==parser._M) {
		int *result = new int[N*81];
		int(*hh)[81] = (int(*)[81])result;
		core.generate(N,parser.mode,hh);
		int r = core.write_sudoku(N,result,"sudoku.txt");
		if(r<0) {
			switch(r) {
				case Core::FILE_OPEN_ERROR:
					printf("Error: file %s could not open\n","sudoku.txt");
					goto error;
					break;
				case Core::FILE_WRITE_ERROR:
					printf("Error: file %s write error\n","sudoku.txt");
					goto error;
					break;
				case Core::MEMORY_ALLOC_ERROR:
					printf("Error: not enough memory!\n");
					goto error;
					break;
				default:
					printf("Unknown error detected.\n");
					goto error;
			}
		}
		delete[] result;
	}
	else if(parser.code_type==parser._R) {
		int *result = new int[N*81];
		int (*hh)[81] = (int(*)[81])result;
		core.generate(N,parser.lower,parser.upper,parser.unique,hh);
		int r = core.write_sudoku(N,result,"sudoku.txt");
		if(r<0) {
			switch(r) {
				case Core::FILE_OPEN_ERROR:
					printf("Error: file %s could not open\n","sudoku.txt");
					goto error;
					break;
				case Core::FILE_WRITE_ERROR:
					printf("Error: file %s write error\n","sudoku.txt");
					goto error;
					break;
				case Core::MEMORY_ALLOC_ERROR:
					printf("Error: not enough memory!\n");
					goto error;
					break;
				default:
					printf("Unknown error detected.\n");
					goto error;
			}
		}
		delete[] result;
	}
	int t1 = clock();
	printf("Clock Time: %d\n",t1-t0);
	return 0;
error:
	return -1;
}

