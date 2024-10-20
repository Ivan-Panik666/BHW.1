#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

class DataBase;

class Student {
private:
	int id;
	string fio;
	float average;
	string faculty;

public:
	//конструктор
	Student(int id, string fio, double average, string faculty)
		: id(id), fio(fio), average(average), faculty(faculty) {};

	Student() : id(-1), fio(""), average(0), faculty("") {
	}
	void print() const {
	 
		std::cout << setprecision(3);
		std::cout << "ID: " << id << " FIO: " << fio << " Average: "  << average
			<< " Faculty: " << faculty << std::endl;
	}
	friend class DataBase;
};

//База данных студентов ИТМО
class DataBase {
private:
	std::vector<Student> v;
	 string filename = "data.txt";


public:
	~DataBase() {
		v.clear();
	}
	//Добавление новых записей с консоли.
	void add(const Student student) { 
		v.push_back(student);
	}
	void print() const {
		if (v.size() == 0) {
			std::cout << "DB is empty\n";
		}
		for (int i = 0; i < v.size(); i++)
		{
			v[i].print();
		}
	}
	// 1. Добавление новых записей с консоли. Хранение записей в файле.
	void writeToFile(const Student student) {
		FILE* file = fopen(filename.c_str(), "a+"); // a+ append дозаписи в файл
		if (file == nullptr)
		{
			std::cout << "Error open file\n";
		}

		else
		{
			fputs(std::to_string(student.id).c_str(), file);
			fputs(" ", file);
			fputs(student.fio.c_str(), file);
			fputs(" ", file);
			fputs(std::to_string(student.average).c_str(), file);
			fputs(" ", file);
			fputs(student.faculty.c_str(), file);
			fputs("\n", file);

		}
		// int fputs ( const char * str, FILE * stream );

		fclose(file);
	}
	// 2. Выводить все записи из файла.
	void ReadFromFile() {
		FILE* file = fopen(filename.c_str(), "r"); // read файл
		if (file == nullptr) {
			std::cout << "Error open file\n";
		}
		char buffer[128] = "\0";
		// char * fgets ( char * buffer, int num, FILE * filebd );
		while (fgets(buffer, 128, file) != nullptr) {
			if (buffer[0] == '\n') {
				std::cout << "Empty string\n";
			}
			std::cout << buffer << "\n";
		}
		if (buffer[0] == '\0') {
			std::cout << "Empty file\n";
		}
		fclose(file);
	}
	// 3. Выбор критерия поиска и поиск существующей записи в файле по полю
	// класса.

	Student search(const string& search) {
		//открыть файл в режиме чтения
		//открылся файл? Может аварийно завершить?
		FILE* file = fopen(filename.c_str(), "r");
		if (file == nullptr) {
			std::cout << "Error open file\n";
			return Student();
		}
		char id[128];
		char surname[128];
		char name[128];
		char secondName[128];
		char average[128];
		char faculty[128];

		while (6 == fscanf(file, "%s%s%s%s%s%s", id, surname, name, secondName, average, faculty))
		{

			id[strlen(id)] = '\0';
			surname[strlen(surname)] = '\0';
			name[strlen(name)] = '\0';
			secondName[strlen(secondName)] = '\0';
			average[strlen(average)] = '\0';
			faculty[strlen(faculty)] = '\0';

			string fio = string(surname) + " " + string(name) + " " + string(secondName);

			if (search == id || search == fio || search == average || search == faculty) {

				fclose(file);
				return Student(atoi(id), fio, atof(average), string(faculty));
			}

		}

		fclose(file);
		return Student();
	};

	bool erase(int searchId) {
		int finded = false;
		for (int i = 0; !finded && i < v.size(); i++)
		{
			if (v[i].id == searchId) {
				finded = true;
				for (int j = i; j < v.size(); j++) {
					v[i] = v[i + 1];
				}
			}
		}
		return finded;
	}

	 
	void eraseFromFile(int searchId) {

		Student student = search(to_string(searchId));
		if (student.id == -1) return;	//нечего удалять

		//записать в какой то файл из filebd все кроме записи student
		// 1 вариант
		// file -> vector
		//vector -> file кроме student
		//2 вариант
		//file -> tempfile кроме student
		//сохранить имя file
		//удалить file
		//tempfile переименовать


		FILE* file = fopen(filename.c_str(), "r"); // read файл 
		if (file == nullptr) {  
			std::cout << "Error open file\n";
			return;
		}

		FILE* temp = fopen("temp.txt", "w");
		if ( temp == nullptr) {
			fclose(file);
			std::cout << "Error open file\n";
			return;
		}
	 
		int id;
		char surname[128];
		char name[128];
		char secondName[128];
		float average;
		char faculty[128];
		//file -> tempfile кроме student
		while (6 == fscanf(file, "%d%s%s%s%f%s", &id, surname, name, secondName, &average, faculty))
		{ 
			if (searchId != id) {
				fprintf(temp, "%d %s %s %s %f %s\n", id, surname, name, secondName, average, faculty);
			}
			
		}
		fclose(temp);
		fclose(file);
		//удалить file
		//tempfile переименовать
		int returnCode = remove(filename.c_str());
		if (returnCode != 0) {
			cout <<  "Error: cannot remove the file.\n";
		}

		rename("temp.txt", filename.c_str()); 

	};

};

int main() {
	setlocale(LC_ALL, "ru");
	DataBase db;
	int id;
	string fio;
	float average;
	string faculty;

	bool Menu_1 = 1;
	int MenuResponse_1;
	while (Menu_1) {
		cout << "\t\t\tВыберете действие:" << endl;
		cout << "Добавление новых записей (Введите 1)" << endl;
		cout << "Вывод всех записей из файла (Введите 2)" << endl;
		cout << "Выбор критерия поиска и поиск существующей записи в файле "
			"(Введите 3)"
			<< endl;
		cout << "Удаление записи по id (Введите 4)" << endl;
		cout << "Завершение работы программы (Введите любое другое число)" << endl;
		cin >> MenuResponse_1;
		switch (MenuResponse_1) {
		case 1: {
			cout << "Выбрано (Добавление новых записей):" << endl;
			std::cout << "Введите id\n";
			cin >> id;
			getchar(); // cin - оставил символ \n в буфере, его ловим чтобы getline Не
			// поймал
			std::cout << "Введите FIO\n";
			getline(cin, fio);
			std::cout << "Введите avarage raiting\n";
			cin >> average;
			std::cout << "Введите faculty\n";
			cin >> faculty;
			db.writeToFile(Student(id, fio, average, faculty));

		} break;
		case 2: {
			cout << "Выбран (Вывод всех записей из файла):" << endl;
			db.ReadFromFile();		
		} break;
		case 3: {
			cout << "Выбран (Выбор критерия поиска и поиск существующей записи в файле):" << endl;
			int choise;
			std::cout << "Поиск по id -1, FIO - 2, avarage raiting - 3, faculty - 4\n";
			cin >> choise;
			switch (choise) {
			case 1: {
				std::cout << "Введите id\n";
				int searchId;
				cin >> searchId;
				Student findedStudent = db.search(to_string(searchId));
				findedStudent.print();
			} break;
			case 2: {
				getchar();
				std::cout << "Введите FIO\n";
				string searchFio;
				getline(cin, searchFio);
				Student findedStudent = db.search(searchFio);
				findedStudent.print();
			} break;
			case 3: {
				std::cout << "Введите avarage raiting\n";
				float average;
				cin >> average;
				Student findedStudent = db.search(to_string(average));
				findedStudent.print();
			} break;
			case 4: {
				std::cout << "Введите faculty\n";
				string faculty;
				cin >> faculty;
				Student findedStudent = db.search(faculty);
				findedStudent.print();
			} break;
			}

		} break;
		case 4: {
			cout << "Выбрано (Удаление записи по id):" << endl;
			int searchId;
			cin >> searchId;
			
			db.eraseFromFile(searchId);

		} break;
		default: {
			cout << "Завершение работы:" << endl;
			Menu_1 = 0;
		} break;
		}
	} // while


	//db.print();


	return 0;
}
