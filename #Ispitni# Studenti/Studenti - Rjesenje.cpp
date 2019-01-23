//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<regex>	
#include<exception>
#include<iterator>
#include<cctype>
#include<thread>
#include<algorithm>
using namespace std;

/*1. Ukoliko je potrebno, implementirati nasljeđivanje 
2. U okviru svih klasa implementirati potrebne (de)konstruktor funkcije 
3. U slučaju nedozvoljenih operacija ili prekoračenja, a koristedi kreiranu klasu GreskaUProgramu, 
   sve funkcije članice trebaju „baciti“ izuzetak sa odgovarajudom porukom tj. objekat tipa GreskaUProgramu
4. Ukoliko smatrate da je neophodno, ispitni zadatak možete proširiti dodatnim funkcionalnostima ili 
   modifikovati postavljene zahtjeve. Proširenje ili modifikacije moraju pratiti adekvatni komentari sa 
   kojima dete obrazložiti potrebu za implementiranim/modifikovanim funkcionalnostima. 
5. Prilikom definisanja funkcija voditi računa da uništavanjem jednog objekta ne utječemo na stanje drugog objekta. 
   Prilikom uništavanja objekata ili bilo kakvih modifikacija voditi računa o dealokaciji zauzete memorije. */
const char* crt = "\n----------------------------------------------------\n";
char* Alociraj(const char* a) {
	if (a == nullptr)
		return nullptr;
	int v = strlen(a) + 1;
	char* temp = new char[v];
	strcpy_s(temp, v, a);
	return temp;
}
class GreskaUProgramu :public exception {
	string _funkcija;
	int _codeLine;
public:
	GreskaUProgramu(const char* msg,string funk,int cL):exception(msg),_funkcija(funk),_codeLine(cL){}

	const char* what()const {
		string x = string("ERROR: ") + exception::what() + "\nCode line: " + to_string(_codeLine) + "\Funkcija: " + _funkcija;
		return Alociraj(x.c_str());
	}
};

template<class T1, class T2, int max>
class Kolekcija {
	T1 * _clan1[max];
	T2 * _clan2[max];
	int * _trenutnoClanova;
	void copyElements(const Kolekcija& k) {
		for (int i = 0; i < *k._trenutnoClanova; i++) {
			_clan1[i] = new T1(*k._clan1[i]);
			_clan2[i] = new T2(*k._clan2[i]);
		}
		for (int i = *k._trenutnoClanova; i < max; i++) {
			_clan1[i] = nullptr;
			_clan2[i] = nullptr;
		}
	}
	void dealocateElements() {
		for (int i = 0; i < *_trenutnoClanova; i++) {
			delete _clan1[i]; _clan1[i] = nullptr;
			delete _clan2[i]; _clan2[i] = nullptr;
		}
	}
public:
	Kolekcija() :_trenutnoClanova(new int(0)) {
		for (int i = 0; i < max; i++) {
			_clan1[i] = nullptr;
			_clan2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& k) :_trenutnoClanova(new int(*k._trenutnoClanova)) {
		copyElements(k);
	}
	~Kolekcija() {
		dealocateElements();
		delete _trenutnoClanova; _trenutnoClanova = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		*_trenutnoClanova = *k._trenutnoClanova;
		dealocateElements();
		copyElements(k);
		return *this;
	}
	int GetTrenutno()const { return *_trenutnoClanova; }	
	int GetMax()const { return max; }	

	T1& GetElement1(int index) {
		if (index < 0 || index >= *_trenutnoClanova)
			throw GreskaUProgramu("Index lokacije elementa nije validan!", __FUNCTION__, __LINE__);
		return *_clan1[index];
	}
	T2& GetElement2(int index) {
		if (index < 0 || index >= *_trenutnoClanova)
			throw GreskaUProgramu("Index lokacije elementa nije validan!", __FUNCTION__, __LINE__);
		return *_clan2[index];
	}
	//2. AddElement :: omogucava dodavanje novih elementa u kolekciju (u clan1 i clan2).Onemoguciti
//dodavanje dva identicna elementa u kolekciju 
	void AddElement(T1 t1, T2 t2) {
		if (max == *_trenutnoClanova) {
			cout << "Dostigli ste maksimum! Ne mozete dodati vise elemenata u kolekciju!\n";
			return;
		}
		for (int i = 0; i < *_trenutnoClanova; i++)
			if (*_clan1[i] == t1 && *_clan2[i] == t2)
				throw GreskaUProgramu("Element vec postoji!", __FUNCTION__, __LINE__);
		_clan1[*_trenutnoClanova] = new T1(t1);
		_clan2[(*_trenutnoClanova)++] =new T2(t2);
	}


	//3. RemoveElement :: uklanja element na zahtijevanoj lokaciji.Prilikom uklanjanja elementa
	//izvrsiti potrebnu dealokaciju, te onemoguciti pojavljivanje neinicijaliziranih elemenata niza.
	void RemoveElement(int lokacija) {
		if (*_trenutnoClanova == 0)
			throw GreskaUProgramu("Trenutno nema elemenata!", __FUNCTION__, __LINE__);
		if (lokacija < 0 || lokacija >= *_trenutnoClanova)
			throw GreskaUProgramu("Lokacija elementa nije validna!", __FUNCTION__, __LINE__);
		delete _clan1[lokacija];
		_clan1[lokacija] = nullptr;
		delete _clan2[lokacija];
		_clan2[lokacija] = nullptr;

		for(int i=lokacija;i<*_trenutnoClanova-1;i++){
			_clan1[i] = _clan1[i + 1];
			_clan2[i] = _clan2[i + 1];
		}
		(*_trenutnoClanova)--;
	}

	//4. operator -=  :: iz postojece kolekcije (clanovi 1 i 2) uklanja elemente koji su identicni
	//nekom od elemenata u kolekciji koja je primljena kao parametar.
	Kolekcija& operator-=(T1 t1) {
		if (*_trenutnoClanova == 0)
			return *this;
		for (int i = 0; i < *_trenutnoClanova; i++)
			if (*_clan1[i] == t1)
				RemoveElement(i);
		return *this;
	}
	bool Postoji(T1 t1) {
		for (int i = 0;i<*_trenutnoClanova; i++)
			if (t1 == *_clan1[i])
				return true;
		return false;
	}
	friend ostream& operator<<<>(ostream&, const Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
}; 
template<class T1,class T2,int max>
ostream& operator<<(ostream& COUT, const Kolekcija<T1, T2, max>& k) {
	for (int i = 0; i < *k._trenutnoClanova; i++)
		cout << *k._clan1[i] << " - " << *k._clan2[i] << endl;
	return COUT;
}
template<class T1,class T2,int max>
bool operator==(Kolekcija<T1, T2, max>& k1, Kolekcija<T1, T2, max>& k2) {
	if (*k1._trenutnoClanova == *k2._trenutnoClanova) {
		for (int i = 0; i < *k1._trenutnoClanova; i++)
			if (*k1._clan1[i] != *k2._clan1[i] || *k1._clan2[i] != *k2._clan2[i])
				return false;
		return true;	
	}
	return false;
}

class Osoba{ 
protected: 
char * _imePrezime;

public:  
	Osoba(const char* imePrezime="NEMA VRIJEDNOST"){
		if (strcmp(imePrezime, "NEMA VRIJEDNOST") != 0 && !regex_match(imePrezime, regex("[A-Z]{1}[a-z]{1,15}\\s{1,2}[A-Z]{1}[a-z]{1,15}")))
			throw GreskaUProgramu("Ime i prezime nisu validni!", __FUNCTION__, __LINE__);
		_imePrezime = Alociraj(imePrezime);
	}
	Osoba(const Osoba& o):_imePrezime(Alociraj(_imePrezime)){}
	virtual ~Osoba() { delete[] _imePrezime; _imePrezime = nullptr; }

	Osoba& operator=(const Osoba& o) {
		if (&o == this)
			return *this;
		delete[] _imePrezime;
		_imePrezime = Alociraj(o._imePrezime);
		return *this;
	}
	__declspec(property(get = GetIndeks))char* ImePrezime;
	char* GetImePrezime()const { return _imePrezime; }

	virtual void Info()const = 0 {
		cout << "Ime i prezime: " << _imePrezime << endl;
	}
	//2. Deklarisati kao apstraktnu klasu 
}; 

class Predmet {
	char * _naziv;
	char * _profesor;
	
public:
	Predmet(const char* naziv = "NEMA VRIJEDNOST", const char* profesor = "NEMA VRIJEDNOST") {
		if (strcmp(naziv, "NEMA VRIJEDNOST") != 0 && !regex_match(naziv, regex("^[a-zA-Z\\s]{3,20}$")))
			throw GreskaUProgramu("Naziv predmeta nije validan!", __FUNCTION__, __LINE__);
		if (strcmp(profesor, "NEMA VRIJEDNOST") != 0 && !regex_match(profesor, regex("[A-Z]{1}[a-z]{1,15}\\s{1,2}[A-Z]{1}[a-z]{1,15}")))
			throw GreskaUProgramu("Ime i prezime profesora nisu validni!", __FUNCTION__, __LINE__);
		_naziv = Alociraj(naziv);
		_profesor = Alociraj(profesor);
	}
	Predmet(const Predmet& p):_naziv(Alociraj(p._naziv)),_profesor(Alociraj(p._profesor)){}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _profesor; _profesor = nullptr;
	}

	Predmet& operator=(const Predmet& p) {
		if (&p == this)
			return *this;
		delete[] _naziv;
		_naziv = Alociraj(p._naziv);
		delete[] _profesor; _profesor = Alociraj(p._profesor);
		return *this;
	}

	__declspec(property(get=GetNaziv))char* Naziv;
	__declspec(property(get=GetProfesor))char* Profesor;
	char* GetNaziv()const { return _naziv; }
	char* GetProfesor()const { return _profesor; }
	friend ostream& operator<<(ostream&, const Predmet&);
	friend bool operator==(Predmet&, Predmet&);
	friend bool operator!=(Predmet&, Predmet&);

};
ostream& operator<<(ostream& COUT, const Predmet& p) {
	cout << "Naziv predmeta: " << p._naziv;
	cout << "\nProfesor: " << p._profesor << endl;
	return COUT;			
}
bool operator==(Predmet& p1, Predmet& p2) {
	return strcmp(p1._naziv, p2._naziv) == 0 && strcmp(p1._profesor, p2._profesor) == 0;
}
bool operator!=(Predmet& p1, Predmet& p2) { return !(p1 == p2); }


class Student :public Osoba {
	static int generatorIndeksa;
	const int _brojIndeksa;
	Kolekcija<Predmet, int, 50>  _polozeniPredmeti;
	vector<char*> _seminarski;//naslovi seminarskih radova 
	void copyValues(const Student& s) {
		for (size_t i = 0; i < s._seminarski.size(); i++)
			_seminarski.push_back(Alociraj(s._seminarski[i]));
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++) {
			Student& temp = *const_cast<Student*>(&s);
			_polozeniPredmeti.AddElement(temp._polozeniPredmeti.GetElement1(i), temp._polozeniPredmeti.GetElement2(i));
		}
	}
	void dealocate() {
		for (size_t i = 0; i < _seminarski.size(); i++) {
			delete[] _seminarski[i]; _seminarski[i] = nullptr;
		}
	}
	string getMalaSlova(string a) {
		transform(a.begin(), a.end(), a.begin(), tolower);
		return a;
	}
public:
	//1. Potrebne konstruktor/destruktor funkcije
	Student(const char* imePrezime):Osoba(imePrezime),_brojIndeksa(generatorIndeksa++){}
	Student(const Student& s) :_brojIndeksa(s._brojIndeksa) {
		copyValues(s);
	}
	~Student() {
		dealocate();
	}

	Student& operator=(const Student& s) {
		if (&s == this)
			return *this;
		dealocate();
		copyValues(s);
		return *this;
	}

	__declspec(property(get = GetIndeks))int Indeks;
	int GetIndeks()const { return _brojIndeksa; }

		//2. DodajPolozeniPredmet :: omogucava dodavanje novopolozenog predmeta.Onemoguciti dodavanje dva
		//istoimena predmeta
	void DodajPolozeniPredmet(Predmet& p,int ocjena) {
		if (ocjena < 6 || ocjena>10)
			throw GreskaUProgramu("Ocjena nije validna(6-10)!",__FUNCTION__, __LINE__);
		if (_polozeniPredmeti.Postoji(p))
			throw GreskaUProgramu("Predmet vec postoji!", __FUNCTION__, __LINE__);
		_polozeniPredmeti.AddElement(p, ocjena);
	}
		//3. UkloniPolozeniPredmet :: na osnovu naziva, uklanja predmet iz liste polozenih predmeta
	void UkloniPolozeniPredmet(const char* nazivPr) {
		if (nazivPr == nullptr)
			return;
		string mala = getMalaSlova(nazivPr);
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
			if (getMalaSlova(_polozeniPredmeti.GetElement1(i).Naziv)==mala)
				_polozeniPredmeti.RemoveElement(i);
	}
		//4. DodajSeminarski :: omogucava dodavanje podataka o seminarskom radu. Onemoguciti dodavanja
		//dva istoimena seminarska rada. U zavisnosti od uspjesnosti obavljene operacije funkcija vraca
		//true/false. Koristiti iteratore.
	bool DodajSeminarski(const char* seminarski) {
		if (seminarski == nullptr)
			return false;
		vector<char*>::iterator it = _seminarski.begin();
		for (it; it != _seminarski.end(); it++)
			if (strcmp(*it, seminarski) == 0)
				return false;
		_seminarski.push_back(Alociraj(seminarski));
		return true;
	}
		//5. UkloniSeminarski :: uklanja podatke o seminarskom radu. U zavisnosti od uspjesnosti
		//obavljene operacije funkcija vraca true/false. Koristiti iteratore.
	bool UkloniSeminarski(const char* seminarski) {
		if (seminarski == nullptr)
			return false;
		string mala = getMalaSlova(seminarski);
		vector<char*>::iterator it = _seminarski.begin();
		for (it; it != _seminarski.end(); it++)
			if (getMalaSlova(*it)==mala) {
				_seminarski.erase(it);
				return true;
			}
		return false;
	}
	void Info()const {
		cout << crt;
		Osoba::Info();
		cout << "Broj indeksa: " << _brojIndeksa << endl;
		cout << crt << ":::::::Polozeni predmeti:::::::" << crt;
		cout << _polozeniPredmeti;
		if (_polozeniPredmeti.GetTrenutno() == 0)
			cout << "--TRENUTNO NEMA POLOZENIH PREDMETA--" << crt;
		cout << crt<<":::::Seminarski radovi:::::" << crt;
		copy(_seminarski.begin(), _seminarski.end(), ostream_iterator<char*>(cout, crt));
	}
		//6. operator <<  :: ispisuje sve dostupne podatke o studentu 
	friend ostream& operator<<(ostream&, const Student&);
};
ostream& operator<<(ostream& COUT, const Student& s) {
	s.Info();
	return COUT;
}
int Student::generatorIndeksa = 190000;
void main() {  
	try {
#pragma region Kolekcija
		Kolekcija<int, int, 5> k1, k2;
		k1.AddElement(1, 3);
		k1.AddElement(2, 6);
		k1.AddElement(3, 9);
		k1.AddElement(4, 12);
		k1.AddElement(5, 15);
		k1.AddElement(6, 18); //Poruka da je dostignut max i ne moze se dodati vise

		cout << k1 << endl;

		k2 = k1;
		k2.RemoveElement(2);
		k2.RemoveElement(3);
		cout <<crt<< k2 << endl;

		Kolekcija<int, int, 5> k3(k1);
		cout << crt << k3 << endl;
		k3 -= 2;
		cout << k3 << endl;

#pragma endregion

#pragma region Predmet
		Predmet p1("PR III", "Denis Music"), p2, p4("BP II", "Jasmin Azemovic"), p5("RS I", "Denis Music");
		cout << p1 << endl;
		p2 = p1;
		cout << crt << p2 << crt;
		Predmet p3(p1);
		cout << p3 << crt;
		if (p1 == p3)
			cout << "ISTI SU\n";

#pragma endregion
#pragma region Student
		Osoba* studenti[3];
		studenti[0] = new Student("John Newman");
		studenti[1] = new Student("Muhammed Kolman");
		studenti[2] = new Student("Jusuf Sa");

		dynamic_cast<Student*>(studenti[0])->DodajPolozeniPredmet(p1, 6);
		dynamic_cast<Student*>(studenti[0])->DodajPolozeniPredmet(p4, 10);
		dynamic_cast<Student*>(studenti[0])->DodajPolozeniPredmet(p5, 9);

		dynamic_cast<Student*>(studenti[0])->DodajSeminarski("Baza dobrovljnih darivatelja krvi");
		dynamic_cast<Student*>(studenti[0])->DodajSeminarski("Web aplikacija");

		dynamic_cast<Student*>(studenti[0])->UkloniPolozeniPredmet("pr iii");
		dynamic_cast<Student*>(studenti[0])->UkloniSeminarski("web aplikacijA");

		studenti[0]->Info(); //AKTIVIRA SE POLIMORFIZAM
		cout << *dynamic_cast<Student*>(studenti[0]) << endl;
#pragma endregion
		//provjeriti validnost izvrsenja svih dostupnih funkcionalnosti 
		//ukratko pojasniti i demonstrirati koncepciju polimorfizma 

		for (int i = 0; i < 3; i++) {
			delete studenti[i]; studenti[i] = nullptr;
		}

	}
	catch (exception& obj) {
		cout << obj.what() << endl; //TU CE SE AKTIVIRATI POLIMORFIZAM, JER JE PREKLOPLJENA KLASA what() u klasi GreskaUProgramu 
									//koja je naslijedila klasu exception
	}
}