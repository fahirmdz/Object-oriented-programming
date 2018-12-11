//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<cstring>
#include<cstdlib>

using namespace std;
#pragma warning(disable:4996) 

const char * crt = "\n----------------------------------------------------\n";

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}


class Datum {
private:
	int * _dan, *_mjesec, *_godina;

public:
	Datum() { _dan = _mjesec = _godina = nullptr; }
	Datum(int d, int m, int g) :_dan(new int(d)), _mjesec(new int(m)), _godina(new int(g)) {}
	Datum(const Datum& d) :_dan(new int(*d._dan)), _mjesec(new int(*d._mjesec)), _godina(new int(*d._godina)) {}
	~Datum() {
		delete _dan;
		_dan = nullptr;
		delete _mjesec;
		_mjesec = nullptr;
		delete _godina;
		_godina = nullptr;
	}
	Datum& operator=(const Datum& d) {
		if (&d == this)
			return *this;
		delete _dan;
		delete _mjesec;
		delete _godina;
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
		return *this;
	}
	bool valid()const {
		if (_dan != nullptr && _mjesec != nullptr && _godina != nullptr)
			if ((*_dan > 0 && *_dan <= 31) && (*_mjesec > 0 && *_mjesec <= 12) && (*_godina > 1920 && *_godina <= 2018))
				return true;
		return false;
	}
	friend bool operator==(const Datum&, const Datum&);
	friend ostream & operator<<(ostream &,const Datum&);
};
ostream& operator<<(ostream& COUT,const Datum & d) {
	if (!d.valid())
		cout << "--NEISPRAVAN FORMAT--\n";
	else
		cout << *d._dan << "/" << *d._mjesec << "/" << *d._godina << endl;
	return COUT;
		
}
bool operator==(const Datum& d1, const Datum& d2) {
	return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
}


template<class T, int velicina>
class FITArray {
private:
	T _niz[velicina];
	int _trenutno;
	bool _dozvoliDupliciranje;

public:
	FITArray(bool dozvoliDupliciranje = true) {
		_dozvoliDupliciranje = dozvoliDupliciranje;
		_trenutno = 0;
	}
	FITArray(const FITArray&);
	~FITArray();

	FITArray& operator=(const FITArray&);
	FITArray operator()(int, int);
	bool operator+=(const T&);
	bool operator-=(int);
	T operator[](int)const;
	bool postoji(const T&);
	bool getDupliciranje()const;

	int GetTrenutno()const { return _trenutno; }
	T * GetNiz() { return _niz; }
	
	friend ostream& operator<< <>(ostream&,const FITArray&);
	friend bool operator==(const FITArray&, const FITArray&);
};
template<class T,int v>
FITArray<T,v>::FITArray(const FITArray<T,v>& fa):_trenutno(fa._trenutno),_dozvoliDupliciranje(fa._dozvoliDupliciranje){
	if (_trenutno > 0)
		for (int i = 0; i < _trenutno; i++)
			_niz[i] = fa._niz[i];
}
template<class T, int v>
FITArray<T, v>::~FITArray() {}

template<class T, int v>
FITArray<T, v>& FITArray<T, v>::operator=(const FITArray<T, v>& fa) {
	if (this == &fa)
		return *this;
	_trenutno = fa._trenutno;
	_dozvoliDupliciranje = fa._dozvoliDupliciranje;
	if (_trenutno > 0)
		for (int i = 0; i < _trenutno; i++)
			_niz[i] = fa._niz[i];
	return *this;
}

template<class T, int v>
bool FITArray<T, v>::operator+=(const T& t) {
	if (_trenutno == v) {
		cout << "Niz pun..\n";
		return false;
	}
	if (_trenutno > 0)
		for (int i = 0; i < _trenutno; i++)
			if (t == _niz[i])
				return false;
	_niz[_trenutno++] = t;
	return true;
}


template<class T, int v>
T FITArray<T, v>::operator[](int index)const {
	return _niz[index];
}

template<class T, int v>
bool FITArray<T, v>::getDupliciranje()const { return _dozvoliDupliciranje; }

template<class T,int v>
ostream& operator<< <>(ostream& COUT,const FITArray<T, v>& f1) {
	cout << "\nTrenutno: " << f1._trenutno << endl;
	if (f1._trenutno > 0)
		for (int i = 0; i < f1._trenutno; i++)
			cout << i + 1 << ". -> " << f1._niz[i] << endl;
	else
		cout << "-- PRAZNO --\n";
	return COUT;
}

template<class T, int v>
bool operator==(const FITArray<T, v>& fa1, const FITArray<T, v>& fa2) {
	if (fa1._trenutno == fa2._trenutno && fa1._dozvoliDupliciranje == fa2._dozvoliDupliciranje) {
		if (fa1._trenutno > 0)
			for (int i = 0; i < fa1._trenutno; i++)
				if (!(fa1._niz[i] == fa2._niz[i]))
					return false;
		return true;
	}
	return false;
}
template<class T, int v>
bool FITArray<T, v>::operator-=(int value) {
	if ( _trenutno<=0 || value<0 || value>_trenutno)
		return false;
	int t = -1;
	for (int i = 0; i < _trenutno; i++)
		if (_niz[i] == _niz[value])
			t = i;
	if (t == -1)
		return false;
	for (int i = t; i < _trenutno - 1; i++)
		_niz[i] = _niz[i + 1];
	_trenutno--;
	return true;
}

template<class T,int v>
FITArray<T, v> FITArray<T, v>::operator()(int a, int b) {
	
	FITArray<T, v> novi;
	if (_trenutno > 0 && a>=0) {
		int granica;
		if (b <= _trenutno)
			granica = b;
		else
			granica = _trenutno;
		for (int i = a; i < granica; i++)
			novi += _niz[i];
	}
	return novi;
}
template<class T, int v>
bool FITArray<T, v>::postoji(const T& t) {
	if (_trenutno <= 0)
		return false;
	for (int i = 0; i < _trenutno; i++)
		if (_niz[i] == t)
			return true;
	return false;
}
class PolozeniPredmet {
private:
	char * _naziv;
	int _ocjena;
	Datum _datumPolaganja;
public:
	PolozeniPredmet():_naziv(nullptr),_ocjena(0){}
	PolozeniPredmet(Datum d,const char *naziv,int ocj):_ocjena(ocj), _naziv(AlocirajNizKaraktera(naziv)),_datumPolaganja(d){}
	PolozeniPredmet(const PolozeniPredmet& p):_naziv(AlocirajNizKaraktera(p._naziv)), _ocjena(p._ocjena), _datumPolaganja(p._datumPolaganja){}
	~PolozeniPredmet() {
		delete[] _naziv;
		_naziv = nullptr;
	}
	PolozeniPredmet& operator=(const PolozeniPredmet& p) {
		if (this == &p)
			return *this;
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(p._naziv);
		_ocjena = p._ocjena;
		_datumPolaganja = p._datumPolaganja;
		return *this;
	}
	int getOcjena()const {
		return _ocjena;
	}
	friend ostream & operator<<(ostream &,const PolozeniPredmet&);
	friend bool operator==(const PolozeniPredmet&, const PolozeniPredmet&);
};
ostream& operator<<(ostream& COUT,const PolozeniPredmet& pp) {
	if (pp._naziv == nullptr)
		return COUT;
	cout << "\nNaziv: " << pp._naziv << endl;
	cout << "Ocjena: " << pp._ocjena << endl;
	cout << "Datum: " << pp._datumPolaganja << endl;
	return COUT;
}
bool operator==(const PolozeniPredmet& pp1, const PolozeniPredmet& pp2) {
	return (strcmp(pp1._naziv, pp2._naziv) == 0 &&( pp1._ocjena == pp2._ocjena) &&( pp1._datumPolaganja == pp2._datumPolaganja));
}

class Student {
	static int _indeksCounter; //sluzi za odredjivanje narednog broja indeksa koji ce biti dodijeljen studentu, pocevsi od 160000 
	const int _indeks;
	char * _imePrezime;
	FITArray<PolozeniPredmet, 40> _polozeniPredmeti;

public:
	Student():_indeks(++_indeksCounter),_imePrezime(nullptr){}
	Student(const char *imePrezime):_indeks(++_indeksCounter),_imePrezime(AlocirajNizKaraktera(imePrezime)){}
	Student(const Student& s) :_indeks(s._indeks), _imePrezime(AlocirajNizKaraktera(s._imePrezime)) {
		_polozeniPredmeti = s._polozeniPredmeti;
	}
	~Student() {
		delete[] _imePrezime;
		_imePrezime = nullptr;
	}
	void DodajPredmet(const PolozeniPredmet& p) {
		if (!_polozeniPredmeti.postoji(p))
			_polozeniPredmeti += p;
	}
	Student& operator=(const Student& s) {
		if (&s == this)
			return *this;
		_polozeniPredmeti = s._polozeniPredmeti;
		return *this;
	}
	double GetProsjek()const {
		if (_polozeniPredmeti.GetTrenutno() <= 0)
			return 0;
		double ocj = 0;
		for (int i = 0; i < _polozeniPredmeti.GetTrenutno(); i++)
			ocj += _polozeniPredmeti[i].getOcjena();
		return ocj / _polozeniPredmeti.GetTrenutno();
	}
	friend ostream& operator<<(ostream&,const Student&);
	//friend bool operator==(const Student&, const Student&);
};
ostream& operator<<(ostream& COUT,const Student& s) {
	if (s._imePrezime == nullptr)
		return COUT;
	cout << "Ime i prezime: " << s._imePrezime << endl;
	cout << "Broj indeksa: " << s._indeks << endl;
	cout << "--Lista polozenih predmeta--\n";
	if (s._polozeniPredmeti.GetTrenutno() > 0)
		for (int i = 0; i < s._polozeniPredmeti.GetTrenutno(); i++)
			cout << i + 1 << ". -> " << s._polozeniPredmeti[i] << endl;
	else
		cout << "--NEMA POLOZENIH PREDMETA--\n";
	return COUT;
}
/*bool operator==(const Student& s1, const Student& s2) {
	return (strcmp(s1._imePrezime, s2._imePrezime) == 0 && (s1._indeks == s2._indeks) && (s1._polozeniPredmeti == s2._polozeniPredmeti));
}*/
int Student::_indeksCounter = 160000;
void main() {

	const int max = 20;
	Datum jucer(20, 11, 2017), danas(21, 11, 2017), sutra(22, 11, 2017);

	cout << jucer << danas << sutra << endl;
	jucer = danas;
	cout << jucer << danas << sutra << endl;

	FITArray<int, max> nizIntegera(false);//parametar tipa bool odredjuje da li je u nizu dozvoljeno dupliciranje elemenata 

	for (size_t i = 0; i < max - 1; i++)
		nizIntegera += i;//dodaje novog clana niza 

	if (!(nizIntegera += 6))//pokusavamo dodati dupli clan niza

		cout << "Element nije dodan u niz" << endl;

	cout << crt << nizIntegera << crt; //ispisuje sve clanove niza 

	nizIntegera -= 9; //uklanja clan niza sa vrijednoscu 9 
	nizIntegera -= 17;

	FITArray<int, max> noviNizIntegera(nizIntegera);

	cout << crt << noviNizIntegera << crt;

	/*parametri odredjuju lokacije (indekse u nizu) elemenata OD - DO koje je potrebno vratiti.
 u slucaju da u nizu ne postoji trazeni broj elemenata funkcija treba da vrati sve element
od lokacije OD pa do posljednje dostupnog elementa */

	cout << "Clanovi niza od lokacije 2 do lokacije 5 su -> " << noviNizIntegera(2, 5) << crt;

	Student adel("Adel Handzic"), jasmin("Jasmin Azemovic");

	PolozeniPredmet prII(jucer, "PRII", 8), prIII(sutra, "PRIII", 7);

	adel.DodajPredmet(prII);
	adel.DodajPredmet(prIII);
	cout << adel << endl;  //vraca prosjecnu ocjenu studenta  

	cout << "Prosjecna ocjena -> " << adel.GetProsjek() << crt;

	jasmin = adel;
	cout << jasmin << endl;
	cout << "Prosjecna ocjena -> " << jasmin.GetProsjek() << crt;
}
