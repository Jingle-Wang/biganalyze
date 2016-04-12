#include "evaluate.h"
#include "load.h"
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include <Eigen/Eigen>

using namespace Eigen;
using namespace std;


vector<vector<double> > BaseMF(vector<vector<double> >  train, vector<vector<double> >  test,int k , double lamda, int usersNum, int itemsNum)
{
	MatrixXd trainsave(usersNum, itemsNum);
	MatrixXd U = MatrixXd::Random(usersNum, k);
	MatrixXd V = MatrixXd::Random(itemsNum, k);
	MatrixXd A(usersNum, itemsNum);
	//cout << "U(1,1)hh: " << U(1,1) << endl; 
	vector<vector<double> > predict(usersNum,vector<double>(itemsNum, 0));
	for(int i = 0; i <usersNum; ++i){
		for(int j = 0; j < itemsNum; ++j){
			if(train[i][j]){
				A(i,j) = 1;
			}
			trainsave(i, j) = train[i][j];
		}		
	}
	double alpha = 0.0001;
	int count = 0;
	MatrixXd U1(usersNum, k);
	MatrixXd V1(itemsNum, k);
	MatrixXd temp(usersNum, itemsNum);
	double pow_temp ;
	double pow_U ;
	double pow_V;
	double purpose;
	for(int i = 0; i < 150; i++){
		count ++;
	
		//cout << "trainsave(1,1): " << trainsave(1,1) << endl;
		temp = A.cwiseProduct(trainsave - U * V.transpose());
		//cout << "temp(1,1): " << temp(1,1)<< endl;
		pow_temp = pow(temp.norm(), 2);
		pow_U = pow(U.norm(), 2);
		pow_V = pow(V.norm(), 2);
		
		
		purpose = 1/2 * pow_temp + lamda * pow_U + lamda*pow_V;
//		cout << "J:" <<purpose << endl;
		
		
		U1 = A.cwiseProduct(U * V.transpose() - trainsave) * V + 2 * lamda * U;
		V1 = A.cwiseProduct(U * V.transpose() - trainsave).transpose() * U + 2 * lamda * V;
		//cout << "U(1,1): " << U(1,1) << endl;
		//cout << "U1(1,1): " << U1(1,1)<< endl;
		U = U - (alpha * U1);
		V = V - (alpha * V1);
		//cout << "After U(1,1): " << U(1,1)<< endl;
		temp = U * V.transpose();
	//	cout << "temp(1,1): " << temp(1,1)<< endl;
		for(int i = 0; i < usersNum; ++i){
			for(int j = 0; j < itemsNum; ++j){
				predict[i][j] = temp(i,j);
			}
		}
		double rmse = ComputeRMSE(predict, test);
		cout << purpose << "\t" << rmse <<endl;
	//	cout << "第 "<< count << "次迭代:" << endl;
    //    cout << "rmse is: " << rmse << endl;
		
	}

	return predict;
	
}

int main()
{
   
	string base_dir = "/home/jiang/homework/biganalyze/";
	
    string FilePath1(base_dir + "dataset/netflix_train.txt");
    string FilePath2(base_dir + "dataset/netflix_test.txt");
    
    int row = 10000;
	int col = 10000;
	string useridfile (base_dir + "dataset/users.txt");
	ifstream input(useridfile.c_str());
	if (!input.is_open())
    {
        cout << "File is not existing, check the path: \n" <<  useridfile << endl;
        exit(0);
    }
	map <string, int> idpair;
	string aline;
	int count = 0;
	map<string,int>::iterator it;
	for(int i = 0; i < 10000; i++)
	{
		input >> aline;
		
		it = idpair.find(aline);
		if(it==idpair.end())
		{
	
			idpair.insert(std::pair<string, int> (aline, count));
		}

		it = idpair.find(aline);

		count++;
	}

	cout << "Map userid FINISHED" << endl;

    vector<vector<double> > train = txtRead<double>(FilePath1,idpair, row, col);
	cout << "Load Train Data FINISHED" << endl;
	vector<vector<double> > test = txtRead<double>(FilePath2,idpair, row, col);
	cout << "Load Test Data FINISHED" << endl;
	vector<vector<double> > predict = BaseMF(train,test, 50, 0.001,row, col);
    
    double rmse = ComputeRMSE(predict,test);
    cout << "ProbeRMSE is:" << rmse <<endl;
    return 0;
}
