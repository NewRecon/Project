#pragma once
// ���������� ����������

// ������ ������ (�� ������ 30)
int cells = 0;

// ������ ������� ��������
int size1;

// ���������� ��������� ������
string pathToData = "..\\Data\\Project_Data.txt";
string pathToAdress = "..\\Data\\Addresses.txt";

// ���������
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