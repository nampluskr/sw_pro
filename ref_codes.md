## Reference Codes

### 시간 측정

```cpp
#include <time.h>

clock_t start = clock();

// codes

int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
(">> Result: %d ms\n", result);
```

### 올림 함수

```cpp
int ceil(int a, int b) {
  int result = a / b;
  if (a % b > 0)
    result += 1;
  return result;
}
```

### 이중 우선순위 큐 (Double Ended Priority Queue)

```cpp

```
