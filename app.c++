#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Product{
    int price;
    int active;
}pList[30000];
int pidx = 0;


struct Node{
    Product* prod;
    Node* next;
}nList[500000];
int nidx = 0;


Node hashList[27000];


unordered_map<string, int> taginfo;
int tidx = 0;


void init(int N){
    taginfo.clear();
    tidx = 0;
    pidx = 0;
    nidx = 0;
    for(register int i = 0; i<500000; i++){
        nList[i].next = nullptr;
    }
    //TLE- 이부분 추가됨
    for (int i = 0; i < 27000; i++) {
        hashList[i].next = 0;
    }
}

/*
int get_tagid(char name[]){
    if(!taginfo[name]){
        taginfo.insert({name, tidx++});
    }
    return taginfo[name];
}이렇게 하면 TLE 발생*/ 

int get_tagid(char name[]){
    auto tag = taginfo.find(name);
    if (tag == taginfo.end()) {
        int tid = tidx++;
        taginfo[name] = tid;
        return tid;
    }
    return tag->second;
}

void addNode(int id, Product* prod){
    Node* ptr = &nList[nidx++];
    
    ptr->prod = prod;
    ptr->next = hashList[id].next;
    hashList[id].next = ptr;
    
}




void addProduct(int mPrice, int tagNum, char tagName[][10]){
    vector<int> tags(tagNum);
    for(register int i = 0; i < tagNum; i++){
        tags[i] = get_tagid(tagName[i]);
    }
    sort(tags.begin(), tags.end());
    
    Product* ptr = &pList[pidx++];
    ptr->price = mPrice;
    ptr->active = 1;
    
    int hash = 0;

    for(register int i = 0; i < tagNum ; i++){
        addNode(tags[i], ptr);
        for(register int j = i + 1; j < tagNum; j++){
            for(register int k = j + 1; k < tagNum; k++){
                hash = tags[i] + tags[j] * 30 + tags[k] * 900;
                addNode(hash, ptr);
            }
        }
    }
}

int buyProduct(char tag1[], char tag2[], char tag3[]){
    vector<int> tmp;
    tmp.push_back(get_tagid(tag1));
    tmp.push_back(get_tagid(tag2));
    tmp.push_back(get_tagid(tag3));
    sort(tmp.begin(), tmp.end());
    
    int hash = tmp[0] + tmp[1] * 30 + tmp[2] * 900; //여기서 get_tagid로 했을 때는 fail이었고 바꾸니까 해결됨.
    
    Node* ptr = hashList[hash].next;
    
    int minPrice = 987654321;
    Node* tmpptr;
    
    while(ptr){
        Product *prod = ptr->prod;
        if(prod->active == 1){
            if(prod->price < minPrice){
                minPrice = prod->price;
                tmpptr = ptr;
            }
        }
        ptr = ptr->next;
    }
    
    if(minPrice != 987654321){
        tmpptr->prod->active = 0;
    }
    else{
        return -1;
    }
    
    return minPrice;
}

void adjustPrice(char tag1[], int changePrice){
    int hash = get_tagid(tag1);
    Node* ptr = hashList[hash].next;
    
    while(ptr){
        ptr->prod->price += changePrice;
        ptr = ptr->next;
    }
}
