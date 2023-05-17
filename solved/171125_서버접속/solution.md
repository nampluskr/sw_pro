# 171125_서버접속

## [최종]

```cpp
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define LOGIN	1
#define LOGOUT	0

struct User {
	string id;
	string password;
	int defaultTime;

	int state;
	int logoutTime;			// 로그아웃 예상시각 = currentTime + defaultTime
};

unordered_map<string, int> userMap;
vector<User> users;

int currentTime;
unordered_map<int, vector<int>> logoutUsers;


/////////////////////////////////////////////////////////////
void Init()
{
	userMap.clear();
	users.clear();

	currentTime = 0;
	logoutUsers.clear();
}

void NewAccount(char id[11], char password[11], int defaulttime)
{
	int uIdx = users.size();
	userMap[string(id)] = uIdx;
	users.emplace_back(User{});

	users[uIdx].id = string(id);
	users[uIdx].password = string(password);
	users[uIdx].defaultTime = defaulttime;
	users[uIdx].logoutTime = currentTime + users[uIdx].defaultTime;
	users[uIdx].state = LOGIN;

	logoutUsers[users[uIdx].logoutTime].emplace_back(uIdx);
}

void Logout(char id[11])
{
	int uIdx = userMap[string(id)];

	if (users[uIdx].state == LOGIN) {
		users[uIdx].state = LOGOUT;
	}
}

void Connect(char id[11], char password[11])
{
	int uIdx = userMap[string(id)];

	if (users[uIdx].state == LOGIN && users[uIdx].password == string(password)) {
		users[uIdx].logoutTime = currentTime + users[uIdx].defaultTime;
		logoutUsers[users[uIdx].logoutTime].emplace_back(uIdx);
	}
}

int Tick()
{
	int logoutUserCnt = 0;
	currentTime += 1;

	// 부분 탐색
	for (auto uIdx : logoutUsers[currentTime]) {
		if (users[uIdx].state == LOGIN && users[uIdx].logoutTime == currentTime) {
			users[uIdx].state = LOGOUT;
			logoutUserCnt++;
		}
	}

	// 완전 탐색
	//for (auto& u : users) {
	//	if (u.state == LOGIN && u.logoutTime == currentTime) {
	//		u.state = false;
	//		logoutUserCnt++;
	//	}
	//}
	return logoutUserCnt;
}
```
