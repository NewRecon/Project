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

//	Прототипы


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

//	Запись в текстовый файл
void writeInFile(Storage pos, string path= "C:\\Users\\99max\\Desktop\\IT\\C++\\Project\\Data\\Project_Data.txt")
{
	ofstream out{ path, ios::app };
	out << pos.num << "|" << pos.weight << "|" << pos.status << "|" << pos.sender << "|" << pos.recipient << "|" << pos.date.day << "." << pos.date.month << "." << pos.date.year << "|" << pos.shelf_life << endl;
	out.close();
}

//	Создание адреса из базы адресов
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

//	Создание новой позиции
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
		// возврат товара
	}
	else
	{
		pos.status = "на складе";
		pos.sender = createAdress();
		pos.recipient = createAdress();
		pos.date.day++;							// сделать условием увеличение месяца и года
		pos.shelf_life = random(3, 20);
		addInStock(stock, pos);
		writeInFile(pos);
	}
}

// доставка заказа
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
	stock[index].status = "выдан";
	writeInFile(stock[index]);
	delInStock(stock, num);
}

// симуляцтя нового дня
void newDay(Storage*& stock)
{
	int num = random(1, 30);	// ищет рандомную позицию, и если не находит то создаёт ее, иначе производит выдачу
	bool flag=true;
	int index;
	for (int i = 0; i < size1; i++)
	{
		if (stock[i].num == num)
		{
			flag = false;
			index = i;								// Функция определения индекса по номеру??? Дальше передавать индекс???
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

//Реализовать сортировку массива ячеек и сделать проверку на вмещаемость нового товара (в функции 3 разных цикла в зависимости от того, какго веса товар)

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
