//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<string>
#include<cstring>
#include<regex>
#include<exception>
#include<cctype>
#include<ctime>
#include<cstdlib>
#include<iomanip>
using namespace std;

const char * crt = "\n-------------------------------------------------------------------\n";

char* AlocirajNizKaraktera(const char* a) {
	if (a == nullptr)
		return nullptr;
	char* temp = new char[strlen(a) + 1];
	strcpy_s(temp, strlen(a) + 1, a);
	return temp;
}

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
	void copyElements(const Kolekcija& k) {
		if (*_trenutno > 0) {
			_elementi1 = new T1[*_trenutno];
			_elementi2 = new T2[*_trenutno];
			for (int i = 0; i < *_trenutno; i++) {
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		else {
			_elementi1 = nullptr;
			_elementi2 = nullptr;
		}
	}
public:
	Kolekcija() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& k) :_trenutno(new int(*k._trenutno)) {
		copyElements(k);
	}
	//1. potrebne funkcije	
	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		delete _trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		_trenutno = new int(*k._trenutno);
		copyElements(k);
		return *this;
	}
	void AddElement(T1 t1, T2 t2) {
		T1* temp1 = new T1[*_trenutno + 1];
		T2* temp2 = new T2[*_trenutno + 1];

		if (*_trenutno > 0)
			for (int i = 0; i < *_trenutno; i++) {
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = nullptr;
		temp2 = nullptr;
		_elementi1[*_trenutno] = t1;
		_elementi2[(*_trenutno)++] = t2;
	}

	int getTrenutno()const { return *_trenutno; }
	T1& getElement1(int index) {
		if (index < 0 || index >= *_trenutno)
			throw exception("Lokacija elementa nije validna!");
		return _elementi1[index];
	}
	T2& getElement2(int index) {
		if (index < 0 || index >= *_trenutno)
			throw exception("Lokacija elementa nije validna!");
		return _elementi2[index];
	}
	void SetElement2(int index, T2 t2) {
		if (index < 0 || index >= *_trenutno)
			throw exception("Lokacija elementa nije validna!");
		_elementi2[index] = t2;
	}
	void swapElements(int index1, int index2) {
		if (index1 < 0 || index1 >= *_trenutno || index2 < 0 || index2 >= *_trenutno)
			throw exception("Lokacija elementa nije validna!");
		swap(_elementi1[index1], _elementi1[index2]);
		swap(_elementi2[index1], _elementi2[index2]);
	}
	friend ostream& operator<<<>(ostream&, const Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
};
template<class T1,class T2>
ostream& operator<<<>(ostream& COUT, const Kolekcija<T1, T2>& k) {
	if (k._elementi1 == nullptr || k._elementi2 == nullptr)
		return COUT;
	for (int i = 0; i < *k._trenutno; i++)
		cout << k._elementi1[i] << " - " << k._elementi2[i] << endl;
	return COUT;
}
template<class T1, class T2>
bool operator==<>(Kolekcija<T1, T2>& k1, Kolekcija<T1, T2>& k2) {
	if (*k1._trenutno == *k2._trenutno) {
		for (int i = 0; i < *k1._trenutno; i++)
			if (k1._elementi1[i] != k2._elementi1[i] || k1._elementi2[i] != k2._elementi2[i])
				return false;
		return true;
	}
	return false;
}

class Datum{
	int *_dan, *_mjesec, *_godina;

	int DanaUMjesecu(int m, int g) {
		if (m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
			return 31;
		else if (m == 2)
			if (g % 400 == 0 && g % 100 == 0 && g % 4 == 0)
				return 29;
			else
				return 28;
		else
			return 30;
	}
	bool validneVrijednost(int d, int m, int g) {
		if (d<1 || d>DanaUMjesecu(m, g) || m < 1 || m>12 || g < 1920)
			return false;

		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);

		int trDan = timeinfo.tm_mday, trMjesec = timeinfo.tm_mon + 1, trGodina = timeinfo.tm_year + 1900;

		if (g > trGodina)
			return false;
		if (g == trGodina && m > trMjesec)
			return false;
		else if (m == trMjesec && d > trDan)
			return false;
		return true;
	}
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000){
		if (!validneVrijednost(dan, mjesec, godina))
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& d):_dan(new int(*d._dan)),_mjesec(new int(*d._mjesec)),_godina(new int(*d._godina)){}
	~Datum(){
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	Datum& operator=(const Datum& d) {
		if (&d == this)
			return *this;
		delete _dan; _dan = new int(*d._dan);
		delete _mjesec; _mjesec = new int(*d._mjesec);
		delete _godina; _godina = new int(*d._godina);
		return *this;
	}
	friend bool operator==(Datum&, Datum&);
	friend bool operator!=(Datum&, Datum&);
	friend ostream& operator<< (ostream &, const Datum &);
};
bool operator==(Datum& d1, Datum& d2) {
	return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan;
}
bool operator!=(Datum& d1, Datum& d2) {
	return *d1._godina != *d2._godina || *d1._mjesec != *d2._mjesec || *d1._dan != *d2._dan;
}
ostream& operator<< (ostream &COUT, const Datum &obj){
	COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
	return COUT;
}
class Osoba abstract {
protected:
	char * _imePrezime;
	Datum * _datumRodjenja;
public:
	Osoba() :_imePrezime(AlocirajNizKaraktera("NEMA VRIJEDNOST")), _datumRodjenja(new Datum()) {}
	Osoba(const char * imePrezime, Datum datum) {
		if (!regex_match(imePrezime, regex("^[A-Z]{1}[a-z]{2,15}\\s{1,2}[A-Z]{1}[a-zA-Z]{2,15}$")))
			throw exception("Ime i prezime nije validno! Prvo slovo imena i prezimena moraju biti velika slova!");
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_datumRodjenja = new Datum(datum);
	}
	Osoba(const Osoba & obj) :_imePrezime(AlocirajNizKaraktera(obj._imePrezime)), _datumRodjenja(new Datum(*obj._datumRodjenja)) {	}
	~Osoba() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
	}
	Osoba& operator=(const Osoba& o) {
		if (&o == this)
			return *this;
		delete[] _imePrezime;
		_imePrezime = AlocirajNizKaraktera(o._imePrezime);
		delete _datumRodjenja;
		_datumRodjenja = new Datum(*o._datumRodjenja);
		return *this;
	}
	char* GetImePrezime()const { return _imePrezime; }
	Datum GetDatumRodjenja()const { return *_datumRodjenja; }

	friend bool operator==(Osoba&, Osoba&);
	friend bool operator!=(Osoba&, Osoba&);
	friend ostream& operator<<(ostream&,const Osoba&);
};
ostream& operator<<(ostream& COUT,const Osoba& o) {
	cout << crt << "Ime i prezime: " << o._imePrezime;
	cout << "\nDatum rodjenja: " << *o._datumRodjenja << endl;
	return COUT;
}
bool operator==(Osoba& o1, Osoba& o2) { return strcmp(o1._imePrezime, o2._imePrezime) == 0 && *o1._datumRodjenja == *o2._datumRodjenja; }
bool operator!=(Osoba& o1, Osoba& o2) { return strcmp(o1._imePrezime, o2._imePrezime) != 0 || *o1._datumRodjenja != *o2._datumRodjenja; }



class Kupac:public Osoba{
	
	float _odobreniPopust;
public:
	Kupac():_odobreniPopust(0){}
	Kupac(const char * imePrezime, Datum datum) :Osoba(imePrezime, datum), _odobreniPopust(0.10) {}

	float GetPopust()const { return _odobreniPopust; }
	void DodajPopust(float popust) { _odobreniPopust += popust; }

	friend bool operator==(Kupac&, Kupac&);
	friend bool operator!=(Kupac&, Kupac&);
	friend ostream& operator<<(ostream&, const Kupac&);
};
bool operator==(Kupac& k1, Kupac& k2) { return *dynamic_cast<Osoba*>(&k1) == *dynamic_cast<Osoba*>(&k2) && k1._odobreniPopust == k2._odobreniPopust; }
bool operator!=(Kupac& k1, Kupac& k2) { return *dynamic_cast<Osoba*>(&k1) != *dynamic_cast<Osoba*>(&k2) || k1._odobreniPopust != k2._odobreniPopust; }
ostream& operator<<(ostream& COUT, const Kupac& k) {
	cout << *dynamic_cast<Osoba*>(const_cast<Kupac*>(&k)) << endl;
	cout << "Odobreni popust: " << setprecision(2) << k._odobreniPopust * 100 << "%" << crt;
	return COUT;
}

class Artikal{
	string _barCode;
	char * _naziv;
	float _cijena;

public:
	Artikal(const char * naziv, float cijena, string barCode){
		if (!regex_match(naziv, regex("^[a-zA-Z0-9\\-\\s]{4,30}$")))
			throw exception("Naziv artikla nije validan!");
		if (!regex_match(barCode, regex("^\\d{11,13}$")))
			throw exception("Bar kode nije validan!");
		if (cijena <= 0 || cijena > 100000)
			throw exception("Cijena artikla nije validna!");
		_naziv = AlocirajNizKaraktera(naziv);
		_cijena = cijena;
		_barCode = barCode;
	}
	Artikal(const Artikal& artikal):_naziv(AlocirajNizKaraktera(artikal._naziv)),_cijena(artikal._cijena),
		_barCode(artikal._barCode){}
	~Artikal() { delete[]_naziv; _naziv = nullptr; }

	Artikal& operator=(const Artikal& ar) {
		if (&ar == this)
			return *this;
		delete _naziv;
		_naziv = AlocirajNizKaraktera(ar._naziv);
		_barCode = ar._barCode;
		_cijena = ar._cijena;
		return *this;
	}
	string GetBarCode()const { return _barCode;}
	char* GetNaziv()const { return _naziv; }
	float GetCijena()const { return _cijena; }
	friend ostream& operator<< (ostream &, const Artikal &);
	friend bool operator==(Artikal&, Artikal&);
	friend bool operator!=(Artikal&, Artikal&);

	//1. potrebne funkcije
};
ostream& operator<< (ostream &COUT, const Artikal &obj){
	COUT << obj._naziv << "\t" << obj._cijena << "\t\t";
	return COUT;
}
bool operator==(Artikal& a1, Artikal& a2) { return a1._barCode == a2._barCode && strcmp(a1._naziv, a2._naziv)==0 && a1._cijena == a2._cijena; }
bool operator!=(Artikal& a1, Artikal& a2) { return a1._barCode != a2._barCode || strcmp(a1._naziv, a2._naziv)!=0 || a1._cijena != a2._cijena; }

static int brojRacuna= 1;
char* GenerisiSljedeciBrojRacuna() {
	int brCifara = 0;
	int slRacun = brojRacuna;
	while (slRacun != 0) {
		slRacun /= 10;
		brCifara++;
	}
	int potrebnoNula = 7 - brCifara;
	char* temp = new char[10];
	strcpy_s(temp, 10, "RN");
	string broj = to_string(brojRacuna);
	for (int i = 0; i < potrebnoNula; i++)
		strcat_s(temp, 10, "0");
	strcat_s(temp, 10, broj.c_str());
	return temp;
}

class Racun {
	//format broja racuna: RN6005491
	//osigurati automatsko generisanje broja racuna pocevsi od RN0000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	//prvi argument je Artikal, a drugi kolicina/broj artikala koju je kupac uzeo
	Kolekcija<Artikal *, int> * _stavkeRacuna;
	Kupac * _kupac;
public:
	Racun() :_kupac(nullptr), _stavkeRacuna(new Kolekcija<Artikal*, int>) {
		strcpy_s(_brojRacuna, "EMPTY");
	}
	Racun(char* brojRacuna, Datum datumKreiranja, Kupac* kupac) {
		if (kupac == nullptr)
			throw exception("Morate navesti kupca!");
		_kupac = kupac;
		_datumKreiranja = datumKreiranja;
		_stavkeRacuna = new Kolekcija<Artikal*, int>;
		strcpy_s(_brojRacuna, brojRacuna);
	}
	Racun(const Racun& r) :_datumKreiranja(r._datumKreiranja), _stavkeRacuna(new Kolekcija<Artikal*, int>), _kupac(r._kupac) {
		for (int i = 0; i < r._stavkeRacuna->getTrenutno(); i++)
			_stavkeRacuna->AddElement(new Artikal(*r._stavkeRacuna->getElement1(i)), r._stavkeRacuna->getElement2(i));
	}
	~Racun() {
		_kupac = nullptr;
		for (int i = 0; i < _stavkeRacuna->getTrenutno(); i++) {
			delete _stavkeRacuna->getElement1(i);
			_stavkeRacuna->getElement1(i) = nullptr;
		}
		delete _stavkeRacuna;
		_stavkeRacuna = nullptr;
	}

	Racun& operator=(const Racun& r) {
		if (&r == this)
			return *this;

		strcpy_s(_brojRacuna, r._brojRacuna);
		 _kupac = r._kupac;
		for (int i = 0; i < _stavkeRacuna->getTrenutno(); i++) {
			delete _stavkeRacuna->getElement1(i);
			_stavkeRacuna->getElement1(i) = nullptr;
		}
		delete _stavkeRacuna;
		_stavkeRacuna = new Kolekcija<Artikal*, int>;
		_datumKreiranja = r._datumKreiranja;

		for (int i = 0; i < r._stavkeRacuna->getTrenutno(); i++)
			_stavkeRacuna->AddElement(new Artikal(*r._stavkeRacuna->getElement1(i)), r._stavkeRacuna->getElement2(i));
		return *this;
	}

	// 2. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. Onemoguciti ponavljanje 
   // artikala na nacin da se uvecava samo kolicina ukoliko korisnik vise puta pokusa dodati isti artikal.
	void DodajArtikal(Artikal& artikal, int kolicina) {
		if (kolicina <= 0) {
			cout << "\nKolicina ne moze biti manja ili jednaka nuli!\n";
			return;
		}
		for (int i = 0; i < _stavkeRacuna->getTrenutno(); i++)
			if (*_stavkeRacuna->getElement1(i) == artikal) {
				_stavkeRacuna->SetElement2(i, _stavkeRacuna->getElement2(i) + kolicina);
				return;
			}
		_stavkeRacuna->AddElement(new Artikal(artikal), kolicina);

	}

	// 4. Sortiraj - sortira artikle (rastuci) po kolicini novca kojom ucestvuju u ukupnom iznosu racuna
	void Sortiraj() {
		int trenutno = _stavkeRacuna->getTrenutno();
		for (int i = 0; i < trenutno - 1; i++) {
			int min = i;
			for (int j = i + 1; j < trenutno; j++)
				if (_stavkeRacuna->getElement2(i) > _stavkeRacuna->getElement2(j))
					min = i;
			if (min != i)
				_stavkeRacuna->swapElements(min, i);
		}
	}
	// 3. operator<< - ispisuje racun u formatu prikazanom na slici(nije obavezno da bude identican, ali je svakako pozeljno).
	//    Nakon ispisa svakog racuna, ukoliko su ispunjeni definisani uslovi, kupcu se uvecava popust.
	friend ostream& operator<<(ostream&, Racun&);
};
//------------------------------------------ -
//FITStore::Racun::RN000002
//| RB Artikal   Cijena Kolicina Ukupno |
//| 1. Milka Milkins  2.55    15      38.25 |
//| 2. Cappy narandza  2.1    20         42 |
//| 3. Meggle jogurt  2.55    20         51 |
//
//| Suma:131.25KM
//| Popust : 0.25KM
//| Za platiti : 131KM
//------------------------------------------ -

//svaki kupac ima popust koji mu se zaracunava prilikom svake kupovine
	//pocetni popust je 0.10%
	//svaka kupovina u iznosu 100 - 500KM povecava iznos popusta za 0.15%
	//svaka kupovina u iznosu 500 - 1000KM povecava iznos popusta za 0.25%
	//svaka kupovina u iznosu vecem od 1000KM povecava iznos popusta za 0.30%
ostream& operator<<(ostream& COUT, Racun& racun) {
	cout << crt << ":::FITStore:::Racun: " << racun._brojRacuna << crt;
	cout << "   | RB  Artikal  \tCijena\t\tKolicina\tUkupno   |" << endl;
	double suma = 0;
	for (int i = 0; i < racun._stavkeRacuna->getTrenutno(); i++) {
		double sumaTemp = racun._stavkeRacuna->getElement1(i)->GetCijena()*racun._stavkeRacuna->getElement2(i);
		suma += sumaTemp;
		cout << "   | " << i + 1 << ".  " << *racun._stavkeRacuna->getElement1(i) << racun._stavkeRacuna->getElement2(i);
		cout << "\t\t" << fixed<<setprecision(4) << sumaTemp << "  |" << endl;
	}
	cout << "\nSuma: " << fixed << setprecision(2) << suma;
	double popust = suma * (double)racun._kupac->GetPopust();

	cout << "\nPopust: " << popust;
	cout << "\nZa platiti: " << suma - popust << crt;
	double konacna = suma - popust;
	double bonusPopust = 0;
	if (konacna >= 100 && konacna <= 500)
		bonusPopust = 0.15;
	else if (konacna >= 500 && konacna <= 1000)
		bonusPopust = 0.25;
	else if (konacna > 1000)
		bonusPopust = 0.30;
	racun._kupac->DodajPopust(bonusPopust);
	return COUT;
}
void main(){

#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeKolekcije
	Kolekcija<int, int> kolekcija1;
	int brojElemenata = 15;
	for (int i = 0; i < brojElemenata; i++)
		kolekcija1.AddElement(i, i);

	cout << kolekcija1.getElement1(0) << " " << kolekcija1.getElement2(0) << endl;
	cout << kolekcija1 << endl;

	Kolekcija<int, int> kolekcija2;
	kolekcija2 = kolekcija1;

	cout << kolekcija2 << crt;
	if (kolekcija1.getTrenutno() == kolekcija2.getTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;

	Kolekcija<int, int> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeKupca
	Kupac jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
	Kupac adel("Adel Handzic", Datum(15, 10, 1981));

	Kupac jasmin2 = jasmin;
	cout << jasmin << endl << jasmin2 << endl;
	jasmin = jasmin2;
	cout << jasmin << endl << jasmin2 << endl;
#pragma endregion

#pragma region TestiranjeArtikla
	Artikal sokCppyNarandza("Cappy narandza", 2.10, "699511236545");
	Artikal cokoladaMilkaMilkins("Milka Milkins", 2.55, "622445875463");
	Artikal jogurtMeggle("Meggle jogurt", 2.55, "155338774458");

	cout << sokCppyNarandza << cokoladaMilkaMilkins << jogurtMeggle << endl;
	sokCppyNarandza = cokoladaMilkaMilkins;
	cout << sokCppyNarandza << endl;

	if (sokCppyNarandza == cokoladaMilkaMilkins)
		cout << "Artikli su isti" << endl;
#pragma endregion

#pragma region TestiranjeRacuna
	/*GenerisiSljedeciBrojRacuna - globalna funkcija koja generise i vraca naredni broj racuna u
	prethodno definisanom formatu*/
	Racun racun1(GenerisiSljedeciBrojRacuna(), danas, &jasmin);

	racun1.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun1.DodajArtikal(sokCppyNarandza, 20);
	racun1.DodajArtikal(jogurtMeggle, 20);

	cout << racun1 << endl;

	Racun racun2(GenerisiSljedeciBrojRacuna(), sutra, &jasmin);
	racun2.DodajArtikal(cokoladaMilkaMilkins, 15);
	racun2.DodajArtikal(sokCppyNarandza, 20);
	racun2.DodajArtikal(jogurtMeggle, 20);
	cout << racun2 << endl;

	Racun racun3(racun2);
	cout << racun3 << endl;
	racun3.Sortiraj();

#pragma endregion
	system("pause");
}
