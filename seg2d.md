## 2D Segment Tree (Range update / point query)

```cpp
// https://www.geeksforgeeks.org/two-dimensional-segment-tree-sub-matrix-sum/
#include <vector>

using namespace std;

#define SIZE 4

int rect[SIZE][SIZE] = {
	{ 1, 2, 3, 4 },
	{ 5, 6, 7, 8 },
	{ 1, 7, 5, 9 },
	{ 3, 0, 6, 2 },
};

// seg[x][y]
// x: col index 
// y: row index

struct SegmentTree2D {
	int ini_seg[4 * SIZE][4 * SIZE];
	int fin_seg[4 * SIZE][4 * SIZE];

	//vector<vector<int>> ini_seg(16, vector<int>(16, 0));
	//vector<vector<int>> fin_seg(16, vector<int>(16, 0));


	void segment(int s, int e, int n, int strip)
	{
		if (e == s) {
			ini_seg[strip][n] = rect[strip][s];
			return;
		}
		int m = (s + e) / 2;
		segment(s, m, 2 * n, strip);
		segment(m + 1, e, 2 * n + 1, strip);
		ini_seg[strip][n] = ini_seg[strip][2 * n] + ini_seg[strip][2 * n + 1];
	}

	void finalSegment(int s, int e, int n)
	{
		if (e == s) {
			for (int i = 1; i < 2 * SIZE; i++)
				fin_seg[n][i] = ini_seg[s][i];
			return;
		}
		int m = (s + e) / 2;
		finalSegment(s, m, 2 * n);
		finalSegment(m + 1, e, 2 * n + 1);

		for (int i = 1; i < 2 * SIZE; i++)
			fin_seg[n][i] = fin_seg[2 * n][i] + fin_seg[2 * n + 1][i];
	}

	int finalQuery(int n, int s, int e, int x1, int x2, int node)
	{
		if (x2 < s || e < x1)
			return 0;
		else if (x1 <= s && e <= x2)
			return fin_seg[node][n];

		int m = (s + e) / 2;
		int p1 = finalQuery(2 * n, s, m, x1, x2, node);
		int p2 = finalQuery(2 * n + 1, m + 1, e, x1, x2, node);
		return p1 + p2;
	}

	int query(int n, int s, int e, int y1, int y2, int x1, int x2)
	{
		if (y2 < s || e < y1)
			return 0;
		else if (y1 <= s && e <= y2)
			return finalQuery(1, 1, 4, x1, x2, n);

		int m = (s + e) / 2;
		int p1 = query(2 * n, s, m, y1, y2, x1, x2);
		int p2 = query(2 * n + 1, m + 1, e, y1, y2, x1, x2);
		return p1 + p2;
	}

	void finalUpdate(int n, int s, int e, int idx, int value, int node)
	{
		if (idx < s || idx > e)
			return;
		else if (s == e) {
			fin_seg[node][n] = value;
			return;
		}
		int m = (s + e) / 2;
		finalUpdate(2 * n, s, m, idx, value, node);
		finalUpdate(2 * n + 1, m + 1, e, idx, value, node);
		fin_seg[node][n] = fin_seg[node][2 * n] + fin_seg[node][2 * n + 1];
	}


	void update(int n, int s, int e, int x, int y, int value)
	{
		if (y < s || y > e)
			return;
		else if (s == e) {
			finalUpdate(1, 1, 4, x, value, n);
			return;
		}
		int m = (s + e) / 2;
		update(2 * n, s, m, x, y, value);
		update(2 * n + 1, m + 1, e, x, y, value);

		for (int i = 1; i < SIZE; i++)
			fin_seg[n][i] = fin_seg[2 * n][i] + fin_seg[2 * n + 1][i];
	}
};

SegmentTree2D seg;

int main()
{
	int n = 1;
	int s = 0;
	int e = 3;
	int result;

	// init: seg.init(rect)
	for (int strip = 0; strip < 4; strip++)
		seg.segment(s, e, 1, strip);
	seg.finalSegment(s, e, 1);

	// query
	result = seg.query(1, 1, 4, 2, 3, 2, 3);
	printf("Sum (y1, y2)=(2, 3), (x1, x2)=(2, 3): %d\n", result);

	// updata & query
	seg.update(1, 1, 4, 2, 3, 100);		// (2, 3) = 100
	result = seg.query(1, 1, 4, 2, 3, 2, 3);
	printf("Sum (y1, y2)=(2, 3), (x1, x2)=(2, 3): %d\n", result);

	return 0;
}
```
