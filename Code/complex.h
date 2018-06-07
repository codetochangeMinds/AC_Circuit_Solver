#include <iostream>
#include <math.h>
#include <vector>
using namespace std;

extern int nodes;
extern int acS;


struct Complex
{
	double real;
	double img;
};

Complex makeComplex(double a,double b)
{
    Complex res;
    res.real=a;
    res.img=b;
    return res;
}

Complex Add(Complex a,Complex b)
{
	Complex res;
	res.real=a.real+b.real;
	res.img=a.img+b.img;
	return res;
}

Complex Sub(Complex a,Complex b)
{
    Complex res;
    res.real=a.real-b.real;
	res.img=a.img-b.img; 
	return res;
}

Complex Mul(Complex a,Complex b)
{
    Complex res;
    res.real=(a.real*b.real)-(a.img*b.img);
    res.img=(a.real*b.img)+(a.img*b.real);
    return res;
}

Complex div(Complex a,Complex b)
{
	Complex res;
	double p=(b.real)*(b.real) + (b.img)*(b.img); 
	//check division by zero
	if(p==0)
	{
		res.real=0;
		res.img=0;
	}
	else
	{	
		res.real=((a.real*b.real)+(a.img*b.img))/(pow(b.real,2)+pow(b.img,2));
		res.img=((a.img*b.real)-(a.real*b.img))/(pow(b.real,2)+pow(b.img,2));
	}
	return res;
}

Complex inv(Complex a)
{
	Complex res;
	res.real=a.real/(pow(a.real,2)+pow(a.img,2));
	res.img=(-1)*(a.img)/(pow(a.real,2)+pow(a.img,2));
	return res;
}

//to calculate magnitude  
double abs(Complex b)
{
	double p=(b.real)*(b.real) + (b.img)*(b.img);
	double res=sqrt(p);
	return res;
}

//to calculate phase
double calcAngle(Complex a)
{
	double res=atan(a.img/a.real);
	return res;
}

/*void out()
{
	const int size = nodes + acS;

	cout<<size<<endl;
	Complex a[size][size];
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			a[i][j].real=0;
			a[i][j].img=1;
			cout<<a[i][j].img<<endl;
		}
		
	}
}*/
//get cofactor of complex matrix
void getCofactor(Complex A[][20], Complex temp[][20], int p, int q, int n)
{
    int i = 0, j = 0;
    Complex A1[nodes+acS][nodes+acS],temp1[nodes+acS][nodes+acS];
    // Looping for each element of the matrix
    for (int i = 0; i < nodes+acS; i++)
    {
        for (int j = 0; j <nodes+acS; j++)
        {
            A1[i][j]=A[i][j];
        }
    }
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp1[i][j++] = A1[row][col];
 
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
    for (int i = 0; i < nodes+acS; i++)
    {
        for (int j = 0; j <nodes+acS; j++)
        {
            temp[i][j]=temp1[i][j];
        }
    }
}

//determinant of complex matrix
Complex determinant(Complex A[][20], int n)
{
    Complex D = makeComplex(0,0); // Initialize result
 
    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0][0];
 
    Complex temp[nodes+acS][20]; // To store cofactors
 
    int sign = 1;  // To store sign multiplier
 
     // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        getCofactor(A, temp, 0, f, n);
        D = Add(D,Mul(makeComplex(sign*(A[0][f].real),sign*(A[0][f].img)) ,makeComplex(sign*determinant(temp, n - 1).real,sign*determinant(temp, n - 1).img)));
         // terms are to be added with alternate sign
        sign = -sign;
    }
 
    return D;
}
//adjoint of complex matrix 
void adjoint(Complex A[][20],Complex adj[][20])
{
    // if (N == 1)
    // {
    //     adj[0][0] = makeComplex(1,0);
    //     return;
    // }
 
    // temp is used to store cofactors of A[][]
    int sign = 1;
    Complex temp[nodes+acS][20];
 
    for (int i=0; i<nodes+acS; i++)
    {
        for (int j=0; j<nodes+acS; j++)
        {
            // Get cofactor of A[i][j]
            getCofactor(A, temp, i, j, nodes+acS);
 
            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i+j)%2==0)? 1: -1;
 
            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j][i].real = (sign)*(determinant(temp,(nodes+acS)-1).real);
            adj[j][i].img = (sign)*(determinant(temp,(nodes+acS)-1).img);
        }
    }
}

//inverse of complex matrix
void inverse(Complex A[][20],Complex inverse[][20])
{
	Complex det = determinant(A, nodes+acS);
    if ((det.real== 0) && (det.img==0))
    {
        //cout << "Singular matrix, can't find its inverse";
        //return makeComplex(0,0);
        //return false;
        for (int i=0; i<nodes+acS; i++)
        for (int j=0; j<nodes+acS; j++)
           {
				 inverse[i][j] = makeComplex(0,0);
			}
    }
 
    // Find adjoint
    Complex adj[nodes+acS][20];
    adjoint(A, adj);
 
    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i=0; i<nodes+acS; i++)
        for (int j=0; j<nodes+acS; j++)
           {
				 inverse[i][j] = div(adj[i][j],det);
			}
	//return inverse;
}

//multiplication of two complex matrix
void multiply(Complex mat1[][20], Complex mat2[][1], Complex res[][1])
{
    int i, j, k;
    for (i = 0; i < nodes+acS; i++)
    {
        for (j = 0; j < nodes+acS; j++)
        {
            res[i][j] = makeComplex(0,0);
            for (k = 0; k < nodes+acS; k++)
                res[i][j] =Add(res[i][j],Mul(mat1[i][k],mat2[k][j]));
        }
    }
}