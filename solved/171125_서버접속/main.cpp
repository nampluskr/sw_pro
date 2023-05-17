#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern void Init();
extern void NewAccount(char id[11], char password[11], int defaulttime);
extern void Logout(char id[11]);
extern void Connect(char id[11], char password[11]);
extern int Tick();

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define MAX_ACCOUNT 50000
#define MAX_TIME 30000

typedef enum {
	INIT,
	NEWACCOUNT,
	LOGOUT,
	CONNECT,
	TICK
}STATE;

typedef struct {
	char id[11];
	char password[11];
	int defaulttime;
}ACCOUNT;
static ACCOUNT account[MAX_ACCOUNT];

static int mSeed;
static int mrand(int num)
{
	mSeed = mSeed * 1103515245 + 12345;
	return (((mSeed >> 16) & 0x7FFF) % num);
}

static void make_account(int cnt)
{
	for (int i = 0; i < cnt; i++) {
		int idl = 5 + mrand(6);
		for (int k = 0; k < idl; k++) {
			int ch = mrand(36);
			if (ch < 10) account[i].id[k] = ch + '0';
			else account[i].id[k] = ch - 10 + 'a';
		}
		account[i].id[idl] = '\0';

		int pal = 5 + mrand(6);
		for (int k = 0; k < pal; k++) {
			int ch = mrand(36);
			if (ch < 10) account[i].password[k] = ch + '0';
			else account[i].password[k] = ch - 10 + 'a';
		}
		account[i].password[pal] = '\0';

		int max_time = cnt;
		if (max_time > MAX_TIME) max_time = MAX_TIME;
		account[i].defaulttime = 1 + mrand(max_time);
	}
}

static void init(int num)
{
	Init();

	make_account(num);
	for (int i = 0; i < num / 3; i++) {
		char id[11], password[11];
		int defaulttime;
		strcpy(id, account[i].id);
		strcpy(password, account[i].password);
		defaulttime = account[i].defaulttime;
		NewAccount(id, password, defaulttime);
	}
}

static int run()
{
	int ret = 1;
	int cmd, param1, param2, num, cmdcnt;

	char id[11], password[11];
	int defaulttime;

	scanf("%d %d %d %d", &cmd, &mSeed, &num, &cmdcnt);
	init(num);

	for (int i = 0; i < cmdcnt; i++) {
		scanf("%d", &cmd);
		if (cmd == NEWACCOUNT) {
			scanf("%d %d", &param1, &param2);
			strcpy(id, account[param1].id);
			strcpy(password, account[param1].password);
			defaulttime = param2;
			NewAccount(id, password, defaulttime);
		}
		else if (cmd == LOGOUT) {
			scanf("%d", &param1);
			strcpy(id, account[param1].id);
			Logout(id);
		}
		else if (cmd == CONNECT) {
			scanf("%d %d", &param1, &param2);
			strcpy(id, account[param1].id);
			strcpy(password, account[param2].password);
			Connect(id, password);
		}
		else if (cmd == TICK) {
			scanf("%d", &param1);
			int result = Tick();
			if (result != param1)
				ret = 0;
		}
	}

	return ret;
}

int main()
{
	clock_t start = clock();

	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T;
	scanf("%d", &T);

	//T = 1;
	for (int tc = 1; tc <= T; tc++)
	{
		int Score = 100;
		if (run() == 0)
			Score = 0;

		printf("#%d %d\n", tc, Score);
	}

	int result = (clock() - start) / (CLOCKS_PER_SEC / 1000);
	printf(">> Result: %d ms\n", result);
	return 0;
}
