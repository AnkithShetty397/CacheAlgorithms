#include<iostream>
#include<unordered_map>

using namespace std;

struct Node{
    int key;
    int val;
    Node* prev;
    Node* next;
    Node(int key,int val):key(key),val(val),next(NULL),prev(NULL){}
};

class LRU{
private:
    unordered_map<int,Node*> cache;
    Node* right;
    Node* left;
    int capacity;

    void removeNode(Node* node){
        node->prev->next=node->next;
        node->next->prev=node->prev;
    }
    void insertNode(Node* node){
        node->next=right;
        node->prev=right->prev;
        right->prev->next=node;
        right->prev=node;
    }
public:
    LRU(int capacity){
        this->capacity=capacity;
        this->cache.clear();
        left = new Node(0,0);
        right = new Node(0,0);
        left->next=right;
        right->prev=left;
    }
    int get(int key){
        if(cache.find(key)!=cache.end()){
            cout<<"Most recently used "<<cache[key]->key<<":"<<cache[key]->val<<endl;
            removeNode(cache[key]);
            insertNode(cache[key]);
            return cache[key]->val;
        }
        return -1;
    }
    void put(int key,int val){
        if(cache.find(key)!=cache.end()){
            removeNode(cache[key]);
        }
        cache[key] = new Node(key,val);
        cout<<"Most recently used "<<cache[key]->key<<":"<<cache[key]->val<<endl;
        insertNode(cache[key]);
        if(cache.size()>capacity){
            Node* lru = left->next;
            cout<<"Evicting "<<lru->key<<":"<<lru->val<<endl;
            removeNode(lru);
            cache.erase(lru->key);
        }
    }
    void printElements(){
        cout<<"=============================="<<endl;
        cout<<"Cache elements: "<<endl;
        for(auto pair:cache){
            cout<<pair.first<<" "<<pair.second->val<<endl;
        }
        cout<<"=============================="<<endl;
    }
};

int main(){
    LRU* c = new LRU(3);
    c->put(10,23432);
    c->put(20,52323);
    c->put(30,34454);
    c->printElements();
    c->put(40,54345);
    c->printElements();
    c->get(20);
    c->printElements();
    c->put(50,44354);
    c->printElements();
    return 0;
}