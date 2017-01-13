//Программа TimeRush
//Игроки ходят по очереди, причем для меня путь каждый раз перерассчитывается
//Игра проиграна, если меня убьют, выиграна, если все в команде противника убиты
//2 команды по 4 игрока (я восьмой)
//Мой игрок может ходить на 4 клетки, все остальные на 1
//Стрелять можно только по прямой
//Выстрел стоит 1 ход, выстрел убивает мгновенно
//Внезапность подразумевает выход на позицию, выстрел и уход с линии огня
//Боты не умеют просчитывать далее 1 позиции и ходят в случайную сторону, поэтому совсем неосторожны =)
//Просчитывание возможно, но будет на два порядка сильнее замедлять программу

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <algorithm>

using namespace std;

class Maps
{
private:
	FILE * Input_data, *Output_data;
	char Map[20][20];
	int x, y, c;
	bool Player[8];
	struct XY
	{
		int x, y;
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
		Player[plrnum - 1] = 0;
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
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 20; j++)
			if (Map[j][i] == plrnum + '0')
			{
				pos.x = i;
				pos.y = j;
				return pos;
			}
		}
	}
	int FindToKill(int x, int y)//Ищем возможность для выстрела из заданной позиции
		//Возвращаемые значения: 1-верх 2-право 3-низ 4-лево 0-нигде
	{
		if ((Map[x][y] >= '1') && (Map[x][y] <= '4'))
		{
			if ((WhatFarAwayUp(x, y) >= '5') && (WhatFarAwayUp(x, y) <= '8')) return 1;
			if ((WhatFarAwayRight(x, y) >= '5') && (WhatFarAwayRight(x, y) <= '8')) return 2;
			if ((WhatFarAwayDown(x, y) >= '5') && (WhatFarAwayDown(x, y) <= '8')) return 3;
			if ((WhatFarAwayLeft(x, y) >= '5') && (WhatFarAwayLeft(x, y) <= '8')) return 4;
		}
		else if ((Map[x][y] >= '5') && (Map[x][y] <= '8'))
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
		if (killway == 1)//Сверху
		{
			do
			{
				x--;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 2)//Справа
		{
			do
			{
				y++;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 3)//Снизу
		{
			do
			{
				x++;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 4)//Слева
		{
			do
			{
				y--;
				t = Map[x][y];
			} while ((t == '0') && (t != 'x'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
	}

	char WhatFarAwayUp(int x, int y)//Разведываем верх
	{
		char t = Map[x][y];

		while (x > 0)
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
		while (y < 19)
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
		while (x < 19)
		{
			x++;
			t = Map[x][y];
			if ((t <= '1') && (t >= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	char WhatFarAwayLeft(int x, int y)//Разведываем лево
	{
		char t = Map[x][y];
		while (y > 0)
		{
			y--;
			t = Map[x][y];
			if ((t <= '1') && (t >= '8'))
			{
				return t;
			}
		}
		return t = 'x';
	}
	void DoMove(int plrnum, int x, int y)
	{
		int randparam = 0, i = 0;
		//Узнаём, какие направления свободны
		bool direction[4] = { 0, 0, 0, 0 };
		if ((y != 0) && (Map[x - 1][y] == '0'))  direction[0] = 1;//Верх
		if ((x != 20) && (Map[x][y + 1] == '0')) direction[1] = 1;//Право
		if ((y != 20) && (Map[x + 1][y] == '0')) direction[2] = 1;//Низ
		if ((x != 0) && (Map[x][y - 1] == '0'))  direction[3] = 1;//Лево
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
			if (i == 1)//Вверх
			{
				Map[x][y] = '0';
				Map[x - 1][y] = plrnum + '0';
			}
			if (i == 2)//Направо
			{
				Map[x][y] = '0';
				Map[x][y + 1] = plrnum + '0';
			}
			if (i == 3)//Вниз
			{
				Map[x][y] = '0';
				Map[x + 1][y] = plrnum + '0';
			}
			if (i == 4)//Налево
			{
				Map[x][y] = '0';
				Map[x][y - 1] = plrnum + '0';
			}
		}
	}
	void DoMyMove(int x, int y)//Производим свой ход
	{
		/*int killway, moves = 4;
		while (((killway = FindToKill(y, x)) != 0) && (moves != 0))
		{
			Kill(killway, y, x);//Если можно кого-то убить, убиваем
			moves--;
		}
		FindClothestEnemy(y, x);//Если никого нельзя убить, ищем путь к врагу
		//Идем по найденному пути
		/*int map[20][20], d = 0;
		map[x][y] = d;
		do
		{
		if (x != 0) if ((map[x - 1][y] != map[x][y] - 1) && (Map[x - 1][y] == '0'))
		map[x - 1][y] = d + 1;
		if (x != 19) if ((map[x + 1][y] != map[x][y] - 1) && (Map[x + 1][y] == '0'))
		map[x + 1][y] = d + 1;
		if (y != 0) if ((map[x][y - 1] != map[x][y] - 1) && (Map[x][y - 1] == '0'))
		map[x][y - 1] = d + 1;
		if (y != 0) if ((map[x][y + 1] != map[x][y] - 1) && (Map[x][y + 1] == '0'))
		map[x][y + 1] = d + 1;

		} while (1);*/
	}
	XY FindClothestEnemy(int x, int y)//Находим ближайшего врага
	{
		XY Position[4], ClothestEnemyPos;
		ClothestEnemyPos = FindPos(1);
		for (int i = 2; i <= 3; i++)
		{
			Position[i] = FindPos(i);
			if (sqrt((ClothestEnemyPos.x - x) ^ 2 + (ClothestEnemyPos.y - y) ^ 2) < sqrt((Position[i].x - x) ^ 2 + (Position[i].y - y) ^ 2))
				ClothestEnemyPos = FindPos(i);
		}
		return ClothestEnemyPos;
	}
	int FindShortestWay(int x, int y)//Алгоритм Ли
	{
		XY ClothestEnemyPos = FindClothestEnemy(x, y);
		/*int neighbor[4];
		neighbor[0] = Map[x - 1][y] - '0';
		neighbor[1] = Map[x][y + 1] - '0';
		neighbor[2] = Map[x + 1][y] - '0';
		neighbor[3] = Map[x][y - 1] - '0';
		for (int i = 0; i < 4; i++)
		{
		if (neighbor[i] == -2) neighbor[i] = 99999;
		}
		sort(neighbor, neighbor + 4);
		for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y + 1; j >= y - 1; j--) {
		if (Map[x, y] == neighbors[0]){
		// и указываем вектору координаты клетки, в которую переместим нашего юнита
		moveTO = new Vector3(x, y, 10);
		}
		}
		}
		//если мы не нашли куда перемещать юнита, то оставляем его на старой позиции.
		// это случается, если вокруг юнита, во всех 8 клетках, уже размещены другие юниты
		if (moveTO == new Vector3(-1, 0, 10))
		moveTO = new Vector3(currentPosition.x, currentPosition.y, 10);

		//и ура, наконец-то мы перемещаем нашего юнита
		// теперь он на 1 клетку ближе к врагу
		transform.localPosition = moveTO;
		
		char WorkMap[20][20];
		for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++) WorkMap[i][j] = Map[i][j];
		int iter(0), iterk(100);
		while (iter < iterk)
		{
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					if (WorkMap[i][j] == iter)
					{
						if (WorkMap[i + 1][j] == '0')WorkMap[i + 1][j] = iter + 1 + '0';
						if (WorkMap[i - 1][j] == '0')WorkMap[i - 1][j] = iter + 1 + '0';
						if (WorkMap[i][j + 1] == '0')WorkMap[i][j + 1] = iter + 1 + '0';
						if (WorkMap[i][j - 1] == '0')WorkMap[i][j - 1] = iter + 1 + '0';

						if (WorkMap[i + 1][j] == WorkMap[ClothestEnemyPos.x][ClothestEnemyPos.y]) break;
						if (WorkMap[i - 1][j] == WorkMap[ClothestEnemyPos.x][ClothestEnemyPos.y]) break;
						if (WorkMap[i][j + 1] == WorkMap[ClothestEnemyPos.x][ClothestEnemyPos.y]) break;
						if (WorkMap[i][j - 1] == WorkMap[ClothestEnemyPos.x][ClothestEnemyPos.y]) break;
					}
				}
			}
			iter++;
		}
		if ((WorkMap[ClothestEnemyPos.x + 1 + 1][ClothestEnemyPos.y + 1] == 'x' || WorkMap[ClothestEnemyPos.x + 1 + 1][ClothestEnemyPos.y + 1] == '0') 
			&& (WorkMap[ClothestEnemyPos.x - 1 + 1][ClothestEnemyPos.y + 1] == '0' || WorkMap[ClothestEnemyPos.x - 1 + 1][ClothestEnemyPos.y + 1] == 'x')
			&& (WorkMap[ClothestEnemyPos.x + 1][ClothestEnemyPos.y] == '0' || WorkMap[ClothestEnemyPos.x + 1][ClothestEnemyPos.y] == 'x') 
			&& (WorkMap[ClothestEnemyPos.x + 1][ClothestEnemyPos.y + 2] == 'x' || WorkMap[ClothestEnemyPos.x + 1][ClothestEnemyPos.y + 2] == '0'))return 0;
		if ((WorkMap[x + 1 + 1][y + 1] == 'x' || WorkMap[x + 1 + 1][y + 1] == '0') && (WorkMap[x - 1 + 1][y + 1] == '0' || WorkMap[x - 1 + 1][y + 1] == 'x') 
			&& (WorkMap[x + 1][y] == '0' || WorkMap[x + 1][y] == 'x') && (WorkMap[x + 1][y + 2] == 'x' || WorkMap[x + 1][y + 2] == '0'))return 0;
		cout << endl;
		for (int i = 1; i < 20 - 1; i++)
		{
			for (int j = 1; j < 20 - 1; j++)
			{
				cout << setw(3) << WorkMap[i][j];
			}
			cout << endl;
		}
		int X = x + 1, Y = y + 1, X1(0), Y1(0);
		char minn('x');
		while (1)
		{
			if (WorkMap[X + 1][Y] < minn)
			{
				minn = WorkMap[X + 1][Y];
				X1 = X + 1;
				Y1 = Y;
			}
			if (WorkMap[X - 1][Y] < minn)
			{
				minn = WorkMap[X - 1][Y];
				X1 = X - 1;
				Y1 = Y;
			}
			if (WorkMap[X][Y + 1] < minn)
			{
				minn = WorkMap[X][Y + 1];
				X1 = X;
				Y1 = Y + 1;
			}
			if (WorkMap[X][Y - 1] < minn)
			{
				minn = WorkMap[X][Y - 1];
				X1 = X;
				Y1 = Y - 1;
			}
			X = X1;
			Y = Y1;
			if (WorkMap[X1][Y1] == '0')break;
			Map[X1][Y1] = '-1';
		}
		//print_map(MAP);
		//return 0;*/
	}

public:
	Maps()
	{
		x = 0;
		y = 0;
		for (int i = 0; i < 8; i++)
		{
			Player[i] = 0;
		}
	}
	bool CheckPlayersToWin()//Проверка победы 1-игра окончена, 0-игра продолжается
	{
		if ((Player[0,1,2,3] == 0) || (Player[7] == 0))
			return 1;
		else return 0;
	}
	bool Alive(int plrnum)//Проверка, есть ли игрок в списке живых
	{
		if (Player[plrnum-1] == 0) return 0;
		else return 1;
	}

	void AddMap(char path[])//Ввод данных карты
	{
		
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
				Map[x][y] =c;//Преобразовываем
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
			if (plrnum != 8)
			{
				if ((killway = FindToKill(PlrPos.y, PlrPos.x)) != 0) Kill(killway, PlrPos.y, PlrPos.x);//Если можно кого-то убить, убиваем
				else DoMove(plrnum, PlrPos.y, PlrPos.x);//Если никого нельзя убить, ходим
			}
			else DoMyMove(PlrPos.y, PlrPos.x);
			//Sleep(50); //И без остановки все передвижения видны на карте в консоли
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
		for (int plr = 1; plr < 9; plr++)//Игроки ходят по очереди
		{
			if (Mp1.Alive(plr)) Mp1.MakeTurn(plr);//Да будет резня ВХАХА
		}
	} while (Mp1.CheckPlayersToWin()==0);//Проверка победы одной из сторон
	Mp1.SaveMap(OutputPath);//Сохраняем в отдельный файл исход битвы	
	return 0;
}
