#pragma once
#include "time.h"
#include "windows.h"
#include "stdio.h"
#include <iostream>
#include <map>
#include <string>
#include <new>
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
	bool DLX_solve(int puzzle[],int solution[]);
	void solve(int number,int *puzzle,int *solution);
	bool isUniqueSolve(int puzzle[]);
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
	static const int FILE_OPEN_ERROR = -2;
	static const int FILE_READ_ERROR = -1;
	static const int FILE_WRITE_ERROR= -1;
	static const int SUDOKU_NUM_ERROR= -1;
	static const int MEMORY_ALLOC_ERROR = -3;
	
	private:
	bool __generate_unique(int num,int maxNum,int index,int result[]);
};

typedef struct DLXNode_st {
	int data;
	int pos;
	int colNum;
	int numRows;
	struct DLXNode_st *left,*right,*top,*bottom;
}DLXNode;

typedef DLXNode ColumnNode;

class NodePool {
	public:
	NodePool() {
		allocatedNum = 0;
		freeNum = 729*4+324;
		pool = new DLXNode[freeNum];
	}
	DLXNode* allocNode() {
		if(freeNum==0) return 0;
		DLXNode* res = &pool[allocatedNum];
		allocatedNum++;
		freeNum--;
		return res;
	}
	void deallocAll() {
		freeNum=729*4+324;
		allocatedNum = 0;
	}
	~NodePool() { delete[] pool; }
	private:
	DLXNode* pool;
	int allocatedNum;
	int freeNum;
};

class DLXSolver 
{
	public:
	DLXSolver(int puzzle[],int num);
	void solve(int result[][81]);
	void extract(int solution[]);
	int getSolvedNum() { return solvedNum; }
	~DLXSolver();

	private:
	int targetNum; // number of solutions aimed to found
	int solvedNum;
	bool *extracted;
	int top;
	int __solution[81];
	DLXNode *head;
	ColumnNode *curCol;
	ColumnNode* colHead;//[324*729];
	int __nodeNum = 324;
	void cover(DLXNode *node);
	void uncover(DLXNode *node);
	inline void addRow(int i,int x);
	inline void deleteNode(DLXNode* node);
	ColumnNode* getColumn(DLXNode *node);
	ColumnNode* chooseNextColumn();
	const int COL_HEADER = -1;
	int count = 0;
};

inline void DLXSolver::deleteNode(DLXNode* node) {
	if(node==0) return;
	printf("Deleting Node %d %d %d\n",node->colNum,node->data,node->pos);
	node->bottom->top = node->top;
	node->top->bottom = node->bottom;
	node->left->right = node->right;
	node->right->left = node->left;
	delete node;
	count--;
	node = 0;
	printf("finish\n");
}

inline void DLXSolver::addRow(int i,int x) {
	int r = i/9;
	int c = i%9;
	int b = 3*(r/3)+(c/3);
	int ik;
	DLXNode *x1,*x2,*x3,*x4;
	// i-th cell is filled
	try {
		x1 = &colHead[__nodeNum];//new DLXNode();
		//colHead[__nodeNum] = x1;
		__nodeNum++;
		++count;
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc: %s\n",e.what());
		exit(1);
	}
	x1->data = x;
	x1->pos = i;
	
	x1->colNum = i;
	
	x1->bottom = colHead[i].bottom;
	x1->top = &colHead[i];
	colHead[i].bottom->top = x1;
	colHead[i].bottom = x1;
	++(colHead[i].numRows);
	// r-th row is filled with x
	try {
		x2 = &colHead[__nodeNum];//new DLXNode();
		//colHead[__nodeNum] = x2;
		__nodeNum++;
		++count;
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc: %s\n",e.what());
		exit(1);
	}
	ik = 81+r*9+x-1;
	x2->data = x;
	x2->pos = i;
	
	x2->colNum = ik;
	
	x2->bottom = colHead[ik].bottom;
	x2->top = &colHead[ik];
	colHead[ik].bottom->top = x2;
	colHead[ik].bottom = x2;
	x2->left = x1;
	x1->right= x2;
	++(colHead[ik].numRows);
	// c-th column is filled with x
	try {
		x3 = &colHead[__nodeNum];//new DLXNode();
		//colHead[__nodeNum] = x3;
		__nodeNum++;
		++count;
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc: %s\n",e.what());
		exit(1);
	}
	ik = 162+c*9+x-1;
	x3->data = x;
	x3->pos = i;
	
	x3->colNum = ik;
	
	x3->bottom = colHead[ik].bottom;
	x3->top = &colHead[ik];
	colHead[ik].bottom->top = x3;
	colHead[ik].bottom = x3;
	x3->left = x2;
	x2->right= x3;
	++(colHead[ik].numRows);
	// b-th block is filled with x
	try {
		x4 = &colHead[__nodeNum];//new DLXNode();
		//colHead[__nodeNum] = x4;
		__nodeNum++;
		++count;
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc: %s\n",e.what());
		exit(1);
	}
	ik = 243+b*9+x-1;
	x4->data = x;
	x4->pos = i;
	
	x4->colNum = ik;
	
	x4->bottom = colHead[ik].bottom;
	x4->top = &colHead[ik];
	colHead[ik].bottom->top = x4;
	colHead[ik].bottom = x4;
	++(colHead[ik].numRows);
	x4->left = x3;
	x3->right= x4;
	// link cycle
	x4->right = x1;
	x1->left = x4;
}

// define a problem for DLXSolver to solve
DLXSolver::DLXSolver(int puzzle[],int num) {
	//printf("initializing matrix...\n");
	try {
		this->extracted = new bool[num];
		this->colHead = new DLXNode[324+4*729];
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc extracted[%d]: %s\n",num, e.what());
		exit(1);
	}
	memset(extracted,0,num*sizeof(bool));
	this->top = 0;
	this->targetNum = num;
	this->solvedNum = 0;
	try{
		head = new ColumnNode();
		++count;
	}
	catch(const std::bad_alloc& e) {
		printf("Failed to alloc: %s\n",e.what());
		exit(1);
	}
	head->data = COL_HEADER;
	head->numRows = 0;
	head->top = head;
	head->bottom = head;
	// 324 = 81*4
	// append column 0
	colHead[0].data = COL_HEADER;
	
	colHead[0].colNum = 0;
	
	colHead[0].numRows = 0;
	colHead[0].left = head;
	head->right = &colHead[0];
	colHead[0].top = &colHead[0];
	colHead[0].bottom = &colHead[0];
	for(int i=1;i<323;++i) {
		// append column i
		colHead[i].data = COL_HEADER;

		colHead[i].colNum = i;

		colHead[i].numRows = 0;
		colHead[i-1].right = &colHead[i];
		colHead[i].left = &colHead[i-1];
		colHead[i].top = &colHead[i];
		colHead[i].bottom = &colHead[i];
	}
	// append final column 323
	colHead[323].data = COL_HEADER;

	colHead[323].colNum = 323;

	colHead[323].numRows = 0;
	colHead[323].right = head;
	colHead[323].left = &colHead[322];
	colHead[322].right= &colHead[323];
	head->left = &colHead[323];
	colHead[323].top = &colHead[323];
	colHead[323].bottom = &colHead[323];
	// add rows
	for(int i=0;i<81;++i) {
		if(puzzle[i]>0) {
			addRow(i,puzzle[i]);
		}
		else {
			for(int j=1;j<=9;++j) {
				addRow(i,j);
			}
		}
	}
	curCol = head;
	//printf("fin.\n");
}
// detroyer
DLXSolver::~DLXSolver() {
	delete[] colHead;
	delete head;
	delete extracted;
}
// extract solution from node queue.
void DLXSolver::extract(int solution[]) {
	memcpy(solution,__solution,81*sizeof(int));
}
// choose next column from current column
ColumnNode* DLXSolver::chooseNextColumn() {
	int min=10;
	int ik=0;
	for(DLXNode* col=head->right;col!=head;col=col->right) {
		//printf("%d : curColumn=%x, col=%x,head=%x,min=%d,numRow=%d, pos=%d, data=%d, next pos %d, data %d\n",col->colNum,curCol,col,head,min,col->numRows,col->bottom->pos,col->bottom->data,col->bottom->bottom->pos,col->bottom->bottom->data);
		if(col->numRows < min) {
			min = col->numRows;
			curCol = col;
		}
	}
	return curCol;
}
// solve
void DLXSolver::solve(int result[][81]) {
	if(head->right==head) {
		// find a solution
		++solvedNum;
		return;
	}
	else {
		ColumnNode *column = chooseNextColumn();
		cover(column);
		for(DLXNode *row=column->bottom;row!=column;row = row->bottom) {
			//__solution[top] = row;
			__solution[row->pos] = row->data;
			//++top;
			for(DLXNode* rNode = row->right;rNode!=row;rNode = rNode->right) {
				cover(rNode);
			}
			solve(result);
			if(solvedNum>0) {
				if(!extracted[solvedNum-1]) {
					//extract(result[solvedNum-1]);
					memcpy(result[solvedNum-1],__solution,81*sizeof(int));
					extracted[solvedNum-1]=true;
				}
				if(solvedNum>=targetNum) {
					break;
				}
			}
			//--top;
			for(DLXNode* lNode = row->left;lNode!=row;lNode = lNode->left) {
				uncover(lNode);
			}
		}
		uncover(column);
	}
}

ColumnNode* DLXSolver::getColumn(DLXNode *node) {
	while(node->data!=COL_HEADER) node = node->top;
	return node;
}

void DLXSolver::cover(DLXNode *node) {
	ColumnNode *column = getColumn(node);
	(column->left)->right = column->right;
	(column->right)->left = column->left;
	for(DLXNode* row = column->bottom;row!=column;row=row->bottom) {
		for(DLXNode* rNode = row->right;rNode!=row;rNode=rNode->right) {
			ColumnNode* x = getColumn(rNode);
			--(x->numRows);
			(rNode->top)->bottom = rNode->bottom;
			(rNode->bottom)->top = rNode->top;
		}
	}
}

void DLXSolver::uncover(DLXNode *node) {
	ColumnNode *column = getColumn(node);
	(column->left)->right = column;
	(column->right)->left = column;
	for(DLXNode* row = column->bottom;row!=column;row=row->bottom) {
		for(DLXNode* rNode = row->right;rNode!=row;rNode=rNode->right) {
			ColumnNode* x = getColumn(rNode);
			++x->numRows;
			(rNode->top)->bottom = rNode;
			(rNode->bottom)->top = rNode;
		}
	}
}

typedef std::map<std::string, int> hashMap;
// same is false
bool Core::check_same(int number,int *solution) {
	if(solution==0||number<=0) return false;
	hashMap hmap;
	char ph[82];
	for(int i=0;i<number;++i) {
		int* ptr = solution+i*81;
		memset(ph,0,82);
		for(int j=0;j<81;++j) {
			ph[j] = ptr[j]+'0';
		}
		ph[81] = '\0';
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
// valid is true
bool Core::check_valid(int *solution) {
	if(solution==0) return false;
	bool empty_value[9][9][3]={ 0 }; // (value, r/c/b number, row/col/block)
	for(int r=0;r<9;++r) {
		for(int c=0;c<9;++c) {
			int b = 3*(r/3)+(c/3);
			int i = r*9+c;
			int v = solution[i]-1;
			if(v>8||v<0) return false;
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
	//data = new int[size>81000000?81000000:size];
	data = (int*)malloc(sizeof(int)*size);
	if(data==0) {
		return MEMORY_ALLOC_ERROR;
	}
	//printf("allocate rdata: %d\n",size*sizeof(char));
	rdata= new char[size];
	//printf("reading file %s...\n",filename);
	r = fread(rdata,1,size,fp);
	if(r<0 || r!=size) {
		delete[] rdata;
		free(data);
		//delete data;
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
	delete[] rdata;
	fclose(fp);
	return n;
}

int Core::write_sudoku(int number,int *puzzle,const char* filename) {
	if(number<1) return FILE_WRITE_ERROR;
	char* buff = (char*)malloc(163*number+1);
	if(buff==0) {
		return MEMORY_ALLOC_ERROR;
	}
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
	int r;
	r = fwrite(buff,163,number,fp);
	if(r<0) {
		//delete buff;
		free(buff);
		return FILE_WRITE_ERROR; // Error
	}
	//printf("finish\n");
	fclose(fp);
	free(buff);
	//delete buff;
	return 0;
}

void Core::generate(int number,int mode,int result[][81]) {
	int i = 0;
	int free_degree = 0;
	int sudoku_num = 0;
	int num = 0;
	int the_time = 0;
	int hollow_num = 0;
	generate(number, result);
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
}

bool Core::__generate_unique(int num, int maxNum, int index, int result[]) {
	if(index>=81||maxNum<=num) return maxNum<=num;
	int x = result[index];
	result[index] = 0;
	if(isUniqueSolve(result)) {
		return __generate_unique(num+1,maxNum,index+1,result);
	}
	else {
		result[index] = x;
		return __generate_unique(num,maxNum,index+1,result);
	}
}

void Core::generate(int number,int lower,int upper,bool unique,int result[][81]) {
	if(unique) {
		generate(number,result);
		srand((unsigned int)time(0));
#pragma omp parallel for
		for(int n=0;n<number;++n) {
			int num = lower;//rand()%(upper-lower+1)+lower;
			__generate_unique(0,num,0,result[n]);
		}
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
	return DLX_solve(puzzle,solution);
	/* Naive solver
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
	return true;*/
}
void Core::solve(int number,int *puzzle,int *solution) {
	#pragma omp parallel for
	for(int i=0;i<number;++i) {
		//if(i%1000==0)printf("Solving %d...\n",i);
		//solve(puzzle+i*81,solution+i*81);
		DLX_solve(puzzle+i*81,solution+i*81);
	}
}

bool Core::DLX_solve(int puzzle[],int solution[]) {
	DLXSolver solver(puzzle,1);
	solver.solve((int(*)[81])solution);
	if(solver.getSolvedNum()>0) return true;
	//printf("No answer\n");
	return false;
}

bool Core::isUniqueSolve(int puzzle[]) {
	int *solution = new int[162];
	memcpy(solution,puzzle,81*sizeof(int));
	DLXSolver solver(puzzle,2);
	solver.solve((int(*)[81])solution);
	delete[] solution;
	return solver.getSolvedNum()==1;
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