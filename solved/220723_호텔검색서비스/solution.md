# 220723_호텔검색서비스

## [최종] 데이터 `vector`, 우선순위큐 `튜플`

```cpp
#include <vector>
#include <unordered_map>
//#include <tuple>
#include <algorithm>		// pair 비교 연산자
#include <queue>

using namespace std;
using tuple_type = tuple<int, int, int, int>;
using pq_type = priority_queue<pair<int, int>>;

#define MAX_HOTELS	1001
#define MAX_ROOMS	100001

// mHotelID: 호텔의 ID (1 ≤ mHotelID ≤ 1000)
// mRoomID : 룸 ID (1 ≤ mRoomID ≤ 100,000)

struct Hotel {
	//int ID;
	vector<int> rooms;						// room ID list
};

struct Room {
	int ID;
	tuple_type roomInfo;
	int price;
	//int hotelID;							// hotelID
	vector<pair<int, int>> bookingInfo;		// (checkInDate, checkOutDate)
};

struct key_hash {
	size_t operator() (const tuple_type& k) const {
		return get<0>(k) ^ get<1>(k) ^ get<2>(k) ^ get<3>(k);
	}
};

vector<Hotel> hotels(MAX_HOTELS);	// Hash table key: mHotelID (1 ≤ mHotelID ≤ 1000)
vector<Room> rooms(MAX_ROOMS);		// Hash table key: mRoomID (1 ≤ mRoomID ≤ 100,000)
unordered_map<tuple_type, pq_type, key_hash> roomsFiltered;		// Max. Heap (-price, -ID)

/// ///////////////////////////////////////////////////////
bool checkBookingInfo(int roomID, int checkInDate, int checkOutDate) {
	for (auto& date : rooms[roomID].bookingInfo)
		if (checkOutDate > date.first && date.second > checkInDate)
			return false;
	return true;
}

void init(int N, int mRoomCnt[])
{
	hotels.clear();
	//hotels.reserve(N);
	for (int cnt = 0; cnt < MAX_HOTELS; cnt++)
		hotels.emplace_back(Hotel{});

	rooms.clear();
	//rooms.reserve(MAX_ROOMS);
	for (int cnt = 0; cnt < MAX_ROOMS; cnt++)
		rooms.emplace_back(Room{});

	roomsFiltered.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	//hotels[mHotelID].ID = mHotelID;
	hotels[mHotelID].rooms.emplace_back(mRoomID);

	rooms[mRoomID].ID = mRoomID;
	rooms[mRoomID].roomInfo = make_tuple(mRoomInfo[0], mRoomInfo[1], mRoomInfo[2], mRoomInfo[3]);
	rooms[mRoomID].price = mRoomInfo[4];
	//rooms[mRoomID].hotelID = mHotelID;

	roomsFiltered[rooms[mRoomID].roomInfo].emplace(-rooms[mRoomID].price, -rooms[mRoomID].ID);
}

int findRoom(int mFilter[])
{
	int result = -1;
	int checkInDate = mFilter[0];
	int checkOutDate = mFilter[1];
	tuple_type roomInfo = make_tuple(mFilter[2], mFilter[3], mFilter[4], mFilter[5]);

	priority_queue<pair<int, int>>& pq = roomsFiltered[roomInfo];
	vector<int> roomsPoped;

	while (not pq.empty()) {
		pair<int, int> room = pq.top(); pq.pop();
		int roomPrice = -room.first;
		int roomID = -room.second;

		// 가격 업데이트 된 것만 탐색
		if (rooms[roomID].price != roomPrice)
			continue;

		roomsPoped.emplace_back(roomID);

		if (checkBookingInfo(roomID, checkInDate, checkOutDate)) {
			rooms[roomID].bookingInfo.emplace_back(checkInDate, checkOutDate);
			result = rooms[roomID].ID;
			break;
		}
	}
	for (int roomID : roomsPoped)
		pq.emplace(rooms[roomID]);

	return result;
}

int riseCosts(int mHotelID)
{
	int sumRoomPrices = 0;

	for (int roomID : hotels[mHotelID].rooms) {
		rooms[roomID].price += rooms[roomID].price / 10;
		sumRoomPrices += rooms[roomID].price;

		// 가격 업데이트 정보 반영
		roomsFiltered[rooms[roomID].roomInfo].emplace(-rooms[roomID].price, -rooms[roomID].ID);
	}
	return sumRoomPrices;
}
```

## 데이터 벡터 / 해시 맵 / 우선순위 큐 `Room` (객체)

```cpp
#include <vector>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <queue>

using namespace std;
using tuple_type = tuple<int, int, int, int>;

#define MAX_HOTELS	1001
#define MAX_ROOMS	100001

// mHotelID: 호텔의 ID (1 ≤ mHotelID ≤ 1000)
// mRoomID : 룸 ID (1 ≤ mRoomID ≤ 100,000)

struct Hotel {
	//int ID;
	vector<int> rooms;						// room ID list
};

struct Room {
	int ID;
	tuple_type roomInfo;
	int price;
	//int hotelID;							// hotelID
	vector<pair<int, int>> bookingInfo;		// (checkInDate, checkOutDate)

	bool operator<(const Room& r) const {
		return make_pair(-this->price, this->ID) > make_pair(-r.price, r.ID);
	}
};

vector<Hotel> hotels;
vector<Room> rooms;

struct key_hash{
	size_t operator() (const tuple_type& k) const {
		return get<0>(k) ^ get<1>(k) ^ get<2>(k) ^ get<3>(k);
	}
};

unordered_map<tuple_type, int, key_hash> filterMap;
vector<priority_queue<Room>> roomsFiltered;

bool checkBookingInfo(int roomID, int checkInDate, int checkOutDate) {
	for (auto& date : rooms[roomID].bookingInfo)
		if (checkOutDate > date.first && date.second > checkInDate)
			return false;
	return true;
}

void init(int N, int mRoomCnt[])
{
	hotels.clear();
	hotels.reserve(N);
	for (int cnt = 0; cnt < N; cnt++)
		hotels.emplace_back(Hotel{});

	rooms.clear();
	rooms.reserve(MAX_ROOMS);
	for (int cnt = 0; cnt < MAX_ROOMS; cnt++)
		rooms.emplace_back(Room{});

	filterMap.clear();
	roomsFiltered.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	//hotels[mHotelID].ID = mHotelID;
	hotels[mHotelID].rooms.emplace_back(mRoomID);

	rooms[mRoomID].ID = mRoomID;
	rooms[mRoomID].roomInfo = make_tuple(mRoomInfo[0], mRoomInfo[1], mRoomInfo[2], mRoomInfo[3]);
	rooms[mRoomID].price = mRoomInfo[4];
	//rooms[mRoomID].hotelID = mHotelID;

	int filterIdx;
	if (filterMap.count(rooms[mRoomID].roomInfo) == 0) {
		filterIdx = roomsFiltered.size();
		filterMap[rooms[mRoomID].roomInfo] = filterIdx;
		roomsFiltered.emplace_back(priority_queue<Room>{});
	}
	else
		filterIdx = filterMap[rooms[mRoomID].roomInfo];

	roomsFiltered[filterIdx].emplace(rooms[mRoomID]);
}

int findRoom(int mFilter[])
{
	int result = -1;
	int checkInDate = mFilter[0];
	int checkOutDate = mFilter[1];
	tuple_type roomInfo = make_tuple(mFilter[2], mFilter[3], mFilter[4], mFilter[5]);

	int filterIdx = filterMap[roomInfo];
	priority_queue<Room>& pq = roomsFiltered[filterIdx];
	vector<int> roomsPoped;

	while (not pq.empty()) {
		Room room = pq.top(); pq.pop();

		// 가격 업데이트 된 것만 탐색
		if (rooms[room.ID].price != room.price)
			continue;

		roomsPoped.emplace_back(room.ID);

		if (checkBookingInfo(room.ID, checkInDate, checkOutDate)) {
			rooms[room.ID].bookingInfo.emplace_back(checkInDate, checkOutDate);
			result = rooms[room.ID].ID;
			break;
		}
	}
	for (int roomID: roomsPoped)
		pq.emplace(rooms[roomID]);

	return result;
}

int riseCosts(int mHotelID)
{
	int sumRoomPrices = 0;

	for (int roomID : hotels[mHotelID].rooms) {
		rooms[roomID].price += rooms[roomID].price / 10;
		sumRoomPrices += rooms[roomID].price;

		// 가격 업데이트 정보 반영
		int filterIdx = filterMap[rooms[roomID].roomInfo];
		roomsFiltered[filterIdx].emplace(rooms[roomID]);
	}
	return sumRoomPrices;
}
```
