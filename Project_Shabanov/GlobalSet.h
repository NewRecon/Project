#pragma once
// Глобальные переменные

// Ячейки склада (не больше 30)
int cells = 0;

// Размер массива структур
int size1;

// Директории текстовых файлов
string pathToData = "..\\Data\\Project_Data.txt";
string pathToAdress = "..\\Data\\Addresses.txt";

// Структуры
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