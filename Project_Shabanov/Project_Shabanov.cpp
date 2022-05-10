#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;

// Макросы
#define random(min,max) min+rand()%(max+1-min)

// Ячейки склада (не больше 30)
int cells = 0;

// Размер массива структур
int size1;

string pathToData = "..\\Data\\Project_Data.txt";
string pathToAdress = "..\\Data\\Addresses.txt";

struct Storage
{
	int cell;
	int num;
	int weight;
	int shelf_life;
	int countSL;
	int day;
	int month;
	int year;
	string status;
	string sender;
	string recipient;
};

struct Date
{
	int day;
	int month;
	int year;
};

//	Прототипы
void writeInFile(Storage*& stock, int num, string path);
string createAdress(string path);
void increaseDate(Storage*& stock, int num);
void addInStock(Storage*& stock, Storage pos);
void delInStock(Storage*& stock, int num);
void createNewPos(Storage*& stock, int num, string path);
void delivPos(Storage*& stock, int num);
void newDay(Storage*& stock);
void showIvent(Storage& pos, Date& d);
void showStock(Storage*& stock);
void showCells();
void showFile(string path);

//	Запись в текстовый файл
void writeInFile(Storage& pos, Date& d, string path = pathToData)
{
	ofstream out{ path, ios::app };
	out << d.day <<"."<< d.month << "." << d.year << ": " << pos.num << "|" << pos.weight << "|" << pos.status << "|" << pos.sender << "|" << pos.recipient << "|" << pos.day << "." << pos.month << "." << pos.year << "|" << pos.shelf_life << endl;
	out.close();
}

//	Создание адреса из базы адресов
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

// Увеличение даты
void incraseDate(Date& d)
{
	d.day++;
	if (d.day > 30 && (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11))
	{
		d.day = 1;
		d.month++;
	}
	else if (d.day > 31 && (d.month == 1 || d.month == 3 || d.month == 5 || d.month == 7 || d.month == 8 || d.month == 10))
	{
		d.day = 1;
		d.month++;
	}
	else if (d.day > 28 && d.month == 2 && d.year%4!=0)
	{
		d.day = 1;
		d.month++;
	}
	else if (d.day > 29 && d.month == 2 && d.year % 4 == 0)
	{
		d.day = 1;
		d.month++;
	}
	else if (d.day > 31 && d.month == 12)
	{
		d.day = 1;
		d.month = 1;
		d.year++;
	}
}

// Добавление в массив структур
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

// Удаление из массива структур
void delInStock(Storage*& stock, int index)
{
	Storage* buf = new Storage[size1 - 1];
	for (int i = 0; i < index; i++)
	{
		buf[i] = stock[i];
	}
	for (int i = index + 1; i < size1; i++)
	{
		buf[i-1] = stock[i];
	}
	delete[]stock;
	stock = buf;
	size1--;
}

//	Создание новой позиции
void createNewPos(Storage*& stock, Date& d, int num, string path = pathToData)
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
		pos.status = "возврат (нет места на складе)";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.day = d.day;
		pos.month = d.month;
		pos.year = d.year;
		pos.shelf_life = random(3, 20);
		writeInFile(pos, d);
		showIvent(pos, d);
	}
	else
	{
		pos.status = "поступил на склад";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.shelf_life = random(3, 20);
		pos.countSL = pos.shelf_life;
		pos.day = d.day;
		pos.month = d.month;
		pos.year = d.year;
		addInStock(stock, pos);
		writeInFile(pos, d);
		showIvent(pos, d);
	}
}

// доставка заказа
void delivPos(Storage*& stock, Date& d, int index)
{
	cells -= stock[index].cell;
	writeInFile(stock[index], d);
	showIvent(stock[index], d);
	delInStock(stock, index);
}

// Уменьшение срока хранения
void decreaseShelfLife(Storage*& stock, Date& d, string path = pathToData)
{
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].countSL == 0)
		{
			stock[i].status = "возврат (истёк срок хранения)";
			delivPos(stock, d, i);
		}
		stock[i].countSL--;
	}
}

// симуляцтя нового дня
void newDay(Storage*& stock, Date& d, string path = pathToData)
{
	system("cls");
	cout << "Наступил новый день!" << endl << endl;
	cout << "Дата: " << d.day << "." << d.month << "." << d.year << endl << endl;;
	cout << "Произошли следующие события: " << endl;
	int num = random(1, 30);
	bool flag=true;
	int index;
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].num == num)
		{
			flag = false;
			index = i;
			break;
		}
	}
	if (flag)
	{
		createNewPos(stock, d, num);
	}
	else
	{
		stock[index].status = "выдан со склада";
		delivPos(stock, d, index);
	}
	decreaseShelfLife(stock, d);
	incraseDate(d);
	cout << endl;
	system("pause");
}

void showIvent(Storage& pos, Date& d)
{
	cout << pos.num << "|" << pos.weight << "|" << pos.status << "|" << pos.sender << "|" << pos.recipient << "|" << pos.day << "." << pos.month << "." << pos.year << "|" << pos.shelf_life << endl;
}

// Отоброжает текущее событие и меню
void Menu(Storage*& stock, Date& d, int& z, string path = pathToData)
{
	int n = 5;
	while (true)
	{
		if (n < 0 || n > 4)
		{
			system("cls");
			cout << "1 - Сгенерировать новый день." << endl;
			cout << "2 - Отобразить все позиции на складе." << endl;
			cout << "3 - Отобразить заполненность склада." << endl;
			cout << "4 - Отобразить историю событий." << endl;
			cout << "0 - Выход из симуляции." << endl;
			cout << "Введите соответствующую цифру для продолжения: " << endl;
			cin >> n;
		}
		else
		{
			switch (n)
			{
			case(1):
				newDay(stock, d);
				break;
			case(2):
				showStock(stock);
				break;
			case(3):
				showCells();
				break;
			case(4):
				showFile(path);
				break;
			case(0):
				z = 0;
				break;
			}
			break;
		}
	}
}

// Отобразить все позиции на складе
void showStock(Storage*& stock)
{
	system("cls");
	if (size1 > 0)
	{
		for (int i = 0; i < size1; i++)
		{
			cout <<i+1<<". "<< stock[i].num << "|" << stock[i].weight << "|" << stock[i].status << "|" << stock[i].sender << "|" << stock[i].recipient << "|" << stock[i].day << "." << stock[i].month << "." << stock[i].year << "|" << stock[i].shelf_life << endl;
		}
	}
	else cout << "Нет позиций на складе.";
	cout << endl;
	system("pause");
}

// Отобразить заполненность склада
void showCells()
{
	system("cls");
	cout << cells << " / 30";
	cout << endl;
	system("pause");
}

void showFile(string path = pathToData)
{
	system("cls");
	ifstream in{ path };
	string str;
	if (in.is_open())
	{
		while (getline(in, str))
		{
			cout << str << endl;
		}
	}
	else cout << "Error! File not found.";
	in.close();
	cout << endl;
	system("pause");
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	size1 = 0;
	Date d;
	int date = 0;
	while (date < 1900 || date > 2123)
	{
		system("cls");
		cout << "Введите начальную дату." << endl;
		cout << "Год: ";
		cin >> date;
	}
	d.year = date;
	date = 0;
	while (date < 1 || date > 12)
	{
		system("cls");
		cout << "Введите начальную дату." << endl;
		cout << d.year << "." << endl;
		cout << "Месяц: ";
		cin >> date;
	}
	d.month = date;
	date = 0;
	while (date < 1 || date > 31)
	{
		system("cls");
		cout << "Введите начальную дату." << endl;
		cout << d.year << "." << d.month << "." << endl;
		cout << "День: ";
		cin >> date;
	}
	d.day = date;
	Storage* stock = new Storage[size1];
	int z=1;
	ofstream out{ pathToData };
	out.close();
	while (z!=0)
	{
		Menu(stock, d, z);
	}
}
