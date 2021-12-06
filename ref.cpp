#include <string.h>
N = 5;
M = 4;
int** arr = new int* [N];
for (register int i = 0; i < N; i++) {
	arr[i] = new int[M];
	memset(arr[i], 0, sizeof(arr[i]));
}
