#define rint register int
#define MAX_TABLE 40001
#define MAXN 20001 //왜 두개가 다른가?

using namespace std;

struct Node {
	int id;
	int key;
	int start;
	int size;
	Node* prev;
	Node* next;
}NODE[MAXN];

int nidx;
Node* nhead, *ntail;

Node* nalloc() {
	return &NODE[nidx++];
}

struct Empty {
	int start;
	int size;
	Empty* prev;
	Empty* next;
}EMPTY[MAXN];

int eidx;
Empty* ehead, *etail;

Empty* ealloc() {
	return &EMPTY[eidx++];
}

Node* htable[MAX_TABLE];
int link_ht[MAX_TABLE];

int total_use;

//!이 함수들은 해시충돌을 막기 위해 필요
int find(int id) {
	unsigned long h = id % MAX_TABLE;
	int cnt = MAX_TABLE;

	while (link_ht[h] != -1 && cnt--) { //끝까지 찾았는데도 없다면 -1을 리턴해야 함
		//과연 여기에서 만약에 h값이 넘어선다면..? 아니다 나머지로 되어있으니까 넘어설 일은 없음
		//최대로 탐색해도 cnt번 탐색하니까 이 while문 조건을 넘어서지 않음
		if (link_ht[h] == id) {
			return h; //찾으면 hash값을 리턴
		}

		h = (h + 1) % MAX_TABLE;//못찾으면 즉 collision이 생길 경우 계속 다음 칸으로 이동함
	}

	return -1;
}

int add(int id) {
	unsigned long h = id % MAX_TABLE;

	while (link_ht[h] != -1) { //값이 없는 칸을 만날 때까지
		if (link_ht[h] == id) {//만약에 기존에 이 id를 가지고 있는 칸을 만난다면, 즉 원래 있던 값이 add되었다면
			return h;//그냥 이 값을 리턴한다.
		}

		h = (h + 1) % MAX_TABLE;//계속 값이 있다면(collision) 다음 칸으로 이동함
	}
	//값이 없는 칸을 만났다면 거기에 집어넣으면 됨
	link_ht[h] = id;
	return h;
}

void init(int N) {
	total_use = 0;
	nidx = eidx = 0;
	nhead = nalloc();
	ntail = nalloc();
	nhead->next = ntail;
	ntail->prev = nhead;

	ehead = ealloc();
	etail = ealloc();
	ehead->next = etail;
	etail->prev = ehead;

	register Empty* e = ealloc(); //!register 넣어준다
	e->start = 0;
	e->size = N;

	e->next = etail;
	e->prev = ehead;
	e->next->prev = e;
	e->prev->next = e;

	for (rint i = 0; i < MAX_TABLE; i++) {
		htable[i] = 0; //!이 테이블도 초기화 필요 
		link_ht[i] = -1;
	}

	return;
}

int access(int fileId, int fileSize) {
	rint hash = find(fileId);
	register Node* node = 0; //!미리 여기서 선언해주고
	
	if (hash != -1) { // 파일이 기존에 존재할 경우 접근 업데이트만 하고 끝
		node = htable[hash];

		node->next->prev = node->prev;
		node->prev->next = node->next;

		node->next = ntail;
		node->prev = node->next->prev;
		node->next->prev = node;
		node->prev->next = node;

		return node->start;
		/*
		* Node* n = nhead->next;
		while (1) {
			if (n && node->id == n->id) {
				node->next->prev = node->prev;
				node->prev->next = node->next;

				node->next = ntail;
				node->prev = node->next->prev;
				node->next->prev = node;
				node->prev->next = node;

				return node->start;
			}
			else {
				n = n->next;
			}
		}
		!이걸 찾을 필요가 없음 node니까 기존 배열에서 가져왔어도 next, prev정보는 링크드 리스트 정보임
		*/

	}
	else { // 파일이 존재하지 않을 경우 새롭게 넣어야 함
		//! 해쉬테이블하고 배열에다가만 새롭게 할당해주고, 그 할당값을 토대로 어디다 넣을지는 다시 찾아봄
		node = nalloc();
		node->size = fileSize;
		node->id = fileId;
		int key = add(fileId); //여기서 할당한 다음 불러와야 하나? -> id-key 테이블에 추가한다.
		node->key = key;

		htable[key] = node; //key-hash table에도 업데이트
	}

	//이제 파일은 존재한다. 테이블에 존재하고 캐쉬에는 들어가지 않은 상태
	//빈 공간을 돌면서 넣을 수 있는지 확인해야 함
	while (1) {
		Empty* e = ehead->next;

		while (e && e->size < fileSize) {//빈칸을 모두 확인해 보면서 들어갈 공간을 찾아야 함
			//!여기 e로 하는게 맞는지 확인 -> 맞긴 한데, 먼저 찾고 그 다음에 로직 진행
			//!만약에 넣을 수 없다면 다음 빈칸으로 넘어가는 방식
			e = e->next;
		}
		if (e) {
			//쭉 돌아보았을때 할당이 가능할 경우
			node->start = e->start;

			node->next = ntail;
			node->prev = node->next->prev;
			node->next->prev = node;
			node->prev->next = node;

			total_use += node->size;

			if (node->size < e->size) { //만약 들어갈 게 더 작다면
				e->start += node->size;
				e->size -= node->size;
			}
			else if (node->size == e->size) { //크기가 같은 노드가 있다면

				e->next->prev = e->prev;
				e->prev->next = e->next;
			}
			return node->start;

		}
		else {
			//만약 들어갈 공간이 마땅치 않을 경우 오래된 파일 하나를 삭제해야 함
			Node* edel = nhead->next;

			//이 노드 앞 뒤에 있는 빈 공간을 따로 가지고 있어야 함
			Empty* e = ehead->next;
			while (e && e != etail) {
				if (e->start >= edel->start) {
					break;
				}
				e = e->next;
			}

			Empty* eadd = ealloc();
			eadd->size = edel->size;
			eadd->start = edel->start;
			eadd->next = e;
			eadd->prev = eadd->next->prev;
			eadd->next->prev = eadd;
			eadd->prev->next = eadd;

			if (eadd->prev->size && ((eadd->prev->start + eadd->prev->size) == eadd->start)) {
				/*
				* eadd->prev로 합치면 삭제 후에 해당 노드에 접근할 방법이 없으므로,
				* eadd에 이전 노드를 합치는 방식으로 해야 함
				eadd->prev->size += eadd->size;
				
				eadd->prev->next = eadd->next;
				eadd->next->prev = eadd->prev;
				*/
				eadd->start = eadd->prev->start;
				eadd->size += eadd->prev->size;

				register Empty* e_prev = eadd->prev;
				e_prev->prev->next = e_prev->next;
				e_prev->next->prev = e_prev->prev;

			}

			if (eadd->next->size && ((eadd->start + eadd->size) == eadd->next->start)) {
				eadd->size += eadd->next->size;

				register Empty* e_next = eadd->next;
				e_next->next->prev = e_next->prev;
				e_next->prev->next = e_next->next;
			}
			//공간을 이제 하나로 다 합쳤음
			//이제 빈 공간을 다시 찾으면 됨

			total_use -= eadd->size;

			//!오래된 캐쉬를 아직 삭제하지 않았음
			edel->next->prev = edel->prev;
			edel->prev->next = edel->next;

			//!캐쉬가 삭제되었으므로 해쉬 키도 -1로 초기화한다.
			link_ht[edel->key] = -1;
			htable[edel->key] = 0;

		}

		return 0;

	}
}

int usage() {
	return total_use;
}
