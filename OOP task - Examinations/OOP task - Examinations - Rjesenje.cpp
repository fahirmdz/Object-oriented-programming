//fahirmdz GitHub


#include "pch.h"
#include<iostream>
#include<cstdlib>
#include<cstring>

using namespace std;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
private:
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;

public:
	Kolekcija():_elementi1(nullptr),_elementi2(nullptr),_trenutnoElemenata(0){}
	Kolekcija(T1* t1, T2 *t2, int brE) : _trenutnoElemenata(brE) {
		if (_trenutnoElemenata > 0 && t1 != nullptr) {
			_elementi2 = new T2[_trenutnoElemenata];
			_elementi1 = new T1[_trenutnoElemenata];
			for (int i = 0; i < _trenutnoElemenata; i++) {
				_elementi1[i] = t1[i];
				_elementi2[i] = t2[i];
			}
		}
	}
	Kolekcija(const Kolekcija& k):_elementi2(new T2(*k._elementi2)),_trenutnoElemenata(k._trenutnoElemenata) {
		if (_trenutnoElemenata > 0 && k._elementi1 != nullptr) {
			_elementi2 = new T2[_trenutnoElemenata];
			_elementi1 = new T1[_trenutnoElemenata];
			for (int i = 0; i < _trenutnoElemenata; i++) {
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
	}
	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		delete[] _elementi1;
		delete[] _elementi2;
		_trenutnoElemenata = k._trenutnoElemenata;
		if (_trenutnoElemenata > 0 && k._elementi2 != nullptr) {
			_elementi2 = new T2[_trenutnoElemenata];
			for (int i = 0; i < _trenutnoElemenata; i++)
				_elementi2[i] = k._elementi2[i];
		}
		else _elementi2 = nullptr;

		if (_trenutnoElemenata > 0 && k._elementi1 != nullptr) {
			_elementi1 = new T1[_trenutnoElemenata];
			for (int i = 0; i < _trenutnoElemenata; i++)
				_elementi1[i] = k._elementi1[i];
		}
		else _elementi1 = nullptr;
		return *this;
	}
	T1 operator[](int index) {
		return _elementi1[index];
	}
	bool valid()const { return _elementi1 != nullptr && _elementi2 != nullptr; }
	void AddElement(T1 t1,T2 t2) {
		T1 *temp = new T1[_trenutnoElemenata + 1];
		T2 *temp2 = new T2[_trenutnoElemenata + 1];
		if(_trenutnoElemenata>0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				temp[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp;
		_elementi2 = temp2;
		_elementi1[_trenutnoElemenata] = t1;
		_elementi2[_trenutnoElemenata++] = t2;
	}
	void RemoveElement(int index) {
		if (index<0 || index>_trenutnoElemenata || _trenutnoElemenata <= 0)
			return;
		for (int i = index; i < _trenutnoElemenata - 1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutnoElemenata--;
	}
	bool postoji(const T1& t)const {
		if (_trenutnoElemenata <= 0)
			return false;
		for (int i = 0; i < _trenutnoElemenata; i++)
			if (_elementi1[i] == t)
				return true;
		return false;
	}
	void sortPoEl2() {
		if (_trenutnoElemenata <= 0)
			return;
		for(int i=0;i<_trenutnoElemenata-1;i++)
			for (int j = i + 1; j < _trenutnoElemenata; j++) {
				if (_elementi2[i] > _elementi2[j])
				{
					T1 temp;
					T2 temp2;
					temp = _elementi1[i];
					temp2 = _elementi2[i];
					_elementi1[i] = _elementi1[j];
					_elementi2[i] = _elementi2[j];
					_elementi2[j] = temp2;
					_elementi1[j] = temp;
				}
			}
	}
	int getTrenutno()const { return _trenutnoElemenata; }
	friend ostream& operator<< <>(ostream&, const Kolekcija<T1, T2>&);
	friend bool operator== <>(Kolekcija<T1, T2>&, Kolekcija<T1, T2>&);
};

template<class T1,class T2>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2>& k) {
	if (!k.valid())
		return COUT;
	if (k._trenutnoElemenata > 0)
		for (int i = 0; i < k._trenutnoElemenata; i++)
			cout << i + 1 << ". -> " << k._elementi1[i] <<" - " <<k._elementi2[i]<< endl;
	return COUT;
}
template<class T1, class T2>
bool operator==(Kolekcija<T1, T2>& k1, Kolekcija<T1, T2>& k2) {
	if (k1._trenutnoElemenata == k2._trenutnoElemenata && *k1._elementi2 == *k2._elementi2) {
		if (k1._trenutnoElemenata > 0)
			for (int i = 0; i < k1._trenutnoElemenata; i++)
				if (!(k1._elementi1[i] == k2._elementi1[i] && k1._elementi2[i] == k2._elementi2[i]))
					return false;
		return true;
	}
	return false;
}

class Datum {
	int *_dan, *_mjesec, *_godina;

public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	Datum& operator=(const Datum& d) {
		delete[] _dan;
		delete[] _mjesec;
		delete[] _godina;
		_dan = _mjesec = _godina = nullptr;
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
		return *this;
	}
	bool valid()const { return (*_dan > 0 && *_dan <= 31) && (*_mjesec > 0 && *_mjesec <= 12) && (*_godina >= 1950 && *_godina <= 2018); }
	friend ostream& operator<< (ostream &,const Datum&);
	friend bool operator==(Datum&, Datum&);
};
ostream& operator<<(ostream& COUT,const Datum& d) {
	if (d.valid())
		cout << *d._dan << " " << *d._mjesec << " " << *d._godina << endl;
	else
		cout << "--POGRESAN FORMAT--\n";
	return COUT;
}
bool operator==(Datum& d1, Datum& d2) {
	return (*d1._dan == *d2._dan) && (*d1._mjesec == *d2._mjesec) && (*d1._godina == *d2._godina);
}

class Student {
private:
	char * _imePrezime;
	Datum * _datumRodjenja;

public:

	Student(const char * imePrezime = "---", Datum d = Datum()) {
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_datumRodjenja = new Datum(d);
	}
	Student(const Student& s) {
		_imePrezime = AlocirajNizKaraktera(s._imePrezime);
		_datumRodjenja = new Datum(*s._datumRodjenja);
	}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	
	Student& operator=(const Student& s) {
		if (&s == this)
			return *this;
		delete[] _imePrezime; 
		delete _datumRodjenja; 
		_imePrezime = AlocirajNizKaraktera(s._imePrezime);
		_datumRodjenja = new Datum(*s._datumRodjenja);
		return *this;
	}
	char *getImePrezime()const { return AlocirajNizKaraktera(_imePrezime); }
	Datum getDatum()const { return *_datumRodjenja; }

	friend ostream& operator<<(ostream&,const Student&);
	friend bool operator==(const Student&,const Student&);
};

ostream& operator<<(ostream& COUT, const Student& s) {
	cout << "Ime i prezime: " << s._imePrezime << " - Datum rodjenja: " << *s._datumRodjenja << endl;
	return COUT;
}
bool operator==(const Student& s1, const Student& s2) {
	return ((strcmp(s1._imePrezime, s2._imePrezime) == 0) && (*s1._datumRodjenja == *s2._datumRodjenja));
}

class Ispit {
private:
	Datum _datumOdrzavanja;
	char * _opisIspita;
	Kolekcija<Student, Datum> _prijave;
	Kolekcija<Student, int> _rezultati;

public:
	Ispit():_opisIspita(nullptr){}
	Ispit(const char* opis,Datum d):_opisIspita(AlocirajNizKaraktera(opis)),_datumOdrzavanja(d){}
	Ispit(const Ispit& isp):_datumOdrzavanja(isp._datumOdrzavanja),_opisIspita(AlocirajNizKaraktera(isp._opisIspita)),
		_prijave(isp._prijave),_rezultati(isp._rezultati) {	}

	Ispit& operator=(const Ispit& isp) {
		delete[] _opisIspita;
		_opisIspita = AlocirajNizKaraktera(isp._opisIspita);
		_datumOdrzavanja = isp._datumOdrzavanja;
		_prijave = isp._prijave;
		_rezultati = isp._rezultati;
	}

	bool DodajPrijavu(const Student& s,const Datum& d) {
		if (_prijave.postoji(s))
			return false;
		_prijave.AddElement(s, d);
		return true;
	}
	bool DodajRezultat(const Student& s,int rez) {
		if (_prijave.postoji(s)) {
			_rezultati.AddElement(s, rez);
			return true;
		}
		return false;
	}
	template<class T>
	void swap(T& t1, T& t2) {
		T temp = t1;
		t1 = t2;
		t1 = temp;
	}
	void Sortiraj() {	_rezultati.sortPoEl2();}
	void ZakljuciRezultate() {
		if (_prijave.getTrenutno() <= 0 || _rezultati.getTrenutno() <= 0)
			return;
		for (int i = 0; i < _prijave.getTrenutno(); i++)
			if (!_rezultati.postoji(_prijave[i]))
				DodajRezultat(_prijave[i], 5);
	}
	friend ostream& operator<<(ostream&, const Ispit&);
	friend bool operator==(Ispit&, Ispit&);
};

ostream& operator<<(ostream& COUT, const Ispit& isp) {
	cout << "Datum odrzavanja: " << isp._datumOdrzavanja << endl;
	cout << "Opis ispita: ";
	if(isp._opisIspita!=nullptr)
	cout << isp._opisIspita << endl;
	else cout << "--NEMA--\n";
	cout << "--Lista prijava--\n" << isp._prijave << endl << endl;
	cout << "--Lista rezultata--\n" << isp._rezultati << endl << endl;
	return COUT;
}
bool operator==(Ispit& isp1, Ispit& isp2) {
	return (isp1._datumOdrzavanja == isp2._datumOdrzavanja && strcmp(isp1._opisIspita, isp2._opisIspita) == 0 && isp1._prijave == isp2._prijave && isp1._rezultati == isp2._rezultati);
}

const char *crt = "\n--------------------------------------------------\n";

void main() { 

#pragma region TestiranjeKolekcije
	Kolekcija<int, double> kolekcija1;
	for (size_t i = 0; i < 50; i++)
		kolekcija1.AddElement(i, i + 0.3);
	cout << kolekcija1 << endl;
	cout << crt;

	Kolekcija<int, double> kolekcija2;
	kolekcija2 = kolekcija1;
	kolekcija1.RemoveElement(2);//UMANJITI VELICINU NIZA
	cout << kolekcija1 << crt;
	if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, double> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion 

#pragma region TestiranjeDatuma

	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;

#pragma endregion 

#pragma region TestiranjeStudenta 

	Student denis("Denis Music", Datum(26, 11, 1990));
	Student jasmin("Jasmin Azemovic", Datum(22, 11, 1990));
	Student goran;  goran = jasmin;
	Student adel("Adel Handzic", Datum(25, 8, 1990));
	Student marija("Marija Herceg", Datum(15, 6, 1990));

#pragma endregion 

#pragma region TestiranjeIspita 

	Ispit prIII("PRIII::26.11.2015", danas);
	//DodajPrijavu - dodaje novog studenta u listu prijavljenih za ispit. onemoguciti dodavanje dva ista studenta 

	if (prIII.DodajPrijavu(denis, danas))
		cout << denis << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(jasmin, danas))
		cout << jasmin << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(goran, danas))
		cout << goran << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(adel, danas))
		cout << adel << " DODAT na spisak" << endl;

	if (prIII.DodajPrijavu(marija, danas))
		cout << marija << " DODAT na spisak" << endl;

	cout << crt << endl;

	/*DodajRezultat - dodaje ocjenu koju je student ostvario na ispitu. za dodavanje ocjene student je prethodno trebao
	biti prijavljen na ispit */

	prIII.DodajRezultat(goran, 7);
	prIII.DodajRezultat(jasmin, 6);
	prIII.DodajRezultat(adel, 8);
	prIII.DodajRezultat(marija, 9);

	/*Sortiraj - sortira rezultate ispita prema ocjeni*/

	prIII.Sortiraj();
	cout << prIII << endl;
	//ZakljuciRezultate - dodaje ocjenu 5 svim studentima koji su prijavili ispit a nije im evidentirana ocjena

	prIII.ZakljuciRezultate();

	//rezultati ispita se smiju ispisivati jedino ukoliko su prethodno zakljuceni 

	cout << prIII << crt;

	Ispit prIII_2(prIII);
	cout << prIII_2 << crt;
	
#pragma endregion 
}