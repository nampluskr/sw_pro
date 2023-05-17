#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

extern void init(int N, int mRoomCnt[]);
extern void addRoom(int mHotelID, int mRoomID, int mRoomInfo[]);
extern int findRoom(int mFilter[]);
extern int riseCosts(int mHotelID);

static int roomCnt[1005];

const static int ADDROOM = 100;
const static int FINDROOM = 200;
const static int RISECOSTS = 300;
const static int END = 400;

static int hotelID = 0;
static int roomID = 0;
static int roomInfo[5] = { 0, };
static int filterInfo[6] = { 0, };

static int run(int _score)
{
	int n, score = _score;
	scanf("%d", &n);

	for (int i = 0; i < n; i++)
		scanf("%d", roomCnt + i);

	init(n, roomCnt);

	int cmd, user_ans, correct_ans;

	for (int q = 0;; q++)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case ADDROOM:
			scanf("%d%d", &hotelID, &roomID);

			for (int i = 0; i < 5; i++)
				scanf("%d", roomInfo + i);

			addRoom(hotelID, roomID, roomInfo);
			break;

		case FINDROOM:
			for (int i = 0; i < 6; i++)
				scanf("%d", filterInfo + i);

			user_ans = findRoom(filterInfo);
			scanf("%d", &correct_ans);
			//printf("[findRoom] ret = %3d | ans = %3d\n", user_ans, correct_ans);

			if (user_ans != correct_ans)
				score = 0;
			break;

		case RISECOSTS:
			scanf("%d", &hotelID);
			user_ans = riseCosts(hotelID);
			scanf("%d", &correct_ans);
			//printf("[riseCostes] ret = %3d | ans = %3d\n", user_ans, correct_ans);

			if (user_ans != correct_ans)
				score = 0;
			break;

		case END:
			return score;

		default:
			score = 0;
			break;
		}
	}
	return score;
}



int main()
{
	clock_t start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int tc, score;
	scanf("%d%d", &tc, &score);

	//tc = 1;
	for (int t = 1; t <= tc; t++)
	{
		printf("#%d %d\n", t, run(score));
	}

	int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
	printf(">> Result: %d ms\n", result);

	return 0;
}
