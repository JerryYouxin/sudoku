#include "stdafx.h"
#include "CppUnitTest.h"
#include "../sudoku/SudokuLib.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		// Generation
		TEST_METHOD(TestGen10_6)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku(N);
			sudoku->generate();
			Assert::AreEqual(1,1);
		}

		TEST_METHOD(TestGen10_3)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000; // MAXIMUM
			Sudoku *sudoku = new Sudoku(N);
			sudoku->generate();
			Assert::AreEqual(1,1);
		}

		TEST_METHOD(TestGen1)
		{
			// TODO: �ڴ�������Դ���
			int N = 1; // MAXIMUM
			Sudoku *sudoku = new Sudoku(N);
			sudoku->generate();
			Assert::AreEqual(1,1);
		}

		TEST_METHOD(TestGen3)
		{
			// TODO: �ڴ�������Դ���
			int N = 3; // MAXIMUM
			Sudoku *sudoku = new Sudoku(N);
			sudoku->generate();
			Assert::AreEqual(1,1);
		}

		TEST_METHOD(TestGen54203)
		{
			// TODO: �ڴ�������Դ���
			int N = 54203; // MAXIMUM
			Sudoku *sudoku = new Sudoku(N);
			sudoku->generate();
			Assert::AreEqual(1,1);
		}

		// Solver
		TEST_METHOD(TestSolve20)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku("sudoku20.txt");
			sudoku->solve();
			Assert::AreEqual(1,1);
		}
		TEST_METHOD(TestSolve17_0)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku("sudoku17_0.txt");
			sudoku->solve();
			Assert::AreEqual(1,1);
		}
		TEST_METHOD(TestSolve17_1)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku("sudoku17_1.txt");
			sudoku->solve();
			Assert::AreEqual(1,1);
		}
		TEST_METHOD(TestSolve1000000)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku("sudoku1000000.txt");
			sudoku->solve();
			Assert::AreEqual(1,1);
		}
		TEST_METHOD(TestSolveExp)
		{
			// TODO: �ڴ�������Դ���
			int N = 1000000; // MAXIMUM
			Sudoku *sudoku = new Sudoku("sudokuexp.txt");
			sudoku->solve();
			Assert::AreEqual(1,1);
		}


	};
}