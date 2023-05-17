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

struct Hotel {
	int ID;
	vector<int> rooms;						// room idx list
};

struct Room {
	int ID;
	tuple_type roomInfo;
	int price;
	int hotel;								// hotelIdx
	vector<pair<int, int>> bookingInfo;		// (checkInDate, checkOutDate)

	bool operator()(const Room& r1, const Room& r2) {
		if (r1.price > r2.price) return true;
		if (r1.price == r2.price && r1.ID > r2.ID) return true;
		return false;
	}
};

unordered_map<int, int> hotelMap;
unordered_map<int, int> roomMap;

vector<Hotel> hotels;
vector<Room> rooms;

struct key_hash : public unary_function<tuple_type, size_t> {
	size_t operator() (const tuple_type& k) const {
		return get<0>(k) ^ get<1>(k) ^ get<2>(k) ^ get<3>(k);
	}
};
struct key_equal : public binary_function<tuple_type, tuple_type, bool> {
	bool operator() (const tuple_type& lhs, const tuple_type& rhs) const {
		if (get<0>(lhs) == get<0>(rhs) && get<1>(lhs) == get<1>(rhs) &&
			get<2>(lhs) == get<2>(rhs) && get<3>(lhs) == get<3>(rhs))
			return true;
		return false;
	}
};

unordered_map<tuple_type, int, key_hash, key_equal> filterMap;
using pq_type = priority_queue<Room, vector<Room>, Room>;
vector<pq_type> roomsFiltered;

bool checkBookingInfo(int roomIdx, int checkInDate, int checkOutDate) {
	for (auto& date : rooms[roomIdx].bookingInfo)
		if (checkOutDate > date.first && date.second > checkInDate)
			return false;
	return true;
}

void init(int N, int mRoomCnt[])
{
	roomMap.clear();
	hotelMap.clear();

	rooms.clear();
	hotels.clear();

	filterMap.clear();
	roomsFiltered.clear();
}

void addRoom(int mHotelID, int mRoomID, int mRoomInfo[])
{
	int hotelIdx, roomIdx;

	if (hotelMap.count(mHotelID) == 0) {
		hotelIdx = hotels.size();
		hotelMap[mHotelID] = hotelIdx;
		hotels.emplace_back(Hotel());
	}
	else
		hotelIdx = hotelMap[mHotelID];

	if (roomMap.count(mRoomID) == 0) {
		roomIdx = rooms.size();
		roomMap[mRoomID] = roomIdx;
		rooms.emplace_back(Room());
	}
	else
		roomIdx = roomMap[mRoomID];

	hotels[hotelIdx].ID = mHotelID;
	hotels[hotelIdx].rooms.emplace_back(roomIdx);

	rooms[roomIdx].ID = mRoomID;
	rooms[roomIdx].roomInfo = make_tuple(mRoomInfo[0], mRoomInfo[1], mRoomInfo[2], mRoomInfo[3]);
	rooms[roomIdx].price = mRoomInfo[4];
	rooms[roomIdx].hotel = hotelIdx;

	int filterIdx;
	if (filterMap.count(rooms[roomIdx].roomInfo) == 0) {
		filterIdx = roomsFiltered.size();
		filterMap[rooms[roomIdx].roomInfo] = filterIdx;
		roomsFiltered.emplace_back(pq_type());
	}
	else
		filterIdx = filterMap[rooms[roomIdx].roomInfo];

	roomsFiltered[filterIdx].push(rooms[roomIdx]);
}

int findRoom(int mFilter[])
{
	int result = -1;
	int checkInDate = mFilter[0];
	int checkOutDate = mFilter[1];
	tuple_type roomInfo = make_tuple(mFilter[2], mFilter[3], mFilter[4], mFilter[5]);

	int filterIdx = filterMap[roomInfo];
	pq_type& pq = roomsFiltered[filterIdx];
	vector<int> roomsPoped;

	while (not pq.empty()) {
		Room room = pq.top(); pq.pop();
		int roomIdx = roomMap[room.ID];

		// 가격 업데이트 된 것만 탐색
		if (rooms[roomIdx].price == room.price) {
			roomsPoped.emplace_back(roomIdx);

			if (checkBookingInfo(roomIdx, checkInDate, checkOutDate)) {
				rooms[roomIdx].bookingInfo.emplace_back(make_pair(checkInDate, checkOutDate));
				result = rooms[roomIdx].ID;
				break;
			}
		}
	}
	for (int idx : roomsPoped)
		pq.push(rooms[idx]);

	return result;
}

int riseCosts(int mHotelID)
{
	int sumRoomPrices = 0;
	int hotelIdx = hotelMap[mHotelID];

	for (int roomIdx : hotels[hotelIdx].rooms) {
		rooms[roomIdx].price += rooms[roomIdx].price / 10;
		sumRoomPrices += rooms[roomIdx].price;

		// 가격 업데이트 정보 반영
		int filterIdx = filterMap[rooms[roomIdx].roomInfo];
		roomsFiltered[filterIdx].push(rooms[roomIdx]);
	}
	return sumRoomPrices;
}
```
