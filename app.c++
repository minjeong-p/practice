#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Product{
    int pid;
    int price;
    bool active;
    Product* next;
    
}pList[30000];

Product* tagList[27000];

unordered_map<char*, int> taginfo;

int tidx = 0;
int pidx = 0;

int get_tagid(char name[]){
    if(!taginfo[name]){
        taginfo.insert({name, tidx++});
    }
    return taginfo[name];
}

void init(int N){
    taginfo.clear();
    tidx = 0;
    pidx = 0;
    for(register int i = 0; i<30000; i++){
        pList[i].next = nullptr;
    }
}


void addProduct(int mPrice, int tagNum, char tagName[][10]){
    Product* ptr = &pList[pidx];
    ptr->pid = pidx;
    ptr->price = mPrice;
    ptr->active = true;
    pidx++;
    
    vector<int> tags(tagNum);
    for(register int i = 0; i < tagNum; i++){
        tags[i] = get_tagid(tagName[i]);
    }
    sort(tags.begin(), tags.end());
    
    int hash = 0;

    for(register int i = 0; i < tagNum ; i++){
        tagList[tags[0]] = &pList[pidx];
        for(register int j = i + 1; j < tagNum; j++){
            for(register int k = j + 1; k < tagNum; k++){
                hash = tags[i] + tags[j] * 30 + tags[k] * 900;
            }
        }
        ptr->next = tagList[hash];
        tagList[hash] = &pList[pidx];
        
    }
}

int buyProduct(char tag1[], char tag2[], char tag3[]){
    vector<int> tmp;
    tmp.push_back(get_tagid(tag1));
    tmp.push_back(get_tagid(tag2));
    tmp.push_back(get_tagid(tag3));
    sort(tmp.begin(), tmp.end());
    
    int hash = get_tagid(tag1) + get_tagid(tag2) * 30 + get_tagid(tag3) * 900;
    
    Product* ptr = tagList[hash];
    
    int minPrice = 987654321;
    int pid = -1;
    Product* tmpptr = nullptr;
    while(ptr){
        if(ptr->active){
            if(ptr->price < minPrice){
            minPrice = ptr->price;
            pid = ptr->pid;
            tmpptr = ptr;
            }
        }
        ptr = ptr->next;
    }
    
    if(minPrice != 987654321){
        tmpptr->active = false;
    }
    else{
        return -1;
    }
    
    return minPrice;
}

void adjustPrice(char tag1[], int changePrice){
    int hash = get_tagid(tag1);
    Product* ptr = tagList[hash];
    
    while(ptr){
        ptr->price += changePrice;
        ptr = ptr->next;
    }
}
