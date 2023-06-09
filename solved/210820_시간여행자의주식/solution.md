# 210820_시간여행자의주식

## [최종] 자료구조 `vector` / 우선순위 큐 `tuple` 사용

```cpp
#include <array>
#include <vector>
#include <queue>
#include <tuple>

using namespace std;

#define MAX_ORDERS	200001
#define MAX_STOCKS	6
#define MAX_PRICE	1000001

struct Order {
	int number;			// (1 <= mNumber <= 200,000)
	int stock;			// (1 <= mStock <= 5)
	int quantity;			// (1 <= mQuantity <= 1,000,000)
	int price;			// (1 <= mPrice <= 1,000,000)
};

struct Stock {
	int maxProfit;
	int minPrice;

	priority_queue<tuple<int, int, int>> sellOrdersPQ;		// Max. Heap: (-price, -number, quantity)
	priority_queue<tuple<int, int, int>> buyOrdersPQ;		// Max. Heap: (price, -number, quantity)
};

//array<Order, MAX_ORDERS> orders;	// Hash table (key: 1 <= mNumber <= 200,000)
//array<Stock, MAX_STOCKS> stocks;	// Hash table (key: 1 <= mStock <= 5)

vector<Order> orders(MAX_ORDERS);				// Hash table (key: 1 <= mNumber <= 200,000)
vector<Stock> stocks(MAX_STOCKS);				// Hash table (key: 1 <= mStock <= 5)


////////////////////////////////////////////////////////////////////////////////
void init()
{
	//// Initialization for array
	//for (int mNumber = 0; mNumber < MAX_ORDERS; mNumber++)
	//	orders[mNumber] = Order{};

	//for (int mStock = 0; mStock < MAX_STOCKS; mStock++)
	//	stocks[mStock] = Stock{ 0, MAX_PRICE, {}, {} };

	// Initialization for vector
	orders.clear();
	for (int i = 0; i < MAX_ORDERS; i++)
		orders.emplace_back(Order{});

	stocks.clear();
	for (int i = 0; i < MAX_STOCKS; i++)
		stocks.emplace_back(Stock{ 0, MAX_PRICE, {}, {} });
}

int buy(int mNumber, int mStock, int mQuantity, int mPrice)
{
	orders[mNumber] = Order{ mNumber, mStock, mQuantity, mPrice };
	Order& buyOrder = orders[mNumber];
	Stock& stock = stocks[mStock];

	while (not stock.sellOrdersPQ.empty())
	{
		tuple<int, int, int> order = stock.sellOrdersPQ.top();	stock.sellOrdersPQ.pop();
		int orderNumber = -get<1>(order);
		int orderQuantiy = get<2>(order);
		Order& sellOrder = orders[orderNumber];

		if (sellOrder.quantity != orderQuantiy)
			continue;

		if (buyOrder.price >= sellOrder.price) {
			int quantity = min(buyOrder.quantity, sellOrder.quantity);
			sellOrder.quantity -= quantity;
			buyOrder.quantity -= quantity;

			stock.minPrice = min(stock.minPrice, sellOrder.price);
			stock.maxProfit = max(stock.maxProfit, sellOrder.price - stock.minPrice);

			if (sellOrder.quantity > 0)
				stock.sellOrdersPQ.emplace(-sellOrder.price, -sellOrder.number, sellOrder.quantity);

			if (buyOrder.quantity == 0)
				break;
		}
		else {
			stock.sellOrdersPQ.emplace(-sellOrder.price, -sellOrder.number, sellOrder.quantity);
			break;
		}
	}
	if (buyOrder.quantity > 0)
		stock.buyOrdersPQ.emplace(buyOrder.price, -buyOrder.number, buyOrder.quantity);

	return buyOrder.quantity;
}

int sell(int mNumber, int mStock, int mQuantity, int mPrice)
{
	orders[mNumber] = Order{ mNumber, mStock, mQuantity, mPrice };
	Order& sellOrder = orders[mNumber];
	Stock& stock = stocks[mStock];

	while (not stock.buyOrdersPQ.empty())
	{
		tuple<int, int, int> order = stock.buyOrdersPQ.top();	stock.buyOrdersPQ.pop();
		int orderNumber = -get<1>(order);
		int orderQuantiy = get<2>(order);
		Order& buyOrder = orders[orderNumber];

		if (buyOrder.quantity != orderQuantiy)
			continue;

		if (buyOrder.price >= sellOrder.price) {
			int quantity = min(sellOrder.quantity, buyOrder.quantity);
			sellOrder.quantity -= quantity;
			buyOrder.quantity -= quantity;

			stock.minPrice = min(stock.minPrice, buyOrder.price);
			stock.maxProfit = max(stock.maxProfit, buyOrder.price - stock.minPrice);

			if (buyOrder.quantity > 0)
				stock.buyOrdersPQ.emplace(buyOrder.price, -buyOrder.number, buyOrder.quantity);

			if (sellOrder.quantity == 0)
				break;
		}
		else {
			stock.buyOrdersPQ.emplace(buyOrder.price, -buyOrder.number, buyOrder.quantity);
			break;
		}
	}
	if (sellOrder.quantity > 0)
		stock.sellOrdersPQ.emplace(-sellOrder.price, -sellOrder.number, sellOrder.quantity);

	return sellOrder.quantity;
}

void cancel(int mNumber)
{
	orders[mNumber].quantity = 0;
}

int bestProfit(int mStock)
{
	return stocks[mStock].maxProfit;
}
```

## 자료구조 `vector` / 우선순위 큐 `Order` (객체) - 최적화 옵션 해제시 시간 초과

```cpp
#include <vector>
#include <array>
#include <string>
#include <queue>

using namespace std;

#define MAX_ORDERS	200001
#define MAX_STOCKS	6
#define MAX_PRICE	1000001

struct Order {
	int number;			// (1 <= mNumber <= 200,000)
	int stock;			// (1 <= mStock <= 5)
	int quantity;			// (1 <= mQuantity <= 1,000,000)
	int price;			// (1 <= mPrice <= 1,000,000)
	string type;			// "sell" or "buy"

	bool operator<(const Order& order) const
	{
		if (this->type == "buy")
			return make_pair(-this->price, this->number) > make_pair(-order.price, order.number);
		if (this->type == "sell")
			return make_pair(this->price, this->number) > make_pair(order.price, order.number);
	}
};

struct Stock {
	int maxProfit;
	int minPrice;

	priority_queue<Order> sellOrdersPQ;
	priority_queue<Order> buyOrdersPQ;
};

//vector<Order> orders(MAX_ORDERS);	// Hash table (key: 1 <= mNumber <= 200,000)
//vector<Stock> stocks(MAX_STOCKS);	// Hash table (key: 1 <= mStock <= 5)

array<Order, MAX_ORDERS> orders;	// Hash table (key: 1 <= mNumber <= 200,000)
array<Stock, MAX_STOCKS> stocks;	// Hash table (key: 1 <= mStock <= 5)

////////////////////////////////////////////////////////////////////////////////
void init()
{
	// Initialization for array
	for (int mNumber = 0; mNumber < MAX_ORDERS; mNumber++)
		orders[mNumber] = Order{};

	for (int mStock = 0; mStock < MAX_STOCKS; mStock++)
		stocks[mStock] = Stock{ 0, MAX_PRICE, {}, {} };

	//// Initialization for vector
	//orders.clear();
	//for (int i = 0; i < MAX_ORDERS; i++)
	//	orders.emplace_back(Order{});

	//stocks.clear();
	//for (int i = 0; i < MAX_STOCKS; i++)
	//	stocks.emplace_back(Stock{ 0, MAX_PRICE, {}, {} });
}

int buy(int mNumber, int mStock, int mQuantity, int mPrice)
{
	orders[mNumber] = Order{ mNumber, mStock, mQuantity, mPrice, "buy" };
	Order& buyOrder = orders[mNumber];
	Stock& stock = stocks[mStock];

	while (not stock.sellOrdersPQ.empty())
	{
		Order order = stock.sellOrdersPQ.top();	stock.sellOrdersPQ.pop();
		Order& sellOrder = orders[order.number];

		if (sellOrder.quantity != order.quantity)
			continue;

		if (buyOrder.price >= sellOrder.price) {
			int quantity = min(buyOrder.quantity, sellOrder.quantity);
			sellOrder.quantity -= quantity;
			buyOrder.quantity -= quantity;

			stock.minPrice = min(stock.minPrice, sellOrder.price);
			stock.maxProfit = max(stock.maxProfit, sellOrder.price - stock.minPrice);

			if (sellOrder.quantity > 0)
				stock.sellOrdersPQ.push(sellOrder);

			if (buyOrder.quantity == 0)
				break;
		}
		else {
			stock.sellOrdersPQ.push(sellOrder);
			break;
		}
	}
	if (buyOrder.quantity > 0)
		stock.buyOrdersPQ.push(buyOrder);

	return buyOrder.quantity;
}

int sell(int mNumber, int mStock, int mQuantity, int mPrice)
{
	orders[mNumber] = Order{ mNumber, mStock, mQuantity, mPrice, "sell" };
	Order& sellOrder = orders[mNumber];
	Stock& stock = stocks[mStock];

	while (not stock.buyOrdersPQ.empty())
	{
		Order order = stock.buyOrdersPQ.top();	stock.buyOrdersPQ.pop();
		Order& buyOrder = orders[order.number];

		if (buyOrder.quantity != order.quantity)
			continue;

		if (buyOrder.price >= sellOrder.price) {
			int quantity = min(sellOrder.quantity, buyOrder.quantity);
			sellOrder.quantity -= quantity;
			buyOrder.quantity -= quantity;

			stock.minPrice = min(stock.minPrice, buyOrder.price);
			stock.maxProfit = max(stock.maxProfit, buyOrder.price - stock.minPrice);

			if (buyOrder.quantity > 0)
				stock.buyOrdersPQ.push(buyOrder);

			if (sellOrder.quantity == 0)
				break;
		}
		else {
			stock.buyOrdersPQ.push(buyOrder);
			break;
		}
	}
	if (sellOrder.quantity > 0)
		stock.sellOrdersPQ.push(sellOrder);

	return sellOrder.quantity;
}

void cancel(int mNumber)
{
	orders[mNumber].quantity = 0;
}

int bestProfit(int mStock)
{
	return stocks[mStock].maxProfit;
}
```
