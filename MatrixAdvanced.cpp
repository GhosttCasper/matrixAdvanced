#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <stdlib.h>

struct matrixInt {
	int** matrix=nullptr;
	int sizeLines = 0;
	int sizeColumns = 0;
};

struct matrixDouble {
	double** matrix = nullptr;
	int sizeLines = 0;
	int sizeColumns = 0;
};

void input(matrixInt* first, matrixInt* second);
void consoleInput(matrixInt* first, matrixInt* second);
void fileInput(matrixInt* first, matrixInt* second);
bool isSaveResultInFile();
void outputInFile(matrixInt sumMatrix, matrixInt multiplicationMatrix, int determinantFirst, matrixDouble inverseMatrix);

void initMatrix(matrixInt* matrix, int sizeLines, int sizeColumns);
void initMatrix(matrixDouble* matrix, int sizeLines, int sizeColumns);
void printMatrix(matrixInt matrix);
void printMatrix(matrixDouble matrix);
matrixInt sumMatrix(matrixInt first, matrixInt second);
bool areSizesEqual(matrixInt first, matrixInt second);
matrixInt multMatrix(matrixInt first, matrixInt second);
bool areMultipliedMatrices(matrixInt first, matrixInt second);
double determinantTwoByTwoMatrix(int** matrix);
bool isSquareMatrix(matrixInt matrix);
matrixDouble invMatrix(matrixInt matrix);
bool isDegenerateMatrix(matrixInt matrix);
matrixDouble transposeMatrix(matrixDouble matrix);
matrixDouble getMatrixAlgebraicComplements(matrixInt matrix);
void freeMatrix(matrixInt* matrix);
void freeMatrix(matrixDouble* matrix);
int getRandRangeInt(int min, int max);
void inputSizeMatrix(matrixInt* matrix);
void fillRandomNumbers(matrixInt* matrix);
int** getMinorMatrix(int** matrix, int row, int column, int n);
double determinantMatrix(int** matrix, int size);

/*
https://yukhym.com/ru/matritsy-i-opredeliteli/obratnaya-matritsa-primery-vychisleniya.html пример определителя, обратной матрицы 4 на 4 из файла
https://planetcalc.ru/1104/ определитель
https://planetcalc.ru/1239/ обратная матрица
*/

int main()
{
	SetConsoleCP(1251);			// Ввод с консоли в кодировке 1251
	SetConsoleOutputCP(1251);	// Установка кодовой страницы win-cp 1251 в поток вывода

	struct matrixInt first, second;
	input(&first, &second);

	struct matrixInt sum;
	if (areSizesEqual(first, second))
	{
		initMatrix(&sum, first.sizeLines, first.sizeColumns);
		sum = sumMatrix(first, second);
		puts("Сумма матриц:");
		printMatrix(sum);
	}

	struct matrixInt multiplicationMatrix;
	if (areMultipliedMatrices(first, second))
	{
		initMatrix(&multiplicationMatrix, first.sizeLines, second.sizeColumns);
		multiplicationMatrix = multMatrix(first, second);
		puts("Произведение матриц:");
		printMatrix(multiplicationMatrix);
	}

	double determinantFirst = 0;;
	if (isSquareMatrix(first))
	{
		determinantFirst = determinantMatrix(first.matrix, first.sizeLines);
		printf("Определитель первой матрицы: %.0lf\n", determinantFirst);
	}

	struct matrixDouble inverseMatrix;
	if (!isDegenerateMatrix(first) && isSquareMatrix(first))
	{
		initMatrix(&inverseMatrix, first.sizeLines, first.sizeColumns);
		inverseMatrix = invMatrix(first);
		puts("Обратная первая матрица:");
		printMatrix(inverseMatrix);
	}

	if (isSaveResultInFile())
		outputInFile(sum, multiplicationMatrix, determinantFirst, inverseMatrix);

	return 0;
}


void input(matrixInt* first, matrixInt* second)
{
	printf("Введите способ ввода (0 - консоль, 1 - файл): ");
	int inputWay = 0;
	scanf_s("%d", &inputWay);
	if (inputWay == 1)
		fileInput(first, second);
	else
		consoleInput(first, second);

	puts("Первая матрица:");
	printMatrix(*first);
	puts("Вторая матрица:");
	printMatrix(*second);
}

void consoleInput(matrixInt* first, matrixInt* second)
{
	puts("Введите размер первой матрицы.");
	inputSizeMatrix(first);
	initMatrix(first, first->sizeLines, first->sizeColumns);
	fillRandomNumbers(first);

	puts("Введите размер второй матрицы: ");
	inputSizeMatrix(second);
	initMatrix(second, second->sizeLines, second->sizeColumns);
	fillRandomNumbers(second);
}

void inputSizeMatrix(matrixInt* matrix)
{
	printf("Введите количество строк: ");
	scanf_s("%d", &matrix->sizeLines);
	printf("Введите количество столбцов: ");
	scanf_s("%d", &matrix->sizeColumns);
}

void fillRandomNumbers(matrixInt* matrix)
{
	srand(time(NULL));
	for (int i = 0; i < matrix->sizeLines; i++)
		for (int j = 0; j < matrix->sizeColumns; j++)
			matrix->matrix[i][j] = getRandRangeInt(-10, 10);
}

int getRandRangeInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void fileInput(matrixInt* first, matrixInt* second)
{
	char nameFile[50];
	printf("Введите название файла: ");
	scanf_s("%s", nameFile, sizeof(nameFile));

	FILE* fileOnlyRead;
	errno_t err = fopen_s(&fileOnlyRead, nameFile, "rb");
	if (err == 0)
		printf("Файл %s открылся\n", nameFile);
	else
	{
		printf("Файл %s не открылся\n", nameFile);
		exit(0);
	}

	fscanf_s(fileOnlyRead, "%d", &first->sizeLines);
	fscanf_s(fileOnlyRead, "%d", &first->sizeColumns);
	initMatrix(first, first->sizeLines, first->sizeColumns);

	for (int i = 0; i < first->sizeLines; i++)
		for (int j = 0; j < first->sizeColumns; j++)
			fscanf_s(fileOnlyRead, "%d", &first->matrix[i][j]);

	fscanf_s(fileOnlyRead, "%d", &second->sizeLines);
	fscanf_s(fileOnlyRead, "%d", &second->sizeColumns);
	initMatrix(second, second->sizeLines, second->sizeColumns);

	for (int i = 0; i < second->sizeLines; i++)
		for (int j = 0; j < second->sizeColumns; j++)
			fscanf_s(fileOnlyRead, "%d", &second->matrix[i][j]);
}

void initMatrix(matrixDouble* matrix, int sizeLines, int sizeColumns)
{
	matrix->sizeLines = sizeLines;
	matrix->sizeColumns = sizeColumns;
	matrix->matrix = (double**)calloc(sizeLines, sizeof(double*));
	for (int i = 0; i < sizeLines; i++) {
		matrix->matrix[i] = (double*)calloc(sizeColumns, sizeof(double));
	}
}

void initMatrix(matrixInt* matrix, int sizeLines, int sizeColumns)
{
	matrix->sizeLines = sizeLines;
	matrix->sizeColumns = sizeColumns;
	matrix->matrix = (int**)calloc(sizeLines, sizeof(int*));
	for (int i = 0; i < sizeLines; i++) {
		matrix->matrix[i] = (int*)calloc(sizeColumns, sizeof(int));
	}
}

void printMatrix(matrixInt matrix)
{
	for (int i = 0; i < matrix.sizeLines; i++)
	{
		for (int j = 0; j < matrix.sizeColumns; j++)
			printf("%d ", matrix.matrix[i][j]);
		printf("\n");
	}
}

void printMatrix(matrixDouble matrix)
{
	for (int i = 0; i < matrix.sizeLines; i++)
	{
		for (int j = 0; j < matrix.sizeColumns; j++)
			printf("%.2lf ", matrix.matrix[i][j]);
		printf("\n");
	}
}

bool isSaveResultInFile()
{
	printf("Сохранить результаты в файле? (1 - да, 0 - нет): ");
	int isSave = 0;
	scanf_s("%d", &isSave);
	if (isSave == 1)
		return true;
	return false;
}

void outputInFile(matrixInt sumMatrix, matrixInt multiplicationMatrix, int determinantFirst, matrixDouble inverseMatrix)
{
	char nameFile[50];
	printf("Введите название файла: ");
	scanf_s("%s", nameFile, sizeof(nameFile));

	FILE* fileWrite;
	errno_t err = fopen_s(&fileWrite, nameFile, "w+");
	if (err == 0)
		printf("Файл %s открылся\n", nameFile);
	else
		printf("Файл %s не открылся\n", nameFile);

	for (int i = 0; i < sumMatrix.sizeLines; i++)
	{
		for (int j = 0; j < sumMatrix.sizeColumns; j++)
			fprintf(fileWrite, "%d ", sumMatrix.matrix[i][j]);
		fprintf(fileWrite, "\n");
	}

	for (int i = 0; i < multiplicationMatrix.sizeLines; i++)
	{
		for (int j = 0; j < multiplicationMatrix.sizeColumns; j++)
			fprintf(fileWrite, "%d ", multiplicationMatrix.matrix[i][j]);
		fprintf(fileWrite, "\n");
	}

	fprintf(fileWrite, "%d\n", determinantFirst);


	for (int i = 0; i < inverseMatrix.sizeLines; i++)
	{
		for (int j = 0; j < inverseMatrix.sizeColumns; j++)
			fprintf(fileWrite, "%.1lf ", inverseMatrix.matrix[i][j]);
		fprintf(fileWrite, "\n");
	}
}

matrixInt sumMatrix(matrixInt first, matrixInt second)
{
	struct matrixInt sumMatrices;
	initMatrix(&sumMatrices, first.sizeLines, first.sizeColumns);
	for (int i = 0; i < first.sizeLines; i++)
		for (int j = 0; j < first.sizeColumns; j++)
			sumMatrices.matrix[i][j] = first.matrix[i][j] + second.matrix[i][j];

	return sumMatrices;
}

bool areSizesEqual(matrixInt first, matrixInt second)
{
	if (first.sizeColumns == second.sizeColumns && first.sizeLines == second.sizeLines)
		return true;
	return false;
}

matrixInt multMatrix(matrixInt first, matrixInt second)
{
	struct matrixInt multiplication;
	initMatrix(&multiplication, first.sizeLines, second.sizeColumns);
	for (int i = 0; i < first.sizeLines; i++)
	{
		for (int j = 0; j < second.sizeColumns; j++)
		{
			multiplication.matrix[i][j] = 0;
			for (int k = 0; k < first.sizeColumns; k++)
				multiplication.matrix[i][j] += first.matrix[i][k] * second.matrix[k][j];
		}
	}
	return multiplication;
}

bool areMultipliedMatrices(matrixInt first, matrixInt second)
{
	if (first.sizeColumns == second.sizeLines)
		return true;
	return false;
}

double determinantMatrix(int** matrix, int size)
{
	if (size == 1)
		return matrix[0][0];
	if (size == 2)
		return determinantTwoByTwoMatrix(matrix);

	double determinant = 0;
	__int64 sign = 1;
	int curSize = size - 1;

	for (int j = 0; j < size; j++)
	{
		determinant += sign * matrix[0][j] * determinantMatrix(getMinorMatrix(matrix, 0, j, curSize), curSize);
		sign = -sign;
	}
	return determinant;
}

double determinantTwoByTwoMatrix(int** matrix)
{
	return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int** getMinorMatrix(int** matrix, int row, int column, int n)
{
	int** minorMatrix = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i < n; i++) {
		minorMatrix[i] = (int*)calloc(n, sizeof(int));
	}

	for (int i = 0, k = 0; i < n; i++, k++)
		for (int j = 0, l = 0; j < n; j++, l++)
		{
			if (k == row)
				k++;
			if (l == column)
				l++;
			minorMatrix[i][j] = matrix[k][l];
		}

	return minorMatrix;
}

bool isSquareMatrix(matrixInt matrix)
{
	if (matrix.sizeColumns == matrix.sizeLines)
		return true;
	return false;
}

matrixDouble invMatrix(matrixInt matrix)
{
	matrixDouble  inverseMatrix;
	initMatrix(&inverseMatrix, matrix.sizeLines, matrix.sizeLines);
	inverseMatrix = getMatrixAlgebraicComplements(matrix);
	inverseMatrix = transposeMatrix(inverseMatrix);

	double det = determinantMatrix(matrix.matrix, matrix.sizeLines);

	for (int i = 0; i < matrix.sizeLines; i++)
		for (int j = 0; j < matrix.sizeLines; j++)
			inverseMatrix.matrix[i][j] /= det;

	return inverseMatrix;
}

bool isDegenerateMatrix(matrixInt matrix)
{
	if (determinantMatrix(matrix.matrix, matrix.sizeLines) == 0)
	{
		puts("Матрица вырожденая. Обратной матрицы не существует.");
		return true;
	}
	return false;
}

matrixDouble transposeMatrix(matrixDouble matrix)
{
	for (int i = 0; i < matrix.sizeLines; i++)
		for (int j = i + 1; j < matrix.sizeLines; j++)
		{
			double temp = matrix.matrix[i][j];
			matrix.matrix[i][j] = matrix.matrix[j][i];
			matrix.matrix[j][i] = temp;
		}
	return matrix;
}

matrixDouble getMatrixAlgebraicComplements(matrixInt matrix)
{
	matrixDouble matrixAC;
	initMatrix(&matrixAC, matrix.sizeLines, matrix.sizeLines);
	for (int i = 0; i < matrix.sizeLines; i++)
		for (int j = 0; j < matrix.sizeLines; j++)
		{
			int sign = ((i + j) % 2 == 0) ? 1 : -1;
			int curSize = matrix.sizeLines - 1;
			matrixAC.matrix[i][j] = sign * determinantMatrix(getMinorMatrix(matrix.matrix, i, j, curSize), curSize);
		}

	return matrixAC;
}

void freeMatrix(matrixInt* matrix)
{
	free(&matrix);
}

void freeMatrix(matrixDouble* matrix)
{
	free(&matrix);
}