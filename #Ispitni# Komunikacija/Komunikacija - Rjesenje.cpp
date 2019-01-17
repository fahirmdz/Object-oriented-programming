//fahirmdz GitHub

#include"pch.h"
#include <iostream>
#include <string>
#include<cstring>
#include<algorithm>
#include<regex>
#include<thread>
#include<mutex>
#include<exception>
#include<cctype>
#include<iomanip>
#include<vector>
#include<future>
using namespace std;

const char *crt = "\n-----------------------------------------------\n";
enum stringFormat { DDMMGG, DDMMGGGG, DDMMGGSSMM, DDMMGGGGSSMM };
enum prikazPoruka { SVE_PORUKE, SAKRIJ_POBRISANE, PRIKAZI_SAMO_POBRISANE };
string defaultString = "NEMA VRIJEDNOST";
vector<string> nedozvoljeneRijeci;
/*
DDMMGG - 29.8.17
DDMMGGGG - 29.8.2017
DDMMGGGGSSMM - 29.8.2017 10:15
*/
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}


class NedozvoljenaOperacija:public exception {
	string _funkcija;
	int _line;
public:
	NedozvoljenaOperacija(const char* message,string funk,int line):exception(message),_funkcija(funk),_line(line){}

	const char* what()const {
		string x = "ERROR: " + string(exception::what()) + "\nFunkcija: " + _funkcija + "\nLinija koda: " + to_string(_line);
		return AlocirajNizKaraktera(x.c_str());
	}
};

template<class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
public:
	Kolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}

	Kolekcija & operator=(Kolekcija<T1, T2> & orig){
		if (this != &orig){
			delete[]_elementi1;
			delete[]_elementi2;
			_trenutno = new int(*orig._trenutno);
			if (*_trenutno > 0) {
				_elementi1 = new T1[*orig._trenutno];
				_elementi2 = new T2[*orig._trenutno];

				for (int i = 0; i < *_trenutno; i++){
					_elementi1[i] = orig._elementi1[i];
					_elementi2[i] = orig._elementi2[i];
				}
			}
			else {
				_elementi1 = nullptr;
				_elementi2 = nullptr;
			}
		}
		return *this;
	}

	bool AddElement(T1 elem1, T2 elem2)
	{
		if (*_trenutno > 0)
			for (int i = 0; i < *_trenutno; i++)
				if (_elementi1[i] == elem1)
					return false;

		T1 * temp1 = new T1[*_trenutno + 1];
		T2 * temp2 = new T2[*_trenutno + 1];

		if (*_trenutno > 0) {
			for (int i = 0; i < *_trenutno; i++)
			{
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		}
		delete[] _elementi1;
		delete[] _elementi2;

		_elementi1 = temp1;
		_elementi2 = temp2;

		_elementi1[*_trenutno] = elem1;
		_elementi2[*_trenutno] = elem2;
		(*_trenutno)++;
		temp1 = nullptr;
		temp2 = nullptr;
		return true;
	}
	~Kolekcija()
	{
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }
	T1 & getElement1(int lokacija) { 
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw NedozvoljenaOperacija("Lokacija elementa nije validna!", __FUNCTION__, __LINE__);

		return _elementi1[lokacija]; }
	T2 & getElement2(int lokacija) {
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw NedozvoljenaOperacija("Lokacija elementa nije validna!", __FUNCTION__, __LINE__);
		return _elementi2[lokacija]; }
	int getTrenutno() { return *_trenutno; }

	friend ostream & operator<< <>(ostream&, Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
};
template<class T1,class T2>
ostream & operator<< <>(ostream & COUT, Kolekcija<T1, T2> & obj){
	for (int i = 0; i < *obj._trenutno; i++)
		COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
	
	return COUT;
}
template<class T1, class T2>
bool operator==<>(Kolekcija<T1, T2>& k1,Kolekcija<T1, T2>& k2) {
	if (*k1._trenutno == *k2._trenutno) {
		if (*k1._trenutno > 0)
			for (int i = 0; i < *k1._trenutno; i++)
				if (k1._elementi1[i] != k2._elementi1[i] || k1._elementi2[i] != k2._elementi2[i])
					return false;
		return true;
	}
	return false;
}


class DatumVrijeme
{
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0){
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& dv):_dan(new int(*dv._dan)),_mjesec(new int(*dv._mjesec)),_godina(new int(*dv._godina)),
		_sati(new int(*dv._sati)),_minuti(new int(*dv._minuti)){}
	~DatumVrijeme(){
		delete _dan; 
		delete _mjesec; 
		delete _godina;
		delete _sati; 
		delete _minuti;
		_dan = _mjesec = _godina = _sati = _minuti = nullptr;
	}

	DatumVrijeme& operator=(const DatumVrijeme& dv) {
		if (&dv == this)
			return *this;
		delete _dan;
		delete _mjesec;
		delete _godina;
		delete _sati;
		delete _minuti;
		_dan = new int(*dv._dan);
		_mjesec = new int(*dv._mjesec);
		_godina = new int(*dv._godina);
		_sati = new int(*dv._sati);
		_minuti = new int(*dv._minuti);
		return *this;
	}

	string ToString(stringFormat format) {
		//DDMMGG, DDMMGGGG, DDMMGGSSMM, DDMMGGGGSSMM -> FORMATI
		string trazeniString, minutiSati = GetVrijeme();

		if (*_dan < 10)
			trazeniString += string("0");
		trazeniString += to_string(*_dan) + ".";
		if(*_mjesec<10)
			trazeniString += string("0");
		trazeniString += to_string(*_mjesec) + ".";

		if (format == DDMMGG)
			return trazeniString + to_string(*_godina % 100);
		else if (format == DDMMGGGG)
			return trazeniString + to_string(*_godina);
		else if (format == DDMMGGSSMM)
			return trazeniString + to_string(*_godina % 100) + " " + minutiSati;
		else if (format == DDMMGGGGSSMM)
			return trazeniString + to_string(*_godina) + " " + minutiSati;

	}
	string GetVrijeme() {
		string minutiSati;
		if (*_sati < 10)
			minutiSati += string("0");
		minutiSati += to_string(*_sati) + ":";
		if (*_minuti < 10)
			minutiSati += "0";
		minutiSati += to_string(*_minuti);
		return minutiSati;
	}
	string GetDatum() {
		string trazeniString;
		if (*_dan < 10)
			trazeniString += string("0");
		trazeniString += to_string(*_dan) + ".";
		if (*_mjesec < 10)
			trazeniString += string("0");
		trazeniString += to_string(*_mjesec) + ".";
		return trazeniString + to_string(*_godina);
	}
	int GetRazlikaMinuta(DatumVrijeme& d) {
		if (*d._godina - *_godina > 1)
			return 30;
		else if (*d._mjesec - *_mjesec > 1)
			return 30;
		else if (*d._dan - *_dan > 1)
			return 30;

		auto isPrestupna = [](int g) {
			return g % 400 == 0 && g % 100 == 0 && g % 4 == 0;
		};
		auto brojDanaMjesec = [=](int g, int i) {
			if (i == 3 || i == 1 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
				return 31;
			else if (i == 2) {
				if (isPrestupna(g))
					return 29;
				else
					return 28;
			}
			else
				return 30;
		};

		long long int d1 = 0, d2 = 0;
		for (int i = 1; i < *_mjesec; i++)
			d1 += brojDanaMjesec(*_godina, i);
		for (int i = 1; i < *d._mjesec; i++)
			d2 += brojDanaMjesec(*d._godina, i);

		d1 += *_godina * 365;
		d2 += *d._godina * 365;

		d1 *= (24 * 60);
		d2 *= (24 * 60);

		d1 += (*_sati * 60);
		d2 += (*d._sati * 60);

		d1 += *_minuti;
		d2 += *d._minuti;

		return d1 - d2;
	}
	friend bool operator==(DatumVrijeme&, DatumVrijeme&);
	friend bool operator!=(DatumVrijeme&, DatumVrijeme&);
	friend ostream & operator<< (ostream&, DatumVrijeme&);
};
ostream & operator<<(ostream & COUT, DatumVrijeme & obj){
	COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
	return COUT;
}
bool operator==(DatumVrijeme& dv1, DatumVrijeme& dv2) {
	return *dv1._godina == *dv2._godina && *dv1._mjesec == *dv2._mjesec && *dv1._dan == *dv2._dan && *dv1._sati == *dv2._sati  && *dv1._minuti == *dv2._minuti;
}
bool operator!=(DatumVrijeme& dv1, DatumVrijeme& dv2) {
	return *dv1._godina != *dv2._godina || *dv1._mjesec != *dv2._mjesec || *dv1._dan != *dv2._dan || *dv1._sati != *dv2._sati  || *dv1._minuti != *dv2._minuti;
}

class Poruka{
	string _posiljalac;
	string _primalac;
	string _sadrzaj;
	DatumVrijeme _vrijeme;
public:
	Poruka():_posiljalac(defaultString),_primalac(defaultString),_sadrzaj(defaultString){}
	Poruka(string posiljalac, string primalac, string sadrzaj, DatumVrijeme vrijeme) :_vrijeme(vrijeme){
		string provjera = "\\w{4,12}";
		if (!regex_match(posiljalac, regex(provjera)) || !regex_match(primalac, regex(provjera)))
			throw NedozvoljenaOperacija("Korisnicko ime primaoca ili posiljaoca nije validno!",__FUNCTION__,__LINE__);
		_posiljalac = posiljalac;
		_primalac = primalac;
		_sadrzaj = sadrzaj;
	}

	string getPrimalac() { return _primalac; }
	string getPosiljalac() { return _posiljalac; }
	string getSadrzaj() { return _sadrzaj; }
	DatumVrijeme& GetDatum() {	return _vrijeme;}

	friend ostream & operator<< (ostream&, Poruka&);
	friend bool operator==(Poruka&, Poruka&);
	friend bool operator!=(Poruka&, Poruka&);
};
ostream& operator<<(ostream & COUT, Poruka & obj)
{
	COUT << "(" << obj._vrijeme.GetVrijeme() << ") " << obj._posiljalac << " -> ";
	COUT << obj._sadrzaj;
	return COUT;
}
bool operator==(Poruka& p1, Poruka& p2) { return p1._posiljalac == p2._posiljalac && p1._primalac == p2._primalac && p1._sadrzaj == p2._sadrzaj && p1._vrijeme == p2._vrijeme; }
bool operator!=(Poruka& p1, Poruka& p2) {
return	p1._posiljalac != p2._posiljalac || p1._primalac != p2._primalac || p1._sadrzaj != p2._sadrzaj || p1._vrijeme!=p2._vrijeme;}

class Komunikacija{
	string _naslov;
	int _ocjena; //ocjena komunikacije 1 - 5 se automatski izracunava
	Kolekcija<Poruka, bool> _poruke;
	//bool se odnosi na podatak da li je poruka pobrisana tj. nece biti vidljiva u prikazu ukoliko se drugacije ne navede
		/*formira ocjenu na nivou komunikacije na nacin da izracunava prosjecno vrijeme odgovora drugog ucesnika izrazeno u
		minutama i to prema sljedecoj skali
		//prosjek > 20 minuta = ocjena 1
		//prosjek > 15 minuta = ocjena 2
		//prosjek > 10 minuta = ocjena 3
		//prosjek > 5 minuta = ocjena 4
		//prosjek <=5 minuta = ocjena 5
		//*/
	void izracunajOcjenu() {
		int brojPoruka = _poruke.getTrenutno();
		if (brojPoruka == 0)
			_ocjena = 1;
		if (brojPoruka == 1)
			_ocjena = 5;
		double prosjek = 0;
		for (int i = 0; i + 1 < brojPoruka; i++ )
			if (_poruke.getElement1(i + 1).getPosiljalac() != _poruke.getElement1(i).getPosiljalac()) {
				int raz = _poruke.getElement1(i + 1).GetDatum().GetRazlikaMinuta(_poruke.getElement1(i).GetDatum());
				prosjek += _poruke.getElement1(i + 1).GetDatum().GetRazlikaMinuta(_poruke.getElement1(i).GetDatum());
			}
		prosjek /= (double)brojPoruka;
		if (prosjek > 20)
			_ocjena = 1;
		else if (prosjek > 15)
			_ocjena = 2;
		else if (prosjek > 10)
			_ocjena = 3;
		else if (prosjek > 5)
			_ocjena = 4;
		else if (prosjek <= 5)
			_ocjena = 5;
	}
public:
	Komunikacija():_naslov(defaultString),_ocjena(-1){}
	Komunikacija(string naslov){
		if (!regex_match(naslov, regex("^\\w{5,30}$")))
			throw NedozvoljenaOperacija("Naslov komunikacije nije validan!", __FUNCTION__, __LINE__);
		_naslov = naslov;
		_ocjena = 1;
	}
	void DodajPoruku(Poruka& p, bool vidljiva) {
		_poruke.AddElement(p, vidljiva);
	}

	bool SetObrisanaPoruka(Poruka& p) {
		for (int i = 0; i < _poruke.getTrenutno(); i++)
			if (_poruke.getElement1(i) == p) {
				_poruke.getElement2(i) = false;
				return true;
			}
		return false;
	}
	void OcijeniKomunikaciju() { izracunajOcjenu(); }
	void Print(prikazPoruka prikaz) {
		cout<< "Naslov: " << _naslov << "\nOcjena komunikacije: " << _ocjena << crt;
		cout << ":::::::::PORUKE::::::::::" << crt;
		for (int i = 0; i < _poruke.getTrenutno(); i++) {
			Poruka* temp = &_poruke.getElement1(i);
			if ((i == 0 || (i > 0 && temp->GetDatum().GetDatum() != _poruke.getElement1(i - 1).GetDatum().GetDatum())))
				cout << "::" << temp->GetDatum().GetDatum() << "::\n";
			if (prikaz == SAKRIJ_POBRISANE)
				if (!_poruke.getElement2(i))
					cout << " PORUKA IZBRISANA\n";
				else cout << *temp << endl;
			else if((prikaz==PRIKAZI_SAMO_POBRISANE && !_poruke.getElement2(i)) || prikaz==SVE_PORUKE)
				cout << *temp << endl;
			temp = nullptr;
		}
	}
	Kolekcija<Poruka, bool> * getPoruke() { return &_poruke; }
	int getOcjena() { return _ocjena; }
	string getNaslov() { return _naslov; }

	friend ostream & operator<<(ostream&, Komunikacija&);
	friend bool operator==(Komunikacija&, Komunikacija&);
};
ostream & operator<< (ostream & COUT, Komunikacija & obj){
	obj.Print(SAKRIJ_POBRISANE);
	return COUT;
}
bool operator==(Komunikacija& k1, Komunikacija& k2) { return k1._naslov == k2._naslov && k1._poruke == k2._poruke; }


class Korisnik
{
	string _korisnickoIme;
	char * _imePrezime;
	//char * - se odnosi na naziv predmeta ili tema gdje æe se èuvati kompletna komunikacija sa 
	//nastavnim osobljem ili korisnikom na odreðenom predmetu/temi
	Kolekcija<char *, Komunikacija> _komunikacija;
public:
	Korisnik(const char * korisnickoIme,const char * imePrezime){
		if (!regex_match(imePrezime, regex("^[a-zA-Z\\s]{4,25}$")) || !regex_match(korisnickoIme,regex("^\\w{4,12}$")))
			throw NedozvoljenaOperacija("Parametri za korisnika nisu validni!", __FUNCTION__, __LINE__);
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_korisnickoIme = korisnickoIme;
	}
	Korisnik(const Korisnik & original) :_korisnickoIme(original._korisnickoIme),_imePrezime(AlocirajNizKaraktera(original._imePrezime)){
		Korisnik& temp = *const_cast<Korisnik*>(&original);
		for (int i = 0; i < temp._komunikacija.getTrenutno(); i++)
			_komunikacija.AddElement(AlocirajNizKaraktera(temp._komunikacija.getElement1(i)), temp._komunikacija.getElement2(i));
	}
	~Korisnik(){
		delete[] _imePrezime; _imePrezime = nullptr;
		for (int i = 0; i < _komunikacija.getTrenutno(); i++) {
			delete[] _komunikacija.getElement1(i);
			_komunikacija.getElement1(i) = nullptr;
		}
	}
	int GetBrojKomunikacija() { return _komunikacija.getTrenutno(); }

	void OcijeniKomunikaciju(const char* naslovKomunikacije) {
		if (naslovKomunikacije == nullptr || _komunikacija.getTrenutno()==0)
			return;
		for (int i = 0; i < _komunikacija.getTrenutno(); i++)
			if (_komunikacija.getElement2(i).getNaslov() == naslovKomunikacije)
				_komunikacija.getElement2(i).OcijeniKomunikaciju();

	}

	void PrikaziKomunikacijuPoNaslovu(const char* naslovKomunikacije, prikazPoruka prikaz) {
		int lokacijaKomunikacije = indexKomunikacije(naslovKomunikacije);
		if (lokacijaKomunikacije == -1)
			return;
		_komunikacija.getElement2(lokacijaKomunikacije).Print(prikaz);
	}

	int indexKomunikacije(const char* naslov) {
		for (int i = 0; i < _komunikacija.getTrenutno(); i++)
			if (strcmp(_komunikacija.getElement1(i), naslov) == 0)
				return i;
		return -1;
	}

	// funkcija prikazuje sve poruke sa korisnikom cije korisnicko ime je definisano parametrom, te vraca ukupan 
	// broj (razmijenjenih poruka sa tim korisnikom)
	int PrikaziKomunikacijuSaKorisnikom(const char* korisnickoIme) {
		if (_komunikacija.getTrenutno() == 0)
			return 0;
		int brojPoruka = 0;
		cout << crt << ":::PREPISKA PORUKA IZMEDJU Korisnika " << korisnickoIme << " i Korisnika " << _korisnickoIme << ":::" << crt;
		for (int i = 0; i < _komunikacija.getTrenutno(); i++) {
			Kolekcija<Poruka, bool>* temp = _komunikacija.getElement2(i).getPoruke();
			Poruka* tempPoruka = &temp->getElement1(i);
			if (tempPoruka->getPrimalac() == korisnickoIme) {
				for (int j = 0; j < temp->getTrenutno(); j++) {
					brojPoruka++;
					cout << temp->getElement1(j) << endl;
				}
			}
			temp = nullptr;
			tempPoruka = nullptr;
		}
		return brojPoruka;
	}
	friend ostream & operator<< (ostream&, Korisnik&);

	Kolekcija<char*, Komunikacija> * GetKomunikacija() { return &_komunikacija; }
	string getKorisnickoIme() { return _korisnickoIme; }
	char* GetImePrezime()const { return _imePrezime; }
};

ostream & operator<< (ostream &COUT, Korisnik &obj)
{
	COUT << "Ime i prezime: " << obj._imePrezime << endl;
	COUT << crt << ":::::KOMUNIKACIJA SA NASTAVNIM OSOBLJEN::::" << crt;
	for (int i = 0; i < obj._komunikacija.getTrenutno(); i++)
		cout << i + 1 << ". ---> " << obj._komunikacija.getElement1(i) << crt << obj._komunikacija.getElement2(i) << crt;
	return COUT;
}

// sve poruke se cuvaju u komunikaciji posiljaoca i primaoca tako da u svakom momentu oba ucesnika u 
	// komunikaciji posjeduju kompletnu historiju poruka. poruke se grupisu na osnovu naslova, pa ce poruke 
	// pod naslovom "Sastanak" izmedju dva korisnika sadrzavati sve poruke koje su razmijenjene na tu temu
	// u slucaju da posiljalac ili primalac ne postoje funkcija treba baciti izuzetak sa odgovarajucom porukom. 
	// takodjer, koristeci regex sve poruke koje sadrze neku od zabranjenih rijeci trebaju biti oznacene kao
void PosaljiPoruku(Korisnik* korisnici[], int max, const char* naslovKomunikacije, string posiljalac, string primalac, const char* sadrzaj, DatumVrijeme& datum){
	if (naslovKomunikacije == nullptr || sadrzaj == nullptr || max == 0)
		return;
	bool flagSadrziNedozovljeneRijeci = false;
	for (size_t i = 0; i < nedozvoljeneRijeci.size(); i++)
		if (regex_search(sadrzaj, regex(nedozvoljeneRijeci[i])))
			flagSadrziNedozovljeneRijeci = true;

	int indexPosiljaoca = -1, indexPrimaoca = -1;
	for (int i = 0; i < max; i++)
		if (posiljalac == korisnici[i]->getKorisnickoIme())
			indexPosiljaoca = i;
		else if (primalac == korisnici[i]->getKorisnickoIme())
			indexPrimaoca = i;
	if (indexPosiljaoca == -1)
		throw NedozvoljenaOperacija(string("Posiljalac "+posiljalac+" ne postoji!").c_str(), __FUNCTION__, __LINE__);
	if (indexPrimaoca == -1)
		throw NedozvoljenaOperacija(string("Primalac " + primalac + " ne postoji!").c_str(), __FUNCTION__, __LINE__);

	int indexKomunikacijePosiljalac = korisnici[indexPosiljaoca]->indexKomunikacije(naslovKomunikacije),
		indexKomunikacijePrimalac = korisnici[indexPrimaoca]->indexKomunikacije(naslovKomunikacije);

	Kolekcija<char*, Komunikacija>* temp1 = korisnici[indexPosiljaoca]->GetKomunikacija();
	Kolekcija<char*, Komunikacija>* temp2 = korisnici[indexPrimaoca]->GetKomunikacija();

	
	Poruka novaPoruka(posiljalac, primalac, sadrzaj, datum);
	if (indexKomunikacijePosiljalac != -1) 
		temp1->getElement2(indexKomunikacijePosiljalac).DodajPoruku(novaPoruka, !flagSadrziNedozovljeneRijeci);
	else {
		temp1->AddElement(AlocirajNizKaraktera(naslovKomunikacije), Komunikacija(naslovKomunikacije));
		temp1->getElement2(temp1->getTrenutno() - 1).DodajPoruku(novaPoruka, !flagSadrziNedozovljeneRijeci);
	}

	if (indexKomunikacijePrimalac != -1)
		temp2->getElement2(indexKomunikacijePrimalac).DodajPoruku(novaPoruka, !flagSadrziNedozovljeneRijeci);
	else {
		temp2->AddElement(AlocirajNizKaraktera(naslovKomunikacije), Komunikacija(naslovKomunikacije));
		temp2->getElement2(temp2->getTrenutno() - 1).DodajPoruku(novaPoruka, !flagSadrziNedozovljeneRijeci);
	}
	temp1 = temp2 = nullptr;
}
void main()
{

	try {
		/*rijeèi koje su zabranjene u komunikaciji*/
		nedozvoljeneRijeci.push_back("ubiti");
		nedozvoljeneRijeci.push_back("mrzim");
		nedozvoljeneRijeci.push_back("rat");

		DatumVrijeme temp,
			datum19062017_1015(19, 6, 2017, 10, 15),
			datum19062017_1020(19, 6, 2017, 10, 31),
			datum19062017_1021(19, 6, 2017, 10, 47),
			datum19062017_1030(19, 6, 2017, 10, 30);

		cout << datum19062017_1015.ToString(DDMMGGGG) << endl;//vraca string: 19.6.2017
		cout << datum19062017_1015.ToString(DDMMGGGGSSMM) << endl;//vraca string: 19.6.2017 10:15


		const int max = 3;
		Korisnik * korisnici[max];
		korisnici[0] = new Korisnik("jasmin", "Jasmin Azemovic");
		korisnici[1] = new Korisnik("adel", "Adel Handzic");
		korisnici[2] = new Korisnik("denis", "Denis Music");


		PosaljiPoruku(korisnici, max, "Sastanak", korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(), "Danas imamo sastanak u 10h!", datum19062017_1015);

		PosaljiPoruku(korisnici, max, "Sastanak", korisnici[1]->getKorisnickoIme(), korisnici[0]->getKorisnickoIme(), "Naravno, vidimo se u sali za sastanke!", datum19062017_1020);

		PosaljiPoruku(korisnici, max, "Sastanak", korisnici[0]->getKorisnickoIme(), korisnici[1]->getKorisnickoIme(), "Sa sobom ponesirat radni materijal!", datum19062017_1021);

		cout << *korisnici[0] << endl;

		// ispisuje sve poruke koje je korisnik razmijenio pod naslovom definisanim parametrom. ispis komunikacije
		// treba biti organizovan po datumu na nacin da se prvo prikaze datum, a nakon toga sve poruke koje su razmijenjene 
		// tog datuma. Na mjestima pobrisanih poruka prikazati tekst "PORUKA IZBRISANA"
		korisnici[0]->OcijeniKomunikaciju("Sastanak");

		korisnici[0]->PrikaziKomunikacijuPoNaslovu("Sastanak", SVE_PORUKE);

		// funkcija prikazuje sve poruke sa korisnikom cije korisnicko ime je definisano parametrom, te vraca ukupan 
		// broj (razmijenjenih poruka sa tim korisnikom)
		future<int> brojPoruka = async(&Korisnik::PrikaziKomunikacijuSaKorisnikom, korisnici[0], "adel");
		int ukupnoPoruka = brojPoruka.get();
		cout << "Broj poruka: " << ukupnoPoruka << endl;
			

		for (size_t i = 0; i < max; i++)
		{
			delete korisnici[i];
			korisnici[i] = nullptr;
		}
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
		delete[] obj.what();
	}

	system("PAUSE");
}