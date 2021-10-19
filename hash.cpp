#include <iostream>
#include<cstring>
using namespace std;

#define lli long long int

int VAL = 31;
int MAX_LEN = 100001;

struct Node {
    int data;
    char* keyname;
    Node* next;
};

Node* node = new Node[VAL];
Node** table = new Node * [MAX_LEN];
int nodeidx = 0;

void init() {
	for (register int i = 0; i < MAX_LEN; i++) {
        table[i] = nullptr;
	};
};

int hash_function(char key[]) {
	lli hash = 0;
	int stl = strlen(key);
	for (register int i = 0; i < stl; i++) {
		hash = (hash + (key[i] - 'a' + 1)) * VAL; //앞 세 글자만 해싱해서 collision의 경우 하나씩 비교하는 시스템으로 해도 되는건가?
	};
	return hash % MAX_LEN; //가능한지?
}

void add(char key[], int data) {
	lli val = hash_function(key);
	val %= MAX_LEN;
	
	Node* ptr = &node[nodeidx++];
	ptr->data = data;
	ptr->next = 0;
    strcpy(ptr->keyname, key);
    if (!table[val]) {
        table[val] = ptr;
    }
    else {
        ptr->next = table[val];
        table[val] = ptr;
    }
}

int find(char key[]) {
	lli val = hash_function(key);
	val %= MAX_LEN;
	Node* ptr = table[val];
	while (ptr) {
		if (ptr->data != val) {
			return ptr->data;
		}
        ptr = ptr->next;
	}
}

void remove(char key[]) {
    lli val = hash_function(key);
    val %= MAX_LEN;
    Node* ptr = table[val];
    while (ptr) {
        if (strcmp(ptr->keyname, key)) {
            ptr->next;
        }

    }
}

/*
#define MAX_SIZE 100001
#define RINT register int
typedef unsigned long long ull;

struct Node{
    ull key;
    int data;
    Node* next;
};

Node node_db[MAX_SIZE];
int node_idx;

Node* hash_table[MAX_SIZE];

void init() {
    node_idx = 0;
    for (RINT i = 0; i < MAX_SIZE; i++) {
        hash_table[i] = nullptr;
    }
}

void add(char key[], int data) {
    register ull hash = 0;
    while (*key) {
        hash = (hash << 5) + (*key++ - 'a' + 1);
    }
    int idx = hash % MAX_SIZE;

    register Node* ptr = &node_db[node_idx++];
    ptr->data = data;
    ptr->key = hash;

    if (!hash_table[idx]) {
        hash_table[idx] = ptr;
    }
    else {
        ptr->next = hash_table[idx];
        hash_table[idx] = ptr;
    }
}

int find(char key[]) {
    register ull hash = 0;
    while (*key) {
        hash = (hash << 5) + (*key++ - 'a' + 1);
    }
    int idx = hash % MAX_SIZE;
    register Node* ptr = hash_table[idx];

    while (ptr) {
        if (ptr->key == hash) {
            return ptr->data;
        }
        ptr = ptr->next;
    }
}
*/
