//fahirmdz GitHub


#include"pch.h"
#include<iostream>
#include<vector>
#include<string>
#include<cstring>
#include<cctype>
#include<exception>
#include<algorithm>
#include<iterator>
#include<cstdlib>
#include<ctime>
using namespace std;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Error :public exception {
	int _codeLine;
	char* _date, *_time, *_function;

public:
	Error(const char* ms, const char* date, const char* time, const char *funct,int cl):exception(ms) {
		_date = AlocirajNizKaraktera(date);
		_time = AlocirajNizKaraktera(time);
		_codeLine = cl;
		_function = AlocirajNizKaraktera(funct);
	}
	~Error() {
		delete[] _date; _date = nullptr;
		delete[] _time; _time = nullptr;
		delete[] _function; _function = nullptr;
	
	}
	void Info() { cout << "ERROR: " << exception::what() << "\nLine: " << _codeLine << "\nDate: " << _date << "\nTime: " << _time << "\nFunction: " << _function << endl; }

};


//Pojam 
class Term { 
char* _key; 
char* _value; 

public: 
Term():_key(nullptr),_value(nullptr){}
Term(const char *key,const char* value){
	if (key == nullptr || value == nullptr || strlen(value) < 2)
		throw Error("Parametri za pojam nisu validni!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
	_key = AlocirajNizKaraktera(key);
	_value = AlocirajNizKaraktera(value);
}
Term(const Term& t):_value(AlocirajNizKaraktera(t._value)),_key(AlocirajNizKaraktera(t._key)){}
~Term() {
	delete[] _key; _key = nullptr;
	delete[] _value; _value = nullptr;
}

Term& operator=(const Term& t) {
	if (this == &t)
		return *this;

	delete[] _key;
	delete[] _value;
	_key = AlocirajNizKaraktera(t._key);
	_value = AlocirajNizKaraktera(t._value);
	return *this;
}

char* GetKey()const { return _key; }
char* GetValue()const { return _value; }

//Operator za ispis
//Operatori poredenja == i != (koristiti samo vrijednost atributa _key)
friend bool operator==(Term&, Term&);
friend bool operator!=(Term&, Term&);
friend bool operator<(Term&, Term&);
friend ostream& operator<<(ostream&, Term&);

};
bool operator==(Term& t1, Term& t2) {
	return strcmp(t1._key, t2._key) == 0;
}
bool operator!=(Term& t1, Term& t2) {
	return strcmp(t1._key, t2._key) != 0;
}
bool operator<(Term& t1, Term& t2) {
	return t1._key[0] < t2._key[0];
}
ostream& operator<<(ostream& COUT, Term& t) {
	if (t._key == nullptr || t._value == nullptr)
		return COUT;
	cout << t._key << " - " << t._value << endl;
	return COUT;
}

//Rijecnik pojmova 
class Dictionary { 
	string _title;
	int _publicationYear;
	
	//Kljucne rijeci bosanskog jezika u atribut _key 
	//Odgovarajuci prevod na engleskom jeziku u atribut _value 
     vector<Term> _terms; 

public: 
Dictionary():_title("-"),_publicationYear(2019){}
Dictionary(const char* title,int year){
	if (title==nullptr  || strlen(title)<2 || year <= 1950 || year > 2019)
		throw Error("Parametri za rijecnik nisu validni!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
	_title = title;
	_publicationYear = year;
}
Dictionary(const Dictionary& d):_title(d._title),_publicationYear(d._publicationYear){
	_terms = d._terms;
}

Dictionary& operator=(Dictionary& d) {
	if (&d == this)
		return *this;

	_title = d._title;
	_publicationYear = d._publicationYear;
	_terms = d._terms;
	return *this;
}

//Operator () za dodavanje novih pojmova u rijecnik (u paru key-value).
//Sprijeciti dodavanje istih vrijednosti u kolekciju (baciti izuzetak). 
//Pojmovi u svakom momentu treba da budu sortirani abecedno 
//(koristiti funkciju sort iz biblioteke algorithm).
Dictionary& operator()(const char* key, const char* value) {
	try {
		vector<Term>::iterator it;
		Term* temp = new Term(key, value);
		for (it = _terms.begin(); it != _terms.end(); it++)
			if (*it == *temp)
				throw Error("Pojam vec postoji u rijecniku! Ne mogu se dodati 2 ista pojma!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
		_terms.push_back(*temp);
		sort(_terms.begin(), _terms.end());  //SORTIRA PO ABECEDNOM REDOSLIJEDU
	}
	catch (Error& obj) {
		obj.Info();
	}
	return *this;
}


//Operator -= za uklanjanje pojma iz rijecnika na osnovu atributa _key.
Dictionary& operator-=(const char* key) {
	vector<Term>::iterator it;
	for (it = _terms.begin(); it != _terms.end(); it++) {
		if (strcmp(it->GetKey(), key) == 0) {
			_terms.erase(it);
		}
	}
	return *this;
}

//Funkciju koja vraca trenutni broj pojmova u rijecniku.
int GetBrojPojmova()const { return _terms.size(); }

//operator[] koji vraca vrijednost na osnovu kljucne rijeci (povratna vrijednost je engleska rijec-value, 
//na osnovu bosanske-key)
char* operator[](const char* key) {
	vector<Term>::iterator it;
	for (it = _terms.begin(); it != _terms.end(); it++)
		if (strcmp(it->GetKey(), key) == 0)
			return it->GetValue();
	return nullptr;
}
Term& operator[](int index) {
	if (index<0 || index>(int)_terms.size() - 1)
		throw Error("Index nije validan!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
	return _terms[index];
}
//Funkciju koja vraca kolekciju kljuceva (bosanskih rijeci) generisanih slucajnim odabirom.
//Generisanje slucajnog broja izmedu min i max: int randNum = rand()%(max-min + 1) + min; 
//Funkcija kao parametar prima broj rijeci koje je potrebno generisati.
vector<string> returnKey(int brk) {
	if ((unsigned int)brk>_terms.size())
		return vector<string>();

	vector<string> keys;
	int i = 0;
	while (i < brk) {
		srand(time(0));
		int randkey = rand() % (int)_terms.size();
		int finalrand = randkey;
		
		int flag = 0;
		for(int j=0;j<(int)keys.size();j++)
			if (strcmp(keys[j].c_str(), _terms[finalrand].GetKey()) == 0) {
				flag = 1;
			}
		if (flag == 0) {
			keys.push_back(_terms[finalrand].GetKey());
			i++;
		}
	}
	return keys;
}

void Info() {
	cout << "------Dictionary '" << _title << "'----------\n----------------------------------------------\n";
	cout << "Publication year: " << _publicationYear << endl << endl;
	vector<Term>::iterator it;
	for (it = _terms.begin(); it != _terms.end(); it++)
		cout << *it << endl;
}
};



//Osmisliti i implementirati klasu Competitor (Takmicar) na nacin da se upotrebom objekta klase Dictionary 
//svakom takmicaru dodijele odredena pitanja (rijeci bosanskog jezika) - koristiti implementiranu funkciju klase Dictionary.
//Svakom pitanju je potrebno pridružiti odgovor takmicara (kolekcija _odgovori).
//Obavezno implementirati funkciju koja vrši kalkulaciju osvojenih bodova (1 tacan odgovor = 1 bod).
//Po potrebi dopuniti klasu dodatnim atributima. 

//Takmicar
class Competitor { 
string _ime;
string _prezime;
vector<string> _pitanja;
vector<string*> _odgovori;
Dictionary _rijecnik;
public:
	Competitor():_ime(""),_prezime(""){}
	Competitor(string ime, string prezime,Dictionary d):_rijecnik(d) {
		if (ime.length() < 2 || prezime.length() < 2)
			throw Error("Parametri za takmicara nisu validni!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
		_ime = ime;
		_prezime = prezime;
		_pitanja = _rijecnik.returnKey(5);
		for (int i = 0; i < (int)_pitanja.size(); i++)
			_odgovori.push_back(new string(""));
	}
	Competitor(const Competitor& cm) :_ime(cm._ime), _prezime(cm._prezime) {
		_pitanja = cm._pitanja;
		for (unsigned int i = 0; i < cm._odgovori.size(); i++)
			_odgovori.push_back(new string(*cm._odgovori[i]));
		_rijecnik = *const_cast<Dictionary*>(&cm._rijecnik);
		_pitanja = cm._pitanja;
	}
	~Competitor() {
		for (unsigned int i = 0; i < _odgovori.size(); i++) {
			delete _odgovori[i];
			_odgovori[i] = nullptr;
		}
	}

	Competitor& operator=(Competitor& c) {
		if (&c == this)
			return *this;
		_ime = c._ime;
		_prezime = c._prezime;
		_pitanja = c._pitanja;
		for (unsigned int i = 0; i < c._odgovori.size(); i++)
			_odgovori.push_back(new string(*c._odgovori[i]));
		_rijecnik = c._rijecnik;
		return *this;
	}
	Competitor& operator-=(int index) {
		if ((unsigned int)index > _odgovori.size() || index < 0)
			throw Error("Index nije validan!", __DATE__, __TIME__, __FUNCTION__, __LINE__);
		vector<string*>::iterator it;
		it = _odgovori.begin() + index;
		_odgovori.erase(it);
		return *this;
	}

	void DodajOdgovor(int rbOdg,const char* odg) { 
		if (rbOdg<0 || rbOdg>(int)_pitanja.size()-1)
			return;
		vector<string*>::iterator it = _odgovori.begin() + rbOdg;
		*it = new string(odg);
	}

	int brojOsvojenihBodova() {
		if (_odgovori.size() != _pitanja.size() || _odgovori.size() == 0) {
			return 0;
		}
		int uk = 0;
		for (unsigned int i = 0; i < _odgovori.size(); i++) {
			if (_odgovori[i]->compare(_rijecnik[_pitanja[i].c_str()]) == 0)
				uk += 1;
		}

		return uk;
	}
	void printPitanja() {
		cout << "--PITANJA--\n---------------------------------\n";
		for (unsigned int i = 0; i < _pitanja.size(); i++)
			cout << i + 1 << ". " << _pitanja[i] << endl;
	}
	void pokreniTest() {
		cout << "\n----TEST ENGLESKOG JEZIKA----\n------------------------------\n";
		int i = 0;


		auto inputOdgovor = [=](int i) {
			char* x = new char[50];
			cout << i + 1 << ". odgovor ---> ";
			cin.getline(x, 50);
			if (strcmp(x, "0") != 0)
				DodajOdgovor(i, x);
			else
				return false;
			return true;
		};
		printPitanja();
		cout << endl << endl;
		bool flag = true;
		do {
			flag = inputOdgovor(i++);
		} while (i != 5 && flag);

		char vrati = 'N', ispraviJos = 'D ';

		do {
			cout << "\nDa li zelite da ispravite neki odgovor?(D/N) ";
			cin >> vrati;

			if (toupper(vrati) == 'D') {
				int rb;
					cout << "-Unesite Rb. odgovora i novi odgovor-\nRedni broj: ";
					do {
						cin >> rb;
						if (rb < 1 || rb>5)
							cout << "Pogresan redni broj! Pokusajte opet..(0-IZLAZ)\n";
					} while (rb < 1 || rb>5);
					cin.ignore();
					inputOdgovor(rb - 1);

				do {
					cout << "Ispravi jos odgovora?(D/N) ";
					cin >> ispraviJos;
				} while (toupper(ispraviJos) != 'N' && toupper(ispraviJos) != 'D');
			}


		} while ((toupper(vrati)!='D' && toupper(vrati)!='N') || toupper(ispraviJos)=='D');

	}

	string GetImePrezime() { return string(_ime + " " + _prezime); }
	void Info() {
		cout << "\n-------TAKMICAR INFO---------\n----------------------------------\n";
		cout << "Ime: " << _ime << "\nPrezime: " << _prezime << endl;
		printPitanja();
		cout << "\n--ODGOVORI--\n--------------------------------\n";
		for (unsigned int i = 0; i < _odgovori.size(); i++)
			cout << i + 1 << ". " << *_odgovori[i] << endl;
	}
};


int main() {

	try {
		//Kreirati rijecnik pojmova. 
		//Kreirati niz objekata tipa Competitor i simulirati takmicenje u poznavanju rijeci engleskog jezika. 
		//Pronaci najboljeg medu takmicarima (najveci broj bodova).
		Term* _pojmovi[10];
		_pojmovi[0] = new Term("Stolica", "Chair");
		_pojmovi[1] = new Term("Crna", "Black");
		_pojmovi[2] = new Term("Zabava", "Entertainment");
		_pojmovi[3] = new Term("Zvijer", "Beast");
		_pojmovi[4] = new Term("Polimorfizam", "Polimorphysm");
		_pojmovi[5] = new Term("Suncane naocare", "Sunglasses");
		_pojmovi[6] = new Term("Voda", "Water");
		_pojmovi[7] = new Term("Pozuri!", "Hurry up!");
		_pojmovi[8] = new Term("Voz", "Train");
		_pojmovi[9] = new Term("Sjediste", "Seat");

		Dictionary _rijecnik("Bosansko-Engleski rijecnik", 2007);
		_rijecnik("Stolica", "Chair");
		_rijecnik("Crna", "Black");
		_rijecnik("Zabava", "Entertainment");
		_rijecnik("Zvijer", "Beast");
		_rijecnik("Polimorfizam", "Polimorphysm");
		_rijecnik("Suncane naocare", "Sunglasses");
		_rijecnik("Voda", "Water");
		_rijecnik("Voz", "Train");
		_rijecnik("Sjediste", "Seat");

		cout << "\n============TESTIRANJE klase Dictionary==============\n\n";
		_rijecnik.Info();


		cout << "\n------Testiranje konstruktora kopije-------\n";
		Dictionary _rijecnik2(_rijecnik);
		cout << "\n---RIJECNIK 2-----\n";
		_rijecnik2.Info();

		cout << "\n------Testiranje operatora dodijele-------\n";
		Dictionary _rijecnik3;
		_rijecnik3 = _rijecnik2;
		cout << "\n---RIJECNIK 3-----\n";
		_rijecnik3.Info();

		cin.get();
		system("cls");
		cout << "\n================================================\n============TESTIRANJE klase Dictionary==============\n\n";

		Competitor c1("John", "Newman", _rijecnik);
		c1.Info();

		cout << "\n------Testiranje konstruktora kopije-------\n";
		cout << "\nTAKMICAR C2\n";
		Competitor c2(c1);
		c2.Info();

		cout << "\n------Testiranje operatora dodijele-------\n";
		cout << "\nTAKMICAR C3\n";
		Competitor c3;
		c3=c2;
		c3.Info();
		cin.get();
		system("cls");

		c1.pokreniTest();
		c1.Info();
		cout << "\n\nTakmicar " << c1.GetImePrezime() << " je osvojio " << c1.brojOsvojenihBodova() << " bodova na testu\n\n";
	}
	catch (Error& obj) {
		obj.Info();
	}



	system("pause"); 
	return 0;
}