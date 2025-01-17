#include "Mine.h"

//광산의 벽부분?에 닿으면 돈이 감소되는 ㄴ
//아니면 광산에 몬스터가 따라와서 몬스터를 피하면서 먹어야하게 하기!!

Mine::Mine(Player* player, int id) {
 	this->player = player;
	playerX = 25;
	playerY = 5;
	mineBool = true;

	this->id = id;

	//광산 이름
	this->mineName = new char[strlen("일반 광산") + 1];
	strcpy(this->mineName, "일반 광산");
	if (!strcmp(mineName, "일반 광산")) {
		produceMineralSec = 2;
		deleteMineralSec = 3;
		//광물 색에 따른 확률.
		mineralPercentage[YELLOW] = 45.0f;
		mineralPercentage[DARK_VIOLET] = 45.0f;
		mineralPercentage[DARK_RED] = 4.9f;
		mineralPercentage[DARK_GREEN] = 2.5f;
		mineralPercentage[DARK_BLUE] = 2.5f;
		mineralPercentage[DARK_SKYBLUE] = 0.1f;
		//fileName = new char[strlen("test1.txt") + 1];
		strcpy(fileName, "test1.txt");
		//fileName = "test1.txt";
	}
	else if (!strcmp(mineName, "중급 광산")) {
		produceMineralSec = 4;
		deleteMineralSec = 4;
		mineralPercentage[YELLOW] = 35.0f;
		mineralPercentage[DARK_VIOLET] = 35.0f;
		mineralPercentage[DARK_RED] = 23.5f;
		mineralPercentage[DARK_GREEN] = 3.0f;
		mineralPercentage[DARK_BLUE] = 3.0f;
		mineralPercentage[DARK_SKYBLUE] = 0.5f;
		strcpy(fileName, "test2.txt");
	}
	else if (!strcmp(mineName, "고급 광산")) {
		produceMineralSec = 5;
		deleteMineralSec = 5;
		mineralPercentage[YELLOW] = 30.0f;
		mineralPercentage[DARK_VIOLET] = 30.0f;
		mineralPercentage[DARK_RED] = 29.0f;
		mineralPercentage[DARK_GREEN] = 5.0f;
		mineralPercentage[DARK_BLUE] = 5.0f;
		mineralPercentage[DARK_SKYBLUE] = 1.0f;
		strcpy(fileName, "test3.txt");
	}
	else if (!strcmp(mineName, "테스트1 광산")) {
		produceMineralSec = 5;
		deleteMineralSec = 5;
		mineralPercentage[YELLOW] = 30.0f;
		mineralPercentage[DARK_VIOLET] = 30.0f;
		mineralPercentage[DARK_RED] = 29.0f;
		mineralPercentage[DARK_GREEN] = 5.0f;
		mineralPercentage[DARK_BLUE] = 5.0f;
		mineralPercentage[DARK_SKYBLUE] = 1.0f;
		strcpy(fileName, "test3.txt");
	}
	else if (!strcmp(mineName, "테스트2 광산")) {
		produceMineralSec = 5;
		deleteMineralSec = 5;
		mineralPercentage[YELLOW] = 30.0f;
		mineralPercentage[DARK_VIOLET] = 30.0f;
		mineralPercentage[DARK_RED] = 29.0f;
		mineralPercentage[DARK_GREEN] = 5.0f;
		mineralPercentage[DARK_BLUE] = 5.0f;
		mineralPercentage[DARK_SKYBLUE] = 1.0f;
		strcpy(fileName, "test3.txt");
	}
}
Mine::~Mine() {
	delete mineName;
	mineName = nullptr;
}
 
//초기화 & player money
void Mine::SetMineInfo(char* name, int produceSec, int deleteSec, float* percentage) {
	this->mineName = new char[strlen(name) + 1];
	strcpy(this->mineName, name);										//광산 이름
	produceMineralSec = produceSec;							//몇초사이마다 랜덤으로 광물이 나오게할지
	deleteMineralSec = deleteSec;							//몇초사이마다 랜덤으로 광물을 삭제할지
	for (int i = 0; i < MINERAL_ITEM_COUNT + 1; i++) {
		mineralPercentage[i] = percentage[i];
	}
 	sprintf(fileName, "%dminePrint.txt", 1); //문제
}

void Mine::MineInit() {
#pragma region 파일로 세팅..

	////광산을 세팅한다.

	//광산의 배경을 불러온다.
	//파일 불러오기
	fstream readfile(fileName);
	char* filecontent = nullptr;
	if (readfile.is_open()) {
		//파일 읽어오기 성공

		//맨끝으로 위치 이동
		readfile.seekg(0, ios::end);
		//맨 처음 위치부터 현재위치(파일 맨 끝)까지의 크기를 반환
		int size = readfile.tellg();
		//size만큼 문자열의 공간을 정해줌
		filecontent = new char[size + 1];
		//파일의 맨 처음 위치로 이동
		readfile.seekg(0, ios::beg);
		//파일의 전체 내용을 filecontent에 저장
		readfile.read(&filecontent[0], size);
		//이부분은 게임 관련 텍스트 이미지를 위한 부분이다.
		if (true) {
			gotoXY(0, 0);
			cout << "수확한 광물>>" << endl << endl;
			int cnt = 0;
			for (int i = 0; i < GAMEPLAY_GROUND_HEIGHT; i++) {
				for (int j = 0; j < GAMEPLAY_GROUND_WIDTH; j++) {
					if (filecontent[i] == '0') {
						//c9a1
						ground[i][j] = "▼";
					}
					else if (filecontent[i] == '1') {
						ground[i][j] = "◇";
					}
					else if (filecontent[i] == '2') {
						ground[i][j] = ROAD;
					}
				}
				cout << endl;
			}
			for (int i = 0; i < GAMEPLAY_GROUND_HEIGHT; i++) {
				for (int j = 0; j < GAMEPLAY_GROUND_WIDTH; j++) {
					gotoXY(j*2, i + COORDINATE_TOP);
					cout << ground[i][j];
				}
			}
		}
	}
	else {
		//파일 읽어오기 실패
		
		return;
	}
	readfile.close();

#pragma endregion

	//게임 플레이 부분의 모든 곳을 초기화
	for (int i = 0; i < GAMEPLAY_GROUND_HEIGHT; i++) {
		for (int j = 0; j < GAMEPLAY_GROUND_WIDTH; j++) {
			ground[i][j] = ROAD;
			item[i][j] = EMPTY;
			if (i == 0 || j == 0 || j == GAMEPLAY_GROUND_WIDTH - 1 || i == GAMEPLAY_GROUND_HEIGHT - 1) {
				ground[i][j] = "▼";
			}
			gotoXY(j * 2, i + COORDINATE_TOP);
			cout << ground[i][j];
		}
	}

	//플레이어의 위치를 세팅
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
	ground[playerY][playerX] = PLAYER_CHARACTER;
	gotoXY(playerX * 2, COORDINATE_TOP + playerY); //왼쪽 위
	cout << PLAYER_CHARACTER;

	prevTime_render = clock(); //시작했을 때의 시간 체크
}

int Mine::KeyInputRelated() {
	//키 관련된 처리

	int input = _getch();
	if (input == 224) { //224는 방향키임을 알려주는 아스키코드값
		input = _getch();

		//지나간 자리를 다시 되돌린다.
		ground[playerY][playerX] = ROAD;
		item[playerY][playerX] = EMPTY;
		gotoXY(playerX * 2, COORDINATE_TOP + playerY);
		cout << ground[playerY][playerX];
		if ((input == UP) && (ground[(playerY-1)][playerX] == ROAD|| ground[(playerY - 1)][playerX] == "■")) {
			playerY -= 1;
		}
		else if ((input == DOWN) && (ground[(playerY+1)][playerX] == ROAD || ground[(playerY + 1)][playerX] == "■")) { 
			playerY += 1;
		}
		else if ((input == LEFT) && (ground[playerY][playerX-1] == ROAD|| ground[playerY][playerX - 1] == "■")) {
			playerX -= 1;
		}
		else if ((input == RIGHT) && (ground[playerY][playerX+1] == ROAD|| ground[playerY][playerX + 1] == "■")) { 
			playerX += 1;
		}
		enemy.playerMoveSave(playerX, playerY);
		
		if (ground[playerY][playerX] == "■") { //움직였는데 광물을 먹었다면
			//플레이어의 위치가 광물이 있는 위치라면
			//플레이어가 어떤 광물에 닿으면 그 광물이 어느 위치에 있는지 체크.
			strcpy(mineral, mineralManager.MineralCheck(playerX, playerY)); //그 위치에 있는 광물의 이름을 받아온다.
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), EMPTY);
			gotoXY(0, 0);
			cout << "수확한 광물>>  " << mineral << "                                                       ";
			player->AddMineral(mineral); //플레이어에게 그 광물을 추가한다.
		}
		//플레이어가 움직인 곳으로 세팅
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
		ground[playerY][playerX] = PLAYER_CHARACTER;
		item[playerY][playerX] = EMPTY;
		gotoXY(playerX * 2, COORDINATE_TOP + playerY);
		cout << ground[playerY][playerX];
	}
	return input;
}


char* Mine::randomMineral(int mineralItem) {
	//광물의 색에 따라 그 광물의 색인 광물을 랜덤으로 생성한다.
	int randomIdx;
	char* res_mineralName;
	//srand(time(NULL));는 메인에..
	switch (mineralItem) {
	case 1:
		//DARK_BLUE

		//랜덤 인덱스 생성
		randomIdx = rand() % BlueMineralCount; // 파란색 광물의 갯수 사이에서 랜덤하게 생성

		//이름 
		res_mineralName = new char[strlen(BlueMineralName[randomIdx])];
		strcpy(res_mineralName, BlueMineralName[randomIdx]);
		break;
	case 2:
		//DARK_GREEN

		randomIdx = rand() % GreenMineralCount;

		res_mineralName = new char[strlen(GreenMineralName[randomIdx])];
		strcpy(res_mineralName, GreenMineralName[randomIdx]);

		break;
	case 3:
		//DARK_SKYBLUE

		randomIdx = rand() % SkyBlueMineralCount;

		res_mineralName = new char[strlen(SkyBlueMineralName[randomIdx])];
		strcpy(res_mineralName, SkyBlueMineralName[randomIdx]);

		break;
	case 4:
		//DARK_RED

		randomIdx = rand() % RedMineralCount;

		res_mineralName = new char[strlen(RedMineralName[randomIdx])];
		strcpy(res_mineralName, RedMineralName[randomIdx]);

		break;
	case 5:
		//DARK_VIOLET

		randomIdx = rand() % VioletMineralCount;

		res_mineralName = new char[strlen(VioletMineralName[randomIdx])];
		strcpy(res_mineralName, VioletMineralName[randomIdx]);

		break;
	case 6:
		//DAKR_YELLOW

		randomIdx = rand() % YellowMineralCount;

		res_mineralName = new char[strlen(YellowMineralName[randomIdx])];
		strcpy(res_mineralName, YellowMineralName[randomIdx]);

		break;
	default:
		res_mineralName = new char[strlen("알 수 없는 광물(오류)")+1];
		strcpy(res_mineralName, "알 수 없는 광물(오류)");
	}
	return res_mineralName;
}

void Mine::Update() {
	enemy.Update(playerX, playerY, ground, &mineBool);

	currentTime_render = clock(); //지금 시각

	renderTimeCheck = (currentTime_render - prevTime_render) / CLOCKS_PER_SEC; //지나간 시간

	if (renderTime <= renderTimeCheck) {
		//랜덤한 시간이 지나면(처음은 3초) 생성을 한다.
		renderTime = rand() % produceMineralSec + 1;//1 ~ produceMineralSec초 사이의 랜덤한 생성
		prevTime_render = clock();

		//생성(광물의 생성을 벽이 있다면 그곳에 생성을 못하도록 하기)
		//x, y값을 생성하고 item은 실제 광물의 역할을 하며 번호에 따른 색이 부여됨.
		//그리고 ground는 땅의 출력을 할 때 사용됨
		do {
			mineralX = rand() % GAMEPLAY_GROUND_WIDTH;
			mineralY = rand() % GAMEPLAY_GROUND_HEIGHT;
			if (ground[mineralY][mineralX] == ROAD) {
				ground[mineralY][mineralX] = "■";
				break;
			}
		} while (ground[mineralY][mineralX] != "■");

		//item(광물의 색)을 랜덤으로 정해줌
		float randomItem = rand() % 10001; //0 ~ 10000
		if (randomItem <= (mineralPercentage[DARK_SKYBLUE]*100)) { item[mineralY][mineralX] = DARK_SKYBLUE; } //가장 희귀한 광물
		else if (randomItem <= (mineralPercentage[DARK_BLUE] * 100)) { item[mineralY][mineralX] = DARK_BLUE; }
		else if (randomItem <= (mineralPercentage[DARK_GREEN] * 100)) { item[mineralY][mineralX] = DARK_GREEN; }
		else if (randomItem <= (mineralPercentage[DARK_RED] * 100)) { item[mineralY][mineralX] = DARK_RED; }
		else if (randomItem <= (mineralPercentage[DARK_VIOLET] * 100)) { item[mineralY][mineralX] = DARK_VIOLET; }
		else{ item[mineralY][mineralX] = DARK_YELLOW; }

		mineralManager.AddMineral(randomMineral(item[mineralY][mineralX]), mineralX, mineralY);//랜덤 광물 생성해서 생성된 광물 추가
	}

	//광물 삭제 랜덤
	renderTimeCheck_delete = (currentTime_render - prevTime_render_delete) / CLOCKS_PER_SEC; //지나간 시간

	if (renderTime_delete <= renderTimeCheck_delete) {
		//랜덤한 시간이 지나면(처음은 10초) 삭제를 한다.
		renderTime_delete = rand() % deleteMineralSec + 1;//1 ~ deleteMineralSec초 사이의 랜덤한 삭제
		prevTime_render_delete = clock(); //이전 시간을 기록

		//랜덤한 광물을 삭제
		mineralManager.RandomMineralDel(ground, item);
	}
}

void Mine::Render() {
	enemy.Render(ground);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), item[mineralY][mineralX]);
	gotoXY(mineralX * 2, COORDINATE_TOP + mineralY);
	cout << ground[mineralY][mineralX];
}