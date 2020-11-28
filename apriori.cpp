// apriori.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <set>
#include <vector>

#include<algorithm>
#include <unordered_map>
#include <map>
#include "trie.h"

using namespace std;
using ItemSet = vector<string>;

const int min_sup_count = 2;
vector < ItemSet> loadDataset();
map< ItemSet, int > create_L1(vector < ItemSet >& dataSet);
Trie* buildHashTree(vector<ItemSet>& Ck);
void calculateSupportCount(ItemSet &trans, int k, Trie* hashTree);

//加载数据集
vector < ItemSet> loadDataset()
{
    vector < ItemSet > dataSet;

    ItemSet s1 = { "I1","I2","I5" };
    ItemSet s2 = { "I2","I4" };
    ItemSet s3 = { "I2","I3" };
    ItemSet s4 = { "I1","I2","I4" };
    ItemSet s5 = { "I1","I3" };
    ItemSet s6 = { "I2","I3" };
    ItemSet s7 = { "I1","I3" };
    ItemSet s8 = { "I1","I3","I2","I5" };
    ItemSet s9 = { "I2","I3","I1" };

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

map< ItemSet,  int > create_C1(vector < ItemSet >& dataSet) {
    map< ItemSet,  int >C1;
    map< ItemSet,  int >::iterator it_C1;
    //迭代器
    vector < ItemSet >::iterator it_data;
    ItemSet::iterator it_item;

    for (it_data = dataSet.begin(); it_data != dataSet.end(); it_data++) {
        for (it_item = (*it_data).begin(); it_item != (*it_data).end(); it_item++) {
            ItemSet tmp_item = {};
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

map< ItemSet,  int > create_L1_by_C1(map< ItemSet,  int >& C1) {
    map< ItemSet,  int >::iterator it_C1;
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

map< ItemSet, int > create_L1(vector < ItemSet >& dataSet) {
    map < ItemSet, int > C1;
    map < ItemSet, int > L1;
    //生成C1  map<项集  支持度计数>
    C1 = create_C1(dataSet);
    //L1
    L1 = create_L1_by_C1(C1);
    return C1;
}

vector< ItemSet > create_C2(map < ItemSet, int > &L1) {
    map < ItemSet, int >::iterator it_L1;
    ItemSet map_key;
    vector< ItemSet > C2;
    ItemSet::iterator it_key,it_key1;
    
    for (it_L1 = L1.begin(); it_L1 != L1.end(); it_L1++) {
        map_key.push_back((it_L1->first)[0]);
    }
    for (it_key = map_key.begin(); it_key != map_key.end() - 1; it_key++) {
        for (it_key1 = it_key + 1; it_key1 != map_key.end(); it_key1++) {
            C2.push_back({ *it_key ,*it_key1 });
        }
    }
    
    return C2;
}

vector<ItemSet> create_Ck(map < ItemSet, int > &frequentSet, int k) {
    vector< ItemSet > Ck;

    if (k == 2) {
        Ck = create_C2(frequentSet);
    }
    else {
        //前缀项集
    }
     
    return Ck;
}

void calculateSupportCount(
    ItemSet& trans,  // Transaction set
    int k,         // The length of combination would generate.
    Trie* p,      // Pointer to the hash tree node.
    int remainder, // The number of vocant positions in combination.
    int lastPos   // The last position of chosen item in itemset.
) {
    //如果事务中剩余位置不足 impossible
    if (trans.size() - lastPos - 1 < remainder) return;

    static ItemSet tmp; 
    if (k == remainder)  tmp.resize(k);
    if (0 == remainder) {
        //TODO::保留结果
        return;
    }

    for (int i = lastPos + 1; i < trans.size(); i++) {
        //TODO:一个被否定的事务
        if (!p->search(trans[i])) continue;
        tmp[k - remainder] = trans[i];
        calculateSupportCount(trans,k,p->nextPtr(trans[i]),remainder - 1,i);
    }
}

//map < ItemSet, int > generateKFrequentSet(vector<ItemSet>& Ck, vector < ItemSet >& dataSet, int k) {
void generateKFrequentSet(vector<ItemSet>& Ck, vector < ItemSet >& dataSet, int k) {
    //先构造哈希树  每个元素插入树中
    auto hashTree = buildHashTree(Ck);
    //对于每个事务计算统计最小支持度计数
    for (auto& trans : dataSet) {
        calculateSupportCount(trans,k,hashTree,k,-1);
    }
   
}
Trie* buildHashTree(vector<ItemSet>& Ck) {
     Trie* hashTree = new Trie();
     for (auto& item : Ck) {
         hashTree->insert(item);
     }
   
     return hashTree;
}


void generate_Lk(vector < ItemSet >& dataSet) {
    //记录项集和统计计数
    map < ItemSet, int > L1;
    map < ItemSet, int > Lk;
    vector<ItemSet> Ck;
    vector<  map< ItemSet, int > > L;

    L1 = create_L1(dataSet);
    L.push_back(L1);
    int k = 1;
    while (L[k - 1].size() > 0) {
        //产生候选集
        Ck = create_Ck(L[k -1],k + 1);
       /* Lk = generateKFrequentSet(Ck, dataSet, k + 1);*/
       generateKFrequentSet(Ck, dataSet, k + 1);
        k++;
        break;
    }
}

int main()
{
    vector < ItemSet > dataSet;
    dataSet = loadDataset();
    generate_Lk(dataSet);
}

