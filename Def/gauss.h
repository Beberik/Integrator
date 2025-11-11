#ifndef gaussSystem
#define gaussSystem
short EqualAccurate(long double a, long double b);
long int NormalizeRow(long double* row, long int rowLength);
void PrintRow(long double* row, long int rowLength);
void PrintTable(long double** table, long int n, long int m);
void SwapRows(long double** a, long double** b);
void SwapColumns(long double** table, long int a, long int b, long int rowLength);
void DecreaseRow(long double* row, long double* minRow, long int rowLength);
long int FindColumnIndex(long double** table, long int columnLength, long int columnNum, long int minValue);
short CheckNoSolution(long double* row, long int rowLength);
int SimplifySystem(long double** table, long int n, long int m, short hideLog, short hideOutput);
long int CheckNonZeroRow(long double* row, long int rowLength);
short InLIntArray(long int* array, long int arrayLength, long int value);
long int FindRowIndex(long double* row, long int rowLength);
void PrintAnswerRow(long double* row, long int rowLength);
void PlaceSolutions(long double** table, long int n, long int m);
void GenerateRandomTable(long double** table, long int n, long int m, long double lowerBound, long double upperBound, short hideOutput);
long double accuracy;
#endif
