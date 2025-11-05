#include <math.h>
#include <stdio.h>
long double accuracy = 0.00001;
// приблизительно сравнивает два значения
short EqualAccurate(long double a, long double b) {
	if (a > b) return a - b < accuracy; else return b - a < accuracy;
	
}
// делит каждый элемент строки на первое в строке отличное от 0 число
long int NormalizeRow(long double* row, long int rowLength) {
	long double mn = 0;
	long int counter = 0;
	while (counter < rowLength && mn == 0) {
		if (!EqualAccurate(row[counter], 0)) mn = row[counter]; 
		counter += 1;
	}
	counter--;
	if (mn != 0){
		if (EqualAccurate(mn, 1)) { return counter; }
		row[counter] = 1;
		for (long int i = counter + 1; i < rowLength; i++) {
			row[i] /= mn;
		}
		return counter;
	}
	return -1;
}
// вычитает из строки другую строку
void DecreaseRow(long double* row, long double* minRow, long int rowLength) {
	for (long int i = 0; i < rowLength; i++) { 
		if (EqualAccurate(row[i], minRow[i])) row[i] = 0; 
		else row[i] -= minRow[i]; }
}
// выводит строку
void PrintRow(long double* row, long int rowLength) {
	for (long int i = 0; i < rowLength; i++) printf("%lf ", row[i]);
	printf("\n");
}
// выводит матрицу
void PrintTable(long double** table, long int n, long int m) {
	for (long int i = 0; i < n; i++) {
		for (long int j = 0; j < m; j++)
			printf("%lf ", table[i][j]); printf("\n");
	}
	printf("\n");
}
// меняет две строки местами
void SwapRows(long double** a, long double** b) {
	long double* c = *a; *a = *b; *b = c;
}
// меняет два столбца местами
void SwapColumns(long double** table, long int a, long int b, long int columnLength) {
	for (long int i = 0; i < columnLength; i++) { long double c = table[i][a]; table[i][a] = table[i][b]; table[i][b] = c; }
}
// возвращает номер строки, в которой на позиции i впервые встретилось отличное от 0 число (номер строки не менее minValue)
long int FindColumnIndex(long double** table, long int columnLength, long int columnNum, long int minValue) {
	for (long int i = minValue; i < columnLength; i++) {
		if (!EqualAccurate(table[i][columnNum], 0)) return i;
	}
	return -1;
}
// возвращает номер столбца, в котором среди строк от minValue до maxValue впервые встретилось отличное от 0 значение столбца (менят ind - номер строки)
long int FindFirstColumnIndex(long double** table, long int columnLength, long int rowStartNum, long int minValue, long int maxValue, long int ind) {
	for (long int i = minValue; i < maxValue; i++) {
		long int temp = FindColumnIndex(table, columnLength, i, rowStartNum);
		if (temp > 0) { ind = temp; return i; }
	}
	return -1;
}
long int FindRowIndex(long double* row, long int rowLength) {
	for (long int i = 0; i < rowLength; i++) {
		if (!EqualAccurate(row[i], 0)) return i;
	}
	return -1;
}
short CheckNoSolution(long double* row, long int rowLength) {
	for (long int i = 0; i < rowLength - 1; i++) {
		if (!EqualAccurate(row[i], 0)) return 0;
	}
	if (EqualAccurate(row[rowLength - 1], 0)) return -1;
	return 1;
}
short CheckNoSolutionTable(long double** table, long int n, long int m) {
	long int cnt = 0;
	for (long int i = 0; i < n; i++) {
		switch (CheckNoSolution(table[i], m))
		{
		case -1: cnt++; break;
		case 1: return 1; break;
		default: break;
		}
	}
	if (cnt == n) return -1;
	return 0;
}
long int CheckNonZeroRow(long double* row, long int rowLength) {
	long int cnt = 0;
	for (long int i = 0; i < rowLength; i++) {
		if (!EqualAccurate(row[i], 0)) cnt++;
	}
	return cnt;
}
// 1 2 3 4 5 6 7 8 9 10 11 12
// 2 4 0 1 2 3 0 4 5 6
// 2 4 1 2 3 4 2 4 6 9
// 2 4 0 0 0 0 0 0 0 0
// 2 4 1 0 2 3 4 0 5 6
// 5 6 2 1 1 1 1 14 1 2 1 1 1 13 1 1 2 1 1 12 1 1 1 2 1 11 1 1 1 1 2 10
// 10 11 10 1 1 1 1 1 1 1 1 1 19 1 10 1 1 1 1 1 1 1 1 19 1 1 10 1 1 1 1 1 1 1 19 1 1 1 10 1 1 1 1 1 1 19 1 1 1 1 10 1 1 1 1 1 19 1 1 1 1 1 10 1 1 1 1 19 1 1 1 1 1 1 10 1 1 1 19 1 1 1 1 1 1 1 10 1 1 19 1 1 1 1 1 1 1 1 10 1 19 1 1 1 1 1 1 1 1 1 10 19

// 9 11 10 1 1 1 1 1 1 1 1 1 19 1 10 1 1 1 1 1 1 1 1 19 1 1 10 1 1 1 1 1 1 1 19 1 1 1 10 1 1 1 1 1 1 19 1 1 1 1 10 1 1 1 1 1 19 1 1 1 1 1 10 1 1 1 1 19 1 1 1 1 1 1 10 1 1 1 19 1 1 1 1 1 1 1 10 1 1 19 1 1 1 1 1 1 1 1 10 1 19

// 2 3 1 2 3 2 4 6

// упрощает систему (система состоит из диагональной матрицы с вкраплениями нулевых столбцов или не имеет решений)
int SimplifySystem(long double** table, long int n, long int m, short hideLog) {
	// -1 - матрица состоит из нулей, 1 - матрица несовместна
	switch (CheckNoSolutionTable(table, n, m))
	{
	case -1: return -2; break;
	case 1: return -1; break;
	default: break;
	}
	if(!hideLog) printf("Solution exists\n");
	long int leftBorder = 0, bottomBorder = n;
	
	for (long int i = 0; i < bottomBorder; i++) {
		long int offset = 0, ind = -1, subInd = -1;
		// ищем строку с ненулевым элементом на позиции i

		//ind = FindFirstColumnIndex(table, bottomBorder, i, i, m - 1, &subInd);
		if (!hideLog)printf("Row %ld, leftBorder = %ld, bottomBorder = %ld\n", i, leftBorder, bottomBorder);
		for (long int j = i; j < m - 1; j++) {
			
			long int temp = FindColumnIndex(table, bottomBorder, j, i);
			if (!hideLog)printf("Checking for columnIndex %ld, temp = %ld\n", j, temp);
			if (temp > -1 && ind < 0) { ind = temp; subInd = j; }
		}
		// subind - номер столбца, ind - номер строки
		if (!hideLog)printf("ind = %ld, subInd = %ld\n", ind, subInd);
		

		if (ind != i) {
			SwapRows(&table[i], &table[ind]);
			if (!hideLog)printf("Swapped rows %ld and %ld\n", i, ind);
		}
		// нормализуем строку i
		if (!hideLog)printf("Normalizing row %ld\n", i);
		NormalizeRow(table[i], m);
		if (!hideLog)PrintRow(table[i], m);
		// нормализуем все следующие строки, если индекс нормы совпадает с i, то вычитаем из новой строки текущую
		for (long int j = i + 1; j < bottomBorder; j++) {
			if (!hideLog)printf("Normalizing row %ld before bottom\n", j);
			long int normalizedElem = NormalizeRow(table[j], m);
			if (!hideLog)printf("normElem = %ld\n", normalizedElem);
			if (!hideLog)PrintRow(table[j], m);
			if (normalizedElem == subInd) {
				if (!hideLog)printf("Decreasing row %ld by row %ld: ", j, i);
				if (!hideLog)PrintRow(table[i], m);
				DecreaseRow(table[j], table[i], m);
				if (!hideLog)PrintRow(table[j], m);
				switch (CheckNoSolution(table[j], m)) {
				case 1: return -1; break;
				case -1: SwapRows(&table[bottomBorder], &table[j]); bottomBorder--; break;
				}
			}
		}
		// получаем единственную строку с ненулевым элементом на i позиции (при этом все нулевые строки оказываются в самом низу матрицы)
		if (!hideLog)printf("After turn %d\n", i+1);
		if (!hideLog)PrintTable(table, bottomBorder, m);
		if (!hideLog)printf("\n");
	}
	if (!hideLog)printf("\n");
	PrintTable(table, bottomBorder, m);
	return bottomBorder;
}
short InLIntArray(long int* array, long int arrayLength, long int value) {
	for (long int i = 0; i < arrayLength; i++) {
		if (value == array[i]) return 1;
	}
	return 0;
}
//int PlaceSolutions() {
//
//}
