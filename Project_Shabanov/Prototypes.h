#pragma once
// Прототипы
void writeInFile(Storage& pos, Date& d, string path);
string createAdress(string path);
void incraseDate(Date& d);
void addInStock(Storage*& stock, Storage pos);
void delInStock(Storage*& stock, int index);
void createNewPos(Storage*& stock, Date& d, int num, string path);
void delivPos(Storage*& stock, Date& d, int index);
void decreaseShelfLife(Storage*& stock, Date& d, string path);
void newDay(Storage*& stock, Date& d, string path);
void showIvent(Storage& pos, Date& d);
void Menu(Storage*& stock, Date& d, bool& flag, string path);
void showStock(Storage*& stock);
void showFile(string path);
void createDate(Date& d);
void welcome();
void clearFile(string path);