//fahirmdz GitHub

#include "pch.h"
#include<iostream>
#include<cstring>
#include<type_traits>
using namespace std; 

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2, int max> 
class FITKolekcija {
private:
int* _trenutno; 
T1 _elementi1[max];
T2* _elementi2[max];

public:  
	FITKolekcija() :_trenutno(new int(0)) {
		for (int i = 0; i < max; i++)
			_elementi2[i] = nullptr;
	}
	FITKolekcija(const FITKolekcija& fk):_trenutno(new int(*fk._trenutno)) {
		if (*_trenutno > 0) {
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = fk._elementi1[i];
				_elementi2[i] = new T2(*fk._elementi2[i]);
			}
		}
	}
	~FITKolekcija() {
		if ( _trenutno!=nullptr && *_trenutno > 0)
			for (int i = 0; i < *_trenutno; i++) {
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
		delete _trenutno; _trenutno = nullptr;
	}
	FITKolekcija& operator=(const FITKolekcija& f) {
		if (&f == this)
			return *this;
		if (*_trenutno > 0)
			for (int i = 0; i < *_trenutno; i++)
				delete _elementi2[i];
		delete _trenutno;
		_trenutno = new int(*f._trenutno);
		if(*_trenutno>0)
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = f._elementi1[i];
				_elementi2[i] = new T2(*f._elementi2[i]);
			}
		return *this;
	}
	FITKolekcija& operator()(const T1& t1, const T2& t2) {
		if (_trenutno != nullptr && *_trenutno < max && *_trenutno >= 0) {
			if (*_trenutno > 0)
				for (int i = 0; i < *_trenutno; i++)
					if (_elementi1[i] == t1 && *_elementi2[i] == t2)
						return *this;
				
			_elementi1[*_trenutno] = t1;
			_elementi2[*_trenutno] = new T2(t2);
			(*_trenutno)++;
		}
		return *this;
	}
	T1 operator[](int index)const {
		return _elementi1[index];
	}
	T2& operator()(int index) {
		return *_elementi2[index];
	}
	FITKolekcija& operator-=(int index) {
		if (index<0 || index>max-1)
			return *this;
		for (int i = index; i < max - 1; i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		delete _elementi2[(*_trenutno) - 1];
		_elementi2[(*_trenutno) - 1] = nullptr;
		(*_trenutno)--;
		return *this;
	}
	int getTrenutno()const { return *_trenutno; }
	friend ostream& operator<< <>(ostream&, FITKolekcija&);
	//Preklopiti operator() na nacin da omoguci dodavanje novih elemenata u kolekciju 
	//Pri tome sprijeciti dodavanja duplih elemenata (odnosi se i na T1 i na T2)   
	//Preklopiti operator[] na nacin da vraca element tipa T1 na osnovu njegove lokacije u nizu 

//Preklopiti operator-= na nacin da uklanja elemente iz kolekcije na osnovu rednog broja proslijedenog kao parametar 

//Preklopiti operator za ispis. Implementaciju raditi izvan tijela klase. 
}; 
template<class T1,class T2,int max>
ostream& operator<< <>(ostream& COUT,FITKolekcija<T1,T2,max>& fk) {
	cout << "Trenutno: " << *fk._trenutno << endl;
	if (*fk._trenutno <= 0)
		cout << "--PRAZNO--\n";
	else
		for (int i = 0; i < *fk._trenutno; i++) 
			cout << *fk._elementi2[i] << " - " <<fk._elementi1[i] << endl;
	return COUT;
}

class Anketa {
		char* _pitanje; 
		int _trajanje;//izraženo u danima 
					 
		FITKolekcija<char*, int, 8> _odgovori; //Kolekcija treba da pohrani moguce odgovore na pitanja, sa pripadajucim rednim brojem  
public: 
	Anketa():_pitanje(nullptr),_trajanje(0){}
	Anketa(const char *pitanje,int tr):_pitanje(AlocirajNizKaraktera(pitanje)),_trajanje(tr){}
	Anketa(const Anketa& a):_pitanje(AlocirajNizKaraktera(a._pitanje)),_trajanje(a._trajanje) {
		_odgovori = a._odgovori;
	}
	Anketa& operator+=(const char *odg) {
		if (_odgovori.getTrenutno() > 0) 
			for (int i = 0; i < _odgovori.getTrenutno(); i++)
				if (strcmp(odg, _odgovori[i]) == 0) {
					cout << "Odgovor vec postoji!'\n";
					return *this;
				}
		_odgovori(AlocirajNizKaraktera(odg), _odgovori.getTrenutno() + 1);
		return *this;
	}
	Anketa& operator-=(int index) {
		if (index<0 || index>_odgovori.getTrenutno() - 1 || _odgovori.getTrenutno()<=0)
			return *this;
		_odgovori -= index;
		for (int i = index; i < _odgovori.getTrenutno(); i++)
			_odgovori(i) -= 1;
		return *this;
	}
	Anketa& operator++() {
		++_trajanje;
		return *this;
	}
	Anketa& operator++(int) {
		_trajanje++;
		return *this;
	}
	friend ostream& operator<<(ostream&,Anketa&);
	friend bool operator==(const Anketa&,const Anketa&);
	//Kreirati potrebne konstruktor i destruktor funkcije
	//Kreirati funkciju DodajOdgovor koja treba da doda novi odgovor za anketu. 
	//Sprijeciti dodavanje istog odgovora više puta. 

//Kreirati funkciju UkloniOdgovor koja uklanja odgovor na osnovu njegovog rednog broja.   
//Preklopiti operator++ na nacin da povecava trajanje ankete za jedan dan 
//Pri tome voditi racuna da se implementiraju dvije verzije ovog operatora (objekat++; ++objekat) 

//Preklopiti operator za ispis 
}; 
ostream& operator<<(ostream& COUT,Anketa& a) {
	if (a._pitanje == nullptr)
		return COUT;
	cout << "Pitanje: " << a._pitanje << "\nTrajanje ankete: " << a._trajanje << endl;
	cout << "Ponudjeni odgovori: \n" << a._odgovori << endl;
	return COUT;
}
bool operator==(const Anketa& a1, const Anketa& a2) {
	if (strcmp(a1._pitanje, a2._pitanje) == 0 && a1._trajanje == a2._trajanje && a1._odgovori.getTrenutno()==a2._odgovori.getTrenutno())
	{
		if (a1._odgovori.getTrenutno() > 0)
			for (int i = 0; i < a1._odgovori.getTrenutno(); i++)
				if (strcmp(a1._odgovori[i], a2._odgovori[i]) != 0)
					return false;
		return true;
	}
	return false;
}

void main() {
	Anketa a("Koji je glavni grad BiH?", 10);

	a += "Zenica";
	a += "Neum";
	a += "Sarajevo";
	a += "Mostar";

	cout << a << endl;

	a -= 1;
	a -= 2;
	cout << a << endl;

	Anketa b("Koji je glavni grad Srbije?", 5);
	b += "Kragujevac";
	b += "Beograd";
	b += "Novi Sad";

	cout << b << endl;

	b -= 0;
	cout << b << endl;

	Anketa c(a);
	cout << c << endl;

	Anketa d;
	d = b;
	cout << d << endl;
}