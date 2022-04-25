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

//	���������


struct Storage
{
	int num;
	int weight;
	int shelf_life;
	string status;
	string sender;
	string recipient;
	struct Date
	{
		int day = 0;
		int month = 1;
		int year = 2022;
	} date;
};

//	������ � ��������� ����
void writeInFile(Storage pos, string path= "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Project_Data.txt")
{
	ofstream out{ path, ios::app };
	out << pos.num << "|" << pos.weight << "|" << pos.status << "|" << pos.sender << "|" << pos.recipient << "|" << pos.date.day << "." << pos.date.month << "." << pos.date.year << "|" << pos.shelf_life << endl;
	out.close();
}

//	�������� ������ �� ���� �������
string createAdress(string path = "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Addresses.txt")
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
	stock = buf;
	size1++;
}

// �������� �� ������� ��������
void delInStock(Storage*& stock, int num)
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
	stock = buf;
	size1--;
}

//	�������� ����� �������
void createNewPos(Storage*& stock, int num, string path = "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Project_Data.txt")
{
	Storage pos;
	pos.num = num;
	pos.weight = random(1,100);
	bool flag = true;
	if (pos.weight <= 50)
	{
		if (cells + 1 <= 30)
		{
			cells += 1;
			flag = false;
		}
	}
	else if (pos.weight <= 80)
	{
		if (cells + 2 <= 30)
		{
			cells += 2;
			flag = false;
		}
	}
	else
	{
		if (cells + 3 <= 30)
		{
			cells += 3;
			flag = false;
		}
	}
	if (flag)
	{
		// ������� ������
	}
	else
	{
		pos.status = "�� ������";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.date.day++;							// ������� �������� ���������� ������ � ����
		pos.shelf_life = random(3, 20);
		addInStock(stock, pos);
		writeInFile(pos);
	}
}

// �������� ������
void delivPos(Storage*& stock, int num)
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
	stock[index].status = "�����";
	writeInFile(stock[index]);
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
		if (stock[i].num == num)
		{
			flag = false;
			index = i;								// ������� ����������� ������� �� ������??? ������ ���������� ������???
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
	string pathToData = "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Project_Data.txt";
	string pathToAdress = "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Addresses.txt";
	size1 = 0;
	Storage* stock = new Storage[size1];
	int z;
	while (true)
	{
		newDay(stock);
		cin >> z;
		if (z == 0) break;
	}
}
