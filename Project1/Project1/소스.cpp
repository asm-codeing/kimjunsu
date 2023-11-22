#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include <time.h>
#include <mmsystem.h>
#include <Digitalv.h>
#pragma comment(lib, "winmm.lib")
#define Attack_speed 30
#define Time 1000
MCI_OPEN_PARMS openBgm;		//bgm 구조체
MCI_PLAY_PARMS playBgm;

int bgm_id_1;
int bgm_id_2;

int Timer = 110;

int Player_X = 20;
int Player_Y = 20;

int Police_X = 10;
int Police_Y = 10;

int Zombies = 5;
int Zombie_X[5];
int Zombie_Y[5];

int Shot_X = 0;
int Shot_Y = 0;

int watch = 0;
int breaktime = 0;

void text_color(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}

enum ColorType {
	BLACK,  	//0
	darkBLUE,	//1
	DarkGreen,	//2
	darkSkyBlue,    //3
	DarkRed,  	//4
	DarkPurple,	//5
	DarkYellow,	//6
	GRAY,		//7
	DarkGray,	//8
	BLUE,		//9
	GREEN,		//10
	SkyBlue,	//11
	RED,		//12
	PURPLE,		//13
	YELLOW,		//14
	WHITE		//15
} COLOR;

void gotoxy(int x, int y) {
	COORD pos = { x * 2, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void main_BGM() {
	openBgm.lpstrElementName = TEXT("C:\\Users\\Summer Overture-1-Clint Mansell.mp3");
	openBgm.lpstrDeviceType = TEXT("mpegvideo");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD64)(LPVOID)&openBgm);
	bgm_id_1 = openBgm.wDeviceID;
	mciSendCommand(bgm_id_1, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD64)(LPVOID)&playBgm);
}

void GameExit() {
	mciSendCommand(bgm_id_1, MCI_STOP, NULL, (DWORD64)(LPVOID)&playBgm);
}

void game_BGM() {
	openBgm.lpstrElementName = TEXT("C:\\Users\\Truth of the Legend.mp3");
	openBgm.lpstrDeviceType = TEXT("mpegvideo");
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD64)(LPVOID)&openBgm);
	bgm_id_2 = openBgm.wDeviceID;
	mciSendCommand(bgm_id_2, MCI_PLAY, MCI_DGV_PLAY_REPEAT, (DWORD64)(LPVOID)&playBgm);
}

void CursorView()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1; //커서 굵기 (1 ~ 100)
	cursorInfo.bVisible = FALSE; //커서 Visible TRUE(보임) FALSE(숨김)
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

unsigned _stdcall timer(void* arg) {
	while (1) {
		if (breaktime == 1) {
			break;
		}
		watch++;
		Sleep(Time);
	}
	return 1;
}

void Shot() {
	srand((unsigned int)time(NULL));
	Shot_X = Player_X;
	Shot_Y = Player_Y;
	if (GetAsyncKeyState(0x41)) {
		PlaySound(TEXT("gun.wav"), NULL, SND_ASYNC);
		for (int i = 1; i < 9; i++) {
			Shot_X -= 1;
			gotoxy(Shot_X, Shot_Y); text_color(darkBLUE); printf("◀");
			for (int j = 0; j < Zombies; j++) {
				if ((Shot_X == Zombie_X[j]) && (Shot_Y == Zombie_Y[j])) {
					PlaySound(TEXT("cry.wav"), NULL, SND_ASYNC);
					Zombie_X[j] = rand() % 102 + 1;
					Zombie_Y[j] = rand() % 59 + 1;
					if ((Zombie_X[j] > Player_X - 5) && (Zombie_X[j] < Player_X + 5)) { //Zombie_X == 70, Player_X = 69, 79
						if (Zombie_X[j] - (Player_X - 5) < Player_X + 5 - Zombie_X[j]) Zombie_X[j] = rand() % Player_X - 5 + 1;
						else if (Zombie_X[j] - (Player_X - 5) > Player_X + 5 - Zombie_X[j]) Zombie_X[j] = Player_X + 5 + rand() % 102 + 1 - Player_X + 5;
					}
					else if ((Zombie_Y[i] > Player_Y - 5) && (Zombie_Y[j] < Player_Y + 5)) { //Zombie_Y == 70, Player_Y = 69, 79
						if (Zombie_Y[j] - (Player_Y - 5) < Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = rand() % Player_Y - 5 + 1;
						else if (Zombie_Y[j] - (Player_Y - 5) > Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = Player_Y + 5 + rand() % 59 + 1 - Player_Y + 5;
					}
					else if ((Zombie_X[j] == Player_X) && (Zombie_Y[j] == Player_Y)) { //Zombie_X == 70, Player_X = 69, 79
						Zombie_X[j] = rand() % 102 + 1 - Player_X + 5;
						Zombie_Y[j] = rand() % 59 + 1 - Player_Y + 5;
					}
				}
			}
			Sleep(Attack_speed);
			gotoxy(Shot_X, Shot_Y); printf("  ");
		}
	}
	if (GetAsyncKeyState(0x57)) {
		PlaySound(TEXT("gun.wav"), NULL, SND_ASYNC);
		for (int i = 1; i < 9; i++) {
			Shot_Y -= 1;
			gotoxy(Shot_X, Shot_Y); text_color(darkBLUE); printf("▲");
			for (int j = 0; j < Zombies; j++) {
				if ((Shot_X == Zombie_X[j]) && (Shot_Y == Zombie_Y[j])) {
					PlaySound(TEXT("cry.wav"), NULL, SND_ASYNC);
					Zombie_X[j] = rand() % 102 + 1;
					Zombie_Y[j] = rand() % 59 + 1;
					if ((Zombie_X[j] > Player_X - 5) && (Zombie_X[j] < Player_X + 5)) { //Zombie_X == 70, Player_X = 69, 79
						if (Zombie_X[j] - (Player_X - 5) < Player_X + 5 - Zombie_X[j]) Zombie_X[j] = rand() % Player_X - 5 + 1;
						else if (Zombie_X[j] - (Player_X - 5) > Player_X + 5 - Zombie_X[j]) Zombie_X[j] = Player_X + 5 + rand() % 102 + 1 - Player_X + 5;
					}
					else if ((Zombie_Y[i] > Player_Y - 5) && (Zombie_Y[j] < Player_Y + 5)) { //Zombie_Y == 70, Player_Y = 69, 79
						if (Zombie_Y[j] - (Player_Y - 5) < Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = rand() % Player_Y - 5 + 1;
						else if (Zombie_Y[j] - (Player_Y - 5) > Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = Player_Y + 5 + rand() % 59 + 1 - Player_Y + 5;
					}
					else if ((Zombie_X[j] == Player_X) && (Zombie_Y[j] == Player_Y)) { //Zombie_X == 70, Player_X = 69, 79
						Zombie_X[j] = rand() % 102 + 1 - Player_X + 5;
						Zombie_Y[j] = rand() % 59 + 1 - Player_Y + 5;
					}
				}
			}
			Sleep(Attack_speed);
			gotoxy(Shot_X, Shot_Y); printf("  ");
		}
	}
	if (GetAsyncKeyState(0x44)) {
		PlaySound(TEXT("gun.wav"), NULL, SND_ASYNC);
		for (int i = 1; i < 9; i++) {
			Shot_X += 1;
			gotoxy(Shot_X, Shot_Y); text_color(darkBLUE); printf("▶");
			for (int j = 0; j < Zombies; j++) {
				if ((Shot_X == Zombie_X[j]) && (Shot_Y == Zombie_Y[j])) {
					PlaySound(TEXT("cry.wav"), NULL, SND_ASYNC);
					Zombie_X[j] = rand() % 102 + 1;
					Zombie_Y[j] = rand() % 59 + 1;
					if ((Zombie_X[j] > Player_X - 5) && (Zombie_X[j] < Player_X + 5)) { //Zombie_X == 70, Player_X = 69, 79
						if (Zombie_X[j] - (Player_X - 5) < Player_X + 5 - Zombie_X[j]) Zombie_X[j] = rand() % Player_X - 5 + 1;
						else if (Zombie_X[j] - (Player_X - 5) > Player_X + 5 - Zombie_X[j]) Zombie_X[j] = Player_X + 5 + rand() % 102 + 1 - Player_X + 5;
					}
					else if ((Zombie_Y[i] > Player_Y - 5) && (Zombie_Y[j] < Player_Y + 5)) { //Zombie_Y == 70, Player_Y = 69, 79
						if (Zombie_Y[j] - (Player_Y - 5) < Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = rand() % Player_Y - 5 + 1;
						else if (Zombie_Y[j] - (Player_Y - 5) > Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = Player_Y + 5 + rand() % 59 + 1 - Player_Y + 5;
					}
					else if ((Zombie_X[j] == Player_X) && (Zombie_Y[j] == Player_Y)) { //Zombie_X == 70, Player_X = 69, 79
						Zombie_X[j] = rand() % 102 + 1 - Player_X + 5;
						Zombie_Y[j] = rand() % 59 + 1 - Player_Y + 5;
					}
				}
			}
			Sleep(Attack_speed);
			gotoxy(Shot_X, Shot_Y); printf("  ");
		}
	}
	if (GetAsyncKeyState(0x53)) {
		PlaySound(TEXT("gun.wav"), NULL, SND_ASYNC);
		for (int i = 1; i < 9; i++) {
			Shot_Y += 1;
			gotoxy(Shot_X, Shot_Y); text_color(darkBLUE); printf("▼");
			for (int j = 0; j < Zombies; j++) {
				if ((Shot_X == Zombie_X[j]) && (Shot_Y == Zombie_Y[j])) {
					PlaySound(TEXT("cry.wav"), NULL, SND_ASYNC);
					Zombie_X[j] = rand() % 102 + 1;
					Zombie_Y[j] = rand() % 59 + 1;
					if ((Zombie_X[j] > Player_X - 5) && (Zombie_X[j] < Player_X + 5)) { //Zombie_X == 70, Player_X = 69, 79
						if (Zombie_X[j] - (Player_X - 5) < Player_X + 5 - Zombie_X[j]) Zombie_X[j] = rand() % Player_X - 5 + 1;
						else if (Zombie_X[j] - (Player_X - 5) > Player_X + 5 - Zombie_X[j]) Zombie_X[j] = Player_X + 5 + rand() % 102 + 1 - Player_X + 5;
					}
					else if ((Zombie_Y[i] > Player_Y - 5) && (Zombie_Y[j] < Player_Y + 5)) { //Zombie_Y == 70, Player_Y = 69, 79
						if (Zombie_Y[j] - (Player_Y - 5) < Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = rand() % Player_Y - 5 + 1;
						else if (Zombie_Y[j] - (Player_Y - 5) > Player_Y + 5 - Zombie_Y[j]) Zombie_Y[j] = Player_Y + 5 + rand() % 59 + 1 - Player_Y + 5;
					}
					else if ((Zombie_X[j] == Player_X) && (Zombie_Y[j] == Player_Y)) { //Zombie_X == 70, Player_X = 69, 79
						Zombie_X[j] = rand() % 102 + 1 - Player_X + 5;
						Zombie_Y[j] = rand() % 59 + 1 - Player_Y + 5;
					}
				}
			}
			Sleep(Attack_speed);
			gotoxy(Shot_X, Shot_Y); printf("  ");
		}
	}
}

void Player_Move() {
	if (GetAsyncKeyState(VK_UP)) {
		Player_Y--;
		if (0 == Player_Y) {
			gotoxy(Player_X, 1); printf("  ");
			Player_Y = 20;
		}
		gotoxy(Player_X, Player_Y); text_color(YELLOW); printf("●");
		gotoxy(Player_X, Player_Y + 1); printf("  ");
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		Player_Y++;
		if (61 == Player_Y) {
			gotoxy(Player_X, 60); printf("  ");
			Player_Y = 20;
		}
		gotoxy(Player_X, Player_Y); text_color(YELLOW); printf("●");
		gotoxy(Player_X, Player_Y - 1); printf("  ");
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		Player_X--;
		if (0 == Player_X) {
			gotoxy(1, Player_Y); printf("  ");
			Player_X = 20;
		}
		gotoxy(Player_X, Player_Y); text_color(YELLOW); printf("●");
		gotoxy(Player_X + 1, Player_Y); printf("  ");
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		if (104 == Player_X) {
			gotoxy(103, Player_Y); printf("  ");
			Player_X = 20;
		}
		Player_X++;
		gotoxy(Player_X, Player_Y); text_color(YELLOW); printf("●");
		gotoxy(Player_X - 1, Player_Y); printf("  ");
	}
}

int Zombie() {
	for (int i = 0; i < Zombies; i++) {
			if (Player_X - Zombie_X[i] > 0) { // Player_X > Zombie_X
				Zombie_X[i]++;
				gotoxy(Zombie_X[i], Zombie_Y[i]); text_color(DarkGreen); printf("◎");
				gotoxy(Zombie_X[i] - 1, Zombie_Y[i]); printf("  ");
			}
			if (Player_X - Zombie_X[i] < 0) { // Player_X < Zombie_X
				Zombie_X[i]--;
				gotoxy(Zombie_X[i], Zombie_Y[i]); text_color(DarkGreen); printf("◎");
				gotoxy(Zombie_X[i] + 1, Zombie_Y[i]); printf("  ");
			}
			if (Player_Y - Zombie_Y[i] > 0) { // Player_Y > Zombie_Y
				Zombie_Y[i]++;
				gotoxy(Zombie_X[i], Zombie_Y[i]); text_color(DarkGreen);; printf("◎");
				gotoxy(Zombie_X[i], Zombie_Y[i] - 1); printf("  ");
			}
			if (Player_Y - Zombie_Y[i] < 0) { // Player_Y < Zombie_Y
				Zombie_Y[i]--;
				gotoxy(Zombie_X[i], Zombie_Y[i]); text_color(DarkGreen); printf("◎");
				gotoxy(Zombie_X[i], Zombie_Y[i] + 1); printf("  ");
			}
			else {
				continue;
			}
	}
	return 1;
}

int main() {
	srand((unsigned int)time(NULL));
	printf("※꼭 전체화면을 한 뒤에 실행하여 주세요!※");
	_getch();
	system("cls");
	while (1) {
		CursorView();
		main_BGM();
		mciSendCommand(bgm_id_2, MCI_CLOSE, NULL, (DWORD64)(LPVOID)&playBgm);
		Timer = 110; int level = 1;
		breaktime = 0;
		watch = 0;
		Player_X = 20;
		Player_Y = 20;
		gotoxy(34, 5); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@@        @@@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
		gotoxy(34, 6); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@@@      @@@@   @@   @@@   @@@@@@@   @@@@@@@@\n");
		gotoxy(34, 7); text_color(DarkGreen); printf("    @@@    @@   @@   @@@@@    @@@@@   @@    @@     @@@     @@@     \n");
		gotoxy(34, 8); text_color(DarkGreen); printf("   @@@     @@   @@   @@ @@@  @@@ @@   @@@@@@@      @@@     @@@@@@@@\n");
		gotoxy(34, 9); text_color(DarkGreen); printf("  @@@      @@   @@   @@  @@@@@@  @@   @@   @@@     @@@     @@@@@@@@\n");
		gotoxy(34, 10); text_color(DarkGreen); printf(" @@@       @@   @@   @@   @@@@   @@   @@    @@     @@@     @@@     \n");
		gotoxy(34, 11); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@    @@    @@   @@   @@@   @@@@@@@   @@@@@@@@\n");
		gotoxy(34, 12); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@          @@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
		gotoxy(35, 19); text_color(DarkRed); printf("                        -Game Start-(1)");
		gotoxy(35, 22); text_color(DarkRed); printf("                        -Game Rule-(2)");
		gotoxy(35, 44); text_color(DarkRed); printf("                        -Game Exit-(0)");
		int a = _getch();
		if (a == '0') {
			system("cls");
			GameExit();
			PlaySound(TEXT("cry.wav"), NULL, SND_SYNC);
			Sleep(500);
			return 0;
		}
		if (a == '1') {
			system("cls");
			gotoxy(34, 5); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@@        @@@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
			gotoxy(34, 6); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@@@      @@@@   @@   @@@   @@@@@@@   @@@@@@@@\n");
			gotoxy(34, 7); text_color(DarkGreen); printf("    @@@    @@   @@   @@@@@    @@@@@   @@    @@     @@@     @@@     \n");
			gotoxy(34, 8); text_color(DarkGreen); printf("   @@@     @@   @@   @@ @@@  @@@ @@   @@@@@@@      @@@     @@@@@@@@\n");
			gotoxy(34, 9); text_color(DarkGreen); printf("  @@@      @@   @@   @@  @@@@@@  @@   @@   @@@     @@@     @@@@@@@@\n");
			gotoxy(34, 10); text_color(DarkGreen); printf(" @@@       @@   @@   @@   @@@@   @@   @@    @@     @@@     @@@     \n");
			gotoxy(34, 11); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@    @@    @@   @@   @@@   @@@@@@@   @@@@@@@@\n");
			gotoxy(34, 12); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@          @@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
			gotoxy(35, 19); text_color(BLUE); printf("            게임 난이도를 1와 2로 난이도를 설정하세요");
			gotoxy(35, 44); text_color(DarkRed); printf("                        -Game Exit-(0)");
			gotoxy(35, 50); text_color(DarkGray); printf("		ESC키를 눌러 홈으로 돌아가실 수 있습니다.");
			gotoxy(45, 21); text_color(DarkYellow); printf("■");
			gotoxy(48, 22); text_color(darkSkyBlue); printf("%2d 레벨", level);
			while (1) {
				int i = _getch();
				if (i == 27) {
					system("cls");
					break;
				}
				if (i == '2') {
					if (level < 10) {
						level += 1;
						for (int i = 0; i < level; i++) {
							gotoxy(45 + i, 21); text_color(DarkYellow); printf("■");
							gotoxy(48, 22); text_color(darkSkyBlue); printf("%2d 레벨", level);
						}
						PlaySound(TEXT("Level.wav"), NULL, SND_ASYNC);
						Sleep(1000);
						gotoxy(36, 30); text_color(DarkGray); printf("게임 난이도를 전부 다 설정하였다면 엔터키를 눌러 시작해주세요");
					}
					else {
						PlaySound(TEXT("warning.wav"), NULL, SND_ASYNC);
						Sleep(300);
						gotoxy(43, 25); text_color(RED); printf("게임 난이도는 최대 10입니다!");
						Sleep(1000);
						gotoxy(43, 25); printf("                            ");
					}
				}
				else if (i == '1') {
					if (level > 1) {
						level -= 1;
						gotoxy(45, 21); printf("                    ");
						for (int i = 0; i < level; i++) {
							gotoxy(45 + i, 21); text_color(DarkYellow); printf("■");
							gotoxy(48, 22); text_color(darkSkyBlue); printf("%2d 레벨", level);
						}
						PlaySound(TEXT("Level.wav"), NULL, SND_ASYNC);
						Sleep(1000);
						gotoxy(36, 30); text_color(DarkGray); printf("게임 난이도를 전부 다 설정하였다면 엔터키를 눌러 시작해주세요");
					}
					else {
						PlaySound(TEXT("warning.wav"), NULL, SND_ASYNC);
						Sleep(300);
						gotoxy(43, 25); text_color(RED); printf("게임 난이도는 최소 1입니다!");
						Sleep(1000);
						gotoxy(43, 25); printf("                            ");
					}
				}
				else if (i == 13) {
					game_BGM();
					Timer = Timer - (level * 10);
					system("cls");
					mciSendCommand(bgm_id_1, MCI_CLOSE, NULL, (DWORD64)(LPVOID)&playBgm);
					_beginthreadex(NULL, 0, timer, 0, 0, NULL);
					gotoxy(20, 20); text_color(YELLOW); printf("●");
					for (int i = 0; i < Zombies; i++) {
						Zombie_X[i] = rand() % 102 + 1;
						Zombie_Y[i] = rand() % 59 + 1;
						if ((Zombie_X[i] > Player_X - 5) && (Zombie_X[i] < Player_X + 5)) { //Zombie_X == 70, Player_X = 69, 79
							if (Zombie_X[i] - (Player_X - 5) < Player_X + 5 - Zombie_X[i]) Zombie_X[i] = rand() % Player_X - 5 + 1;
							else if (Zombie_X[i] - (Player_X - 5) > Player_X + 5 - Zombie_X[i]) Zombie_X[i] = Player_X + 5 + rand() % 102 + 1 - Player_X + 5;
						}
						else if ((Zombie_Y[i] > Player_Y - 5) && (Zombie_Y[i] < Player_Y + 5)) { //Zombie_Y == 70, Player_Y = 69, 79
							if (Zombie_Y[i] - (Player_Y - 5) < Player_Y + 5 - Zombie_Y[i]) Zombie_Y[i] = rand() % Player_Y - 5 + 1;
							else if (Zombie_Y[i] - (Player_Y - 5) > Player_Y + 5 - Zombie_Y[i]) Zombie_Y[i] = Player_Y + 5 + rand() % 59 + 1 - Player_Y + 5;
						}
						else if ((Zombie_X[i] == Player_X) && (Zombie_Y[i] == Player_Y)) { //Zombie_X == 70, Player_X = 69, 79
							Zombie_X[i] = rand() % 102 + 1 - Player_X + 5;
							Zombie_Y[i] = rand() % 59 + 1 - Player_Y + 5;
						}
						gotoxy(Zombie_X[i], Zombie_Y[i]); text_color(DarkGreen); printf("◎");
					}
					while (1) {
						CursorView();
						for (int i = 0; i < Zombies; i++) {
							if ((Zombie_X[i] == Player_X) && (Zombie_Y[i] == Player_Y)) {
								breaktime = 1;
							}
						}
						if (breaktime == 1) {
							system("cls");
							gotoxy(45, 30); printf("당신의 생존 시간은 %d초입니다..", watch);
							Sleep(Time);
							system("cls");
							break;
						}
						for (int i = 0; i < 105; i++) {
							gotoxy(i, 1); printf("■");
						}
						for (int i = 1; i < 62; i++) {
							gotoxy(104, i); printf("■");
						}
						for (int i = 104; i > 0; i--) {
							gotoxy(i, 61); printf("■");
						}
						for (int i = 61; i > 1; i--) {
							gotoxy(0, i); printf("■");
						}
						Shot();
						Player_Move();
						Zombie();
						gotoxy(100, 0); text_color(darkSkyBlue); printf("%3d초", watch);
						gotoxy(90, 0); text_color(YELLOW); printf("레벨:  %3d", level);
						Sleep(Timer);
					}
					break;
				}
			}
		}
		else if (a == '2') {
			system("cls");
			while (1) {
				gotoxy(34, 5); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@@        @@@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
				gotoxy(34, 6); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@@@      @@@@   @@   @@@   @@@@@@@   @@@@@@@@\n");
				gotoxy(34, 7); text_color(DarkGreen); printf("    @@@    @@   @@   @@@@@    @@@@@   @@    @@     @@@     @@@     \n");
				gotoxy(34, 8); text_color(DarkGreen); printf("   @@@     @@   @@   @@ @@@  @@@ @@   @@@@@@@      @@@     @@@@@@@@\n");
				gotoxy(34, 9); text_color(DarkGreen); printf("  @@@      @@   @@   @@  @@@@@@  @@   @@   @@@     @@@     @@@@@@@@\n");
				gotoxy(34, 10); text_color(DarkGreen); printf(" @@@       @@   @@   @@   @@@@   @@   @@    @@     @@@     @@@     \n");
				gotoxy(34, 11); text_color(DarkGreen); printf("@@@@@@@@   @@@@@@@   @@    @@    @@   @@   @@@   @@@@@@@   @@@@@@@@\n");
				gotoxy(34, 12); text_color(DarkGreen); printf("@@@@@@@@    @@@@@    @@          @@   @@@@@@@    @@@@@@@   @@@@@@@@\n");
				gotoxy(35, 19); text_color(YELLOW); printf("	● : 당신의 캐릭터 입니다. 좀비에 닿지 않게 조심하세요!!");
				gotoxy(35, 22); text_color(DarkGreen); printf("	◎ : 좀비 입니다. 당신의 캐릭터를 쫒아 다니니 피하세요!!");
				gotoxy(35, 25); text_color(DarkPurple); printf("	W  : 총알이 위쪽으로 날아갑니다.");
				gotoxy(35, 27); text_color(DarkPurple); printf("	A  : 총알이 왼쪽으로 날아갑니다.");
				gotoxy(35, 29); text_color(DarkPurple); printf("	S  : 총알이 아래쪽으로 날아갑니다.");
				gotoxy(35, 31); text_color(DarkPurple); printf("	D  : 총알이 오른쪽으로 날아갑니다.");
				gotoxy(35, 50); text_color(DarkGray); printf("		ESC키를 눌러 홈으로 돌아가실 수 있습니다.");
				if (_getch() == 27) {
					system("cls");
					break;
				}
			}
		}
	}
	return 0;
}