#include <omp.h>

#include <smmintrin.h>
#include "load.h"
#include "evaluate.h"
#include <vector>
#include <string>
#include <cmath>
#include <assert.h>
#include <Eigen/Eigen>

using namespace Eigen;
using namespace std;




vector<vector<double> > UserBasedCF(vector<vector<double> > train, vector<vector<double> > test, int usersNum, int itemsNum)
{
	
    vector<vector<double> > predict(usersNum, vector<double>(itemsNum, 0));
    //#pragma omp parallel for
	//vector<vector<double> > simsave(userNum, vector<double>(userNum, 0));
	MatrixXd simsave(usersNum, usersNum);
    MatrixXd trainsave(usersNum,itemsNum);
	MatrixXd predictsave(usersNum, itemsNum);

	for(int i = 0; i < usersNum; ++i){
		for(int j = 0; j < itemsNum; ++j){
			trainsave(i,j) = train[i][j];
		}
	}
	VectorXd normA(usersNum);
	#pragma omp parallel for
	for(int m = 0; m < usersNum; m++){
		normA(m) = trainsave.row(m).norm();
	}
	
	simsave = trainsave * trainsave.transpose();
	cout << "Compute Sim FINISHED" << endl;
	for (int i = 0; i < usersNum; i++){
		#pragma omp parallel for
		for(int k =0; k < usersNum;k++){
			
			simsave(i,k) = simsave(i,k) / (normA(i) * normA(k));
		}
		cout << "User: " << i << " FINISHED!" << endl;
	}
	

	
	cout << "Compute Similarity FINISHED!" << endl;
	cout << "Predict start..." << endl;
	predictsave = simsave * trainsave;
	int rowsize = simsave.rows();
    VectorXd Simsum(rowsize);
	#pragma omp parallel for
	for(int it = 0; it < rowsize; ++it){
		Simsum(it) = simsave.row(it).sum();
	}
	for(int i = 0; i < usersNum; i++){
		#pragma omp parallel for
		for(int j = 0; j < itemsNum; j++){
			predict[i][j] = predictsave(i,j) / Simsum(i);
		}
		cout << "User" << i  << " Predict"  << " FINISHED" << endl;
	}
	

    return predict;
}

int main()
{
	string base_dir("/home/jiang/homework/biganalyze/");
    string FilePath1(base_dir+"/dataset/netflix_train.txt");
    string FilePath2(base_dir+"/dataset/netflix_test.txt");
    
   // int row = 943;
    //int col = 1682;
    int row = 10000;
	int col = 10000;
	string useridfile (base_dir+"/dataset/users.txt");
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
		//   cout << "hello" << endl;
		   idpair.insert(std::pair<string, int> (aline, count));
		}
		//cout << "userid:" << aline << "newdafid:" << count <<endl;
		//cout << "user:" << aline << endl;
		it = idpair.find(aline);
		//cout << "newid:" << it->second << endl;
		count++;
	}
	cout << "Map userid FINISHED" << endl;
	
	//map<string,int>::iterator it;
	//cout << "hello:" << count << endl;
	//it = idpair.find(aline_stripe[0]);
	//cout << "hello:" << count << endl;
   
    vector<vector<double> > train = txtRead<double>(FilePath1,idpair, row, col);
	cout << "load trainning data FINISHED" << endl;
	
	vector<vector<double> > test = txtRead<double>(FilePath2,idpair, row, col);
	
	cout << "load test data FINISHED" << endl;
	//test on test_set, return perdict;
	double start, end;
	start = getUTtime();
	vector<vector<double> > predict = UserBasedCF(train, test, row, col);
	end  = getUTtime ();
	double dul = end - start;
    cout << "All Predict Finished" << endl;
    double rmse = ComputeRMSE(predict,test);
    cout << "RMSE is " << rmse <<endl;
	cout << "Compute Time: " << dul << " s"<< endl;
    return 0;
}
