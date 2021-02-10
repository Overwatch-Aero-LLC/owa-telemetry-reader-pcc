/*! \file
	\brief Matrix operations

	This module provides matrix functions such as multiplication, addition,
	transpose, etc.
*/

#ifndef _MATRIX_H
#define _MATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

int SolveAugmentedMatrix(double  *pA, double  *pb, double  *px, int n);
void TransposeGeneral(const double  *pA, int rows, int cols, double *pB);
void TransposeSquare(double *pA, int n);
void TransposeSquaref(float *pA, int n);
int MatrixMultiply(const double *pA, int rowsA, int innerD,
				   const double *pB, int colsB, double *pC);
int MatrixMultiplyf(const float *pA, int rowsA, int innerD,
				   const float *pB, int colsB, float *pC);
void MatrixAdd(const double *pA, const double *pB, int rows, int cols, double *pC);
void MatrixAddf(const float *pA, const float *pB, int rows, int cols, float *pC);
void MatrixMulScalar(double a, const double *pA, int rows, int cols, double *pB);
void MatrixEye(int rows, int cols, double *pA);
void MatrixZeros(int rows, int cols, double *pA);
void MatrixCopy(const double *pA, int rows, int cols, double *pB);
void MatrixDiag(const double *pV, int dim, double *pA);
void MatrixGetDiag(const double *pA, int dim, double *pV);
void ModChol(const double *pP, int dim, double *pU, double *pD);
void ModCholInv(const double *pU, const double *pD, int dim, double *pP);

double VectorMagnitude(const double *pV, int size);
void VectorNormalize(const double *pV, int size, double *pVn);
void SubtractVectors(const double Vector1[], const double Vector2[], double Result[], int size);
void AddVectors(const double Vector1[], const double Vector2[], double Result[], int size);
void MultiplyVectorByScaler(double Scaler, const double Vector[], double Result[], int size);

void MatrixConcatenateV(const double *pA, int rowsA, const double *pB, int rowsB, int cols, double *pC);
void MatrixConcatenateH(const double *pA, int colsA, const double *pB, int colsB, int rows, double *pC);

void MatrixUpperTriangular(const double *pV, int dim, double *pA);
void MatrixGetUpperTriangular(const double *pA, int dim, double *pV);

void HouseQR(const double *A, int rows, int cols, double *Q, double *R);
void Chol(const double *A, int size, double *R);
void CholEx(const double *A, int size, double *R, double* ATemp);

double MatrixDet2(const double A[4]);
void MatrixInv2(const double A[4], double Ainv[4]);
double MatrixDet3(const double A[9]);
float MatrixDet3f(const float A[9]);
void MatrixInv3(const double A[9], double Ainv[9]);
void MatrixInv3f(const float A[9], float Ainv[9]);
double MatrixDet4(const double A[16]);
void MatrixInv4(const double A[16], double Ainv[16]);
void MatrixInverse(const double *A, int n, double *Ainv);

//! Invert a matrix by LU factorization
int MatrixLUInverse(const double *A, int n, double *Ainv);

//! Factorize a square matrix into a lower-triangular and an upper-triangular matrix
int MatrixLU(const double *A, int n, double *LU, int *indx);

//! Factorize a square matrix into LU form, in place
int MatrixLUInPlace(double* LU, int n, int* indx);

//! Solve a linear system by back-substitution, for a system matrix given in LU form
void BacksubLU(const double *LU, const double *b, double *x, int *indx, int n);


void MatrixSumCols(const double *A, int nRows, int nCols, double *v);

void srotg(double *sa, double *sb, double *c, double *s);
double sdot(const double *a, const double *b, int n);
void schud(const double *R, int n, const double *X, double *R1);
void schdd(const double *R, int n, const double *X, double *R1);
void CholUpdate(const double *R, int nR, const double *X, int mX, double u, double *R1);

//! Create a 3x3 skew-symmetric matrix from a 3-element vector
void SkewSymmetric3(const double vec[3], double mat[9]);

//! Compute the eigenvalues and eigenvectors of a square matrix using QR algorithm
void EigQR(const double *A, int n, double* lambda, double* V);

//! Get the infinity norm of a matrix
double MatrixNormInf(const double *A, int rows, int cols);

//! Calculate a matrix product of the form A'*A
void MatrixSqr(const double* A, int nRows, int nCols, double* sqrA);

//! Singular value decomposition
int SVDcmpf(float** A, int m, int n, float* W, float** V);

//! Pseudo-inverse for non-square and singular matricies
int MatrixInverseSVDf(float** A, int m, int n, float** Ainv);

#ifdef __cplusplus
}
#endif

#endif //!_MATRIX_H
