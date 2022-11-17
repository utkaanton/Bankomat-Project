#include <iostream>//библиотека потока ввода/вывода
#include <cstdlib>//библиотека для работы со случайными числами
#include <string>//библиотека для работы со строками
#include <fstream>//библиотека для работы с файлами
#include <ctime>//библиотека для работы со временем
#include <regex>//библиотека для работы с регулярными выражениями

using namespace std;//пространство имен std

class Client//класс для работы с клиентом
{
private:
	int ID;//ID пользователя
	int PIN[4];//ПИН-код
	int numbrOfCard[16];//номер карты
	int dateReg[3];//дата регестрации
	int SVV[3];// SVV код
	long int cash;// баласн
	string name;//имя
	string lastname;//фамилия
	int DOB[3];//дата рождения

	void ManualInputData(){//ручной ввод
		string buf;//буфер
		string bufchar;//буфер
		for(;;){//зацикливание
			cout <<"Введите имя: ";
			cin >> name;
			if(CheckLetter(name))break;//выход
			cout << "Некоректный ввод! Попробуйте снова." << endl;
		}
		for(;;){//зацикливание
			cout <<"Введите Фамилию: ";
			cin >> lastname;
			if(CheckLetter(lastname))break;//выход
			cout << "Некоректный ввод! Попробуйте снова." << endl;
		}
		cout << "Введите дату рождения(в формате день.месяц.год):"<<endl;
		
		for(;;){//зацикливание
			cout << "Введите день:";
			cin >> buf;
			if(CheckInt(buf)){
				DOB[0]=stoi(buf);
				if(DOB[0]<=31 && DOB[0]>0) break;//выход
			}
			cout << "Некоректный ввод! Попробуйте снова."<< endl;
		}
		for(;;){//зацикливание
			cout << "Введите месяц:";
			cin >> buf;
			if(CheckInt(buf)){
				DOB[1]=stoi(buf);
				if(DOB[1]<=12 && DOB[1]>0) break;//выход
			}
			cout << "Некоректный ввод! Попробуйте снова."<< endl;
		}
		for(;;){//зацикливание
			cout << "Введите год:";
			cin >> buf;
			if(CheckInt(buf)){
				DOB[2]=stoi(buf);
				if(DOB[2]<=2008 && DOB[2]>=1920) break;//выход
			}
			cout << "Некоректный ввод! Попробуйте снова."<< endl;
		}
		
		for(;;){//зацикливание
			cout << "Введите пинкод(4 цифры):";
			cin >> buf;
			if(CheckInt(buf) && buf.length()==4){
				for (int i =0;i<4;i++){//перебор пин-кода
					bufchar=buf[i];
					PIN[i]=stoi(bufchar);
				}
				break;//выход
			}
			cout << "Некоректный ввод! Попробуйте снова."<< endl;
		}
	}
	void InDB(string nameFile){//добавление в базу данных(принимает имя файла)
		ofstream file;//объект для записи в файл
		file.open(nameFile, ios_base::app);//открытие файла для дополнительной записи
		file << ID <<" ";//запись в файл
		for(int i=0;i<4;i++){
			file << PIN[i];//запись в файл
		}
		file << " ";//запись в файл
		file<<name<<" "<<lastname<<" ";//запись в файл
		for(int i = 0;i<3;i++){//перебор даты рождения
			file << DOB[i];//запись в файл
			if (i!=2)file << ".";//запись в файл
		}
		file <<" ";
		for(int i = 0;i<16;i++){//перебор номера карты
			file << numbrOfCard[i];//запись в файл
		}
		file <<" ";
		for(int i = 0;i<3;i++){//перебор даты регистрации
			file << dateReg[i];//запись в файл
			if (i!=2)file << ".";//запись в файл
		}
		file <<" ";//запись в файл
		for(int i = 0;i<3;i++){// перебор SVV
			file << SVV[i];//запись в файл
		}
		file <<" ";//запись в файл
		file << cash << " "<<endl;//запись в файл

		file.close();//закрытие файла
	}
	int IDAdd(){//присвоение ID
		ifstream file;//объект для чтения файла
		file.open("database.txt");//путь к файлу
		string line = " ";//буфер для строки
		int maxID=0;// максимальный ID
		string buf;//буфер
		while (!file.eof())//проверка на конец файла
		{
			getline(file, line);//запись строки файла в буфер
			for(int i=0;line[i]!=' ';i++){//поиск ID в строке
				buf+=line[i];//запись ID в буфер
			}
			if (CheckInt(buf)){
				if (stoi(buf)>maxID)maxID=stoi(buf);//проверка на максимум
			}
			buf="";//перезапись буфера
		}
		ID = maxID+1;//ID новый 
		file.close();//закрытие файла
		return ID;
	}
	string LineByWords(string str,int num){//слово из строки(принимает строку данных и номер слова)
		string buf;//буфер
		int counter=1;//счетчик слов
		for (int i =0;i<str.length();i++)//перебор строки
		{
			buf+=str[i];//запись слова
			if(str[i+1]==' '){ //если "пробел"
				if(counter==num) return buf;//возвращаем слово
				counter ++;
				i++;
				buf="";//очиста буфера
			}
		}
		return "ERR";
	}
	bool CheckPIN(string PIN){//проверка PIN-кода(принимает строку с пин-кодом)
		string checkPIN;//пин-код
		int wrong =3;//количество ошибок
		for(;;){
			cout << "Введите пин-код: ";
			cin >> checkPIN;
			if(checkPIN==PIN) return true;//есои совпали
			else if(wrong!=0) {//если неправильно и если лимит ошибок не 0
				cout << "Пин-код неверный! Осталось " << wrong << " Попыток" <<endl;
				wrong--;//-счетчик ошибок
			}
			else {//если счетчик ошибо 0
				cout << "Забериет карту." <<endl;
				return false;
			}

		}
	}
	void InitializationUser(string data_line){//инициализация объекта(принимет данные пользователя)
		string buf_char="";//буфер символов
		string buf_word="";//буфер слов
		ID=stoi(LineByWords(data_line,1));//присвоение 
		name = LineByWords(data_line,3);//присвоение 
		lastname = LineByWords(data_line,4);//присвоение 
		cash = stoi(LineByWords(data_line,9));//присвоение 
		buf_word = LineByWords(data_line,2);//присвоение 
		for(int i=0;i<buf_word.length();i++){//присвоение 
			buf_char=buf_word[i];
			PIN[i]=stoi(buf_char);
		}
		buf_word = LineByWords(data_line,6);//присвоение 
		for(int i=0;i<buf_word.length();i++){//присвоение 
			buf_char=buf_word[i];
			numbrOfCard[i]=stoi(buf_char);
		}
		buf_word = LineByWords(data_line,8);//присвоение 
		for(int i=0;i<buf_word.length();i++){//присвоение 
			buf_char=buf_word[i];
			SVV[i]=stoi(buf_char);
		}
		buf_word = LineByWords(data_line,5);//присвоение 
		int num=0;
		buf_char="";
		for(int i=0;i<buf_word.length();i++){//присвоение 
			if (buf_word[i]!='.' && i!=buf_word.length()-1) buf_char+=buf_word[i];
			else {
				if(i==buf_word.length()-1)buf_char+=buf_word[i];
				DOB[num]=stoi(buf_char);
				buf_char="";
				num++;
			}
		}
		buf_word = LineByWords(data_line,7);//присвоение 
		num =0;
		buf_char="";
		for(int i=0;i<buf_word.length();i++){//присвоение 
			if (buf_word[i]!='.' && i!=buf_word.length()-1) buf_char+=buf_word[i];
			else {
				if(i==buf_word.length()-1)buf_char+=buf_word[i];
				dateReg[num]=stoi(buf_char);
				buf_char="";
				num++;
			}
		}
	}
	bool CheckInt(string choice){//проверка на числа(принимает строку для проверки)
		regex rgnum("^[0-9]+$");//регулярные выражения для чисел
		if(regex_match(choice, rgnum)==1)return true;//если строка из чисел то правда
		else return false;//если нет то ложь
	}
	bool CheckLetter(string str){//проверка на буквы
		regex rgx("^[a-zA-Z]+$");//регулярное выражение для букв
		if(regex_match(str, rgx)==1)return true;// если строка из букв то правда
		else return false;//если нет то ложь
	}
public:
	void RegistOfCadr(){//регистрация карты
		cout << "-----------------Регистрация карты-----------------"<<endl;
		time_t now =time(0);//объект времени
		tm *ltm = localtime(&now);
		srand(time(NULL));//генерация случайных чисел
		ManualInputData();
		dateReg[0]=ltm->tm_mday;//запись дня
		dateReg[1]=1+ltm->tm_mon;//запись месяца
		dateReg[2]=1900+ltm->tm_year;//запись года
		for(int i =0;i<16;i++){//генерация номера карты
			numbrOfCard[i]=rand()%10;
		}
		for(int i =0 ;i<3;i++){//генерация SVV
			SVV[i]=rand()%10;
		}
		cash=0.00;//баланс
		cout << "Ваш ID - "<< IDAdd() << endl;
		InDB("database.txt");
	}
	bool GetClient(int ID){//данные пользователя из БД
		ifstream file;//объект для чтения файла
		file.open("database.txt");//путь к файлу
		string buf_line = " ";//буфер для строки
		string buf_char;//буфер для даты
		while (!file.eof())//проверка на конец файла
		{
			getline(file, buf_line);//запись строки файла в буфер
			buf_char = LineByWords(buf_line, 1);//запись ID из строки
			if(CheckInt(buf_char))//в строке только цифры?
			{
				if(stoi(buf_char)==ID)//ID из файла == ID полученному?
				{
					file.close();//закрытие файла
					if(CheckPIN(LineByWords(buf_line, 2)))//проверка пин-кода
					{
						InitializationUser(buf_line);
						return true;
					}
					else return false;//не введен пин-код
				}
			}	
		}
		cout << "Карта не найдена" << endl;
		file.close();//закрытие файла
		return false;
	}
	void PrivateData(){//вывод личных данных пользователя
		cout << endl << "------------------Данные о пользователе------------------"<<endl;
		cout << "ID - "<<ID<<endl;
		cout << "Имя - " << name << endl;
		cout << "Фамилия - " << lastname << endl;
		cout << "Дата рождения -";
		for (int i =0;i<3;i++){
			if(i!=2)cout << DOB[i] << ".";
			else cout << DOB[i];
		}
		cout << endl<<"=========================================================="<<endl; 
		cout << endl<<endl;
	}
	void DataOfCard(){//вывод данных о карте
		cout << endl <<"------------------Данные о карте------------------"<< endl;
		cout <<"Номер карты - ";
		for (int i=0;i<16;i++){
			cout << numbrOfCard[i];
			if ((i+1)%4==0 && i!=15)cout << " ";
		}
		cout << endl << "Конец обслуживания карты - "<<dateReg[1]<<"/"<<dateReg[2]+4<<endl;
		cout << "SVV код - " << SVV[0]<<SVV[1]<<SVV[2]<<endl;
		cout << "======================================================="<<endl; 
		cout << endl<<endl;
	}
	void InfOFCash(){//проверка баланса
		cout << endl <<"------------------Данные о балансе------------------"<< endl;
		cout << "Ваш баласн " << cash << " рублей"<< endl;
		cout << "======================================================="<<endl; 
		cout << endl<<endl;
	}
	void PutCash(int act){// снять/внести наличные(принимает внести/получить)
		ofstream file_w;//объект для записи в файл
		ofstream file_buf_w;//объект для записи в файл-буфер
		ofstream file_NULL;//объект для очистки файл
		ofstream file_buf_NULL;//объект для очистки файл-буфера
		ifstream file;//объект для чтения файла
		ifstream file_buf;//объект для чтения файла-буфера
		file_buf.open("bufer.txt");//путь к файлу-буферу для чтения
		file.open("database.txt");//путь к файлу для чтения 

		string line = " ";//буфер для строки
		string buf;//буфер
		string cash;//сумма
		for(;;){//проверка на ввод суммы
			if(act == 1){//если вносим
				cout << "Внесите сумму >>> ";
				cin >>cash;
				if(CheckInt(cash)){//цифры?
					if(stoi(cash)>0){//боьше 0?
						this->cash+=stoi(cash);//прибавляем
						break;//выход
					}
				}
			}
			if(act == -1){//если снимаем
				cout << "Снять сумму >>> ";
				cin >>cash;
				if(CheckInt(cash)){//цифры?
					if(stoi(cash)>0){//больше 0?
						if(this->cash < stoi(cash)){//хватает?
							cout << "Недостаточно средств!"<<endl;
							break;//выход
						}
						else{
							this->cash-=stoi(cash);//минус со счета
							break;//выход
						}
					}
				}
			}
		}
		file_buf_w.open("bufer.txt", ios_base::app);//открытие файла-буфер для дополнительной записи

		while (!file.eof()){//перебор осн. файла по строкам/поиск строки пользователя 
			getline(file, line);//запись строки файла в буфер
			buf=LineByWords(line,1);//получение ID из строки
			if(ID==stoi(buf)){//проверка ID
				
				file.close();//закрытие файла
				file.open("database.txt");//путь к файлу для чтения 
				
				while (!file.eof()){//переписываем основной файл в файл-буфер исключая полученную строку
					getline(file, line);//запись строки
					if(line=="")break;//если пусто то выход
					buf=LineByWords(line,1);//запись ID в буфер
					if(ID!=stoi(buf)){//совподают?
						file_buf_w << line << endl;//запись в файл
					}
				}
				InDB("bufer.txt");//добавляем измененную строку пользователя в файл-буфер
				

				file_NULL.open("database.txt");//открытие файла для очистки
				file_NULL << "";//очищаем основонй файл
				file_NULL.close();//закрытие файла

				file_w.open("database.txt", ios_base::app);//открытие файла для дополнительной записи
				while (!file_buf.eof()){//проверка на конец файла
					getline(file_buf,line);//запись строки 
					if(line=="")break;//если пусто то выход
					file_w << line << endl;//запись в файл
				}
				file_w.close();//закрытие файла
			
				break;//выход
			}
			
		}
		file_buf_w.close();//закрытие файла

		file_buf_NULL.open("bufer.txt");//открытие файла-буфера для очистки
		file_buf_NULL << "";//очистка
		file_buf_NULL.close();//закрытие файла-буфера 

		file_buf.close();//закрытие файла
		file.close();//закрытие файла
	}
	void Transfer(){//перевод 
		ofstream file_w;//объект для записи в файл
		ofstream file_buf_w;//объект для записи в файл
		ofstream file_NULL;//объект для очистки файл
		ofstream file_buf_NULL;//объект для очистки файл-буфера
		ifstream file;//объект для чтения файла
		ifstream file_buf;//объект для чтения файла-буфера
		file_buf.open("bufer.txt");//путь к файлу для чтения
		file.open("database.txt");//путь к файлу для чтения 

		string ID;//ID
		string cash="";//сумма
		string line="";//буфер
		string buf="";//буфер
		string receiver, sender;//данные отправителя/получателя
		file_buf_w.open("bufer.txt", ios_base::app);//открытие файла-буфер для дополнительной записи

		for(;;){//проверка ID
			cout << "Введите ID получателя --> ";
			cin >> ID;
			if(CheckInt(ID)){//цифры?
				while (!file.eof()){//пербор файла
					getline(file, line);//запись строки файла в буфер
					buf=LineByWords(line,1);//получение ID из строки
					if(ID==buf){//ID верный?
						buf="true";
						break;//выход
					}
				}
				file.close();//закрытие файла
				file.open("database.txt");//путь к файлу для чтения
				if(buf=="true")break;//выход
			}
			cout<< "ID не найден! " << endl;
		}

		for(;;){//проверка суммы
			cout << "Введите сумму перевода --> ";
			cin >> cash;
			if(CheckInt(cash)){//число?
				if(stoi(cash)>this->cash)cout << "Недостаточно средств!" << endl;//если недостаточно средств
				else break;
			}
			else cout << "Некоректный ввод!" << endl;
		}

		while (!file.eof()){//передор файла
			getline(file, line);//запись строки файла в буфер
			buf=LineByWords(line,1);//получение ID из строки
			if(CheckInt(buf)){//проверка на число
				if(stoi(buf)==this->ID)sender=line;//данные отправителя
				else if(stoi(buf)==stoi(ID))receiver=line;//данные получателя
				else file_buf_w << line <<endl;//запись в файл
			}
		}
		file_buf_w.close();//закрытие файла
	
		InitializationUser(receiver+=" ");//инициализация получателя
		this->cash+=stoi(cash);//+на счет
		InDB("bufer.txt");//запись в буфер
		InitializationUser(sender+=" ");//инициализация отправителя
		this->cash-=stoi(cash);//-со счета
		InDB("bufer.txt");//запись в буфер

		file_NULL.open("database.txt");//открытие файла для очистки
		file_NULL << "";//очищаем основонй файл
		file_NULL.close();//закрытие файла

		file_w.open("database.txt", ios_base::app);//открытие файла
		while (!file_buf.eof()){//перебор файла
			getline(file_buf, line);//запись строки файла в буфер
			if(line!="") file_w << line << endl;//запись в основной файл
		}
		file_w.close();//закрытие файла

		file_buf_NULL.open("bufer.txt");//открытие файла-буфера для очистки
		file_buf_NULL << "";//очистка
	 	file_buf_NULL.close();//закрытие файла-буфера 

		file_buf.close();//закрытие файла
		file.close();//закрытие файла
	}	
};

int main()
{
	setlocale(LC_ALL, "RUSSIAN");//перевод на русский
	Client A;//иниуиализация клиента
	string choice="";//выбор
	regex rgnum("^[0-9]+$");//регулярное выражение для чисел
	cout << "===================== Имитация работы банкомата =====================" << endl;
	cout << "---------------------------------------------------------------------" <<endl;
	for(;;){//зацикливание программы
		cout << endl <<"Выберите пункт меню: "<< endl;
		cout << "| 1 | Регистрация пользователя" << endl <<"| 2 | Вставить кару" <<endl<<"|   | Выйти введите любую клавишу --->";
		cin >> choice;
		cout << endl;
		if(choice == "1"){
			A.RegistOfCadr();
			cout << endl;
		}  
		else if (choice == "2"){
			cout << "Введите ID --->";
			cin >> choice;
			if (regex_match(choice,rgnum)){
				if(A.GetClient(stoi(choice))){
					for(;;){//зацикливание меню пользователя
						cout << endl <<"Выберите пункт меню: "<< endl;
						cout << "| 1 | Информация о пользователе" << endl <<"| 2 | Информация о карте" <<endl<<"| 3 | Проверить баланс" << endl << "| 4 | Пополнить баланс" << endl << "| 5 | Снять наличные" << endl << "| 6 | Перевод денежных средств" << endl << "|   | Забрать карту - введите любую клавишу --->";
						cin >> choice;
						cout<< endl<< endl;
						if(choice == "1")A.PrivateData();
						else if(choice == "2")A.DataOfCard();
						else if (choice == "3")A.InfOFCash();
						else if (choice == "4")A.PutCash(1);
						else if (choice == "5")A.PutCash(-1);
						else if (choice == "6")A.Transfer();
						else break;
					}
				}
			}
			else cout << "Карта не найдена" << endl;
		}
		else break;
	}

}


