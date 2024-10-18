#include<iostream>
#include<unordered_map>

using namespace std;

struct Node{
    int val;
    Node* next;
    Node* prev;
    Node(int val,Node* prev,Node* next):val(val),prev(prev),next(next){}
};

class LinkedList{
private:
    Node* left;
    Node* right;
    unordered_map<int,Node*> map;
public:
    LinkedList(){
        left = new Node(0,NULL,NULL);
        right = new Node(0,left,NULL);
        left->next = right;
    }
    
    int length(){return map.size();}

    void pushRight(int val){
        Node* node = new Node(val,right->prev,right);
        map[val]=node;
        right->prev = node;
        node->prev->next = node;
    }
    
    void pop(int val){
        if(map.find(val)!=map.end()){
            Node* node = map[val];
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }

    //used to remove LRU (leaset recently used node stays at the left most position)
    int popLeft(){
        int res = left->next->val;
        pop(res);
        return res;
    }
};

class LFU{
private:
    int capacity;
    int lfuCount;  //LFU pointer - holds the least used element's frequecy - used for cache eviction
    unordered_map<int,int> valMap;  //key->val      -  maps keys into values
    unordered_map<int,int> cntMap;  //key->count    -  holds the frequecy of elements
    unordered_map<int,LinkedList*> listMap; //count->LinkedList
public:
    LFU(int capacity){
        this->capacity=capacity;
        this->lfuCount=0;
        valMap.clear();
        cntMap.clear();
        listMap.clear();
    }

    int get(int key){
        if(valMap.find(key)==valMap.end())
            return -1;
        counter(key);
        return valMap[key];
    }

    void put(int key,int val){
        if(capacity==0) return;
        if(valMap.find(key)==valMap.end() && valMap.size()==capacity){
            int res = listMap[lfuCount]->popLeft();
            valMap.erase(res);
            cntMap.erase(res);
        }
        if(valMap.find(key)==valMap.end()){
            cntMap[key]=1;
            if(listMap.find(1)==listMap.end()){
                listMap[1]=new LinkedList();
            }
            listMap[1]->pushRight(key);
            lfuCount=1;
        }else{
            counter(key);
        }
        valMap[key]=val;
    }

    void counter(int key){
        int count = cntMap[key];
        cntMap[key]++;
        listMap[count]->pop(key);
        if(listMap.find(count+1)==listMap.end()){
            listMap[count+1] = new LinkedList();
        }
        listMap[count+1]->pushRight(key);
        if(listMap[lfuCount]->length()==0){
            lfuCount++;
        }
    }
    
    void printElements(){
        cout<<"Cache elements:"<<endl;
        for(auto pair:valMap){
            cout<<pair.first<<":"<<pair.second<<endl;
        }
    }
};  

int main(){
    LFU* c1 = new LFU(3);
    c1->put(12,42323);
    c1->put(23,32223);
    c1->put(32,32334);
    c1->get(12);
    c1->get(23);
    c1->get(12);
    c1->get(23);
    c1->printElements();
    c1->put(42,34232);
    c1->printElements();
    return 0;
}