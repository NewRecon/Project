#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;

#define random(min,max) min+rand()%(max+1-min)

/*
	����������� �� ����� ������ cells, � ����� ����� (����� ����� �� � ���������)
	����������� ��������, ��� ����� ������ 30
	����������� ���������� �� ������ ������ (����� � ������ ����������) ??
	����������� �������, ������� ������ ����� �������� ������� ������� �� ����, � ����� ������:
		�������� � ������� ������� (����� �� �����)
		����� ���� ������� (�� ������ ������� �� �����??)
		����������� ������� ����� ������ (.. / 30)

	����������� ������� ����
	����������� ����������� ��������� �� ������� (���������� shelf_life ���� �� 0??)
	����������� ������� �������� ������
*/

// ������ ������ (�� ������ 30)
int cells = 0;

// ������ ������� ��������
int size1;

string pathToData = "..\\Data\\Project_Data.txt";
string pathToAdress = "..\\Data\\Addresses.txt";

struct Storage
{
	int cell;
	int num;
	int weight;
	int shelf_life;
	string status;
	string sender;
	string recipient;
	struct Date
	{
		int day;
		int month;
		int year;
	} date;
};

//	���������
void writeInFile(Storage*& stock, int num, string path);
string createAdress(string path);
int searchIndex(Storage*& stock, int num);
void increaseDate(Storage*& stock, int num);
void addInStock(Storage*& stock, Storage pos);
void delInStock(Storage*& stock, int num);
void createNewPos(Storage*& stock, int num, string path);
void delivPos(Storage*& stock, int num);
void newDay(Storage*& stock);

//	������ � ��������� ����
void writeInFile(Storage*& stock, int num, string path = pathToData)
{
	int i = searchIndex(stock, num);
	ofstream out{ path, ios::app };
	out << cells << " - " << stock[i].num << "|" << stock[i].weight << "|" << stock[i].status << "|" << stock[i].sender << "|" << stock[i].recipient << "|" << stock[i].date.day << "." << stock[i].date.month << "." << stock[i].date.year << "|" << stock[i].shelf_life << " - " << stock[i].cell << endl;
	out.close();
}

//	�������� ������ �� ���� �������
string createAdress(string path = pathToAdress)
{
	string str, adress;
	int n = 1 + rand() % 50;
	ifstream in{ path };
	if (in.is_open())
	{
		while (getline(in, str) && n!=0)
		{
			adress = str;
			n--;
		}
	}
	else cout << "Error! File not found.";
	in.close();
	return adress;
}

// ���������� ������� ���������� �� ������
int searchIndex(Storage*& stock, int num)
{
	int index = 0;
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].num == num)
		{
			index = i;
			break;
		}
	}
	return index;
}

// ���������� ����
void increaseDate(Storage*& stock, int num)
{
	if (size1 == 1)
	{
		stock[0].date.day = 1;
		stock[0].date.month = 1;
		stock[0].date.year = 2022;
	}
	else
	{
		int i = searchIndex(stock, num);
		if (stock[i - 1].date.day == 30 && (stock[i - 1].date.month == 4 || stock[i - 1].date.month == 6 || stock[i - 1].date.month == 9 || stock[i - 1].date.month == 11))
		{
			stock[i].date.day = 1;
			stock[i].date.month = stock[i - 1].date.month + 1;
		}
		else if (stock[i - 1].date.day == 31 && (stock[i - 1].date.month == 1 || stock[i - 1].date.month == 3 || stock[i - 1].date.month == 5 || stock[i - 1].date.month == 7 || stock[i - 1].date.month == 8 || stock[ - 1].date.month == 10))
		{
			stock[i].date.day = 1;
			stock[i].date.month = stock[i - 1].date.month + 1;
		}
		else if (stock[i - 1].date.day == 28 && stock[i - 1].date.month == 2)
		{
			stock[i].date.day = 1;
			stock[i].date.month = stock[i - 1].date.month + 1;
		}
		else if (stock[i - 1].date.day == 31 && stock[i - 1].date.month == 12)
		{
			stock[i].date.day = 1;
			stock[i].date.month = 1;
			stock[i].date.year = stock[i - 1].date.year + 1;
		}
		else
		{
			stock[i].date.day = stock[i - 1].date.day + 1;
			stock[i].date.month = stock[i - 1].date.month;
			stock[i].date.year = stock[i - 1].date.year;
		}
	}
}

// ���������� � ������ ��������
void addInStock(Storage*& stock, Storage pos)
{
	Storage* buf = new Storage[size1 + 1];
	for (int i = 0; i < size1; i++)
	{
		buf[i] = stock[i];
	}
	buf[size1] = pos;
	delete[]stock;
	stock = nullptr;
	stock = buf;
	size1++;
}

// �������� �� ������� ��������
void delInStock(Storage*& stock, int num)				// ��� �������� ������� �����!
{
	int index = searchIndex(stock, num);
	Storage* buf = new Storage[size1 - 1];
	for (int i = 0; i < index; i++)
	{
		buf[i] = stock[i];
	}
	for (int i = index + 1; i < size1-1; i++)
	{
		buf[i-1] = stock[i];
	}
	delete[]stock;
	stock = nullptr;
	stock = buf;
	size1--;
}

//	�������� ����� �������
void createNewPos(Storage*& stock, int num, string path = pathToData)
{
	Storage pos;
	pos.num = num;
	pos.weight = random(1,100);
	bool flag = true;
	if (pos.weight <= 50)
	{
		if (cells + 1 <= 30)
		{
			pos.cell = 1;
			cells += pos.cell;
			flag = false;
		}
	}
	else if (pos.weight <= 80)
	{
		if (cells + 2 <= 30)
		{
			pos.cell = 2;
			cells += pos.cell;
			flag = false;
		}
	}
	else
	{
		if (cells + 3 <= 30)
		{
			pos.cell = 3;
			cells += pos.cell;
			flag = false;
		}
	}
	if (flag)
	{
		pos.status = "��� ����� (�������)";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		//increaseDate(stock, num);
		pos.shelf_life = random(3, 20);
		writeInFile(stock, num);
	}
	else
	{
		pos.status = "�� ������";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.shelf_life = random(3, 20);
		addInStock(stock, pos);
		increaseDate(stock,num);
		writeInFile(stock, num);
	}
}

// �������� ������
void delivPos(Storage*& stock, int num)
{
	int index = searchIndex(stock, num);
	stock[index].status = "�����";
	cells -= stock[index].cell;
	writeInFile(stock, num);
	delInStock(stock, num);
}

// ��������� ������ ���
void newDay(Storage*& stock)
{
	int num = random(1, 30);	// ���� ��������� �������, � ���� �� ������� �� ������ ��, ����� ���������� ������
	bool flag=true;
	int index;
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].num == num)			// ���� ����� �������� ����� ������ �����-���� �������, �� �� ���� ����� �� ������� � �������
		{
			flag = false;
			index = i;
			break;
		}
	}
	if (flag)
	{
		createNewPos(stock, num);
	}
	else
	{
		delivPos(stock, num);
	}
}

//����������� ���������� ������� ����� � ������� �������� �� ����������� ������ ������ (� ������� 3 ������ ����� � ����������� �� ����, ����� ���� �����)

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	size1 = 0;
	Storage* stock = new Storage[size1];
	int z=0;
	while (z!=50)
	{
		newDay(stock);
		z++;
	}
}
