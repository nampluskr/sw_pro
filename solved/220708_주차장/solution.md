# 220708_주차장

## [최종]

```cpp
#include <vector>
#include <unordered_map>
#include <queue>					// priority_queue
#include <tuple>

using namespace std;

#define PARKING 1
#define WAITING 0
#define LEFT	-1

struct Car {
	//int ID;							// mCar
	int state{ LEFT };					// PARKING, WAITING, LEFT
	int parkingStartTime;
	int waitingStartTime;
	int totalParkingTime;
	int totalWaitingTime;
};

struct ParkingLot {
	int baseTime, baseFee, unitTime, unitFee, capacity;
	int waitingCnt;
	int parkingCnt;

	int ceil(int a, int b) {
		int result = a / b;
		if (a % b > 0)
			result += 1;
		return result;
	}
	int cal_parkingFee(int parkingTime) {
		int fee = this->baseFee;
		if (parkingTime > this->baseTime)
			fee += ceil(parkingTime - this->baseTime, this->unitTime) * this->unitFee;
		return fee;
	}
};

unordered_map<int, int> carMap;		// cIdx = carMap[mCar]	10억
vector<Car> cars;					// cars[cIdx]			30만
ParkingLot P;

// Max. heap (priority, -waitingStartTeim, mCar)	
// 1. priority 클수록 2. waitingStartTime 빠를수록
priority_queue<tuple<int, int, int>> waitingCars;

///////////////////////////////////////////////////////////////////////////////////

void init(int mBaseTime, int mBaseFee, int mUnitTime, int mUnitFee, int mCapacity) {
	carMap.clear();
	cars.clear();
	P = ParkingLot{ mBaseTime , mBaseFee, mUnitTime, mUnitFee, mCapacity };

	while (not waitingCars.empty())
		waitingCars.pop();
}


int arrive(int mTime, int mCar) {
	// 처음 주차장에 도착한 차량이면 => 차량 등록
	int cIdx;
	auto ret = carMap.find(mCar);
	if (ret == carMap.end()) {
		cIdx = cars.size();
		carMap[mCar] = cIdx;
		cars.emplace_back(Car{});
	}
	else
		cIdx = ret->second;

	// 주차장 용량이 남아 있으면 => 주차 처리
	if (P.parkingCnt < P.capacity) {
		cars[cIdx].state = PARKING;
		cars[cIdx].parkingStartTime = mTime;
		P.parkingCnt += 1;
	}
	// 주차장 용량이 남아 있지 않으면 => 대기열에 추가
	else {
		cars[cIdx].state = WAITING;
		cars[cIdx].waitingStartTime = mTime;
		P.waitingCnt += 1;

		// 우선순위 큐에 저장 (Max. heap)
		int pValue1 = cars[cIdx].totalWaitingTime - cars[cIdx].totalParkingTime - mTime;	// 클수록
		int pValue2 = cars[cIdx].waitingStartTime;											// 작을수록
		waitingCars.emplace(pValue1, -pValue2, mCar);
	}
	return P.waitingCnt;
}


int leave(int mTime, int mCar) {
	int cIdx = carMap[mCar];

	// 주차중인 차량이면 => 주차 요금 정산
	if (cars[cIdx].state == PARKING) {
		cars[cIdx].state = LEFT;
		cars[cIdx].totalParkingTime += mTime - cars[cIdx].parkingStartTime;
		P.parkingCnt -= 1;

		// 대기차량이 있으면 => 주차 처리
		while (not waitingCars.empty()) {
			tuple<int, int, int> car = waitingCars.top(); waitingCars.pop();
			int waitingStartTime = -get<1>(car);
			int wIdx = carMap[get<2>(car)];

			if (cars[wIdx].state != WAITING)
				continue;
			
			if (cars[wIdx].waitingStartTime != waitingStartTime)
				continue;

			cars[wIdx].state = PARKING;
			cars[wIdx].parkingStartTime = mTime;
			P.parkingCnt += 1;

			cars[wIdx].totalWaitingTime += mTime - cars[wIdx].waitingStartTime;
			P.waitingCnt -= 1;
			break;
		}
		return P.cal_parkingFee(mTime - cars[cIdx].parkingStartTime);
	}

	// 대기중인 차량이면 => 대기열에서 삭제
	else if (cars[cIdx].state == WAITING) {
		cars[cIdx].state = LEFT;
		cars[cIdx].totalWaitingTime += mTime - cars[cIdx].waitingStartTime;
		P.waitingCnt -= 1;

		return -1;
	}
}

```

## 우선순위 큐에 `Car` 객체 저장

```cpp
#include <vector>
#include <unordered_map>
#include <queue>					// priority_queue
#include <tuple>

using namespace std;

#define PARKING 1
#define WAITING 0
#define LEFT	-1

struct Car {
	int ID;							// mCar
	int state{ LEFT };				// PARKING, WAITING, LEFT
	int parkingStartTime;
	int waitingStartTime;
	int totalParkingTime;
	int totalWaitingTime;

	int pValue1, pValue2;

	bool operator<(const Car& c) const {
		return make_pair(-this->pValue1, this->pValue2) > make_pair(-c.pValue1, c.pValue2);

		//if (this->pValue1 < c.pValue1)
		//	return true;
		//if (this->pValue1 == c.pValue1 && this->pValue2 > c.pValue2)
		//	return true;
		//return false;
	}
};

struct ParkingLot {
	int baseTime, baseFee, unitTime, unitFee, capacity;
	int waitingCnt;
	int parkingCnt;

	int ceil(int a, int b) {
		int result = a / b;
		if (a % b > 0)
			result += 1;
		return result;
	}
	int cal_parkingFee(int parkingTime) {
		int fee = this->baseFee;
		if (parkingTime > this->baseTime)
			fee += ceil(parkingTime - this->baseTime, this->unitTime) * this->unitFee;
		return fee;
	}
};

unordered_map<int, int> carMap;		// cIdx = carMap[mCar]	10억
vector<Car> cars;					// cars[cIdx]			30만
ParkingLot P;

priority_queue<Car> waitingCars;

///////////////////////////////////////////////////////////////////////////////////

void init(int mBaseTime, int mBaseFee, int mUnitTime, int mUnitFee, int mCapacity) {
	carMap.clear();
	cars.clear();
	P = ParkingLot{ mBaseTime , mBaseFee, mUnitTime, mUnitFee, mCapacity };

	while (not waitingCars.empty())
		waitingCars.pop();
}


int arrive(int mTime, int mCar) {
	// 처음 주차장에 도착한 차량이면 => 차량 등록
	int cIdx;
	auto ret = carMap.find(mCar);
	if (ret == carMap.end()) {
		cIdx = cars.size();
		carMap[mCar] = cIdx;
		cars.emplace_back(Car{});
		cars[cIdx].ID = mCar;
	}
	else
		cIdx = ret->second;

	// 주차장 용량이 남아 있으면 => 주차 처리
	if (P.parkingCnt < P.capacity) {
		cars[cIdx].state = PARKING;
		cars[cIdx].parkingStartTime = mTime;
		P.parkingCnt += 1;
	}
	// 주차장 용량이 남아 있지 않으면 => 대기열에 추가
	else {
		cars[cIdx].state = WAITING;
		cars[cIdx].waitingStartTime = mTime;
		P.waitingCnt += 1;

		// 우선순위 큐에 저장 (Max. heap)
		cars[cIdx].pValue1 = cars[cIdx].totalWaitingTime - cars[cIdx].totalParkingTime - mTime;	// 클수록
		cars[cIdx].pValue2 = cars[cIdx].waitingStartTime;											// 작을수록
		waitingCars.emplace(cars[cIdx]);
	}
	return P.waitingCnt;
}


int leave(int mTime, int mCar) {
	int cIdx = carMap[mCar];

	// 주차중인 차량이면 => 주차 요금 정산
	if (cars[cIdx].state == PARKING) {
		cars[cIdx].state = LEFT;
		cars[cIdx].totalParkingTime += mTime - cars[cIdx].parkingStartTime;
		P.parkingCnt -= 1;

		// 대기차량이 있으면 => 주차 처리
		while (not waitingCars.empty()) {
			Car car = waitingCars.top(); waitingCars.pop();
			int wIdx = carMap[car.ID];

			if (cars[wIdx].state != WAITING)
				continue;

			if (cars[wIdx].waitingStartTime != car.waitingStartTime)
				continue;

			cars[wIdx].state = PARKING;
			cars[wIdx].parkingStartTime = mTime;
			P.parkingCnt += 1;

			cars[wIdx].totalWaitingTime += mTime - cars[wIdx].waitingStartTime;
			P.waitingCnt -= 1;
			break;
		}
		return P.cal_parkingFee(mTime - cars[cIdx].parkingStartTime);
	}

	// 대기중인 차량이면 => 대기열에서 삭제
	else if (cars[cIdx].state == WAITING) {
		cars[cIdx].state = LEFT;
		cars[cIdx].totalWaitingTime += mTime - cars[cIdx].waitingStartTime;
		P.waitingCnt -= 1;

		return -1;
	}
}
```
