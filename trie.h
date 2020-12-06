#include <unordered_map>

using namespace std;
using ItemSet = vector<string>;

class TrieNode {
public:
    TrieNode() = default;
    TrieNode(const TrieNode&) = delete;

    void insert(string key);
    void insert(const ItemSet& item);
    TrieNode* next(string);
    bool search(string key);
    //bool search(const ItemSet& item);

private:
    unordered_map<string, TrieNode*> hashTable;
};

void TrieNode::insert(string key) {
    if (hashTable.count(key)) {
        return;
    }
    hashTable.insert(make_pair(key, new TrieNode()));
}

void TrieNode::insert(const ItemSet& item) {
    insert(item[0]);
    TrieNode* p = hashTable[item[0]];
    for (unsigned int i = 1; i < item.size(); ++i)
    {
        p->insert(item[i]);
        p = p->next(item[i]);
    }
}

bool TrieNode::search(string key) {
    return hashTable.count(key);
}

TrieNode* TrieNode::next(string key) {
    if (!hashTable.count(key))
        return nullptr;
    return hashTable[key];
}