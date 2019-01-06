#include <iostream> 
using namespace std; 

const char * ln = "\n---------------------------------------\n";

const char * Alociraj(const char * sadrzaj) { 
	/*  implementirati funkciju koja ce za sadrzaj koji je proslijedjen kao parametar  izvrsiti 
	alociranje memorije, kopiranje karaktera i vracanje pokazivaca na   alociranu lokaciju.
	funkciju iskoristiti na svim mjestima gdje je potrebna  alokacija niza karaktera  */ 
	
	return nullptr;
}

/////////////////////////////DATUM////////////////////////////////////// 
class Datum {
int _dan, _mjesec, _godina;

public: 
	Datum(int d = 1, int m = 1, int g = 2000) {   _dan = d;   _mjesec = m;   _godina = g;  }
	
	friend ostream & operator<< (ostream & COUT, Datum & obj);  //KLASU DATUM PROSIRITI OPERATORSKIM FUNKCIJA KOJE CE BITI 
																//POTREBNE U NASTAVKU PROGRAMA 
}; 

ostream & operator<< (ostream & COUT, Datum & obj) { 
	COUT << obj._dan << "/" << obj._mjesec << "/" << obj._godina;
	return COUT; 
}

/////////////////////////////PREDMET////////////////////////////////////// 
class Predmet { 
protected:  
int _predmetID; 
char * _naziv;  
char * _predavaci[5]; 
int _brojPredavaca;

public:  
	Predmet(int id, const char * naziv, const char * predavac) { 
		_predmetID = id; 
		int size = strlen(naziv) + 1; 
		_naziv = new char[size];  
		strcpy_s(_naziv, size, naziv); 
		_brojPredavaca = 0;   //POKUSAJTE NA DRUGACIJI NACIN DODATI PREDAVACA NA PREDMET
		size = strlen(predavac) + 1; 
		_predavaci[_brojPredavaca] = new char[size]; 
		strcpy_s(_predavaci[_brojPredavaca], size, predavac); 
		_brojPredavaca++; 
	} 

		Predmet(Predmet & obj) { 
			_predmetID = obj._predmetID; 
			int size = strlen(obj._naziv) + 1; 
			_naziv = new char[size]; 
			strcpy_s(_naziv, size, obj._naziv); 
			_brojPredavaca = obj._brojPredavaca;  
			for (int i = 0; i < _brojPredavaca; i++) { 
				size = strlen(obj._predavaci[i]) + 1; 
				_predavaci[i] = new char[size]; 
				strcpy_s(_predavaci[i], size, obj._predavaci[i]); 
			} 
		}  
		~Predmet() {
			delete[] _naziv;   _naziv = nullptr;
			for (int i = 0; i < _brojPredavaca; i++) {
				delete[] _predavaci[i];    _predavaci[i] = nullptr;
			}
		}

		int GetPredmetID() { return _predmetID; }

		void DodajPredavaca(char * ime) {
			if (_brojPredavaca == 5)  
				throw exception("Prekoracen broj predavaca ");  
			for (int i = 0; i < _brojPredavaca; i++)  
				if (strcmp(_predavaci[i], ime) == 0)   
					throw exception("Preavac vec postoji "); 
			int size = strlen(ime) + 1; 
			_predavaci[_brojPredavaca] = new char[size];  
			strcpy_s(_predavaci[_brojPredavaca], size, ime); 
			_brojPredavaca++; 
		}

		friend ostream & operator<< (ostream & COUT, Predmet & obj);
	};

	ostream & operator<< (ostream & COUT, Predmet & obj) { 
		
		COUT << obj._predmetID << " " << obj._naziv << ln;  
		for (int i = 0; i < obj._brojPredavaca; i++)   
			COUT << obj._predavaci[i]; 
		return COUT;
	}

	/////////////////////////////POLOZENI PREDMET////////////////////////////////////// 

	class PolozeniPredmet :public Predmet {
	Datum _datumPolaganja; 
	int _ocjena; 
	public: 
		PolozeniPredmet() :Predmet(0, "-", "-") { _ocjena = 5; }  
		PolozeniPredmet(int id, const char * naziv, const char * predavac, Datum dp, int ocj) : Predmet(id, naziv, predavac), _datumPolaganja(dp) {
			_ocjena = ocj;
		}  
		PolozeniPredmet(PolozeniPredmet & obj) : Predmet(obj), _datumPolaganja(obj._datumPolaganja) { _ocjena = obj._ocjena; } 
		bool operator==(PolozeniPredmet & obj) {   //U PROVJERU JEDNAKOSTI UKLJUCITI I SVE OSTALE ATRIBUTE KLASE PolozeniPredmet  
			if (strcmp(obj._naziv, _naziv) == 0 && obj._predmetID == _predmetID &&    obj._brojPredavaca == _brojPredavaca)   
				return true; 
			return false; 
		}  
		
		PolozeniPredmet & operator=(PolozeniPredmet & obj) {   //OPERATOR= IMPLEMENTIRATI NA DRUGACIJI NACIN, A TOM PRILIKOM VODITI RACUNA O  
															   //1. POTREBNIM DEALOKACIJA  
															   //2. PRAVILNOM PONASANJU FUNKCIJE U SLUCAJU KADA SE RADI O ISTIM OBJEKTIMA  
			_predmetID = obj._predmetID; 
			int size = strlen(obj._naziv) + 1;  
			_naziv = new char[size];  
			strcpy_s(_naziv, size, obj._naziv); 
			_brojPredavaca = obj._brojPredavaca;

			for (int i = 0; i < _brojPredavaca; i++) {
				size = strlen(obj._predavaci[i]) + 1;   
				_predavaci[i] = new char[size]; 
				strcpy_s(_predavaci[i], size, obj._predavaci[i]); 
			}  

			_datumPolaganja = obj._datumPolaganja;  
			_ocjena = obj._ocjena;   return *this; 

		}  friend ostream & operator<< (ostream & COUT, PolozeniPredmet & obj);

	};
	
	ostream & operator<< (ostream & COUT, PolozeniPredmet & obj) { 
		
		COUT << (Predmet&)obj << ln; 
		COUT << obj._datumPolaganja << " " << obj._ocjena;  
		
		return COUT;
	} /////////////////////////////KOLEKCIJA////////////////////////////////////// 
template<class T>
class Kolekcija { 

T * _elementi;
int _max;
int _trenutno;

public: 
	Kolekcija(int max = 10) {   _max = max;   _trenutno = 0;   _elementi = new T[_max];  } 
	~Kolekcija() {   delete[] _elementi;   _elementi = nullptr;  }
	bool AddElement(T & ele) { 
		for (int i = 0; i < _trenutno; i++)  
			if (ele == _elementi[i])   
				return false;  
		if (_trenutno == _max) { 
			_max += 5;
			T * temp = new T[_max];  
			for (int i = 0; i < _trenutno; i++) 
				temp[i] = _elementi[i];  
			delete[] _elementi;   
			_elementi = temp;
		}   _elementi[_trenutno] = ele;  
		_trenutno++; 
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
	
	T * operator[] (int lokacija) {
		if (lokacija >= 0 && lokacija < _trenutno) 
		return &_elementi[lokacija]; 
		return nullptr;
	}  
	
	int GetTrenutno() { return _trenutno; }

	friend ostream & operator<< <>(ostream & COUT, Kolekcija<T> & obj);

}; 

template<class T>
ostream & operator<< <> (ostream & COUT, Kolekcija<T> & obj) { 
	
	for (int i = 0; i < obj._trenutno; i++) 
		COUT << obj._elementi[i] << ln; 
	return COUT;
}

class Student {
int _brojIndeksa; 
char * _imePrezime;  
Kolekcija<PolozeniPredmet> _polozeni;

public: 
	Student(int brInd, const char * ip) { 
		_brojIndeksa = brInd;   int size = strlen(ip) + 1; 
		_imePrezime = new char[size]; 
		strcpy_s(_imePrezime, size, ip);
	} 
	
	~Student() { delete[] _imePrezime;   _imePrezime = nullptr; }

bool DodajPolozeni(int id, const char * naziv, const char * predavac, Datum dp, int ocj) {
	PolozeniPredmet * temp = new PolozeniPredmet(id, naziv, predavac, dp, ocj);   //temp->DodajPredavaca(predavac);
																				  //<<STA BISMO TREBALI OCEKIVATI OD OVE LINIJE CODE-A  
	return _polozeni.AddElement(*temp);
}

bool UkloniPredmetPoIDu(int id) {   //UMJESTO OPERATORA[] PREDLOZITI NEKO DRUGO RJESENJE ZA PRISTUP CLANOVIMA KLASE KOLEKCIJA
	PolozeniPredmet * temp = nullptr;

	for (int i = 0; i < _polozeni.GetTrenutno(); i++) {
		temp = _polozeni[i];
		if (temp != nullptr) {
			if (temp->GetPredmetID() == id) {
				_polozeni.RemoveElement(*temp);
				return true;
			}
		}
	}
	return false;
}

friend ostream & operator<< (ostream & COUT, Student & obj);
};

ostream & operator<< (ostream & COUT, Student & obj) { 
	
	COUT << obj._brojIndeksa << " " << obj._imePrezime << ln;  
	COUT << obj._polozeni << ln; 
	return COUT; } //RAZVITI HIJERARHIJU KLASA KOJE CE SE KORISTI ZA BACANJE IZUZETAKA U SPECIFICNIM IZVRSENJIMA 
				   //PROGRAMA. KAO BAZNU KLASU ISKORISTITI KLASU exception 

void main() {

	try {
		//KREIRATI NIZ OBJEKATA TIPA STUDENT I TESTIRATI DOSTUPNE FUNKCIONALNOSTI KLASA  
		Student a(1122, "Chuck Norris");
		Datum d(12, 5, 2012);

		a.DodajPolozeni(1, "Uvod u programiranje", "doc.dr. Emina Junuz", d, 8);
		a.DodajPolozeni(2, "Programiranje 1", "doc.dr. Jasmin Azemovic", d, 10);
		a.DodajPolozeni(3, "Programiranje 2", "doc.dr. Emina Junuz", d, 9);

		cout << a;
		a.UkloniPredmetPoIDu(2);
		cout << ln << "NAKON UKLANJANJA PREDMETA SA ID-om: 2" << ln << endl;
		cout << a;
	}
	catch (exception & obj) {

		cout << "Greska: " << obj.what() << endl;
	}

	system("pause");
}