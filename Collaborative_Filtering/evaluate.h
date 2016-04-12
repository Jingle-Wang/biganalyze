#ifndef EVALUATE_H
#define EVALUATE_H
#include <cmath>
#include <vector>
#include <sys/time.h>

double getUTtime()
{
    timeval t;
    gettimeofday(&t, 0);
    return t.tv_sec + t.tv_usec * 1e-6;
}


double ComputeRMSE(vector<vector<double> > predict, vector<vector<double> > test)
{
    int Counter = 0;
    double sum = 0;
    for (vector<vector<double> >::size_type i = 0; i < test.size(); ++i)
    {
		#pragma omp parallel for
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

#endif
