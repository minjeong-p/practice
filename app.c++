#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Product{
    int pid;
    int price;
    vector<int> tag;
    Product* next;
    
}pList[30000];

Product* tagList[27000];

unordered_map<char*, int> taginfo;

int tidx = 0;
int get_tagid(char tag[]){
    if(!taginfo[tag]){
        taginfo.insert({tag, tidx++});
    }
    return taginfo[tag];
}

void init(){
    for(register int i = 0; i<30000; i++){
        pList[i].next = nullptr;
    }
}

int pidx = 0;

void addProduct(int mPrice, int tagNum, char tagName[][10]){
    Product* ptr = pList[pidx];
    ptr->pid = pidx;
    ptr->price = mPrice;
    
    vector<int> tags;
    for(register int i = 0; i < tagNum; i++){
        tags.push_back(get_tagid(tagName[i]);
    }
    sort(tags.begin(), tags.end());
    ptr->tag = tags;
    
    int hash = 0;
    
    ptr->next = tagList[tags[0]];
    tagList[tags[0]] = &pList[pidx];

    for(register int i = 0; i < tagNum ; i++){
        for(register int j = i + 1; j < tagNum; j++){
            for(register int k = j + 1; k < tagNum; k++){
                hash = tags[i] + tags[j] * 30 + tags[k] * 900;
            }
        }
        ptr->next = tagList[hash];
        tagList[hash] = &pList[pidx];
        //put tag in Product struct?
    }
}

int buyProduct(char tag1[], char tag2[], char tag3[]){
    vector<int> tmp;
    tmp.push_back(get_tagid(tag1));
    tmp.push_back(get_tagid(tag2));
    tmp.push_back(get_tagid(tag3));
    
    int hash = get_tagid(tag1) + get_tagid(tag2) * 30 + get_tagid(tag3) * 900;
    
    ptr = tagList[hash];
    
    int minPrice = 987654321;
    int pid = -1;
    Product* tmpptr = nullptr
    while(ptr){
        if(ptr->price < minPrice){
            minPrice = ptr->price;
            pid = ptr->pid;
            tmpptr = ptr;
        }
        ptr = ptr->next;
    }
    
    //delete sold product
    
    return minPrice;
}

void adjustPrice(char tag1[], int changePrice){
    int hash = get_tagid(tag1)
    Product* ptr = tagList[hash];
    
    while(ptr){
        ptr->price += changePrice;
    }
}
