
#include"Game.h"
#include <iostream>
#include<stdio.h>
#define EXT_KEY			0xffffffe0	//확장키 인식값 
#define KEY_LEFT		0x4b
#define KEY_RIGHT		0x4d
#define KEY_UP			0x48
#define KEY_DOWN		0x50 

Game::Game() {
	printer = new Printer();
}

void Game::run() {
	printer->show_logo();
	while (1)
	{
		score = 0;
		lines = 0;
		if (is_gameover == 3) {
			printer->show_logo();
		}

		
		input_data();
		if (isReverseWorld(level)) {
			init_Reversed();
		}
		else {
			init();
		}
		
	
		is_gameover = 0;

		curr_block = new Block(stages->get_stick_rate(level));
		//printer->show_next_block(*curr_block, level);

		next_block = new Block(stages->get_stick_rate(level));
		printer->show_next_block(*next_block, level);

		//printer->show_next_block(*next_block, level);
		printer->show_total_block(total_block, level);

		if (isReverseWorld(level)) {
			curr_block->start_Reversed();
		}else {
			curr_block->start();
		}
		

		printer->show_gamestat(level, score, stages->get_clear_line(level) - lines);

		for (i = 1;1;i++)
		{
			if (_kbhit())
			{
				keytemp = _getche();
				if (keytemp == EXT_KEY)
				{
					keytemp = _getche();
					switch (keytemp)
					{
					case KEY_UP:		//회전하기
						rotate();
						break;
					case KEY_LEFT:		//왼쪽으로 이동
						if (curr_block->get_x() > 1)
						{
							printer->erase_cur_block(*curr_block);
							curr_block->move_left();
							if (strike_check() == 1)
								curr_block->move_right();

							printer->show_cur_block(*curr_block);
						}
						break;
					case KEY_RIGHT:		//오른쪽으로 이동

						if (curr_block->get_x() < 14)
						{
							printer->erase_cur_block(*curr_block);
							curr_block->move_right();
							if (strike_check() == 1)
								curr_block->move_left();
							printer->show_cur_block(*curr_block);
						}
						break;
					case KEY_DOWN:		//아래로 이동
						if (isReverseWorld(level)) {
							is_gameover = move_block_Reversed();
						}else {
							is_gameover = move_block();
						}

						if (is_gameover != 1 && is_gameover != 3) {
							printer->show_cur_block(*curr_block);
						}
						break;
					}
				}
				else {
					//특수 키 아닌 경우 버퍼 지우기
					while (_kbhit()) (void)_getch();
				}

				if (keytemp == 32)	//스페이스바를 눌렀을때
				{
					while (is_gameover == 0)
					{
						if (isReverseWorld(level)) {
							is_gameover = move_block_Reversed();
						}else {
							is_gameover = move_block();
						}
					}
					printer->show_cur_block(*curr_block);
					while (_kbhit()) (void)_getch();

				}
			}
			if (i % stages->get_speed(level) == 0)
			{
				if (isReverseWorld(level)) {
					is_gameover = move_block_Reversed();
				}else {
					is_gameover = move_block();
				}

				if (is_gameover != 1 && is_gameover != 3) {
					printer->show_cur_block(*curr_block);
				}
			}


			if (is_gameover == 1)
			{
				printer->show_gameover();
				printer->SetColor(GRAY);
				is_gameover = 0;
				while (_kbhit()) (void)_getch();
				system("cls");
				break;
			}
			if (is_gameover == 3) {
				while (_kbhit()) (void)_getch();
				system("cls");
				break;
			}

			printer->gotoxy(77, 23);
			Sleep(15);
			printer->gotoxy(77, 23);
		}
	}
	return;
}

int Game::init() {
	int i, j;

	srand((unsigned)time(NULL));


	for (i = 0;i < 20;i++)
	{
		for (j = 0;j < 14;j++)
		{
			if ((j == 0) || (j == 13))
			{
				total_block[i][j] = 1;
			}
			else {
				total_block[i][j] = 0;
			}
		}
	}

	for (j = 0;j < 14;j++)			//화면의 제일 밑의 줄은 1로 채운다.
		total_block[20][j] = 1;

	//전역변수 초기화
	lines = 0; //라인도 0으로

	stages = new Stages();

	return 0;
}

int Game::init_Reversed()
{
	int i, j;

	srand((unsigned)time(NULL));


	for (i = 0;i < 20;i++)
	{
		for (j = 0;j < 14;j++)
		{
			if ((j == 0) || (j == 13))
			{
				total_block[i][j] = 1;
			}
			else {
				total_block[i][j] = 0;
			}
		}
	}

	//전역변수 초기화
	lines = 0; //라인도 0으로

	stages = new Stages();

	return 0;
}

int Game::input_data()
{
	int i = 0;

	char buf[5];

	//입력값 오류 해결 --> scanf_s()로 받게 되는 경우 1.0도 1로 인식하는 경우 생김, 
	//또한 선행 0을 허용하기 때문에 문제를 해결하기 위해 문자를 입력받는것으로 바꿈
	while (1) {
		printer->show_keys();

		fgets(buf, sizeof(buf), stdin);

		// '\n'이 버퍼 안에 포함되어 있으면 → 그냥 제거하고 넘어감
		if (strchr(buf, '\n')) {
			buf[strcspn(buf, "\n")] = '\0';
		}
		// '\n'이 없으면 → 길어서 초과된 것 → getchar로 나머지 버퍼 비우기
		else {
			int ch;
			while ((ch = getchar()) != '\n' && ch != EOF);
		}

		// 문자열이 정수형 숫자 하나로만 이루어졌는지 확인
		if (strlen(buf) == 1 && buf[0] >= '1' && buf[0] <= '8') {
			i = buf[0] - '0'; // 문자 → 정수로 변환
			break;
		}
		else {
			printer->gotoxy(10, 4);
			printf("올바른 숫자를 입력해주세요.");
			Sleep(1000);
			printer->gotoxy(10, 4);
			printf("                            ");
			system("cls");
		}
	}

	level = i - 1;
	system("cls");
	return 0;
}

//블록을 아래로 내려주는 함수, 바닥에 닿거나 다른 블록과 충돌하면 멈추며 고정
int Game::move_block()
{ 
	printer->erase_cur_block(*curr_block);

	curr_block->move_down();	//블럭을 한칸 아래로 내림
	if (strike_check() == 1)
	{
		//이 내용은 새로 추가된 부분으로 기존에 이 코드가 없었을 때는 블록의 위치가 -3에서 시작하는 경우에 대해서 그 상황에서 방향 조작키를 입력하고 스페이스를 입력하는 경우 갑자기 게임 종료되는 문제가 있어서 코드를 추가했습니다
		if (curr_block->get_y() < 0) {
			return 0;
		}

		curr_block->move_up();
		if (curr_block->get_y() < 0)	//게임오버
		{

			//printer->SetColor(3);
			//printf("%d %d \n", curr_block->get_x(), curr_block->get_y());
			//system("pause");

			printer->SetColor(DARK_GRAY);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (curr_block->get_number(i, j) == 1) {
						int dx = curr_block->get_x() + j;
						int dy = curr_block->get_y() + i;
						if (dx >= 0 && dx <= 14 && dy >= 0 && dy < 20) {
							printer->gotoxy(dx * 2 + printer->get_x(), dy + printer->get_y());
							printf("■");
						}
					}
				}
			}
			Sleep(100);


			return 1;
		}

		int is_over = merge_block();

		if (is_over == 3) {
			return 3;
		}
		delete curr_block;
		curr_block = next_block;
		//만약 클리어한 라인의 수가 깨야되는 줄의 반이라면 콤보가 발동해 다음블록은 무조건 일자 블록이 나온다
		if (lines != 0 && stages->get_clear_line(level) / lines == 2) {
			next_block = new Block(stages->get_stick_rate(level), true);
		}
		else {
				next_block = new Block(stages->get_stick_rate(level));
		}
	
		printer->show_next_block(*next_block, level);

		if (isReverseWorld(level)) {
			curr_block->start_Reversed();
		}
		else {
			curr_block->start();
		}
		printer->show_next_block(*next_block, level);
		return 2;
	}
	printer->erase_cur_block(*curr_block);

	return 0;
}

int Game::move_block_Reversed()
{

	printer->erase_cur_block(*curr_block);

	curr_block->move_up();	//블럭을 한칸 위로 올림
	if (strike_check_Reversed() == 1)
	{
		if (curr_block->get_y() > 20) {
			return 0;
		}

		curr_block->move_down();
		if (curr_block->get_y() > 20)	//게임오버
		{

			//printer->SetColor(3); 갑작스러운 종료에 대한 디버깅 흔적
			//printf("%d %d \n", curr_block->get_x(), curr_block->get_y());
			//system("pause");

			printer->SetColor(DARK_GRAY);
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (curr_block->get_number(i, j) == 1) {
						int dx = curr_block->get_x() + j;
						int dy = curr_block->get_y() + i;
						if (dx >= 0 && dx <= 14 && dy >= 0 && dy < 20) {
							printer->gotoxy(dx * 2 + printer->get_x(), dy + printer->get_y());
							printf("■");
						}
					}
				}
			}
			Sleep(100);


			return 1;
		}

		int is_over = merge_block();

		if (is_over == 3) {
			return 3;
		}
		delete curr_block;
		curr_block = next_block;

		//만약 클리어한 라인의 수가 깨야되는 줄의 반이라면 콤보가 발동해 다음블록은 무조건 일자 블록이 나온다
		if (lines != 0 && (stages->get_clear_line(level) / lines) == 2) {
			next_block = new Block(stages->get_stick_rate(level), true);
		}
		else {
			next_block = new Block(stages->get_stick_rate(level));
		}
		printer->show_next_block(*next_block, level);

		//이것도 필수적인게 단계 클리어가 되는 경우도 있기 때문에 거꾸로 나라에 해당하는 스테이지인지 아닌지에 따라 블록의 시작 위치가 변합니다
		if (isReverseWorld(level)) {
			curr_block->start_Reversed();
		}
		else {
			curr_block->start();
		}
		printer->show_next_block(*next_block, level);
		return 2;
	}
	printer->erase_cur_block(*curr_block);

	return 0;
}

//충돌여부 확인
int Game::strike_check()
{
	int i, j;
	int block_dat;

	
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			

			if (((curr_block->get_x() + j) == 0) || ((curr_block->get_x() + j) == 13))
				block_dat = 1;
			else
				block_dat = total_block[curr_block->get_y() + i][curr_block->get_x() + j];


			if ((block_dat == 1) && (curr_block->get_number(i, j) == 1))																							//좌측벽의 좌표를 빼기위함
			{
				return 1;
			}
		}
	}
	return 0;
}

int Game::strike_check_Reversed()
{

	int i, j;
	int block_dat;


	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			//추가된 내용 
			if (curr_block->get_y() <= 0) {
				return 1;
			}

			if (((curr_block->get_x() + j) == 0) || ((curr_block->get_x() + j) == 13))
				block_dat = 1;
			else
				block_dat = total_block[curr_block->get_y() + i][curr_block->get_x() + j];


			if ((block_dat == 1) && (curr_block->get_number(i, j) == 1))																							//좌측벽의 좌표를 빼기위함
			{
				return 1;
			}
		}
	}
	return 0;
}

//떨어지고 있는 블럭이 다른 블럭이나 바닥에 부딪혔을때 고정 시키는 함수
int Game::merge_block()
{
	int i, j;
	for (i = 0;i < 4;i++)
	{
		for (j = 0;j < 4;j++)
		{
			total_block[curr_block->get_y() + i][curr_block->get_x() + j] |= curr_block->get_number(i, j);
		}
	}
	int is_clear = check_full_line();
	if (is_clear == 3) {
		return 3;
	}
	printer->show_total_block(total_block, level);

	return 0;
}

//꽉 차있는 줄을 삭제하고 위의 블록들을 내려줌.
int Game::check_full_line()
{
	int i, j, k;
	for (i = 0;i < 20;i++)
	{
		for (j = 1;j < 13;j++)
		{
			if (total_block[i][j] == 0)
				break;
		}
		if (j == 13)	//한줄이 다 채워졌음
		{
			
			lines++;
			//줄과 레벨 확인



			if (stages->get_clear_line(level) <= lines) {
				if (level < 9) {
					lines = 0;
					level++;
					//추가된 내용 다음 단계가 거꾸로 나라인지 아닌지에 따라서 다르게 화면이 초기화 되게 내용을 추가했습니다
					if (isReverseWorld(level)) {
						init_Reversed();
						
					}
					else {
						init();
						
					}
					printer->show_gamestat(level, score, stages->get_clear_line(level) - lines);
				}
				//만약 레벨이 10이 되는 경우 쇼우 클리어를 진행
				else {
					//show_clear_screen이라는 것에서 1의 값을 받으면 3을 반환
					// --> merge_block()에서 이 값에 따른 반환값을 만들도록함
					//메인 함수에서 is_game_over이 3일때 break함
					//이때 화면에서 show_logo()가 잘 보이지 않는다는 것을 확인하여 show_logo() 앞 cls를 통해 지우고 다시 그리도록 함.

					printer->show_total_block(total_block, level);
					lines = 0;
					printer->show_gamestat(level, score, stages->get_clear_line(level) - lines);
					if (printer->show_clear_screen(score)) return 3;
				}
			}

			printer->show_total_block(total_block, level);
			printer->SetColor(BLUE);
			printer->gotoxy(1 * 2 + printer->get_x(), i + printer->get_y());
			for (j = 1;j < 13;j++)
			{
				printf("□");
				Sleep(10);
			}
			printer->gotoxy(1 * 2 + printer->get_x(), i + printer->get_y());
			for (j = 1;j < 13;j++)
			{
				printf("  ");
				Sleep(10);
			}

			for (k = i;k > 0;k--)
			{
				for (j = 1;j < 13;j++)
					total_block[k][j] = total_block[k - 1][j];
			}
			for (j = 1;j < 13;j++)
				total_block[0][j] = 0;
			score += 100 + (level * 10) + (rand() % 10);
			printer->show_gamestat(level, score, stages->get_clear_line(level) - lines);
		}

	}

	return 0;
}

int Game::rotate() {
	int old_angle = curr_block->get_angle();
	int new_angle = (old_angle + 1) % 4;

	curr_block->rotate(new_angle);
	if (strike_check() == 0)
	{
		curr_block->rotate(old_angle);
		printer->erase_cur_block(*curr_block);
		curr_block->rotate(new_angle);
		printer->show_cur_block(*curr_block);
		return 0;
	}

	curr_block->rotate(old_angle);
	return 0;
}

//거꾸로 나라의 스테이지인지 여부를 확인하는 함수 추가됨
bool Game::isReverseWorld(int level)
{
	if (level == 0 || level == 3 || level == 6)
		return true;
	else {
		return false;
	}
	
}

