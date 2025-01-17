﻿// apriori.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <set>
#include <vector>
#include <fstream>
#include<algorithm>
#include <unordered_map>
#include <map>
#include "trie.h"
#include <string>
#include <sstream>
#include <chrono>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

using ItemSet = vector<string>;

/*最小支持度计数 */
const double min_sup_rate = 0.3;
/*最小置信度阈值 */
const double min_conf = 0.7;
/*规则 */
struct Rule {
    ItemSet antecedent;
    ItemSet consequent;
    double conf;
};

map< ItemSet, int > create_L1(vector < ItemSet >& dataSet, int min_sup_count);
vector< ItemSet > create_C2(map < ItemSet, int >& L1);
TrieNode* buildHashTree(vector<ItemSet>& Ck);
void visualization(const vector<Rule>& rules);

//加载数据集
vector < ItemSet >  loadDataset(int &tranSize)
{
    vector < ItemSet >  dataSet;
    string filePath = "mushroom.dat";
   //string filePath = "retail.dat";
    
  ifstream fs(filePath, ios::in);
    string line;

    while (getline(fs,line)) {
       if (line.back() == '\n') line.pop_back(); // pop back the enter.
        ItemSet itemset;
        stringstream strm(line);
        string item;
        while (strm >> item) {
          itemset.push_back((item));
      }
        tranSize++;
       sort(itemset.begin(), itemset.end());
        dataSet.push_back(move(itemset));
    }

    //!!!数据必须按照字典序排序
 /*  ItemSet s1 = { "I1","I2","I4","I5","I6" };
    ItemSet s2 = { "I2","I4","I6","I7","I8" };
    ItemSet s3 = { "I2","I3","I6" ,"I7","I8" };
    ItemSet s4 = { "I1","I2","I4","I6","I7" };
    ItemSet s5 = { "I1","I3","I6" ,"I7","I8" };
    ItemSet s6 = { "I2","I3" ,"I4","I5" };
    ItemSet s7 = { "I1","I3","I4","I7" };
    ItemSet s8 = { "I1","I2","I3","I5" ,"I6" };
    ItemSet s9 = { "I1","I2","I3" ,"I4","I7" };

       ItemSet s1 = { "I1","I2","I5" };
    ItemSet s2 = { "I2","I4" };
    ItemSet s3 = { "I2","I3" };
    ItemSet s4 = { "I1","I2","I4" };
    ItemSet s5 = { "I1","I3" };
    ItemSet s6 = { "I2","I3" };
    ItemSet s7 = { "I1","I3" };
    ItemSet s8 = { "I1","I2","I3","I5" };
    ItemSet s9 = { "I1","I2","I3" };
    dataSet.push_back(s1);
    dataSet.push_back(s2);
    dataSet.push_back(s3);
    dataSet.push_back(s4);
    dataSet.push_back(s5);
    dataSet.push_back(s6);
    dataSet.push_back(s7);
    dataSet.push_back(s8);
    dataSet.push_back(s9);*/

    return dataSet;
}

map< ItemSet, int >  create_Lk_by_Ck(map< ItemSet, int >& C1, int min_sup_count) {
    map< ItemSet, int > L1;
    for (auto &item:C1) {
        if (item.second >= min_sup_count) {
            L1.insert({ item.first,item.second });
        }
    }

    return L1;
}

map< ItemSet, int > create_L1(vector < ItemSet >& dataSet,int min_sup_count) {
    map< ItemSet, int >C1;
    map< ItemSet, int >L1;
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

    L1 = create_Lk_by_Ck(C1,  min_sup_count);

    return L1;
}

vector< ItemSet > create_C2(map < ItemSet, int >& L1) {
    map < ItemSet, int >::iterator it_L1;
    ItemSet map_key;
    vector< ItemSet > C2;
    ItemSet::iterator it_key, it_key1;

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

bool is_apriori(const ItemSet &ProItemSet,map < ItemSet, int >& Lk) {
    for (auto& item : Lk) {
        if (ProItemSet == item.first) {
            return true;
        }
    }

    return false;
}

vector<ItemSet> create_Ck(map < ItemSet, int >& Lk, int k) {
    vector< ItemSet > Ck;
    vector< ItemSet >::iterator  it_Ck;
    /*key是(k - 1)项，value是第k项*/
    map < ItemSet, ItemSet > trie_ItemSet = {};
    ItemSet::iterator it_key,it_value, it_value1;
    string lastElement;
    vector<string> key;
    vector<string> tmp,tmp2;

    // k + 1是当前要生成的项集
    if (k == 1) {
        return create_C2(Lk);
    }
  
    //用前缀项存储 123 4 ;123 5; 123 6 => 123 456  
    //45 46 56  =>12345 12346 12356
    for (auto& item : Lk) {
        tmp = item.first;
        lastElement = tmp.back();
        tmp.pop_back();
        if (trie_ItemSet.find(tmp) == trie_ItemSet.end()) {
            trie_ItemSet.insert({ tmp,{lastElement} });
        }
        else {
            trie_ItemSet[tmp].push_back(lastElement);
        }
    }

    //连接的时候剪枝 {1:2,3,5}
    //2,3,5按照字典序组合 判断这个结果在(k - 1)项频繁集中是否出现，未出现则剪枝;否则合并
    for (auto& item : trie_ItemSet) {
        key = item.first;
        //如果后面的k只有一个，则无法存储
        if (item.second.size() >= 2) {
            for (it_value = (item.second).begin(); it_value != (item.second).end() - 1; it_value++) {
                for (it_value1 = it_value + 1; it_value1 != (item.second).end(); it_value1++) {
                    tmp = { *it_value ,*it_value1 };
                    //当 k = 3时候，后缀存储的是二项集
                    if ((k == 2 && is_apriori(tmp, Lk)) || k > 2) {
                        tmp2 = key;
                        tmp2.push_back(*it_value);
                        tmp2.push_back(*it_value1);
                        Ck.push_back(tmp2);
                    }
                }
            }
        }
    }
 
    return Ck;
}

void calculateSupportCount(
    map < ItemSet, int >& Lk,
    const ItemSet& trans,  // Transaction set
    int k,         // The length of combination would generate.
    TrieNode* p,      // Pointer to the hash tree node.
    int remainder, // The number of vocant positions in combination.
    int lastPos   // The last position of chosen item in itemset.
) {
    //如果事务中剩余位置不足 impossible
    if ((int)trans.size() - lastPos - 1 < remainder) return;

    static ItemSet tmp;
    if (k == remainder)  tmp.resize(k);
    if (0 == remainder) {
        if (Lk.find(tmp) == Lk.end()) {
            Lk.insert({ tmp,1 });
        }
        else {
            ++Lk[tmp];
        }
        return;
    }

    for (unsigned int i = lastPos + 1; i < trans.size(); i++) {
        if (!p->search(trans[i]))   continue;
        tmp[k - remainder] = trans[i];
        calculateSupportCount(Lk, trans, k, p->next(trans[i]), remainder - 1, i);
    }
}

map < ItemSet, int > generateKFrequentSet(vector<ItemSet>& Ck, vector < ItemSet >& dataSet, int k,int min_sup_count) {
    map < ItemSet, int > Lk;
    map < ItemSet, int >::iterator it_Lk;

    //先构造哈希树  每个元素插入树中
    auto hashTree = buildHashTree(Ck);
    //对于每个事务计算统计最小支持度计数
    for (auto& trans : dataSet) {
        calculateSupportCount(Lk, trans, k, hashTree, k, -1);
    }

    Lk = create_Lk_by_Ck(Lk,min_sup_count);

    return Lk;
}

TrieNode* buildHashTree(vector<ItemSet>& Ck) {
    TrieNode* hashTree = new TrieNode();
    for (auto& item : Ck) {
        hashTree->insert(item);
    }

    return hashTree;
}

vector<  map< ItemSet, int > > generate_Lk(vector < ItemSet >& dataSet,int min_sup_count) {
    /*<k频繁项集,支持度计数>*/
    map < ItemSet, int > L1;
    map < ItemSet, int > Lk;
   /* k候选项集*/
    vector<ItemSet> Ck;
    vector<  map< ItemSet, int > > L;

    L1 = create_L1(dataSet, min_sup_count);
    L.push_back(move(L1));
    size_t k = 1;
    while (!L[k - 1].empty()) {
        Ck = create_Ck(L[k - 1], k);
        if (Ck.empty()) {
            break;
        }
        Lk = generateKFrequentSet(Ck, dataSet, k + 1, min_sup_count);
        if (!Lk.empty()) {
            L.push_back(move(Lk));
            ++k;
        }
        else {
            break;
        }
    }

    return L;
}

vector<ItemSet> apriori_gen(vector<ItemSet>& consequentSet) {
     vector<ItemSet>::iterator it_cs,it_cs1;
     vector<string> combItem;
     set<ItemSet> tmpSet;
     vector<ItemSet> consequentNewSet;
     
    for (it_cs = consequentSet.begin(); it_cs != consequentSet.end() - 1; it_cs++) {
        for (it_cs1 = (it_cs)+1; it_cs1 != consequentSet.end(); it_cs1++) {
           //bc bd=>bcd bd cd=>bcd
            combItem = {};
            set_union((*it_cs).begin(),(*it_cs).end(), (*it_cs1).begin(), (*it_cs1).end(), back_inserter(combItem));
            if (combItem.size() == (*it_cs).size() + 1) {
                tmpSet.insert(combItem);
            }
        }
    }
    //set转vector
    consequentNewSet.assign(tmpSet.begin(), tmpSet.end());

    return consequentNewSet;
}

//i是L[i]的下标
void generateRuleByItemset( vector<  map< ItemSet, int > >& L,
    const ItemSet &itemSet, vector<ItemSet> &consequentSet, 
    vector<Rule> &ans, int consequent_num,
    int i ,
    int itemLength) {
   
    double conf;
    ItemSet consequent;
    ItemSet antecedent;
    vector<ItemSet> consequentNewSet = {};
    vector<ItemSet> consequentTempSet = {};
    vector<ItemSet>::iterator it;
    int antecedent_num;

    //前件至少有1个，前件加后件如果大于当前事务，则无法生成
    if (consequent_num + 1 > itemLength) return;

    for (it = consequentSet.begin(); it != consequentSet.end();  ++it)
    {
        antecedent = {};
        consequent = (*it);
        //itemSet - consequent = antecedent
        set_difference(itemSet.begin(), itemSet.end(), consequent.begin(), consequent.end(), std::back_inserter(antecedent));
        antecedent_num = antecedent.size() - 1;
        conf = (L[i][itemSet]) * 1.0 / (L[antecedent_num][antecedent]);
        conf = round(conf * 100) / 100.0;
        if (conf >= min_conf) {
            ans.push_back(move(Rule{ antecedent, consequent, conf }));
            consequentTempSet.push_back(consequent);
        }
        //else {
        //    //如果bcd=>a置信度不满足，则剪枝；即从后件项集中删除该元素
        //    it = consequentSet.erase(it);
        //}
    }

    if (!empty(consequentTempSet)) {
        consequentNewSet = apriori_gen(consequentTempSet);
        generateRuleByItemset(L,itemSet, consequentNewSet, ans, consequent_num + 1, i, itemLength);
    }
}

vector<Rule>  generate_associate_rules( vector<  map< ItemSet, int > >&L, const double min_conf) {
    vector<ItemSet> consequentSet;
    vector<Rule> ans;
    //L[1]存储的是二项集
    for (int i = 1; i < L.size(); i++) {
        for (auto& itemSet : L[i]) {
            consequentSet = {};
            //规则的1-项后件
            for (auto& item : (itemSet.first)) {
                consequentSet.push_back({ item });
            }
            generateRuleByItemset(L,itemSet.first, consequentSet, ans, 1, i, i + 1);
        }
    }

    return ans;
}

void  visualization(const vector<Rule>& rules) {
    ItemSet antecedent;
    ItemSet consequent;
    ofstream ofs("output.txt");


    for (auto& rule : rules)
    {
        antecedent = rule.antecedent;
        for (auto& item : antecedent) {
            ofs << item;
            int size = antecedent.size() - 1;
            if (item == antecedent[size])
                ofs << " ";
            else
                ofs << "^";
        }

        ofs << " => ";

        consequent = rule.consequent;
        for (auto& item : consequent) {
            ofs << item;
            int size = consequent.size() - 1;
            if (item == consequent[size])
                ofs << " ";
            else
                ofs << "^";
        }

        ofs << "   confidence  : " << rule.conf;

        ofs << endl;
    }
}

int main()
{
    /*事务数目:*/
    int tranSize = 0;
    int min_sup_count;
    vector < ItemSet > dataSet;
    vector<  map< ItemSet, int > > L;
    high_resolution_clock::time_point beginTime = high_resolution_clock::now();

    dataSet = loadDataset(tranSize);
    min_sup_count = ceil(tranSize * min_sup_rate);
   
    L = generate_Lk(dataSet, min_sup_count);
    vector<Rule> rules = generate_associate_rules(L,min_conf);
    visualization(rules);
  
    high_resolution_clock::time_point endTime = high_resolution_clock::now();
    milliseconds timeInterval = std::chrono::duration_cast<milliseconds>(endTime - beginTime);
    std::cout << timeInterval.count() << "ms\n";
    
    return 0;
}

