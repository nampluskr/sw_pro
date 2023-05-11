## Reference Codes

### 시간 측정

```cpp
#include <time.h>

clock_t start = clock();

// codes

int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
printf(">> Result: %d ms\n", result);
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

### 컨테이너 자료구제 객체 저장 및 활용 예제

```cpp
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>
#include <set>

using namespace std;

struct User {
	int ID;
	string name;
	int age;
	int score;
	int state;		// added 1, deleted 0

	// 우선순위 큐를 위한 우선순위 정의 (1. 점수가 클수록 2. 나이가 많을수록 3. ID 가 적을수록)
	bool operator()(const User& u1, const User& u2) {
		return make_tuple(u1.score, u1.age, -u1.ID) < make_tuple(u2.score, u2.age, -u2.ID);
		//if (u1.score < u2.score)
		//	return true;
		//if (u1.score == u2.score && u1.age < u2.age)
		//	return true;
		//if (u1.score == u2.score && u1.age == u2.age && u1.ID > u2.ID)
		//	return true;
		//return false;
	}

	// Set 에 저장하기 위한 비교 연산자 정의
	bool operator<(const User& u) const {
		return make_tuple(this->score, this->age, -this->ID) <
			make_tuple(u.score, u.age, -u.ID);
	}
};

// pair 를 위한 해시 함수 정의
struct key_hash {
	size_t operator()(const pair<int, int>& p) const {
		return p.first ^ p.second;
	}
};

unordered_map<int, int> userMap;
vector<User> users;

priority_queue<User, vector<User>, User> usersPQ;
unordered_map<pair<int, int>, vector<int>, key_hash> selectedUsers;
set<User> usersDEPQ;


void init()
{
	userMap.clear();
	users.clear();
	selectedUsers.clear();

	while (not usersPQ.empty())
		usersPQ.pop();

	usersDEPQ.clear();
}

void addUser(int mID, string mName, int mAge, int mScore)
{
	//// 컨테이너에 User 객체 추가 (ID 중복 사전 체크 필요 없을 때)
	//userMap[mID] = users.size();
	//users.emplace_back(User{ mID, mName, mAge, mScore, 1 });

	// 컨테이너에 User 객체 추가 (ID 중복 사전 체크 해야할 때)
	int userIdx;
	auto ret = userMap.find(mID);

	if (ret == userMap.end()) {
		userIdx = users.size();
		userMap[mID] = userIdx;
		users.emplace_back(User{});
	}
	else
		userIdx = ret->second;

	// 해당 인덱스의 User 객체 정보 입력
	users[userIdx].ID = mID;
	users[userIdx].name = mName;
	users[userIdx].age = mAge;
	users[userIdx].score = mScore;
	users[userIdx].state = 1;		// added
}

void delUser(int mID)
{
	users[userMap[mID]].state = 0;	// deleted
}

void print(User& u)
{
	printf(">> [ID: %d] Name: %s, Age: %d, Score: %d, State: %d\n",
		u.ID, u.name.c_str(), u.age, u.score, u.state);
}

int main()
{
	init();

	addUser(101, "Kim5", 10, 300);
	addUser(201, "Kim4", 30, 300);
	addUser(301, "Kim3", 30, 300);
	addUser(401, "Kim2", 50, 500);
	addUser(501, "Kim1", 50, 500);
	addUser(601, "Kim6", 50, 500);

	for (auto& u : users) {
		int userIdx = userMap[u.ID];

		// 우선순위 큐 대입 (객체의 복사본 저장됨 - 동기화 안됨)
		usersPQ.emplace(u);

		// 부분탐색 위한 링크드 리스트 대입 (객체의 인덱스를 저장)
		selectedUsers[make_pair(u.score, u.age)].emplace_back(userIdx);

		// 이중 우선순위 큐 대입 (객체의 복사본 저장됨 - 동기화 안됨)
		usersDEPQ.emplace(u);
	}

	delUser(401);
	delUser(201);

	// 입력 순서대로 출력
	printf("[users] Num. of users: %zd\n", users.size());
	for (auto& u : users) {
		print(u);
	}

	// 우선순위 높은 순서로 출력 (state == 1)
	printf("\n[usersPQ] Num. of users: %zd\n", usersPQ.size());
	while (not usersPQ.empty()) {
		User u = usersPQ.top(); usersPQ.pop();
		int userIdx = userMap[u.ID];
		if (users[userIdx].state == 1) {
			print(u);
			//delUser(u.ID);
		}
	}
	printf("[usersPQ] Num. of users: %zd\n", usersPQ.size());

	// 링크드 리스트 출력 (state == 1)
	printf("\n[selectedUsers] Num. of users: %zd\n", selectedUsers.size());
	for (auto& u : selectedUsers) {
		auto& key = u.first;
		printf("(%d, %d): \n", key.first, key.second);
		for (auto& userIdx : u.second) {
			if (users[userIdx].state == 1) {
				print(users[userIdx]);
				//delUser(users[userIdx].ID);
			}
		}
	}

	// 이중 우선순위 큐에서 최소값 출력 (state == 1)
	printf("\nGet min. user:\n");
	while (not usersDEPQ.empty()) {
		auto u = *(usersDEPQ.begin());	usersDEPQ.erase(usersDEPQ.begin());
		int userIdx = userMap[u.ID];
		if (users[userIdx].state == 1) {
			print(users[userIdx]);
			//delUser(u.ID);
			break;
		}
	}
	printf("[usersDEPQ] Num. of users: %zd\n", usersDEPQ.size());

	// 이중 우선순위 큐에서 최대값 출력 (state == 1)
	printf("\nGet min. user:\n");
	while (not usersDEPQ.empty()) {
		auto u = *(--usersDEPQ.end());	usersDEPQ.erase(--usersDEPQ.end());
		int userIdx = userMap[u.ID];

		if (users[userIdx].state == 1) {
			print(users[userIdx]);
			//delUser(u.ID);
			break;
		}
	}
	printf("[usersDEPQ] Num. of users: %zd\n", usersDEPQ.size());

	return 0;
}
```
