//Программа TimeRush
//Игроки ходят по очереди, причем для меня путь каждый раз перерассчитывается
//Игра проиграна, если меня убьют, выиграна, если все в команде противника убиты
//2 команды по 4 игрока (я восьмой)
//Мой игрок может ходить на 4 клетки, все остальные на 1 за ход
//Стрелять можно только по прямой
//Выстрел стоит 1 ход, выстрел убивает мгновенно
//Внезапность подразумевает выход на позицию, выстрел и уход с линии огня
//Боты не умеют просчитывать далее 1 позиции и ходят в случайную сторону, поэтому совсем неосторожны =)

#include "stdafx.h"
#include <iostream>
#include "windows.h"
//#include <algorithm>

using namespace std;

class Maps
{
private:
	FILE * Input_data, *Output_data;
	char Map[20][20];
	//int x, y, c;
	bool Player[8];
	struct XY
	{
		int x = 99, y = 99;
	};
	struct XYD
	{
		XY xy;
		int d = 99;
	};
	bool AddPlayer(int c) //Проверка игроков при добавлении на карту
	{
		int num;
		try
		{
			if (((num = (char)c - '0')<9) && (num>0) && Player[num - 1] != 1)
				Player[num - 1] = 1;
			else if (Player[num - 1] == 1) throw invalid_argument("Неправильный ввод игроков!");
		}
		catch (invalid_argument& e)
		{
			cerr << e.what() << endl;
			return 0;
		}
		return 0;
	}
	void KillPlr(int plrnum)//Удаляем игрока из списка живых
	{
		Player[plrnum-1] = 0;
	}
	void RefreshMap()//Обновление карты в консоли
	{
		system("cls");
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++) cout << Map[i][j];
			cout << endl;
		}
	}
	XY FindPos(int plrnum)//Ищем координаты данного игрока
	{
		XY pos;
		plrnum++;//На карте игроки обозначены от 1 до 8
		if (Alive(plrnum-1)) for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			if (Map[j][i] == plrnum + '0')
			{
				pos.x = i;
				pos.y = j;
				return pos;
			}
		}
		else
		{
			pos.x = 99;
			pos.y = 99;
			return pos;
		}
	}
	int FindToKill(int plrnum, int x, int y)
		//Ищем возможность для выстрела из заданной позиции
		//Возвращаемые значения: 1-верх 2-право 3-низ 4-лево 0-нигде
	{
		if ((plrnum >= 0) && (plrnum <= 3))
		{
			if ((WhatFarAwayUp(x, y) >= '5') && (WhatFarAwayUp(x, y) <= '8')) return 1;
			if ((WhatFarAwayRight(x, y) >= '5') && (WhatFarAwayRight(x, y) <= '8')) return 2;
			if ((WhatFarAwayDown(x, y) >= '5') && (WhatFarAwayDown(x, y) <= '8')) return 3;
			if ((WhatFarAwayLeft(x, y) >= '5') && (WhatFarAwayLeft(x, y) <= '8')) return 4;
		}
		else if ((plrnum >= 4) && (plrnum <= 7))
		{
			if ((WhatFarAwayUp(x, y) >= '1') && (WhatFarAwayUp(x, y) <= '4')) return 1;
			if ((WhatFarAwayRight(x, y) >= '1') && (WhatFarAwayRight(x, y) <= '4')) return 2;
			if ((WhatFarAwayDown(x, y) >= '1') && (WhatFarAwayDown(x, y) <= '4')) return 3;
			if ((WhatFarAwayLeft(x, y) >= '1') && (WhatFarAwayLeft(x, y) <= '4')) return 4;
		}
		return 0;
	}
	void Kill(int killway, int x, int y)//Удаляем с карты игрока (убиваем) в заданном направлении с текущей позиции
	{
		char t;
		switch (killway)
		{
		case (1) ://Сверху		
			do
			{
				x--;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
			break;
		case (2) ://Справа		
			do
			{
				y++;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
			break;
		case (3) ://Снизу		
			do
			{
				x++;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
			break;
		case (4) ://Слева
			do
			{
				y--;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
			break;
		}
		RefreshMap();
	}
	char WhatFarAwayUp(int x, int y)//Разведываем верх
	{
		char t = Map[x][y];

		while ((x > 0) && (Map[x][y] != 'x'))
		{
			x--;
			t = Map[x][y];
			if ((t >= '1') && (t <= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	char WhatFarAwayRight(int x, int y)//Разведываем право
	{
		char t = Map[x][y];
		while ((y < 19) && (Map[x][y] != 'x'))
		{
			y++;
			t = Map[x][y];
			if ((t >= '1') && (t <= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	char WhatFarAwayDown(int x, int y)//Разведываем низ
	{
		char t = Map[x][y];
		while ((x < 19) && (Map[x][y] != 'x'))
		{
			x++;
			t = Map[x][y];
			if ((t >= '1') && (t <= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	char WhatFarAwayLeft(int x, int y)//Разведываем лево
	{
		char t = Map[x][y];
		while ((y > 0) && (Map[x][y] != 'x'))
		{
			y--;
			t = Map[x][y];
			if ((t >= '1') && (t <= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	void DoMove(int plrnum, int x, int y)
	{
		plrnum++;
		int randparam = 0, i = 0;
		//Узнаём, какие направления свободны
		bool direction[4] = { 0, 0, 0, 0 };
		if ((x != 0) && (Map[x - 1][y] == '0'))  direction[0] = 1;//Верх
		if ((y != 19) && (Map[x][y + 1] == '0')) direction[1] = 1;//Право
		if ((x != 19) && (Map[x + 1][y] == '0')) direction[2] = 1;//Низ
		if ((y != 0) && (Map[x][y - 1] == '0'))  direction[3] = 1;//Лево
		//Случайно выбираем из доступных
		for (int i = 0; i <= 3; i++) if (direction[i] == 1) randparam++;
		i = 0;
		if (randparam != 0)
		{
			randparam = rand() % randparam + 1;
			do
			{
				if (direction[i] == 1) randparam--;
				i++;
			} while (randparam != 0);
			switch (i)
			{
			case (1) ://Вверх
				Map[x][y] = '0';
				Map[x - 1][y] = plrnum + '0';
				break;
			case (2) ://Направо
				Map[x][y] = '0';
				Map[x][y + 1] = plrnum + '0';
				break;
			case (3) ://Вниз
				Map[x][y] = '0';
				Map[x + 1][y] = plrnum + '0';
				break;
			case (4) ://Налево
				Map[x][y] = '0';
				Map[x][y - 1] = plrnum + '0';
				break;
			}
		}
	}
	void MakeMyTurn(int x, int y)//Производим свои ходы
	{
		XY MyPos;
		MyPos.x = x;
		MyPos.y = y;
		XYD EnemyPos;
		XY MySafePos;
		int inc = 1;//Счетчик произведенных передвижений до цели за данный ход (Используется при передвижениях)
		bool DrowMap = 0;
		int killway, moves = 4;
		int WorkMap[20][20];//Рабочая карта расстояний до врагов
		for (int i = 0; i <= 19; i++)
		for (int j = 0; j <= 19; j++)
		{
			if ((Map[i][j] >= '1') && (Map[i][j] <= '7') || (Map[i][j] == 'x'))
			{
				WorkMap[i][j] = 111;
			}
			else WorkMap[i][j] = Map[i][j] - '0';
		}
		int(*WorkMapLink)[20] = WorkMap;//Создаем массив ссылок на рабочий дубликат карты для экономии памяти
		while (moves > 0)
		{
			Sleep(400);
			killway = FindToKill(7, MyPos.x, MyPos.y);
			if ((moves != 1) && (killway != 0))//Если не последний ход и можно кого-то убить, убиваем 
			{
				DrowMap = 0;
				CleanWorkMap(WorkMapLink, MyPos.x, MyPos.y);
				inc = 1;//Если мы кого-то убили(да), обнуляем счетчик сделанных передвижений до цели
				Kill(killway, MyPos.x, MyPos.y);
				moves--;
				RefreshMap();
				continue;
			}
			if (DrowMap == 0)//Если никого нельзя убить и путь не нарисован, рисуем путь и производим 1 передвижение
			{
				DrowMap = 1;
				SpreadWaveBig(MyPos.x, MyPos.y, WorkMapLink);//Создаем карту дальностей хода
				EnemyPos = FindClothestEnemy(WorkMapLink);
				if (EnemyPos.xy.x == 99)
				{//не работает ????
					moves = 0;
					continue;//Если враги недоступны, заканчиваем наш ход
				}
				int increment = inc;//Обход костыля
				DrawShortestWay(WorkMapLink, EnemyPos);//Рисуем кратчайший путь и делаем ход
				inc = increment;//Почему-то иногда после функции DrawShortestWay inc становится равным -1 ????????????
				if ((moves == 1) && (killway != 0))//Если всех убили, заканчиваем ход
				{
					Map[MySafePos.x][MySafePos.y] = '8';
					Map[MyPos.x][MyPos.y] = '0';
					moves = 0;
					continue;
				}
				else MySafePos = MyPos;
				MyPos = DoMyMove(MyPos.x, MyPos.y, WorkMapLink, inc, moves, MySafePos);
				inc++;
				moves--;
				continue;
			}
			if ((moves == 1) && (killway != 0))
			{
				Map[MySafePos.x][MySafePos.y] = '8';
				Map[MyPos.x][MyPos.y] = '0';
				moves = 0;
				continue;
			}
			else MySafePos = MyPos;
			MyPos = DoMyMove(MyPos.x, MyPos.y, WorkMapLink, inc, moves, MySafePos);//Если никого нельзя убить и путь нарисован, просто идем по нему
			inc++;
			moves--;
		}
	}
	void CleanWorkMap(int(*WorkMapLink)[20], int x, int y)
	{
		for (int i = 0; i <= 19; i++)
		for (int j = 0; j <= 19; j++)
		{
			if (WorkMapLink[i][j] != 111)
			{
				WorkMapLink[i][j] = 0;
			}
		}
		WorkMapLink[x][y] = 8;
	}
	XY DoMyMove(int x, int y, int(*WorkMapLink)[20], int inc, int moves, XY MySafePos)//Производим 1 свое безопасное передвижение и возвращаем новые наши координаты
	{
		/*Для отладки
		int mmap[20][20];
		for (int i = 0; i <= 19; i++)
		for (int j = 0; j <= 19; j++)
			mmap[i][j] = WorkMapLink[i][j];
		*/
		RefreshMap();
		XY MyPos;
		if (x != 0) if (WorkMapLink[x - 1][y] == -inc)
		{
			if (moves == 1)
			{
				if (MoveIsSafe(x - 1, y))
				{
					Map[x][y] = '0';
					WorkMapLink[x][y] = 0;
					x--;
					return MyPos = Go(WorkMapLink, x, y);
				}
				else
				{
					MyPos.x = MySafePos.x;
					MyPos.y = MySafePos.y;
					return MyPos;
				}
			}
			else
			{
				Map[x][y] = '0';
				WorkMapLink[x][y] = 0;
				x--;
				return MyPos = Go(WorkMapLink, x, y);
			}
		}
		if (x != 19) if (WorkMapLink[x + 1][y] == -inc)
		{
			if (moves == 1)
			{
				if (MoveIsSafe(x + 1, y))
				{
					Map[x][y] = '0';
					WorkMapLink[x][y] = 0;
					x++;
					return MyPos = Go(WorkMapLink, x, y);
				}
				else
				{
					MyPos.x = MySafePos.x;
					MyPos.y = MySafePos.y;
					return MyPos;
				}
			}
			else
			{
				Map[x][y] = '0';
				WorkMapLink[x][y] = 0;
				x++;
				return MyPos = Go(WorkMapLink, x, y);
			}
		}
		if (y != 0) if (WorkMapLink[x][y - 1] == -inc)
		{
			if (moves == 1)
			{
				if (MoveIsSafe(y - 1, y))
				{
					Map[x][y] = '0';
					WorkMapLink[x][y] = 0;
					y--;
					return MyPos = Go(WorkMapLink, x, y);
				}
				else
				{
					MyPos.x = MySafePos.x;
					MyPos.y = MySafePos.y;
					return MyPos;
				}
			}
			else
			{
				Map[x][y] = '0';
				WorkMapLink[x][y] = 0;
				y--;
				return MyPos = Go(WorkMapLink, x, y);
			}
		}
		if (y != 19) if (WorkMapLink[x][y + 1] == -inc)
		{
			if (moves == 1)
			{
				if (MoveIsSafe(y + 1, y))
				{
					Map[x][y] = '0';
					WorkMapLink[x][y] = 0;
					y++;
					return MyPos = Go(WorkMapLink, x, y);
				}
				else
				{
					MyPos.x = MySafePos.x;
					MyPos.y = MySafePos.y;
					return MyPos;
				}
			}
			else
			{
				Map[x][y] = '0';
				WorkMapLink[x][y] = 0;
				y++;
				return MyPos = Go(WorkMapLink, x, y);
			}
		}
		return MyPos;//Это означает ошибку
	}
	bool MoveIsSafe(int x,int y)
	{
		int killway = FindToKill(7, x, y);
		if (killway == 0) return 1;
		else return 0;
	}
	XY Go(int(*WorkMapLink)[20], int x, int y)
	{
		XY MyPos;
		Map[x][y] = '8';
		WorkMapLink[x][y] = 8;
		MyPos.x = x;
		MyPos.y = y;
		return MyPos;
	}
	void DrawShortestWay(int(*WorkMapLink)[20], XYD EnemyPos)//Помечаем кратчайший путь отрицательными расстояниями
	{
		int d = EnemyPos.d;
		WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x] = -d-1;
		while (d != 0)
		{
			if (EnemyPos.xy.x!=0) if(WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x - 1] == d)
			{
				WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x - 1] = -d;
				d--;
				EnemyPos.xy.x -= 1;
				continue;
			}
			if (EnemyPos.xy.x!= 19) if (WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x + 1] == d)
			{
				WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x + 1] = -d;
				d--;
				EnemyPos.xy.x += 1;
				continue;
			}
			if (EnemyPos.xy.y!= 19) if (WorkMapLink[EnemyPos.xy.y + 1][EnemyPos.xy.x] == d)
			{
				WorkMapLink[EnemyPos.xy.y + 1][EnemyPos.xy.x] = -d;
				d--;
				EnemyPos.xy.y += 1;
				continue;
			}
			if (EnemyPos.xy.y!= 0) if (WorkMapLink[EnemyPos.xy.y - 1][EnemyPos.xy.x] == d)
			{
				WorkMapLink[EnemyPos.xy.y - 1][EnemyPos.xy.x] = -d;
				d--;
				EnemyPos.xy.y -= 1;
			}
		}
	}
	XYD FindClothestEnemy(int(*WorkMapLink)[20])//Ищем координаты ближайшего врага
	{
		XYD Position[4];
		XY ClothestEnemyXY;
		int ShortestEnmDist=99;
		for (int i = 0; i <= 3; i++)
		{
			Position[i].xy = FindPos(i);
			if (Position[i].xy.x != 99)
			{
				Position[i] = TryToFindEnemyXYD(Position[i], WorkMapLink);
				if (ShortestEnmDist == 99)
				{
					ShortestEnmDist = Position[i].d;
					ClothestEnemyXY = Position[i].xy;
				}
				else if (ShortestEnmDist >= Position[i].d)
				{
					ShortestEnmDist = Position[i].d;
					ClothestEnemyXY = Position[i].xy;
				}
			}
		}
		XYD EnemyPos;
		EnemyPos.d = ShortestEnmDist;
		EnemyPos.xy = ClothestEnemyXY;
		return EnemyPos;
	}
	XYD TryToFindEnemyXYD(XYD EnemyPos, int(*WorkMapLink)[20])//Доработка алгоритма Ли к нескольким целям
	{
		int a, direction = 0;
		if ((EnemyPos.xy.x != 19) && ((a = WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x + 1]) != 0) && (a < EnemyPos.d))
		{
			EnemyPos.d = a;
			direction = 1;
		}
		if ((EnemyPos.xy.x != 0) && ((a = WorkMapLink[EnemyPos.xy.y][EnemyPos.xy.x - 1]) != 0) && (a < EnemyPos.d))
		{
			EnemyPos.d = a;
			direction = 2;
		}
		if ((EnemyPos.xy.y != 19) && ((a = WorkMapLink[EnemyPos.xy.y + 1][EnemyPos.xy.x]) != 0) && (a < EnemyPos.d))
		{
			EnemyPos.d = a;
			direction = 3;
		}
		if ((EnemyPos.xy.y != 0) && ((a = WorkMapLink[EnemyPos.xy.y - 1][EnemyPos.xy.x]) != 0) && (a < EnemyPos.d))
		{
			EnemyPos.d = a;
			direction = 4;
		}
		return EnemyPos;
	}
	void SpreadWaveBig(int x, int y, int(*WorkMapLink)[20])//Полное распространение волны
	{
		int ChangedSells = 1;//Количество изменённых полей
		SpreadWaveL(x, y, WorkMapLink, 0, ChangedSells);
		int d = 1;
		while (ChangedSells != 0)
		{
			ChangedSells = 0;
			//Попробовать циклы до 20
			for (int i = 0; i <= d; i++)
			for (int j = 0; j <= d; j++)
			{
				if ((x + i < 20) && (y + j < 20) && (WorkMapLink[x + i][y + j] == d))
				{
					ChangedSells += SpreadWaveL(x + i, y + j, WorkMapLink, d, ChangedSells);
				}
				if ((x - i >= 0) && (y + j < 20) && (WorkMapLink[x - i][y + j] == d))
				{
					ChangedSells += SpreadWaveL(x - i, y + j, WorkMapLink, d, ChangedSells);
				}
				if ((x + i < 20) && (y - j >= 0) && (WorkMapLink[x + i][y - j] == d))
				{
					ChangedSells += SpreadWaveL(x + i, y - j, WorkMapLink, d, ChangedSells);
				}
				if ((x - i >= 0) && (y - j >= 0) && (WorkMapLink[x - i][y - j] == d))
				{
					ChangedSells += SpreadWaveL(x - i, y - j, WorkMapLink, d, ChangedSells);
				}
			}
			d++;
		}
	}
	int SpreadWaveL(int x, int y, int(*WorkMapLink)[20], int d, int ChangedSells)//Локальное распространение волны
	{
		if (x - 1 >= 0)
		if ((WorkMapLink[x - 1][y] != 111) && (WorkMapLink[x - 1][y] == 0))
		{
			WorkMapLink[x - 1][y] = d + 1;
			ChangedSells++;
		}
		if (x + 1 < 20)
		if ((WorkMapLink[x + 1][y] != 111) && (WorkMapLink[x + 1][y] == 0))
		{
			WorkMapLink[x + 1][y] = d + 1;
			ChangedSells++;
		}
		if (y - 1 >= 0)
		if ((WorkMapLink[x][y - 1] != 111) && (WorkMapLink[x][y - 1] == 0))
		{
			WorkMapLink[x][y - 1] = d + 1;
			ChangedSells++;
		}
		if (y + 1 < 20)
		if ((WorkMapLink[x][y + 1] != 111) && (WorkMapLink[x][y + 1] == 0))
		{
			WorkMapLink[x][y + 1] = d + 1;
			ChangedSells++;
		}
		return ChangedSells;
	}

public:
	Maps()
	{
		for (int i = 0; i < 8; i++)
		{
			Player[i] = 0;
		}
	}
	bool CheckPlayersToWin()//Проверка победы 1-игра окончена, 0-игра продолжается
	{
		if (((Player[0] == 0)&(Player[1] == 0)&(Player[2] == 0)&(Player[3] == 0))
			|| (Player[7] == 0))	return 1;
		return 0;
	}
	bool Alive(int plrnum)//Проверка, есть ли игрок в списке живых
	{
		if (Player[plrnum] == 0) return 0;
		else return 1;
	}
	void AddMap(char path[])//Ввод данных карты
	{
		int x = 0, y = 0, c;
		cout << "Reading file...\n";
		fopen_s(&Input_data, path, "r");
		if (Input_data == NULL) exit(0);
		//if (fgets(a, 20, Input_data) != NULL)
		do
		{
			c = fgetc(Input_data);
			do
			{
				AddPlayer(c);
				Map[x][y] = c;//Преобразовываем
				y++;
				cout << (char)c;
				c = fgetc(Input_data);
			} while ((c != -1) && (c != '\n'));
			cout << '\n';
			x++;
			y = 0;
		} while ((c != -1) && (feof(Input_data) == 0));
		fclose(Input_data);
	}
	void SaveMap(char path[])//Сохраняем в отдельный файл исход битвы
	{
		int c;
		cout << "\nWriting file...";
		Output_data = fopen(path, "w");
		for (int i = 0; i <= 19; i++)//Cтроки
		{
			for (int j = 0; j <= 19; j++)//Столбцы
			{
				c = Map[i][j];//Преобразовываем
				fputc(c, Output_data);
			}
			fputc('\n', Output_data);
		}
		cout << "\nProfit.";
	}
	void MakeTurn(int plrnum)//Производим ход
	{
		int killway;
		XY PlrPos = FindPos(plrnum);//Ищем позицию текущего игрока
		if (plrnum != 7)
		{
			Sleep(400);
			if ((killway = FindToKill(plrnum, PlrPos.y, PlrPos.x)) != 0) 
				Kill(killway, PlrPos.y, PlrPos.x);//Если можно кого-то убить, убиваем
			else DoMove(plrnum, PlrPos.y, PlrPos.x);//Если никого нельзя убить, ходим
		}
		else MakeMyTurn(PlrPos.y, PlrPos.x);
		RefreshMap();//Обновляем карту в консоли
	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	Maps Mp1;
	char InputPath[] = "C:\\Users\\САША\\Documents\\Visual Studio 2013\\Projects\\TimeRush\\TimeRushIn.txt";
	char OutputPath[] = "C:\\Users\\САША\\Documents\\Visual Studio 2013\\Projects\\TimeRush\\TimeRushOut.txt";
	setlocale(LC_ALL, "rus");
	Mp1.AddMap(InputPath);//Загружаем карту из файла	
	//Запускаем ходы
	do
	{
		for (int plr = 0; plr < 8; plr++)//Игроки ходят по очереди
		{
			if (Mp1.Alive(plr))
			{
				Mp1.MakeTurn(plr);//Да будет резня ВХАХА
			}
		}
	} while (Mp1.CheckPlayersToWin() == 0);//Проверка победы одной из сторон
	Mp1.SaveMap(OutputPath);//Сохраняем в отдельный файл исход битвы	
	return 0;
}
