#define MAXN 20001
#define MAX_TABLE 40001
#define rint register int
 
//캐쉬
struct NODE {
 
    int id, key, st, size;
    NODE *prev, *next;
}node[MAXN];
int nidx;
/// 캐시 자체를 linked list로 구현해야 함
 
NODE *phead, *ptail; ///캐시에 접근하기 위해서 만든 포인터?
NODE *htable[MAX_TABLE]; //캐쉬 저장
 
NODE *myalloc() {
    return &node[nidx++];
}
 
//빈공간
struct EMPTY {
 
    int st, size;
    EMPTY *prev, *next;
}empty[MAXN];
int eidx;
/// 빈 노드도 따로 만들어?
 
EMPTY *ehead, *etail;
 
EMPTY *emyalloc() {
    return &empty[eidx++];
}
 
int nn;
int totaluse;
 
typedef struct
{
    int key;
}Hash;
Hash tb[MAX_TABLE]; ///결국 int list와 같다는 뜻
 
int find(int key)
{
    unsigned long h = key % MAX_TABLE;
    int cnt = MAX_TABLE;
 
    while (tb[h].key != -1 && cnt--)
        {
        if (tb[h].key == key)
        {
            return h;
        }
        h = (h + 1) % MAX_TABLE;///다 이동하면서 찾아봄
    }
    return -1;
}
 
int add(int key)
{
    unsigned long h = key % MAX_TABLE;
 
    while (tb[h].key != -1)
    {
        if (tb[h].key == key)
        {
            return h;
        }
 
        h = (h + 1) % MAX_TABLE; /// 다음 칸에 저장하는 방식으로 구현되어 있음. open addressing 아님.
    }
    tb[h].key = key; ///key만 저장되어 있는 해시 테이블. key를 가지고 빠르게 찾을 수 있도록 이렇게 구현
    return h;
}
 
void init(int N) {
 
    nn = N;
    nidx = eidx = totaluse = 0;
 
    //해쉬테이블 초기화, 캐쉬 저장 초기화
    for (rint i = 0; i < MAX_TABLE; i++) {
        htable[i] = 0;
        tb[i].key = -1;
    }
 
    //캐쉬 리스트 초기화
    phead = myalloc();
    ptail = myalloc();///새롭게 할당하는 함수
    phead->next = ptail;
    ptail->prev = phead;///새롭게 head, tail 포인터를 할당, 두 개가 붙어 있는 상태로 할당
 
    //빈공간 초기화
    ehead = emyalloc();
    etail = emyalloc();///새롭게 할당하는 함수
    ehead->next = etail;
    etail->prev = ehead;///위와 동일
 
    //시작시 빈공간에 0 ~ N 까지의 공간을 하나 넣는다.
    register EMPTY *e = emyalloc();///하나 할당하고 싶을 때 편하게 할당하기 위해서 함수화함
    e->st = 0;
    e->size = N;///빈공간의 사이즈가 N인 한 칸을 만든다는 뜻. 나중에 캐시가 생기게 되면, 캐시와 빈 공간을 끊어서 관리하기 위해. 
    /// empty는 빈공간을 의미하며 node는 데이터가 있는 공간을 의미함. 빈 공간 N에서 데이터가 저장되면 데이터 s와 빈 공간 N-s 로 두 노드가 생기게 됨
 
    e->next = etail;
    e->prev = e->next->prev;
    e->next->prev = e;
    e->prev->next = e;
 
    return;
}
 
int access(int fileId, int fileSize) {
 
    rint key = find(fileId);   //기존에 있는 id 인지 체크
    register NODE *p = 0;
    if (key == -1) {       //없는 id면 캐쉬에 추가
 
        key = add(fileId);  //해쉬값 찾음
        p = myalloc();
        p->id = fileId;
        p->key = key;
        p->size = fileSize; ///새롭게 하나 할당(p)
 
        htable[key] = p;  //캐쉬 저장 in htable
        ///htable은 캐쉬를 저장하는 해시테이블, key번째 인수에 p를 저장
        ///아 기존에 있는 id인지 이미 체크하는구나
    }
    else {     //기존에 존재하는 id 인 경우
        ///Q. 기존에 있었던 경우 id%MAX_TABLE 이 유니크하다고 보장할 수 없지 않나?
        ///Q. 만약에 같은 key를 갖는 새로운 id가 주어졌을 경우 (ex. 10과 40011) 기존 걸 삭제해버린다고?
 
        p = htable[key];   //캐쉬를 찾아서 기존 리스트에서 삭제
        /// htable에 캐쉬를 저장해 둔 것은 그대로 유지,
        /// phead~ptail 링크드리스트에서 p를 삭제
        p->next->prev = p->prev;
        p->prev->next = p->next;
 
        //삭제하고 나서 시간 순대로 tail 에 붙인다.
        //가장 오래된 캐쉬는 head->next 에 있는 노드
        ///이 p를 다시 ptail과 기존 p 사이에 끼워 넣는 구성으로, 새롭게 한번 접속했으므로 맨 마지막에 넣는다고 생각하면 됨. 이건 특별히 htable에 들어가지 않음. 새로운 노드가 아니니까.
        p->next = ptail;///ptail은 위치는 node[1]에 있고 p는 node[n]에 있지만 next가 ptail이 되면 linked list구성됨 
        p->prev = p->next->prev;
        p->next->prev = p;
        p->prev->next = p;
 
        return p->st; ///여기서 끝내버림!
    }
 
    //빈공간을 확보하고 캐쉬를 저장한다.
    //빈공간이 부족하면 가장 오래된 캐쉬를 삭제하고 재확인을 반복한다.
    while (1) {
 
        //저장 할 빈공간이 있는 지 빈공간 리스트를 순회하면서 체크
        register EMPTY *e = ehead->next;
        while (e && e->size < fileSize) { ///넣을 수 있는 경우라면 이 조건을 만족하지 않으므로 OUT
            e = e->next;///넣을 수 없는 경우
        }
 
        //만약 빈공간이 할당 가능하면
        if (e) { ///넣을 수 있는 e에 대해서. e는 empty 포인터이므로 있다면 empty노드가 있다는 뜻.
 
            totaluse += fileSize;
            p->st = e->st;       //빈공간의 시작이 캐쉬의 시작
 
            p->next = ptail;     //캐쉬는 가장 tail 에 시간순으로 넣는다.
            /// 이 phead, ptail의 경우 캐쉬의 입력 순서를 관리하기 위해 사용함. 더블 포인터 링크드 리스트로 앞에서 삭제하고 뒤에 추가하는 기능을 수행.
            /// 이걸 통해서 재접속한 노드의 경우 새롭게 끼워넣을 수 있음.
            p->prev = p->next->prev;
            p->next->prev = p;
            p->prev->next = p;
 
            if (e->size > fileSize) {  //빈공간이 저장될 캐쉬보다 크다면
 
                e->st += fileSize;
                e->size -= fileSize;///empty의 스타트와 사이즈만 바꿔주면 됨. 어차피 한 묶음으로 관리하기 때문에.
            }
            else {  //빈공간이 저장될 캐쉬와 같은 경우
                     //빈공간은 삭제된다.
                e->next->prev = e->prev;
                e->prev->next = e->next;
            }
            return p->st;///할당 성공했으니 할당 성공한 시작점을 리턴하면 끝
        }
        else {   //빈공간에 없다면 오래된 캐쉬 삭제후에 추가
 
            register NODE *edel = phead->next; //가장 오래된 캐쉬(edel)를 찾는다. 
            /// phead에 바로 달려있는 노드가 제일 오래된 노드임.
            if (edel) {   //있다면...
 
                register EMPTY *iter = ehead->next;
                while (iter && iter != etail) { ///빈 공간을 순서대로 보면서 제일 오래된 노드 뒤에 붙어 있는 빈 공간을 찾는다
 
                    if (edel->st <= iter->st)   //앞에서 부터 찾는다.
                        break;
 
                    iter = iter->next;
                }
 
                //빈공간 노드를 만들어서 추가한다.
                ///제일 오래된 노드 뒤에 붙어 있는 빈 공간을 찾은 다음(iter)
                ///지금 찾은 제일 오래된 노드와 동일한 크기의 빈 노드를 만들고(eadd)
                register EMPTY *eadd = emyalloc();
                eadd->st = edel->st;
                eadd->size = edel->size;
                
                ///eadd를 iter앞에 붙이면 이 만큼의 빈 공간이 추가된다
                eadd->next = iter;  //순서대로 iter 앞에...
                eadd->prev = eadd->next->prev;
                eadd->next->prev = eadd;
                eadd->prev->next = eadd;
 
                //빈공간의 이전 시작 + 크기 와 현재의 시작 이 같은 경우
                //이전의 정보를 현재로 옮기고, 이전 공간은 삭제.
                if (eadd->prev->size && ((eadd->prev->st + eadd->prev->size) == eadd->st)) {
 
                    eadd->st = eadd->prev->st;
                    eadd->size += eadd->prev->size;
 
                    register EMPTY *eadd_prev = eadd->prev;
                    eadd_prev->next->prev = eadd_prev->prev;
                    eadd_prev->prev->next = eadd_prev->next;
                }
 
                //추가할 빈공간 시작  + 크기 가 다음 빈공간의 시작 이 같은 경우
                //다음의 정보를 현재로 옮기고, 다음 공간은 삭제.
                if (eadd->next->size && ((eadd->st + eadd->size) == eadd->next->st)) {
 
                    eadd->size += eadd->next->size;
 
                    register EMPTY *eadd_next = eadd->next;
                    eadd_next->next->prev = eadd_next->prev;
                    eadd_next->prev->next = eadd_next->next;
                }
                /// 빈 공간을 하나로 합치게 됨
 
                totaluse -= edel->size;
 
                //오래된 캐쉬 삭제
                edel->next->prev = edel->prev;
                edel->prev->next = edel->next;
 
                //캐쉬가 삭제되었으므로 해쉬 키로 -1 로 초기화 한다.
                tb[edel->key].key = -1;

                ///여기서는 삭제까지만 하고, 다시 원점으로 돌아가서 할당 가능한 곳에 할당하면 된다.
            }
            else {
                break; ///빈공간도 없고 오래된 노드도 없을 경우는 존재하지 않으므로 break
            }
        }
    }
 
    return 0;
}
 
int usage() {
 
    return totaluse;
}
