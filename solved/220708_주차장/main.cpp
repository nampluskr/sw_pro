#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int mBaseTime, int mBaseFee, int mUnitTime, int mUnitFee, int mCapacity);
extern int arrive(int mTime, int mCar);
extern int leave(int mTime, int mCar);

/////////////////////////////////////////////////////////////////////////
#define CMD_INIT 1
#define CMD_ARRIVE 2
#define CMD_LEAVE 3

static bool run() {
	int q;
	scanf("%d", &q);

	int basetime, basefee, unittime, unitfee, capacity, mtime, mcar;
	int cmd, ans, ret = 0;
	bool okay = false;

	for (int i = 0; i < q; ++i) {
		scanf("%d", &cmd);

		switch (cmd) {
		case CMD_INIT:
			scanf("%d %d %d %d %d", &basetime, &basefee, &unittime, &unitfee, &capacity);
			init(basetime, basefee, unittime, unitfee, capacity);
			okay = true;
			break;

		case CMD_ARRIVE:
			scanf("%d %d %d", &mtime, &mcar, &ans);
			ret = arrive(mtime, mcar);
			//printf(">> [time = %d, arrive] ret = %d, ans = %d\n", mtime, ret, ans);

			if (ans != ret)
				okay = false;
			break;

		case CMD_LEAVE:
			scanf("%d %d %d", &mtime, &mcar, &ans);
			ret = leave(mtime, mcar);
			//printf(">> [time = %d, leave] ret = %d, ans = %d\n", mtime, ret, ans);

			if (ans != ret)
				okay = false;
			break;

		default:
			okay = false;
			break;
		}
	}
	return okay;
}



int main() {

	clock_t start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	//T = 1;
	for (int tc = 1; tc <= T; tc++) {
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}
	int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
	printf(">> Result: %d ms\n", result);

	return 0;
}
