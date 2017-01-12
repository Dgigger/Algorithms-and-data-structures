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
			} while ((t <= '1') && (t >= '8'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 2)//Справа
		{
			do
			{
				y++;
				t = Map[x][y];
			} while ((t <= '1') && (t >= '8'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 3)//Снизу
		{
			do
			{
				x++;
				t = Map[x][y];
			} while ((t <= '1') && (t >= '8'));
			KillPlr(Map[x][y] - '0');
			Map[x][y] = '0';
		}
		if (killway == 4)//Слева
		{
			do
			{
				y--;
				t = Map[x][y];
			} while ((t <= '1') && (t >= '8'));
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
	void DoMyMove(int x, int y)
	{

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
		if (((Player[0] == 0) && (Player[1] == 0) && (Player[2] == 0) && (Player[3] == 0)) || (Player[7] == 0))
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
				if ((killway = FindToKill(PlrPos.y, PlrPos.x)) == 0) DoMove(plrnum, PlrPos.y, PlrPos.x);//Если никого нельзя убить, ходим
				else Kill(killway, PlrPos.y, PlrPos.x);//Если можно, убиваем
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
			try            //Проверка на победку
			{
				if (Mp1.CheckPlayersToWin()) throw invalid_argument("Игра окончена");
			}
			catch (invalid_argument& e)
			{
				Mp1.SaveMap(OutputPath);//Сохраняем в отдельный файл исход битвы	
				return 0;
			}
		}
	} while (1);
	/*Mp1.SaveMap(OutputPath);//Сохраняем в отдельный файл исход битвы	
	return 0;*/
}
