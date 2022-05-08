#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <ctime>
#include <windows.h>
using namespace std;

#define random(min,max) min+rand()%(max+1-min)

/*
	Реализовать не через массив cells, а через сумму (сумма будет не в структуре)
	Реализовать проверку, что сумма меньше 30
	Реализовать сортировку по номеру товара (может и другие сортировки) ??
	Реализовать менюшку, которая всегда будет выводить текущее событие за день, а также кнопки:
		перехода в историю событий (вывод из файла)
		вывод всех позиций (со срокам храниня из файла??)
		отображения сколько ячеек занято (.. / 30)

	Реализовать течения дней
	Реализовать отслеживане просрочек по товарам (уменьшение shelf_life пока не 0??)
	Реализовать функцию возврата товара
*/

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

struct Date
{
	int day;
	int month;
	int year;
};

//	Прототипы
void writeInFile(Storage*& stock, int num, string path);
string createAdress(string path);
int searchIndex(Storage*& stock, int num);
void increaseDate(Storage*& stock, int num);
void addInStock(Storage*& stock, Storage pos);
void delInStock(Storage*& stock, int num);
void createNewPos(Storage*& stock, int num, string path);
void delivPos(Storage*& stock, int num);
void newDay(Storage*& stock);

//	Запись в текстовый файл
void writeInFile(Storage& pos, Date& d, string path = pathToData)
{
	ofstream out{ path, ios::app };
	out << "Дата: " << d.day << "." << d.month << "." << d.year << " : " << cells << " - " << pos.num << "|" << pos.weight << "|" << pos.status << "|" << pos.sender << "|" << pos.recipient << "|" << pos.date.day << "." << pos.date.month << "." << pos.date.year << "|" << pos.shelf_life << " - " << pos.cell << endl;
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

// нахождение индекса экземпляра по номеру
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
	else if (d.day > 28 && d.month == 2)
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
void delInStock(Storage*& stock, int num)
{
	int index = searchIndex(stock, num);
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
		pos.status = "Нет места (возврат)";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.date.day = d.day;
		pos.date.month = d.month;
		pos.date.year = d.year;
		pos.shelf_life = random(3, 20);
		writeInFile(pos, d);
	}
	else
	{
		pos.status = "на складе";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.shelf_life = random(3, 20);
		pos.countSL = pos.shelf_life;		// ?
		pos.date.day = d.day;
		pos.date.month = d.month;
		pos.date.year = d.year;
		addInStock(stock, pos);
		writeInFile(pos, d);
	}
}

// доставка заказа
void delivPos(Storage*& stock, Date& d, int num)
{
	int index = searchIndex(stock, num);
	stock[index].status = "выдан";
	cells -= stock[index].cell;
	writeInFile(stock[index], d);
	delInStock(stock, num);
}

// Уменьшение срока хранения
void decreaseShelfLife(Storage*& stock, Date& d, string path = pathToData)
{
	for (int i = 0; i < size1; i++)
	{
		stock[i].countSL--;
		if (stock[i].countSL == 0)
		{
			stock[i].status = "возврат (истёк срок хранения)";
			cells -= stock[i].cell;
			writeInFile(stock[i], d);
			delInStock(stock, i);
		}
	}
}

// симуляцтя нового дня
void newDay(Storage*& stock, Date& d, string path = pathToData)
{
	int num = random(1, 30);	// ищет рандомную позицию, и если не находит то создаёт ее, иначе производит выдачу
	bool flag=true;
	int index;
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].num == num)			// Если номер присвоен после выдачи какой-либо позиции, то он этот номер не находит в массиве
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
		delivPos(stock, d, num);
	}
	incraseDate(d);
	decreaseShelfLife(stock, d);
}

//Реализовать сортировку массива ячеек и сделать проверку на вмещаемость нового товара (в функции 3 разных цикла в зависимости от того, какго веса товар)

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));
	size1 = 0;
	Date d {1,1,2022};
	Storage* stock = new Storage[size1];
	int z=0;
	while (z!=370)
	{
		newDay(stock, d);
		z++;
	}
}
