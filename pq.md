
## 매뉴얼 우선순위큐 (Min. Heap) 수정 항목

1. struct PriorityQue {}; 로 감싸기
2. '<' 비교 대상 바꾸기 (3 군데)
3. Data {ID, value1, value2}, operator<() 정의
4. temp int -> Data (2군데)
5. int HeapPush(int value) -> void push()
6. int HeapPop(* value) -> void pop()
7. Data top() { return heap[0]; }
8. bool empty() { return heapSize == 0; }

9. inti(): heap.clear(); heapSize=0; 
10. push(): if (heapSize >= heap.size()) heap.push_back({});


```cpp
#include <stdio.h>
#include <vector>

using namespace std;

struct Data {
    int ID, value1, value2;

    bool operator<(const Data& data) const {
        return (this->value1 < data.value1) || 
            (this->value1 == data.value1 && this->value2 > data.value2);
    }
};

struct PriorityQueue {
    vector<Data> heap;
    int heapSize;

    void init()
    {
        heap.clear();
        heapSize = 0;
    }

    void push(const Data& data)
    {
        if (heapSize >= heap.size())
            heap.push_back({});

        heap[heapSize] = data;
        int current = heapSize;

        while (current > 0 && heap[(current - 1) / 2] < heap[current]) {
            Data temp = heap[(current - 1) / 2];
            heap[(current - 1) / 2] = heap[current];
            heap[current] = temp;
            current = (current - 1) / 2;
        }
        heapSize = heapSize + 1;
    }
    void pop()
    {
        heapSize = heapSize - 1;
        heap[0] = heap[heapSize];

        int current = 0;
        while (current * 2 + 1 < heapSize)
        {
            int child;
            if (current * 2 + 2 == heapSize)
                child = current * 2 + 1;
            else
                child = heap[current * 2 + 2] < heap[current * 2 + 1] ? current * 2 + 1 : current * 2 + 2;

            if (heap[child] < heap[current])
                break;

            Data temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;
            current = child;
        }
    }
    Data top() { return heap[0]; }
    bool empty() { return heapSize == 0; }
    vector<int> top(int k)
    {
        vector<int> topk;
        int cnt = 0;
        while (!empty()) {
            Data data = top(); pop();

            // if () {}

            topk.push_back(data.ID);
            cnt += 1;
        }
        for (int id : topk)
            push({ id, 1, 2 });
        return topk;
    }
};

PriorityQueue PQ;

int main()
{
    const int NUM_DATA = 10;
    int arr[NUM_DATA] = { 10, 49, 38, 17, 56, 92, 8, 1, 13, 55, };

    PQ.init();
     
    for (int i = 0; i < NUM_DATA / 2; i++) {
        PQ.push({ i, arr[i], 1});
        //PQ.heapPush(arr[i]);
    }

    while (!PQ.empty()) {
        Data data = PQ.top();  PQ.pop();
        printf("[ID, value1, value2] = [%d, %d, %d], heapSize=%d, heap.size()=%d \n", data.ID, data.value1, data.value2, PQ.heapSize, PQ.heap.size());

        //int data = PQ.heap[0];  PQ.heapPop();
        //printf("%d ", data);
    }

    //////////////////////////////////////////////////////////////////////////
    printf("\n");
    for (int i = 0; i < NUM_DATA; i++) {
        PQ.push({ i, arr[i], 1 });
        //PQ.heapPush(arr[i]);
    }

    while (!PQ.empty()) {
        Data data = PQ.top();  PQ.pop();
        printf("[ID, value1, value2] = [%d, %d, %d], heapSize=%d, heap.size()=%d \n", data.ID, data.value1, data.value2, PQ.heapSize, PQ.heap.size());

        //int data = PQ.heap[0];  PQ.heapPop();
        //printf("%d ", data);
    }

    //////////////////////////////////////////////////////////////////////////
    printf("\n");
    for (int i = 0; i < NUM_DATA; i++) {
        PQ.push({ i, arr[i], 1 });
        //PQ.heapPush(arr[i]);
    }
    for (int i = 0; i < NUM_DATA; i++) {
        PQ.push({ i, arr[i], 1 });
        //PQ.heapPush(arr[i]);
    }

    while (!PQ.empty()) {
        Data data = PQ.top();  PQ.pop();
        printf("[ID, value1, value2] = [%d, %d, %d], heapSize=%d, heap.size()=%d \n", data.ID, data.value1, data.value2, PQ.heapSize, PQ.heap.size());

        //int data = PQ.heap[0];  PQ.heapPop();
        //printf("%d ", data);
    }

    return 0;
}
```
