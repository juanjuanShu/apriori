// apriori.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include<algorithm>
#include <unordered_map>


using namespace std;

//加载数据集
vector < vector<string>> loadDataset()
{
    vector < vector<string> > dataSet;

    vector <string> s1 = { "I1","I2","I5" };
    vector <string> s2 = { "I2","I4" };
    vector <string> s3 = { "I2","I3" };
    vector <string> s4 = { "I1","I2","I4" };
    vector <string> s5 = { "I1","I3" };
    vector <string> s6 = { "I2","I3" };
    vector <string> s7 = { "I1","I3" };
    vector <string> s8 = { "I1","I3","I2","I5" };
    vector <string> s9 = { "I2","I3","I1" };

    dataSet.push_back(s1);
    dataSet.push_back(s2);
    dataSet.push_back(s3);
    dataSet.push_back(s4);
    dataSet.push_back(s5);
    dataSet.push_back(s6);
    dataSet.push_back(s7);
    dataSet.push_back(s8);
    dataSet.push_back(s9);

    return dataSet;
}

//unordered_map < vector<string>, int >  create_C1(vector < vector<string> >& dataSet) {
//    unordered_map < vector<string>, int > C1;
//    unordered_map < vector<string>, int >::iterator it_C1;
//    //迭代器
//    vector < vector<string> >::iterator it_data;
//    vector<string>::iterator it_item;
//
//    for (it_data = dataSet.begin(); it_data != dataSet.end(); it_data++) {
//        for (it_item = (*it_data).begin(); it_item != (*it_data).end(); it_item++) {
//            vector<string> tmp_item = {};
//            tmp_item.push_back(*it_item);
//            auto ret = C1.insert({ tmp_item,1 });
//            //insert插入时候，由于map的key要求唯一，如果已经插入了，则返回值中的second的值为False
//            //说明插入这个元素了
//            if (!ret.second)
//                ++ret.first->second;
//        }
//    }
//
//    return C1;
//}



void generate_Lk(vector < vector<string> >& dataSet, int min_sup_count) {
    //记录项集和统计计数
    unordered_map < string, int > C1;
    map < vector<string>, int > C1;
    unordered_map < vector<string>, int > C1; 
    


    //生成C1  map<项集  支持度计数>
    /*C1 = create_C1(dataSet);*/

}

int main()
{
    vector < vector<string> > dataSet;
    int min_sup_count = 2;
    dataSet = loadDataset();
    generate_Lk(dataSet, min_sup_count);
}

