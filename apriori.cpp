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

map< vector<string>,  int > create_C1(vector < vector<string> >& dataSet) {
    map< vector<string>,  int >C1;
    map< vector<string>,  int >::iterator it_C1;
    //迭代器
    vector < vector<string> >::iterator it_data;
    vector<string>::iterator it_item;

    for (it_data = dataSet.begin(); it_data != dataSet.end(); it_data++) {
        for (it_item = (*it_data).begin(); it_item != (*it_data).end(); it_item++) {
            vector<string> tmp_item = {};
            tmp_item.push_back(*it_item);
            auto ret = C1.insert({ tmp_item,1 });
            //insert插入时候，由于map的key要求唯一，如果已经插入了，则返回值中的second的值为False
            //说明插入这个元素了
            if (!ret.second)
                ++ret.first->second;
        }
    }


    return C1;
}

map< vector<string>,  int > create_L1_by_C1(map< vector<string>,  int >& C1,  int min_sup_count) {
    map< vector<string>,  int >::iterator it_C1;
    it_C1 = C1.begin();
    while (it_C1 != C1.end()) {
        if (it_C1->second < min_sup_count) {
            C1.erase(it_C1++);
        }
        else {
            it_C1++;
        }
    }

    return C1;
}

map< vector<string>, vector<string> > create_map_L1(map< vector<string>,  int > L1) {
    //前缀项集存储
    map< vector<string>, vector<string> > map_L1;
    map< vector<string>,  int >::iterator it_L1;
    vector<string> map_value;
    vector<string> map_key;

    for (it_L1 = L1.begin(); it_L1 != L1.end(); it_L1++) {
        map_value.push_back((it_L1->first)[0]);
    }

    map_key.push_back("0");
    map_L1.insert(map<vector<string>, vector<string>>::value_type(map_key, map_value));

    return map_L1;
}

map< vector<string>,int > create_map_C2(map < vector<string>, int > L1) {
    map < vector<string>, int >::iterator it_L1;
    map< vector<string>, int> map_C2;
    vector<string> map_key;
    vector<string> tmp;
    vector< vector<string> > ve_C2;
    vector<string>::iterator it_key,it_key1;
    

    for (it_L1 = L1.begin(); it_L1 != L1.end(); it_L1++) {
        map_key.push_back((it_L1->first)[0]);
    }
    for (it_key = map_key.begin(); it_key != map_key.end() - 1; it_key++) {
        for (it_key1 = it_key + 1; it_key1 != map_key.end(); it_key1++) { 
            ve_C2.push_back({ *it_key ,*it_key1 });
        }
    }

    return map_C2;
}

void generate_Lk(vector < vector<string> >& dataSet, int min_sup_count) {
    //记录项集和统计计数
    map < vector<string>, int > C1;
    map < vector<string>, int > C2;
    map < vector<string>, int > L1;
    map < vector<string>, int > Lk;

    //L2确定后 用前缀项集存储；这样做的好处：一是自连接方便 二是剪枝
    map< vector<string>, vector<string> >map_Lk;
   
    //生成C1  map<项集  支持度计数>
    C1 = create_C1(dataSet);
    //L1
    L1 = create_L1_by_C1(C1, min_sup_count);
    //获得map存储
    C2 = create_map_C2(L1);
}

int main()
{
    vector < vector<string> > dataSet;
    int min_sup_count = 2;
    dataSet = loadDataset();
    generate_Lk(dataSet, min_sup_count);
}

