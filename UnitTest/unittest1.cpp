#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/SudokuLib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestRead)
		{
			Core core;
			int refRes[]={
				9, 0, 8, 0, 6, 0, 1, 2, 4,
				2, 3, 7, 4, 5, 1, 9, 6, 8,
				1, 4, 6, 0, 2, 0, 3, 5, 7,
				0, 1, 2, 0, 7, 0, 5, 9, 3,
				0, 7, 3, 0, 1, 0, 4, 8, 2,
				4, 8, 0, 0, 0, 5, 6, 0, 1,
				7, 0, 4, 5, 9, 0, 8, 1, 6,
				8, 9, 0, 7, 4, 6, 2, 0, 0,
				3, 0, 5, 0, 8, 0, 7, 0, 9,

				8, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 3, 6, 0, 0, 0, 0, 0,
				0, 7, 0, 0, 9, 0, 2, 0, 0,
				0, 5, 0, 0, 0, 7, 0, 0, 0,
				0, 0, 0, 0, 4, 5, 7, 0, 0,
				0, 0, 0, 1, 0, 0, 0, 3, 0,
				0, 0, 1, 0, 0, 0, 0, 6, 8,
				0, 0, 8, 5, 0, 0, 0, 1, 0,
				0, 9, 0, 0, 0, 0, 4, 0, 0,
			};
			int* result;
			core.read_sudoku(&result,"sudokuexp.txt");
			for(int i=0;i<162;++i)
				Assert::AreEqual(refRes[i],result[i]);
			delete[] result;
		}
		TEST_METHOD(TestWrite)
		{
			Core core;
			int refRes[]={
				9, 0, 8, 0, 6, 0, 1, 2, 4,
				2, 3, 7, 4, 5, 1, 9, 6, 8,
				1, 4, 6, 0, 2, 0, 3, 5, 7,
				0, 1, 2, 0, 7, 0, 5, 9, 3,
				0, 7, 3, 0, 1, 0, 4, 8, 2,
				4, 8, 0, 0, 0, 5, 6, 0, 1,
				7, 0, 4, 5, 9, 0, 8, 1, 6,
				8, 9, 0, 7, 4, 6, 2, 0, 0,
				3, 0, 5, 0, 8, 0, 7, 0, 9,

				8, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 3, 6, 0, 0, 0, 0, 0,
				0, 7, 0, 0, 9, 0, 2, 0, 0,
				0, 5, 0, 0, 0, 7, 0, 0, 0,
				0, 0, 0, 0, 4, 5, 7, 0, 0,
				0, 0, 0, 1, 0, 0, 0, 3, 0,
				0, 0, 1, 0, 0, 0, 0, 6, 8,
				0, 0, 8, 5, 0, 0, 0, 1, 0,
				0, 9, 0, 0, 0, 0, 4, 0, 0,
			};
			int* result;
			core.write_sudoku(2,refRes,"sudoku.txt");
			core.read_sudoku(&result,"sudoku.txt");
			for(int i=0;i<162;++i)
				Assert::AreEqual(refRes[i],result[i]);
			delete[] result;
		}
		// Generation
		TEST_METHOD(TestGen10_3)
		{
			Core core;
			const int N = 1000; // MAXIMUM
			//int result[N][81];
			int* result = new int[N*81];
			core.generate(N,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

		TEST_METHOD(TestGen1)
		{
			Core core;
			const int N = 1; // MAXIMUM
			int result[N][81];
			core.generate(N,result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
		}

		TEST_METHOD(TestGen3)
		{
			Core core;
			const int N = 3; // MAXIMUM
			int result[N][81];
			core.generate(N,result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
		}

		TEST_METHOD(TestGen54203)
		{
			Core core;
			const int N = 54203; // MAXIMUM
			int* result = new int[N*81];
			//int result[N][81];
			core.generate(N,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}
		// Solver
		TEST_METHOD(TestSolve20)
		{
			Core core;
			const int N = 1; // MAXIMUM
			int* result = new int[N*81];
			//int result[N][81];
			//int* result = (int*)malloc(sizeof(N*81));
			int* puzzle;
			core.read_sudoku(&puzzle,"sudoku20.txt");
			core.solve(N,puzzle,(int*)result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			//free(result);
			delete[] result;
			delete[] puzzle;
		}
		TEST_METHOD(TestSolve17_0)
		{
			Core core;
			const int N = 5; // MAXIMUM
			int* result = new int[N*81];
			//int result[N][81];
			int* puzzle;
			core.read_sudoku(&puzzle,"sudoku17_0.txt");
			core.solve(N,puzzle,(int*)result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
			delete[] puzzle;
		}
		TEST_METHOD(TestSolve17_1)
		{
			Core core;
			const int N = 7; // MAXIMUM
			//int result[N][81];
			int* result = new int[N*81];
			int* puzzle;
			core.read_sudoku(&puzzle,"sudoku17_1.txt");
			core.solve(N,puzzle,(int*)result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
			delete[] puzzle;
		}
		TEST_METHOD(TestSolveExp)
		{
			Core core;
			const int N = 2; // MAXIMUM
			int* result = new int[N*81];
			//int result[N][81];
			int* puzzle;
			core.read_sudoku(&puzzle,"sudokuexp.txt");
			core.solve(N,puzzle,(int*)result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
			delete[] puzzle;
		}

		TEST_METHOD(TestSolveFail)
		{
			Core core;
			const int N = 1; // MAXIMUM
			int puzzle[] ={
				9, 0, 8, 0, 6, 0, 1, 2, 4,
				2, 3, 7, 4, 5, 1, 9, 6, 8,
				1, 4, 6, 0, 2, 0, 3, 5, 7,
				0, 1, 2, 0, 7, 0, 5, 9, 3,
				0, 7, 3, 0, 1, 0, 4, 8, 2,
				4, 8, 0, 0, 0, 5, 6, 0, 1,
				7, 0, 4, 5, 9, 0, 8, 1, 6,
				8, 1, 0, 7, 4, 6, 2, 0, 0,
				3, 0, 5, 0, 8, 0, 7, 0, 9,
			};
			int result[81]={0};
			Assert::IsFalse(core.DLX_solve(puzzle,(int*)result));
		}

		TEST_METHOD(TestGenRU10_3)
		{
			Core core;
			const int N = 1000; // MAXIMUM
								//int result[N][81];
			int* result = new int[N*81];
			core.generate(N,55,55,true,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

		TEST_METHOD(TestGenR10_3)
		{
			Core core;
			const int N = 1000; // MAXIMUM
								//int result[N][81];
			int* result = new int[N*81];
			core.generate(N,55,55,false,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

		TEST_METHOD(TestGenModeEasy10_4)
		{
			Core core;
			const int N = 10000; // MAXIMUM
			int* result = new int[N*81];
			core.generate(N,1,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			core.solve(N,result,result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

		TEST_METHOD(TestGenModeNorm10_4)
		{
			Core core;
			const int N = 10000; // MAXIMUM
			int* result = new int[N*81];
			core.generate(N,2,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			core.solve(N,result,result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

		TEST_METHOD(TestGenModeHard10_4)
		{
			Core core;
			const int N = 10000; // MAXIMUM
			int* result = new int[N*81];
			core.generate(N,3,(int(*)[81])result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			core.solve(N,result,result);
			Assert::AreEqual(core.check_valid(N,(int*)result),0);
			Assert::IsTrue(core.check_same(N,(int*)result));
			delete[] result;
		}

	};
}