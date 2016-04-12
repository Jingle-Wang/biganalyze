#ifndef LOAD_H
#define LOAD_H
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <map>
using namespace std;
std::vector<std::string> split(const string& src, string separate_character)
{
    vector<string> strs;
    int separate_characterLen = separate_character.size();//分割字符串长度，这样就可以支持多字 符串的分隔符  
    int lastPosition = 0, index = -1;
    while (-1 != (index = src.find(separate_character, lastPosition)))
    {
        strs.push_back(src.substr(lastPosition, index - lastPosition));
        lastPosition = index + separate_characterLen;
    }
    string lastString = src.substr(lastPosition);
    if (!lastString.empty())
		strs.push_back(lastString);
    return strs;
}

template <typename T>
std::vector<std::vector<T> > txtRead(string FilePath, map<string, int> idpair, int row, int col)
{
    ifstream input(FilePath.c_str());
	
	
    if (!input.is_open())
    {
        cout << "File is not existing, check the path: \n" <<  FilePath << endl;
        exit(0);
    }
	
	string aline;
	int count = 0;
	std::vector<std::vector<T> > data(row, vector<T>(col,0)); 
	while(getline(input, aline))
	{
		
       // cout << aline  << endl;
	    vector<string> aline_stripe  = split(aline, " ");
//		int row = atoi(aline_stripe[0].c_str());
		//cout << row << endl;
        //cout << "ss:" << aline_stripe[0] << endl;
		//int row;
		map<string,int>::iterator it;
		
		it = idpair.find(aline_stripe[0]);
		if(it == idpair.end())
		{
			cout << "User id Not Found" << endl;
			exit(0);
		}
		else 
		{
			row = it-> second;
		}
		int col = atoi(aline_stripe[1].c_str()) - 1;
		//cout << "col:" << col << endl;
		//cout << "row:" << row << endl;
		//cout << (double) atoi(aline_stripe[2].c_str()) << endl;
		
		data[row][col] = (double) atoi(aline_stripe[2].c_str());
		
	}
 /*   
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
        {
            input >> data[i][j];
        }
    }
*/
    return data;
}

#endif
