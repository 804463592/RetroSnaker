#include<Windows.h>
#include<stdio.h>
#include<conio.h>
#include<time.h>

// �󷽺�������
void restart();
void gotoxy(int x, int y);

// �����Ӧֵ
enum MoveDir { RIGHT = 0, UP = 1, LEFT = 2, DOWN = 3 };

// ��ʼʱ���, ����ʱ���
time_t start, end;

int food[25][20] = { 0 };  // ��ʵ��Ӧ����
int snakel[25][20] = { 0 };// �����Ӧ����

int score = 0;      // �÷�
int x = 0, y = 0;   // ����
int direct = RIGHT; // �ƶ�����
int mark = 0;       // ���ԭ����

int length = 0;     // ����������
int slength = 1;    // �����¼����

// ��������
int  dir[4][2] = {
		{ 1,  0 }, // 0 ����  x���� +1  y���� +0
		{ 0, -1 }, // 1 ����  x���� +0  y���� -1
		{-1,  0 }, // 2 ����  x���� -1  y���� +0
		{ 0,  1 }  // 3 ����  x���� +0  y���� +1
};

/*
	snake �ṹ�� ���ڹ���̰��������
*/
struct snake
{
	int x, y;
	struct snake *next, *pre;
} *head, *tail, *p; // �����ṹ���ͬʱ����ͷָ�롢βָ�롢��ʱָ��


DWORD WINAPI ThreadProc(LPVOID lpPraram)
{
	SYSTEMTIME sys; // ϵͳʱ��ṹ��

	// �߳�����ʱ, ͷβָ���Ϊ��
	head = tail = NULL;
	int a, b;

	// �漴��ȡ��ʵ���� (a,b)
	a = (rand() + start) % 25; // [0,24]
	b = (rand() + start) % 20; // [0,19]

	/* Note ��ʼʱ�� start + ���ص����ֵ rand() = һ�������ɵ����ֵ
	 *      ��� % ��ģ 25 ���������������0��25֮�� */

	 // ���߳�ѭ����ӡ����͹�ʵ
	while (1)
	{
		//��ӡ�Ѻ���Ϸʱ��;
		end = time(NULL);// ��ǰʱ��
		gotoxy(35, 4);
		printf("%03d", (int)difftime(end, start)); // ��ǰʱ���뿪ʼʱ��Ĳ�ֵ

		GetLocalTime(&sys); // ��ȡ��ǰʱ��

		//��ӡϵͳʱ��;
		gotoxy(28, 18);
		printf("%4d/%02d/%02d", sys.wYear, sys.wMonth, sys.wDay);
		gotoxy(29, 19);
		printf("%02d:%02d:%02d", sys.wHour, sys.wMinute, sys.wSecond);

		//�жϹ�ʵ�Ƿ񱻳���
		if (food[a][b] != '*' || snakel[a][b] == 1)
		{
			a = (rand() + start) % 25; // [0,24]
			b = (rand() + start) % 20; // [0,19]
			food[a][b] = '*';
		}

		// ����Ļ��(a,b)�����ӡ��ʵ
		gotoxy(a, b); printf("*");

		//��ȡ��һ������
		x += dir[direct][0];
		y += dir[direct][1];
		x += 25; x %= 25;
		y += 20; y %= 20;

		// �����ʵ(food)�����е�ǰ�����ӦΪ��ʵ
		if (food[x][y] == '*')
		{
			slength++;  // ����������
			score++;    // ��������
			// ˢ�·���
			gotoxy(35, 3); printf("%03d", score);
			// �����ʵ(food)����
			food[x][y] = 0;
		}
		// �����ǹ�ʵ���жϵ�ǰ�����Ƿ�Ϊ����
		else if (snakel[x][y] == 1)
		{
			// �����������ҳԵ��Լ�����Ϸ����
			restart();
		}

		snakel[x][y] = 1; // �����Ӧ��������1

		// �����������Ӧ������ڵ�
		p = (snake*)malloc(sizeof(snake));
		p->x = x;
		p->y = y;
		p->next = p->pre = NULL;

		if (head == NULL) {
			// ��ֻ��һ���ڵ㣨���ߵĳ���Ϊ1��������Ϊ0��
			head = tail = p;
		}
		else
		{
			// ����ֻһ���ڵ�
			head->pre = p; // ͷ����ǰһ��ָ�� p �ڵ�
			p->next = head;    // p �ڵ���һ��ָ��ǰͷ���
			head = p;         // �µ� p ���ͷ�����
		}

		length++; // �����ȼ�1 

		// �������꣬Ҳ����ͷ����ӡ #
		gotoxy(head->x, head->y);
		printf("#");

		// ���������Ϣ
		gotoxy(49, 10); printf("length: %d slength: %d", length, slength);

		// ɾ����β
		if (length > slength)
		{
			// �����β�����Ӧֵ
			snakel[tail->x][tail->y] = 0;
			// ����̨�����ָ���ƶ�����β
			gotoxy(tail->x, tail->y);
			// ����ո�
			printf(" ");

			// ��ʱָ��ָ����β
			p = tail;
			// βָ��ǰ��
			tail = tail->pre;
			// �ͷ�ԭ����βָ��
			free(p);
			// ���������ȼ�1
			length--;
		}

		/*
		 * �����ٶ���ʱ����
		 * �����Ҫ������Ϸ�ٶ�ֻ��Ҫ�޸����ߵ�ֵ����
		 */
		Sleep(50);

		// ��ǵ�ǰ����
		mark = direct;
	}
}

/*
	��ʼ������
*/
void init()
{
	int i;

	// ������Χ�߿�
	for (i = 0; i < 20; i++)
	{
		gotoxy(39, i); printf("|");
		gotoxy(25, i); printf("|");
	}
	for (i = 0; i < 40; i++)
	{
		gotoxy(i, 20); printf("~");
	}

	// �Ҳ���Ϸ��¼, ����˵��
	gotoxy(26, 3); printf("��Ϸ�÷�:%03d", score);
	gotoxy(26, 4); printf("��Ϸʱ��:");
	gotoxy(29, 17); printf("ϵͳʱ��");

	gotoxy(49, 1); printf("@lellansin");
	gotoxy(49, 2); printf("www.lellansin.com");

	gotoxy(49, 4); printf("һȺ  10191598");
	gotoxy(49, 5); printf("��Ⱥ 163859361");
	gotoxy(49, 6); printf("��Ⱥ  10366953");

	gotoxy(49, 8); printf("��ESC���˳�");

	// ��ʼ����Ϸ��ʼʱ��
	start = time(NULL);
}

int main()
{
	int i = 0;
	HANDLE sonThreadHandle; // ���߳̾��
	DWORD dwThreadId;       // ���߳�ID

	// �����ϰ벿���Լ��°벿��
	char keyCodePart1, keyCodePart2;

	// ִ�г�ʼ��
	init();

	//�����߳�
	sonThreadHandle = CreateThread(
		NULL,       // ���ܱ��ӽ��̼̳�
		0,          // Ĭ�϶�ջ��С
		ThreadProc, // �̵߳��ú�������
		NULL,       // ���ݲ���
		0,          // ����������ִ��
		&dwThreadId // ���洴������߳�ID
	);

	// ����߳̾��Ϊ�գ����½��߳�ʧ��
	if (sonThreadHandle == NULL)
	{
		ExitProcess(i); // �˽���, �������
	}

	// ���߳�ѭ����ȡ����
	while (1)
	{
		// getch һ��ֻ�ܴ��������ж�ȡһ���ֽ�
		// ����һ������ͨ���������ֽ�
		keyCodePart1 = _getch(); // ��ȡ��һ���ֽ�

		// �ж��Ƿ�������Ѽ�(Esc)�ĵ�һ���ֽ�
		if (keyCodePart1 == 0x1b)
		{
			ExitProcess(2); // �ǵĻ����˳�����
		}

		// �������һ���ֽڵ�ֵ���� -32
		if (keyCodePart1 == -32)
		{
			keyCodePart2 = _getch(); // ��ȡ�ڶ����ֽ�
			switch (keyCodePart2)
			{
			case(72):
				// direct=1;
				direct = UP;
				break;
			case(75):
				// direct=2;
				direct = LEFT;
				break;
			case(77):
				// direct=0;
				direct = RIGHT;
				break;
			case(80):
				// direct=3;
				direct = DOWN;
				break;
			}
			// �����ǰ������ԭ�����෴
			if (direct != mark && (direct + mark == RIGHT + LEFT || direct + mark == UP + DOWN))
				direct = mark;
		}
		/*
			Ϊʲô�� 0x1b Ϊʲô�� -32 ֮���ֵ��
			��Щ����ԭ����ͬ���İ취������ getch ֮��
			��ӡ����¼�¸���������ֵ�õ����ж�
		*/
	}
	return 0;
}

void restart()
{
	//�ж�����,���¿�ʼ��Ϸ;
	MessageBox(NULL, (LPCSTR)"ɵ��,���˰�!!!Once More???", (LPCSTR)"GAME VOER!", MB_OK);

	// ��������
	score = 0; gotoxy(35, 3); printf("%03d", score);
	// ��ʼʱ���ؼ�
	start = time(NULL);
	// ��Ϸ�ѽ���ʱ���ӡΪ0
	gotoxy(35, 4); printf("%03d", 0);

	// ��β����ʼѭ����������
	while (tail)
	{
		// �������ӻ�����ʱ��
		Sleep(200);
		snakel[tail->x][tail->y] = 0;   // �����ǰ������ֵ
		// �ƶ�����Ļ��Ӧ�����������
		gotoxy(tail->x, tail->y); printf(" ");
		// ��ʱָ��ָ��βָ���ǰһ���ڵ�
		p = tail->pre;
		// �ͷ����һ���ڵ�
		free(tail);
		// ��ʱ�ڵ����µ�β��
		tail = p;
	}
	// ���нڵ��ͷ���ϣ�ͷβָ�����
	head = tail = NULL;
	slength = 1; // ��������1
	length = 0;    // ��������0
}


/*
	��������̨��(x,y)����
*/
void gotoxy(int x, int y)
{
	COORD coord; // ����̨����ṹ��
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), // ��ȡ����̨������ľ��
		coord   // �������λ�õ�����
	);
}