#define MAX_LEN 40001
#define MAX_N 20001
#define rint register int 

using namespace std;

struct File{
    int id;
    int key;
    int start;
    int size;
    File* prev;
    File* next;
}file[MAX_LEN];

int fidx = 0;

File* fhead, * ftail;

struct Empty{
    int start;
    int size;
    Empty* prev;
    Empty* next;
}empty[MAX_LEN];

int eidx = 0;

Empty* ehead, * etail;

File* htable[MAX_LEN];
int table[MAX_LEN];

int cacheN;
int total_use = 0;

int find(int key){
    unsigned long h = key % MAX_LEN;
    int cnt = MAX_LEN;

    while(table[h] != -1 && cnt--){
        if(table[h] == key){
            return h;
        }
        h = (h+1) % MAX_LEN;
    }
    return -1;
}

int add(int key){
    unsigned long h = key % MAX_LEN;
    while (table[h] != -1){
        if(table[h] == key){
            return h;
        }
        h = (h+1) % MAX_LEN;
    }
    table[h] = key;
    return h;
} // 해시 값을 넣으면 key값을 갖고 있는 배열을 갖고 있음
//이 배열이 왜 필요할까? key값을 알고 있어야 할까? collision 이 나지 않기 위해?

void init(int N){
    cacheN = N;
    fidx = 0;
    eidx = 0;
    total_use = 0;
    for(rint i = 0; i < MAX_LEN; i++){
        htable[i] = 0;
        table[i] = -1;
    }

    fhead = &file[fidx++];
    ftail = &file[fidx++];
    fhead->next = ftail;
    ftail->prev = fhead;

    ehead = &empty[eidx++];
    etail = &empty[eidx++];
    ehead->next = etail;
    etail->prev = ehead; //head, tail은 더미 노드

    register Empty *e = &empty[eidx++];
    e->size = N;
    e->start = 0;

    //dll 체크
    e->next  = etail;
    e->prev = e->next->prev;
    e->next->prev = e;
    e->prev->next = e;


    return;

}

int access(int fileId, int fileSize){
    //int hash = fileId % MAX_LEN;
    rint key = find(fileId);
    register File *f = 0;
    if(key == -1){ // 현재 없는 경우 -> 없는 경우를 명확히 하기 위해서 key라는 배열을 만들어 낸 것?
        key = add(fileId); //해쉬값을 찾아서 넣고, 배열에도 하나 새롭게 넣는다. 
        File* f = &file[fidx++];
        f->id = fileId;
        f->size = fileSize;

        Empty* now = ehead;
        while(now != nullptr){
            if(now->size >= fileSize){//크기가 충분한 빈 공간이 있다면
                f->start = now->start;
                now->start += f->size;
                now->size -= f->size;

                //추가함
                ftail->next = f;
                f->prev = ftail;
                f->next = nullptr;
                ftail = f;

                total_use += f->size;
            }
            else{
                now = now->next;
            }
        }//만약 이렇게 계속 돌았는데도 없다면
//없다는 것 조건문 필요함
        //file 내역에서 맨 앞 노드 삭제
        File* deleted = fhead;
        fstart->next->prev = nullptr;
        fstart = fstart->next;

        //File 내역에서 해당 위치를 empty 에 추가


    }
    else{ // 현재 있는 경우 조회 순서만 최신화
        File* target = htable[hash];
        File* now = fstart;
        while(now != nullptr){
            if(now->id == target->id){
                //이 파일을 삭제하고 맨 뒤에 새로 추가함
                break;
            }
            else{
                now = now->next;
            }
        }
        
    }
}

int usage(){
    return total_use;
}


