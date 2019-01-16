#include"pch.h"


const char * ln = "\n---------------------------------------\n\0";

char * Alociraj(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);

	return temp;
}

class NedozvoljenaOperacija : public exception {
	int _codeLine;

public:
	NedozvoljenaOperacija(const char* poruka, int line) :exception(poruka), _codeLine(line) {}

	void ErrorInfo() {
		cout << "Greska: " << exception::what();
		cout << "Linija koda: " << _codeLine << endl;
	}
};
/////////////////////////////DATUM////////////////////////////////////// 
class Datum {
	int _dan, _mjesec, _godina;
	void setDefault() {
		_dan = 1;
		_mjesec = 1;
		_godina = 2000;
	}
public:
	Datum(int d = 1, int m = 1, int g = 2000) {
		if (d < 1 || d>31 || m < 1 || m>12 || g < 1920 || g>2019) {
			setDefault();
			cout << "Neispravne vrijednosti! Datum postavljen na defaultne vrijednosti (1.1.2000)\n";
		}
		else
			_dan = d;   _mjesec = m;   _godina = g;
	}

	int getDan()const { return _dan; }
	int getMjesec()const { return _mjesec; }
	int getGodina()const { return _godina; }

	friend bool operator==(Datum&, Datum&);
	friend ostream & operator<< (ostream &, Datum &);  //KLASU DATUM PROSIRITI OPERATORSKIM FUNKCIJAMA KOJE CE BITI 
																//POTREBNE U NASTAVKU PROGRAMA 
};

ostream & operator<< (ostream & COUT, Datum & obj) {
	COUT << "Datum: " << obj._dan << "/" << obj._mjesec << "/" << obj._godina;
	return COUT;
}
bool operator==(Datum& d1, Datum& d2) {
	return d1._dan == d2._dan && d1._mjesec == d2._mjesec && d1._godina == d2._godina;
}

/////////////////////////////PREDMET////////////////////////////////////// 
class Predmet {
protected:
	const int _predmetID;
	char * _naziv;
	char * _predavaci[5];
	int _brojPredavaca;

	void setDefaultValues() {
		_naziv = Alociraj("N/A");
		_brojPredavaca = 0;
		for (int i = 0; i < 5; i++)
			_predavaci[i] = nullptr;
	}
public:
	static int _predmetIDGenerator;

	Predmet(const char * naziv, const char * predavac) :_predmetID(_predmetIDGenerator++) {
		if (naziv == nullptr || strlen(naziv) < 3 || predavac == nullptr || strlen(predavac) < 2)
			throw NedozvoljenaOperacija("Neispravni parametri za predmet..\n", __LINE__);
		_naziv = Alociraj(naziv);
		_predavaci[0] = Alociraj(predavac);
		for (int i = 1; i < 5; i++)
			_predavaci[i] = nullptr;
	}

	Predmet(Predmet & obj) :_predmetID(obj._predmetID), _naziv(Alociraj(obj._naziv)), _brojPredavaca(obj._brojPredavaca) {
		for (int i = 0; i < _brojPredavaca; i++)
			_predavaci[i] = Alociraj(obj._predavaci[i]);
	}
	Predmet(Predmet&& obj) :_predmetID(obj._predmetID), _brojPredavaca(obj._brojPredavaca) {
		_naziv = obj._naziv;
		for (int i = 0; i < 5; i++) {
			_predavaci[i] = obj._predavaci[i];
			obj._predavaci[i] = nullptr;
		}
		obj._naziv = nullptr;
	}
	~Predmet() {
		cout << "~Predmet()\n";
		delete[] _naziv;   _naziv = nullptr;
		for (int i = 0; i < _brojPredavaca; i++) {
			delete[] _predavaci[i];
		}
	}

	int GetPredmetID() { return _predmetID; }
	char* getNaziv() { return _naziv; }

	void DodajPredavaca(char * ime) {
		if (_brojPredavaca == 5)
			throw NedozvoljenaOperacija("Prekoracen broj predavaca ", __LINE__);
		for (int i = 0; i < _brojPredavaca; i++)
			if (strcmp(_predavaci[i], ime) == 0)
				throw NedozvoljenaOperacija("Predavac vec postoji.\n", __LINE__);
		_predavaci[_brojPredavaca++] = Alociraj(ime);
	}

	friend bool operator==(Predmet&, Predmet&);
	friend ostream & operator<< (ostream&, Predmet&);
};
int Predmet::_predmetIDGenerator = 0;

ostream & operator<< (ostream & COUT, Predmet & obj) {

	COUT << "ID: " << obj._predmetID << " - Naziv: " << obj._naziv << endl;
	for (int i = 0; i < obj._brojPredavaca; i++)
		COUT << obj._predavaci[i];
	return COUT;
}
bool operator==(Predmet& p1, Predmet& p2) {
	if (p1._naziv == nullptr || p2._naziv == nullptr)
		return false;
	return strcmp(p1._naziv, p2._naziv) == 0 && p1._brojPredavaca == p2._brojPredavaca && p1._predmetID == p2._predmetID;
}

/////////////////////////////POLOZENI PREDMET////////////////////////////////////// 

class PolozeniPredmet :public Predmet {
	Datum _datumPolaganja;
	int _ocjena;
public:
	PolozeniPredmet() :Predmet("-", "-") { _ocjena = 5; }
	PolozeniPredmet(const char * naziv, const char * predavac, Datum dp, int ocj) : Predmet(naziv, predavac) {
		try {
			_datumPolaganja = dp;
		}
		catch (NedozvoljenaOperacija& obj) {
			obj.ErrorInfo();
		}
		if (naziv == nullptr || predavac == nullptr || ocj < 5 || ocj>10) {
			throw NedozvoljenaOperacija("Neispravni parametri za polozeni predmet..\n", __LINE__);
		}
		_ocjena = ocj;
	}
	PolozeniPredmet(PolozeniPredmet & obj) : Predmet(obj), _datumPolaganja(obj._datumPolaganja), _ocjena(obj._ocjena) {	}


	friend ostream & operator<< (ostream&, PolozeniPredmet&);
	friend bool operator==(PolozeniPredmet&, PolozeniPredmet&);
};
bool operator==(PolozeniPredmet & pp1, PolozeniPredmet& pp2) {   //U PROVJERU JEDNAKOSTI UKLJUCITI I SVE OSTALE ATRIBUTE KLASE PolozeniPredmet  
	return (Predmet&)pp1 == (Predmet&)pp2 && pp1._datumPolaganja == pp2._datumPolaganja && pp1._ocjena == pp2._ocjena;
}

ostream & operator<< (ostream & COUT, PolozeniPredmet & obj) {

	COUT << (Predmet&)obj;
	COUT << obj._datumPolaganja << "\nOcjena: " << obj._ocjena << ln;

	return COUT;
}

/////////////////////////////KOLEKCIJA////////////////////////////////////// 
template<class T>
class Kolekcija {

	T * _elementi;
	const int _max;
	int _trenutno;

public:
	Kolekcija(int max = 10) :_max(max), _trenutno(0) {
		_elementi = new T[_max];
	}
	~Kolekcija() {
		delete[] _elementi;
		_elementi = nullptr;
	}
	bool AddElement(T & ele) {
		if (_max == _trenutno)
			return false;

		_elementi[_trenutno++] = ele;
		return true;
	}

	bool RemoveElement(T & ele) {
		for (int i = 0; i < _trenutno; i++)
			if (ele == _elementi[i]) {
				for (int j = i; j < _trenutno - 1; j++) {
					_elementi[j] = _elementi[j + 1];
				}
				cout << endl;
				_trenutno--;
				return true;
			}
		return false;
	}

	T& operator[] (int lokacija) {
		if (lokacija < 0 || lokacija > _trenutno - 1)
			throw NedozvoljenaOperacija("Indeks nije validan.", __LINE__);
		return _elementi[lokacija];
	}

	T* getNiz() { return _elementi; }
	int GetTrenutno() { return _trenutno; }
	int GetMax() { return _max; }


	friend ostream & operator<< <>(ostream & COUT, Kolekcija<T> & obj);

};

template<class T>
ostream & operator<< <> (ostream & COUT, Kolekcija<T> & obj) {

	for (int i = 0; i < obj._trenutno; i++)
		COUT << obj._elementi[i] << ln;
	return COUT;
}

class Student {
	static int _brojIndeksaGenerator;
	const int _brojIndeksa;
	char * _imePrezime;
	Kolekcija<PolozeniPredmet*> _polozeni;

public:
	Student(const char * ip) :_brojIndeksa(_brojIndeksaGenerator++),_polozeni(40) {
		if (ip == nullptr || strlen(ip) < 2)
			throw NedozvoljenaOperacija("Neispravan format imena i prezimena!\n", __LINE__);
		_imePrezime = Alociraj(ip);
	}

	~Student() {
		delete[] _imePrezime;   _imePrezime = nullptr;
		for (int i = 0; i < _polozeni.GetTrenutno(); i++) {
			delete _polozeni[i];
		}
	}

	bool DodajPolozeni(const char * naziv, const char * predavac, Datum dp, int ocj) {
		if (_polozeni.GetTrenutno() == 40)
		{
			cout << "Ne mozete dodati vise predmeta!\n";
			return false;
		}
		try {
			for (int i = 0; i < _polozeni.GetTrenutno(); i++)  //PROVJERA SE VRSI U OVOJ KLASI JER SE RADI O KOLEKCIJA POKAZIVACA NA PolozeniPredmet( Razlog: Konstanta u klasi Predmet)
				if (strcmp(_polozeni[i]->getNaziv(), naziv) == 0)
					return false;
			PolozeniPredmet* temp = new PolozeniPredmet(naziv, predavac, dp, ocj);
			return _polozeni.AddElement(temp);
			temp = nullptr;
		}
		catch (NedozvoljenaOperacija& obj) {

			obj.ErrorInfo();
			return false;
		}

	}

	bool UkloniPredmetPoIDu(int id) {   //UMJESTO OPERATORA[] PREDLOZITI NEKO DRUGO RJESENJE ZA PRISTUP CLANOVIMA KLASE KOLEKCIJA
		if (id<0 || id>Predmet::_predmetIDGenerator)
			throw NedozvoljenaOperacija("Neispravan ID predmeta!\n", __LINE__);
		PolozeniPredmet * temp = nullptr;
		for (int i = 0; i < _polozeni.GetTrenutno(); i++) {
			temp = _polozeni[i];
			if (temp != nullptr && temp->GetPredmetID() == id) {
				_polozeni.RemoveElement(temp);
				delete temp;
				return true;
			}
		}
		return false;
	}

	friend ostream & operator<< (ostream &T, Student &);
};

ostream & operator<< (ostream & COUT, Student & obj) {
	if (obj._imePrezime == nullptr)
		return COUT;
	cout << "\nBroj indeksa: " << obj._brojIndeksa << "\nIme i prezime: " << obj._imePrezime << ln << "--POLOZENI PREDMETI--\n\n";
	for (int i = 0; i < obj._polozeni.GetTrenutno(); i++)
		cout << *obj._polozeni[i] << endl;
	return COUT;
}

//RAZVITI HIJERARHIJU KLASA KOJE CE SE KORISTI ZA BACANJE IZUZETAKA U SPECIFICNIM IZVRSENJIMA 
//PROGRAMA. KAO BAZNU KLASU ISKORISTITI KLASU exception 

int Student::_brojIndeksaGenerator = 180000;

void main() {

	try {
		//KREIRATI NIZ OBJEKATA TIPA STUDENT I TESTIRATI DOSTUPNE FUNKCIONALNOSTI KLASA  
		Datum d(1, 1, 2019);
		Student a("Chuck Norris");

		if (a.DodajPolozeni("UPR", "doc.dr. Emina Junuz", d, 8))
			cout << "Predmet uspjesno dodan..\n";
		if (a.DodajPolozeni("Programiranje 1", "doc.dr. Jasmin Azemovic", d, 10))
			cout << "Predmet uspjesno dodan..\n";
		if (a.DodajPolozeni("Programiranje 2", "doc.dr. Emina Junuz", d, 9))
			cout << "Predmet uspjesno dodan..\n";

		cout << a;
		a.UkloniPredmetPoIDu(2);
		cout << ln << "NAKON UKLANJANJA PREDMETA SA ID-om: 2" << ln << endl;
		cout << a;
	}
	catch (NedozvoljenaOperacija& obj) {

		obj.ErrorInfo();
	}
	catch (const std::exception& e) {
		cout << e.what() << endl;
	}

	system("pause");
}