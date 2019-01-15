//fahirmdz GitHub

#include"pch.h"
#include <iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<regex>
#include<exception>
#include<algorithm>
#include<windows.h>
#include<thread>
using namespace std;

const char *crt = "\n-------------------------------------------\n";
enum OznakaKrvneGrupe { O, A, B, AB };
enum Sortiranje { ASC, DESC };
const char* OznakaKrvneGrupeString[] = { "0", "A", "B", "AB" };
const char *oznake[] = { "0+","0-","A+","A-","B+","B-","AB+","AB-" };

class NedozvoljenaOperacija :public exception {
	int codeLine;
public:
	NedozvoljenaOperacija(const char* msg, int cl) :exception(msg), codeLine(cl) {}

	const char* what()const {
		string obj = (string)exception::what() + " : Line: " + to_string(codeLine);
		char* temp = new char[100];
		strcpy_s(temp, 100, obj.c_str());
		return temp;
	}

};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
class Datum
{
	int * _dan, *_mjesec, *_godina;
public:
	Datum() :_dan(new int(1)), _mjesec(new int(1)), _godina(new int(2000)) {}
	Datum(int dan, int mjesec, int godina)
	{
		_dan = new int(dan), _mjesec = new int(mjesec), _godina = new int(godina);
	}
	Datum(const Datum& d) :_dan(new int(*d._dan)), _mjesec(new int(*d._mjesec)), _godina(new int(*d._godina)) {}
	~Datum() { delete _dan; delete _mjesec; delete _godina; _dan = _mjesec = _godina = nullptr; }

	Datum& operator=(const Datum& d) {
		if (&d == this)
			return *this;
		delete _dan;
		_dan = new int(*d._dan);
		delete _mjesec;
		_mjesec = new int(*d._mjesec);
		delete _godina;
		_godina = new int(*d._godina);
		return *this;
	}
	int brojDanaIzmedju(Datum* d1) {
		long int d1d = 0, d2d = 0;
		auto isPrestupna = [](int g) {
			return g % 400 == 0 && g % 100 == 0 && g % 4 == 0;
		};
		if (isPrestupna(*d1->_godina))
			d1d += 366;
		else
			d1d += 365;

		if (isPrestupna(*_godina))
			d2d += 366;
		else
			d2d += 365;
		bool d1prestupna = isPrestupna(*d1->_godina), d2prestupna = isPrestupna(*_godina);

		auto brojDana = [](int x,bool prestupna) {
			int brd = 0;
			
			for (int i = 1; i < x; i++)
				if (i == 3 || i == 1 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
					brd += 31;
				else if (i == 2) {
					if (prestupna)
						brd += 29;
					else
						brd += 28;
				}
				else
					brd += 30;
			return brd;
		};
		d1d += brojDana(*d1->_mjesec,d1prestupna);
		d2d += brojDana(*_mjesec, d2prestupna);
		d1d += *d1->_dan;
		d2d += *_dan;
		return d1d - d2d;
	}
	friend ostream &operator<<(ostream&, const Datum&);
};
ostream& operator<<(ostream& COUT, const Datum& obj) {
	COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina << endl;
	return COUT;
}

template <class T1, class T2, int max, Sortiranje sortiranje = ASC>
class Kolekcija
{
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutnoElemenata;
	bool _sortiranje;
	Sortiranje sortNacin;
	void sort() {
		for (int i = 0; i < _trenutnoElemenata - 1; i++)
			for (int j = i + 1; j < _trenutnoElemenata; j++) {
				if (sortNacin==ASC && (*_elementi1[i]) > (*_elementi1[j])) {
					swap(*_elementi1[i], *_elementi1[j]);
					swap(*_elementi2[i], *_elementi2[j]);
				}
				else if (sortNacin == DESC && (*_elementi1[i]) < (*_elementi1[j])) {
					swap(*_elementi1[i], *_elementi1[j]);
					swap(*_elementi2[i], *_elementi2[j]);
				}
			}
	}
	void copyElements(const Kolekcija& k) {

		for (int i = 0; i < k._trenutnoElemenata; i++)
			if (k._elementi1[i] != nullptr && k._elementi2[i] != nullptr) {
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
		for (int i = k._trenutnoElemenata; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
public:
	Kolekcija(bool sort = true)
	{
		sortNacin = sortiranje;
		_trenutnoElemenata = 0;
		_sortiranje = sort;
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& k) :_trenutnoElemenata(k._trenutnoElemenata) {
		copyElements(k);
	}
	~Kolekcija()
	{
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	Kolekcija& operator=(Kolekcija& k) {
		if (&k == this)
			return *this;
		_trenutnoElemenata = k._trenutnoElemenata;
		for (int i = 0; i < max; i++) {
			delete _elementi1[i];
			delete _elementi2[i];
		}
		copyElements(k);
		return *this;
	}
	bool AddElement(T1 ele1, T2 ele2)
	{
		if (_trenutnoElemenata == max)
			return false;
		_elementi1[_trenutnoElemenata] = new T1(ele1);
		_elementi2[_trenutnoElemenata++] = new T2(ele2);
		if (_sortiranje && _trenutnoElemenata > 1)
			sort();
		return true;
	}
	void SetElement2(int lokacija, T2 ele2) {
		if (lokacija<0 || lokacija>max - 1)
			throw NedozvoljenaOperacija("Index nije validan!", __LINE__);
		delete _elementi2[lokacija];
		_elementi2[lokacija] = new T2(ele2);
	}
	void SetSortiranje(Sortiranje sortiranje)
	{
		sortNacin = sortiranje;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw NedozvoljenaOperacija("Nepostojeca lokacija!", __LINE__);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw NedozvoljenaOperacija("Nepostojeca lokacija!", __LINE__);
		return *_elementi2[lokacija];
	}
	friend ostream& operator<< <>(ostream&, const Kolekcija &);
};
template<class T1, class T2, int max, Sortiranje sortiranje = ASC>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1, T2, max, sortiranje>& obj) {
	for (int i = 0; i < obj.GetTrenutno(); i++)
		COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
	return COUT;
}

class KrvnaGrupa {
	/*postoje 4 krvne grupe koje su navedene u enumeracije OznakaKrvneGrupe, pri cemu svaka od navedenih moze imati
	pozitivan (+) i negativan (-) Rh faktor*/
	OznakaKrvneGrupe _oznaka;//npr: AB
	char _rhFaktor; // + ili -
	//prethodno pomenuto je izuzetno bitno iz razloga sto postoje jasna pravila vezana za darivanje krvi tj. 
	//koji primalac moze primiti krv od kojeg donatora sto je prikazano u tabeli, a naredna dva vector-a su zaduzena
	//da cuvaju informacije o tome, npr. za krvnu grupu A+ vector donatori ce cuvati vrijednosti: A+ i AB+ 
	vector<KrvnaGrupa *> _donatori; //krvne grupe kojima odredjena krvna grupa moze donirati krv.
	vector<KrvnaGrupa *> _primaoci; //krvne grupe od kojih odredjena krvna grupa moze primiti krv.
public:
	KrvnaGrupa() {}
	KrvnaGrupa(OznakaKrvneGrupe oznaka, char rhFaktor) {
		if (rhFaktor != '-' && rhFaktor != '+')
			throw NedozvoljenaOperacija("RH faktor nije validan!", __LINE__);
		if (oznaka != A && oznaka != B && oznaka != AB && oznaka != O)
			throw NedozvoljenaOperacija("Oznaka krvne grupe nije validna!", __LINE__);
		_rhFaktor = rhFaktor;
		_oznaka = oznaka;
	}
	KrvnaGrupa(const KrvnaGrupa& kg) :_oznaka(kg._oznaka), _rhFaktor(kg._rhFaktor) {
		for (size_t i = 0; i < kg._donatori.size(); i++)
			_donatori.push_back(new KrvnaGrupa(*kg._donatori[i]));

		for (size_t i = 0; i < kg._primaoci.size(); i++)
			_primaoci.push_back(new KrvnaGrupa(*kg._primaoci[i]));
	}
	~KrvnaGrupa() {
		for (size_t i = 0; i < _donatori.size(); i++) {
			delete _donatori[i];
			_donatori[i] = nullptr;
		}
		for (size_t i = 0; i < _primaoci.size(); i++) {
			delete _primaoci[i];
			_primaoci[i] = nullptr;
		}
	}

	KrvnaGrupa& operator=(const KrvnaGrupa& kg) {
		if (&kg == this)
			return *this;
		_oznaka = kg._oznaka;
		_rhFaktor = kg._rhFaktor;
		for (size_t i = 0; i < kg._donatori.size(); i++)
			_donatori.push_back(new KrvnaGrupa(*kg._donatori[i]));

		for (size_t i = 0; i < kg._primaoci.size(); i++)
			_primaoci.push_back(new KrvnaGrupa(*_primaoci[i]));
		return *this;
	}
	OznakaKrvneGrupe GetOznaka()const { return _oznaka; }
	char GetRHFaktor()const { return _rhFaktor; }
	void SetDonatori(vector<KrvnaGrupa *> donatori) {
		for (size_t i = 0; i < donatori.size(); i++)
			_donatori.push_back(new KrvnaGrupa(*donatori[i]));
	}
	
	string ToString() {
		return (string)OznakaKrvneGrupeString[_oznaka] +_rhFaktor;
	}
	void SetPrimaoci(vector<KrvnaGrupa *> primaoci) {
		for (size_t i = 0; i < primaoci.size(); i++)
			_primaoci.push_back(new KrvnaGrupa(*primaoci[i]));
	}
	friend ostream& operator<<(ostream&, KrvnaGrupa &);
	friend bool operator==(KrvnaGrupa&, KrvnaGrupa&);
	friend bool operator<(KrvnaGrupa&, KrvnaGrupa&);
	friend bool operator>(KrvnaGrupa&, KrvnaGrupa&);

};
bool operator<(KrvnaGrupa& k1, KrvnaGrupa& k2) {
	return k1._oznaka < k2._oznaka;
}
bool operator>(KrvnaGrupa& k1, KrvnaGrupa& k2) {
	return k1._oznaka > k2._oznaka;
}

bool operator==(KrvnaGrupa& k1, KrvnaGrupa& k2) {
	return k1._oznaka == k2._oznaka && k1._rhFaktor == k2._rhFaktor;
}
ostream& operator<<(ostream& COUT, KrvnaGrupa& kg) {
	cout << "Krvna grupa: " << kg._oznaka << endl;
	cout << "RH faktor: " << kg._rhFaktor << endl;
	cout << "--DONATORI--\n";
	for (size_t i = 0; i < kg._donatori.size(); i++)
		cout << i + 1 << ". " << *kg._donatori[i] << endl;
	cout << "--PRIMAOCI--\n";
	for (size_t i = 0; i < kg._primaoci.size(); i++)
		cout << i + 1 << ". " << *kg._primaoci[i] << endl;
	return COUT;
}


class Osoba {
protected:
	char* _imePrezime;
	KrvnaGrupa* _krvnaGrupa;
public:
	Osoba() :_imePrezime(nullptr), _krvnaGrupa(nullptr) {}
	Osoba(const char* imePrezime, KrvnaGrupa *krvnaGrupa) {
		if (imePrezime == nullptr || !regex_search(string(imePrezime), regex(string("[a-zA-Z\\s]{4,40}"))) || krvnaGrupa == nullptr)
			throw NedozvoljenaOperacija("Parametri nisu validni!", __LINE__);
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_krvnaGrupa = new KrvnaGrupa(*krvnaGrupa);
	}
	Osoba(const Osoba& o) :_imePrezime(AlocirajNizKaraktera(o._imePrezime)), _krvnaGrupa(new KrvnaGrupa(*o._krvnaGrupa)) {}
	~Osoba() {
		delete[] _imePrezime; _imePrezime = nullptr;
		delete _krvnaGrupa; _krvnaGrupa = nullptr;
	}

	Osoba& operator=(const Osoba& o) {
		if (&o == this)
			return *this;
		delete[] _imePrezime;
		_imePrezime = AlocirajNizKaraktera(o._imePrezime);
		delete _krvnaGrupa;
		_krvnaGrupa = new KrvnaGrupa(*o._krvnaGrupa);
		return *this;

	}

	char* GetImePrezime()const { return _imePrezime; }
	KrvnaGrupa& GetKrvnaGrupa()const { return *_krvnaGrupa; }

	virtual void Info() = 0;
};

class Donator : public Osoba
{
	//u slucaju da email nije u validnom formatu potrebno ga je postaviti na podrazumijevanu vrijednost: info@donatori.ba
	string _email; //regex: nekiTekst@nekaDomena.com ili .ba ili .org
	Datum _datumPosljednjegDoniranja;
	bool _kontaktirajMe;
	bool ValidanFormat(string x) {
		return regex_match(x, regex("[a-zA-Z]{4,20}\\@[a-z]{3,15}\\.(com|ba|rs)"));
	}
public:
	Donator() :_email("info@donatori.ba") {}
	Donator(const char* imePrezime, KrvnaGrupa *krvnaGrupa, string email, Datum dpd, bool contact = true)
		: Osoba(imePrezime, krvnaGrupa) {
		if (ValidanFormat(email))
			_email = email;
		else
			_email = "info@donatori.ba";
		_datumPosljednjegDoniranja = dpd;
		_kontaktirajMe = contact;
	}

	string GetEmail()const { return _email; }
	Datum GetDatumPoslednjegDoniranja()const { return _datumPosljednjegDoniranja; }
	bool GetKontaktirajMe()const { return _kontaktirajMe; }

	void SetNoviDatumPoslednjegDoniranja(Datum d) { _datumPosljednjegDoniranja = d; }
	void Info() {
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << *_krvnaGrupa << endl;
		cout << "E-mail: " << _email << endl;
		cout << "Datum poslednjeg doniranja: ";
		cout << _datumPosljednjegDoniranja << endl;
	}
};

class Zahtjev {
	string _ustanova;
	KrvnaGrupa *_krvnaGrupa;
	double _kolicina;
public:
	Zahtjev() :_krvnaGrupa(nullptr), _ustanova(""), _kolicina(0) {}
	Zahtjev(string ustanova, KrvnaGrupa *krvnaGrupa, double kolicina) {
		if (!regex_search((ustanova), regex("^[a-zA-Z0-9\\s]{5,40}$")) || krvnaGrupa == nullptr || kolicina <= 0)
			throw NedozvoljenaOperacija("Parametrti za zahtjev nisu validni!", __LINE__);
		_kolicina = kolicina;
		_krvnaGrupa = new KrvnaGrupa(*krvnaGrupa);
		_ustanova = ustanova;
	}
	Zahtjev(const Zahtjev& zh) :_ustanova(zh._ustanova), _krvnaGrupa(new KrvnaGrupa(*zh._krvnaGrupa)), _kolicina(zh._kolicina) {}
	~Zahtjev() {
		delete _krvnaGrupa; _krvnaGrupa = nullptr;
	}

	Zahtjev& operator=(const Zahtjev& zh) {
		if (&zh == this)
			return *this;

		delete _krvnaGrupa;
		_krvnaGrupa = new KrvnaGrupa(*zh._krvnaGrupa);
		_ustanova = zh._ustanova;
		_kolicina = zh._kolicina;
		return *this;
	}
	friend ostream& operator<<(ostream&, const Zahtjev&);
	friend bool operator==(Zahtjev&, Zahtjev&);
};
bool operator==(Zahtjev& z1, Zahtjev& z2) {
	return z1._ustanova == z2._ustanova && *z1._krvnaGrupa == *z2._krvnaGrupa && z1._kolicina == z2._kolicina;
}
ostream& operator<<(ostream& COUT, const Zahtjev& obj)
{
	COUT << "Ustanova: " << obj._ustanova << endl;
	COUT << "Krvna grupa: " << obj._krvnaGrupa->ToString() << endl;
	COUT << "Kolicina: " << obj._kolicina << endl;
	return COUT;
}

class TransfuzijskiCentar {
	Kolekcija<KrvnaGrupa, double, 8, DESC> _zalihe;  //stanje zaliha za svaku pojedinu krvnu grupu
	Kolekcija<Osoba *, double, 100> _donacije;  	//evidentira svaku donaciju krvi
	vector<Zahtjev> _zahtjevi;

	vector<int> kompatibilneGrupe(const char* oznaka) {
		vector<int> *kompatibilne[]{ new vector<int>{0,1},new vector<int>{1},new vector<int>{2,3,0,1},new vector<int>{3,1},
			new vector<int>{4,5,0,1},new vector<int>{5,1},new vector<int>{0,1,2,3,4,5,6,7},new vector<int>{7,3,5,1} };

		int indexOznake = -1;
		for (int i = 0; i < 8; i++)
			if (strcmp(oznaka, oznake[i]) == 0)   //OZNAKE DODATE KAO GLOBALNI CONST CHAR*[] kao (oznaka+rhFaktor)
				indexOznake = i;
		if (indexOznake == -1)
			throw NedozvoljenaOperacija("Oznaka nije validna!", __LINE__);
		vector<int> k = *kompatibilne[indexOznake];

		for (int i = 0; i < 8; i++) {
			delete kompatibilne[i];
			kompatibilne[i] = nullptr;
		}
		return k;
	}
	void kontaktirajDonatorEmail(Donator* d) {
		cout << "Donator " << d->GetImePrezime() << " kontaktiran putem e-maila: " << d->GetEmail() << " i ocekuje se njegov odziv..\n";
	}
public:
	TransfuzijskiCentar() :_donacije(false) {}
	TransfuzijskiCentar(const TransfuzijskiCentar& tc) :_donacije(false) {
		for (int i = 0; i < tc._zalihe.GetTrenutno(); i++)
			_zalihe.AddElement(tc._zalihe.GetElement1(i), tc._zalihe.GetElement2(i));
		for (int i = 0; i < tc._donacije.GetTrenutno(); i++)
			_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(tc._donacije.GetElement1(i))), tc._donacije.GetElement2(i));
		for (size_t i = 0; i < tc._zahtjevi.size(); i++)
			_zahtjevi.push_back(tc._zahtjevi[i]);
	}
	~TransfuzijskiCentar() {
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			delete _donacije.GetElement1(i);
			_donacije.GetElement1(i) = nullptr;
		}

	};

	TransfuzijskiCentar& operator=(const TransfuzijskiCentar& tc) {
		if (&tc == this)
			return *this;

		for (int i = 0; i < tc._zalihe.GetTrenutno(); i++)
			_zalihe.AddElement(tc._zalihe.GetElement1(i), tc._zalihe.GetElement2(i));
		for (int i = 0; i < tc._donacije.GetTrenutno(); i++) {
			delete _donacije.GetElement1(i);
			_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(tc._donacije.GetElement1(i))), tc._donacije.GetElement2(i));
		}
		_zahtjevi.clear();
		for (size_t i = 0; i < tc._zahtjevi.size(); i++)
			_zahtjevi.push_back(tc._zahtjevi[i]);
		return *this;
	}


	// prilikom svake donacije je potrebno povecati zalihe za tu krvnu grupu
		// ukoliko krvna grupa vec postoji na zalihi onda se uvecava samo njena kolicina
		// takodjer, donatoru se postavlja nova vrijednost datuma posljednje donacije.
		// ukoliko je neki od dodantora darivao krv prije isteka 3 mjeseca od posljednjeg
		// darivanja, funkcija baca izuzetak
		//datum donacije, donator, kolicina
	void AddDonaciju(Datum date, Osoba* o, double kolicina) {
		if (kolicina <= 0)
			return;
		bool flag = false;
		int indexDonatora = 0;
		//-->Slucaj 1: Krvna grupa vec postoji na zalihi i uvecava se samo njena kolicina, uz uslov da je proslo 3 mjeseca
		for (int i = 0; i < _donacije.GetTrenutno(); i++)
			if (strcmp(_donacije.GetElement1(i)->GetImePrezime(), o->GetImePrezime()) == 0) {
				Donator* donator = dynamic_cast<Donator*>(_donacije.GetElement1(i));
				int raz = donator->GetDatumPoslednjegDoniranja().brojDanaIzmedju(&date);
				if (donator->GetDatumPoslednjegDoniranja().brojDanaIzmedju(&date) < 92)
					throw NedozvoljenaOperacija("Ne mozete donirati krv, jer nije prosao period od 3 mjesec!", __LINE__);
				_donacije.SetElement2(i, _donacije.GetElement2(i) + kolicina);
				flag = true;
				indexDonatora = i;
			}
		if (!flag) {
			_donacije.AddElement(new Donator(*dynamic_cast<Donator*>(o)), kolicina);
			indexDonatora = _donacije.GetTrenutno() - 1;
		}
		dynamic_cast<Donator*>(_donacije.GetElement1(indexDonatora))->SetNoviDatumPoslednjegDoniranja(date);
		int pozicijaGrupe = -1;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++)
			if (_zalihe.GetElement1(i).GetOznaka() == o->GetKrvnaGrupa().GetOznaka() && _zalihe.GetElement1(i).GetRHFaktor() == o->GetKrvnaGrupa().GetRHFaktor())
				pozicijaGrupe = i;
		if (pozicijaGrupe == -1)
			_zalihe.AddElement(o->GetKrvnaGrupa(), kolicina);
		else
			_zalihe.SetElement2(pozicijaGrupe, _zalihe.GetElement2(pozicijaGrupe) + kolicina);
	}


	double ProvjeriStanje(const char *oznaka) {
		if (oznaka == nullptr || strlen(oznaka) > 3 || strlen(oznaka) < 2)
			return false;
		
		vector<int> kompatibilne = kompatibilneGrupe(oznaka);

		double stanjeSum = 0;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++) {
			KrvnaGrupa& kg = _zalihe.GetElement1(i);
			string oznakaTemp = kg.ToString();

			for (size_t j = 0; j < kompatibilne.size(); j++) {
				if (oznakaTemp == oznake[kompatibilne[j]])
					stanjeSum += _zalihe.GetElement2(i);
			}
		}
		
		return stanjeSum;  //VRACA STANJE SUME KOLICINE ZALIHA ZA SVE ZAMJENSKE KRVNE GRUPE
	}

	// ukoliko transfuzijski centar posjeduje zahtijevane kolicine na stanju, odmah ce ih ustupiti prema zahtjevu,
		// a u slucaju da ne posjeduje onda ce (koristeci multithreading) kontaktirati (poslati email poruku sa
		// odgovarajucim sadrzajem) sve donore koji zadovoljavaju sljedece uslove:
		// - mogu donirati krv zahtijevanoj krvnoj grupi
		// - oznacili su da zele biti kontaktirani
		// - nisu davali krv u posljednja 3 mjeseca
	bool KontaktirajDonora(const char* oznaka) {
		SYSTEMTIME st;
		GetSystemTime(&st);
		Datum dateTemp(st.wDay, st.wMonth, st.wYear); //TRENUTNI DATUM (SISTEMSKI)

		vector<int> kompatibilne = kompatibilneGrupe(oznaka);
		int brojD = 0;
		for (int i = 0; i < _donacije.GetTrenutno(); i++) {
			Donator* donator = dynamic_cast<Donator*>(_donacije.GetElement1(i));
			int raz = donator->GetDatumPoslednjegDoniranja().brojDanaIzmedju(&dateTemp);
			if(donator->GetDatumPoslednjegDoniranja().brojDanaIzmedju(&dateTemp) < -92 && donator->GetKontaktirajMe())
				for (size_t j = 0; j < kompatibilne.size(); j++) {
					if (donator->GetKrvnaGrupa().ToString()==oznake[kompatibilne[j]]) {
						thread t(&TransfuzijskiCentar::kontaktirajDonatorEmail, this, donator);
						t.join();
						brojD++;
						break;
					}
				}
			donator = nullptr;
		}
		if (brojD == 0)
			return false;
		else
			return true;

	}
	void DodajZahtjev(Zahtjev& z) {
		for (size_t i = 0; i < _zahtjevi.size(); i++)
			if (_zahtjevi[i] == z)
				throw NedozvoljenaOperacija("Ne mozete dodati isti zahtjev..\n", __LINE__);
		_zahtjevi.push_back(z);
	}
	void IspisiMatricuKompatibilnosti(vector<KrvnaGrupa*> krvneGrupe) {
		int vel = krvneGrupe.size();
		bool **matrixCompatibility = new bool*[vel];
		for (size_t i = 0; i < krvneGrupe.size(); i++)
			matrixCompatibility[i] = new bool[vel]{ false };
		for (size_t i = 0; i < krvneGrupe.size(); i++) {
			vector<int> temp = kompatibilneGrupe(krvneGrupe[i]->ToString().c_str());
			for (size_t j = 0; j < temp.size(); j++)
				matrixCompatibility[i][temp[j]] = true;

		}
		cout << "\n----------------------------------------------------------------------\n";
		cout<<"\t   ::::TABLICA KOMPATIBILNOSTI KRVNIH GRUPA:::::\n----------------------------------------------------------------------\n";
		cout << "  GRUPA |  ";
		for (int i = 0; i < vel; i++)
			cout << krvneGrupe[i]->ToString() << "\t ";
		cout << "\n--------------------------------------------------------------------\n";

		for (int i = 0; i < vel; i++) {
			cout << "   "<<krvneGrupe[i]->ToString();
			if (i < vel - 2)
				cout << "   | ";
			else
				cout << "  | ";
			for (int j = 0; j < vel; j++) {
				if (matrixCompatibility[i][j])
					cout << " DA \t";
				else
					cout << " NE \t";
			}
			if (i == vel - 1)
				cout << "\n--------------------------------------------------------------------\n";
			else
				cout << endl;
		}
		

	}
	void Info() {
		cout << crt << ":::Tranfuzijski centar:::" << crt;
		cout << "--->Zalihe krvi<----" << crt;
		for (int i = 0; i < _zalihe.GetTrenutno(); i++)
			cout << _zalihe.GetElement1(i).ToString() << "   -  Kolicina: " << _zalihe.GetElement2(i) << " boca" << crt;
	}
};
void main()
{
	/**************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME
	// ERROR CE BITI OZNACENO KAO "RE"
	// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU code.TXT
	// 5. NAZIVI FUNKCIJA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.OSTALE, POMOANE FUNKCIJE
	// MOŽETE IMENOVATI PO ŽELJI. TAKO?ER, MOŽETE DODAVATI KOLIKO ŽELITE TESTNIH PODATAKA
	****************************************************************************/
	try {
		Datum dat_12_03_2018(12, 3, 2018), dat_01_02_2018(1, 2, 2018);
		Datum danas(dat_12_03_2018);
		cout << danas << endl;

		Kolekcija<int, int, 10> kolekcija1;
		for (size_t i = 0; i < kolekcija1.GetMax() - 1; i++)
			if (!kolekcija1.AddElement(10 + i, 170000 + i))
				cout << "Elementi " << 10 + i << " i " << 170000 + i << " nisu dodati u kolekciju" << endl;

		cout << kolekcija1 << endl;
		kolekcija1.SetSortiranje(DESC);
		kolekcija1.AddElement(16, 170016);
		cout << kolekcija1 << endl;

		Kolekcija<int, int, 10> kolekcija2(kolekcija1);
		cout << kolekcija2 << endl;


		vector<KrvnaGrupa*> krvneGrupe;
		krvneGrupe.push_back(new KrvnaGrupa(O, '+')); //0 - O_poz
		krvneGrupe.push_back(new KrvnaGrupa(O, '-')); //1 - O_neg
		krvneGrupe.push_back(new KrvnaGrupa(A, '+')); //2 - A_poz
		krvneGrupe.push_back(new KrvnaGrupa(A, '-')); //3 - A_neg
		krvneGrupe.push_back(new KrvnaGrupa(B, '+')); //4 - B_poz
		krvneGrupe.push_back(new KrvnaGrupa(B, '-')); //5 - B_neg
		krvneGrupe.push_back(new KrvnaGrupa(AB, '+'));//6 - AB_poz
		krvneGrupe.push_back(new KrvnaGrupa(AB, '-'));//7 - AB_neg

		vector<KrvnaGrupa*> donori_O_poz{ krvneGrupe[0], krvneGrupe[2], krvneGrupe[4], krvneGrupe[6] };
		krvneGrupe[0]->SetDonatori(donori_O_poz);
		//ili krace napisano
		krvneGrupe[0]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[1]});
		krvneGrupe[2]->SetDonatori(vector<KrvnaGrupa*>{krvneGrupe[2], krvneGrupe[6]});
		krvneGrupe[2]->SetPrimaoci(vector<KrvnaGrupa*>{krvneGrupe[0], krvneGrupe[2], krvneGrupe[1], krvneGrupe[3]});

		krvneGrupe[4]->SetDonatori(vector<KrvnaGrupa *>{krvneGrupe[4], krvneGrupe[6]});
		krvneGrupe[4]->SetPrimaoci(vector<KrvnaGrupa *>{krvneGrupe[0], krvneGrupe[4], krvneGrupe[1], krvneGrupe[5]});

		Osoba * jasmin = new Donator("Jasmin Azemovic", krvneGrupe[1], "jasmin@fit.ba", Datum(12, 2, 2017), true);
		Osoba * adel = new Donator("Adel Handzic", krvneGrupe[0], "adelfit.ba", Datum(9, 1, 2017), true);
		Osoba * goran = new Donator("Goran Skondric", krvneGrupe[1], "goran@fit.net", Datum(9, 3, 2017), true);


		TransfuzijskiCentar tcMostar;

		try
		{
			tcMostar.AddDonaciju(Datum(20, 5, 2017), jasmin, 2.5);
			tcMostar.AddDonaciju(Datum(20, 5, 2017), adel, 3);
			tcMostar.AddDonaciju(Datum(6, 5, 2017), goran, 1.2);
		}
		catch (exception & err){
			cout << "Greska-> " << err.what() << endl;
			delete[] err.what();

		}
		try
		{
			tcMostar.AddDonaciju(Datum(10, 9, 2017), jasmin, 2);
			tcMostar.AddDonaciju(Datum(18, 10, 2017), adel, 2.8);
			tcMostar.AddDonaciju(Datum(15, 9, 2017), goran, 3.8);
		}
		catch (exception & err){
			cout << "Greska-> " << err.what() << endl;
			delete[] err.what();

		}

		try {
			Zahtjev zahtjev_0_poz("Tranfuziologija KCUS", krvneGrupe[0], 15),
				zahtjev_0_neg("Tranfuziologija Bakir Nakas", krvneGrupe[1], 8);
			//ukoliko na stanju postoji dovoljno iste ili zamjense krvne grupe
			if (tcMostar.ProvjeriStanje("0+") >= 15)
				tcMostar.DodajZahtjev(zahtjev_0_poz);


			if (tcMostar.ProvjeriStanje("0-") >= 20)
				tcMostar.DodajZahtjev(zahtjev_0_neg);
			else if (tcMostar.KontaktirajDonora("0-"))
				cout << "Nema dovoljno zaliha krvi za trazenu grupu, ali smo kontaktirali donora/e i ocekuje se donacija u najkracem roku!\n";
			else
				cout << "Trenutno nema dovoljno zaliha za trazenu grupu, niti potencijalnih donora za istu.. Zao nam je..\n";


		}
		catch (exception & err)	{
			cout << "Greska-> " << err.what() << endl;
			delete[] err.what();
		}

		tcMostar.Info();

		// ispisuje matricu ili tablicu iz koje je vidljiva kompatibilnost izme?u pojedinih krvnih grupa. 
		// pored naziva i rh faktora krvne grupe, u zagradi ispisati i trenutnu kolieinu na stanju. matrica 
		// prikazana na kraju zadatka
		tcMostar.IspisiMatricuKompatibilnosti(krvneGrupe);
		delete adel;
		delete jasmin;
		delete goran;

		for (size_t i = 0; i < krvneGrupe.size(); i++)
			delete krvneGrupe[i];
	}
	catch (exception& err) {
		cout << err.what() << endl;
		delete[] err.what();

	}
}