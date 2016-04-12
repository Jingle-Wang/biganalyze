#include <cmath>
#include <vector>
#include <Eigen/Eigen>

using namespace Eigen;
using namespace std;

double ComputeRMSE(vector<vector<double> > predict, vector<vector<double> > test)
{
    int Counter = 0;
    double sum = 0;
    for (vector<vector<double> >::size_type i = 0; i < test.size(); ++i)
    {
        for (vector<double>::size_type j = 0; j < test[0].size(); ++j)
        {
            if (predict[i][j] && test[i][j])
            {
                ++Counter;
                sum += pow((test[i][j] - predict[i][j]), 2);
            }
        }
    }
    return sqrt(sum / Counter);
}
/*
double ComputeRMSE1(MatrixXd predict, MatrixXd test, int usersNum, int itemsNum)
{
	MatrixXd temp(usersNum, itemsNum);
    temp = test - predict;
	double ra = 
}
 */
