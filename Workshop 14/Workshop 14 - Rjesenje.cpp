//fahirmdz GitHub

#include"pch.h"
#include <iostream> 
#include <string>
#include<cstring>
#include<regex>
#include<thread>
#include <vector>
#include<exception>
#include<iomanip>
#include<algorithm>
#include<mutex>

using namespace std;
const char* crt = "\n--------------------------------------------------\n";

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
	void copyElements(const Kolekcija& k) {
		for (int i = 0; i < k._trenutno; i++) {
			if (k._elementi1[i] != nullptr && k._elementi2[i] != nullptr) {
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}
			else {
				_elementi1[i] = nullptr;
				_elementi2[i] = nullptr;
			}
		}
	}

public:
	Kolekcija() {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	Kolekcija(const Kolekcija& k) {
		_trenutno = k._trenutno;
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		if (_trenutno > 0)
			copyElements(k);
	}
	~Kolekcija() {
		for (int i = 0; i < _trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;

		_trenutno = k._trenutno;
		for (int i = 0; i < max; i++)
		{
			delete _elementi1[i];
			delete _elementi2[i];
		}
		if (_trenutno > 0)
			copyElements(k);
		return *this;
	}

	void Dodaj(T1& el1, T2& el2) {
		if (_trenutno == max)
			throw exception("Dostigli ste kapacitet kolekcije.");
		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno++] = new T2(el2);
	}
	T1& GetElement1(int index) {
		if (index >= 0 && index < _trenutno)
			return *_elementi1[index];
		throw exception("Vrijednost indeksa nije validna.");
	}
	T2& GetElement2(int index) {
		if (index >= 0 && index < _trenutno)
			return *_elementi2[index];
		throw exception("Vrijednost indeksa nije validna.");
	}
	int GetTrenutno()const {
		return _trenutno;
	}
};


class Uplata {
	char* _datum;
	double _iznos;

public:
	Uplata():_datum(AlocirajNizKaraktera("01/01/2000")),_iznos(0){}
	Uplata(const char* datum, double iznos) {
		if (!regex_match(datum, regex("\\d{1,2}[-./]{1}\\d{1,2}[-./]{1}\\d{4}")))
			throw exception("Datum uplate nije validan!");
		if (iznos <= 0)
			throw exception("Iznos ne mozete biti manji ili jednak 0!");
		_datum = AlocirajNizKaraktera(datum);
		_iznos = iznos;
	}
	Uplata(const Uplata& u) :_iznos(u._iznos), _datum(AlocirajNizKaraktera(u._datum)) {}
	~Uplata() { delete[] _datum; _datum = nullptr; }

	Uplata& operator=(const Uplata& u) {
		if (&u == this)
			return *this;
		delete _datum;
		_datum = AlocirajNizKaraktera(u._datum);
		_iznos = u._iznos;
		return *this;
	}
	char* GetDatum()const { return _datum; }
	double GetIznos()const { return _iznos; }

	friend bool operator==(Uplata&, Uplata&);
	friend ostream& operator<<(ostream&, const Uplata&);
};
bool operator==(Uplata& u1, Uplata& u2) {
	return strcmp(u1._datum, u2._datum) == 0 && u1._iznos == u2._iznos;
}
ostream& operator<<(ostream& cout, const Uplata& u) {
	cout << "Datum uplate: " << u._datum << endl; 
	cout << "Iznos: " << u._iznos << endl;
	return cout;
}


class Oprema {
string _naziv; 
string* _proizvodjac; 
double _cijena; 
int _dostupnoNaStanju;

public: 
	Oprema():_naziv(""),_proizvodjac(new string("")),_cijena(0),_dostupnoNaStanju(0){}
	Oprema(string naziv, string proizvodjac, double cijena, int dostupno) {
		if (!regex_match(naziv, regex("^[a-zA-Z0-9\\s]{5,30}$")))
			throw exception("Naziv opereme nije validan!");
		if (!regex_match(proizvodjac, regex("^[a-zA-Z0-9\\s]{5,30}$")))
			throw exception("Ime proizvodjaca nije validno!");
		if (cijena <= 0)
			throw exception("Cijena opreme ne moze biti manja ili veca od 0!");
		_naziv = naziv;
		_proizvodjac = new string(proizvodjac);
		_cijena = cijena;
		_dostupnoNaStanju = dostupno;
	}
	Oprema(const Oprema& o):_naziv(o._naziv),_proizvodjac(new string(*o._proizvodjac)),_cijena(o._cijena),
		_dostupnoNaStanju(o._dostupnoNaStanju){}
	~Oprema() { delete _proizvodjac; _proizvodjac = nullptr; }

	Oprema& operator=(const Oprema& o) {
		if (&o == this)
			return *this;

		delete _proizvodjac;
		_proizvodjac = new string(*o._proizvodjac);
		_cijena = o._cijena;
		_naziv = o._naziv;
		_dostupnoNaStanju = o._dostupnoNaStanju;
		return *this;
	}
	Oprema& operator--() {
		_dostupnoNaStanju--;
		return *this;
	}
	Oprema& operator++() {
		_dostupnoNaStanju++;
		return *this;
	}

	string GetNaziv()const { return _naziv; }
	string GetProizvodjac()const { return *_proizvodjac; }
	double GetCijena()const { return _cijena; }
	int GetNaStanju()const { return _dostupnoNaStanju; }

	friend ostream& operator<<(ostream&, Oprema&);
	friend bool operator==(Oprema&, Oprema&);

};
ostream& operator<<(ostream& COUT, Oprema& o) {
	cout << "Naziv: " << o._naziv;
	cout << "\nProizvodjac: " << *o._proizvodjac;
	cout << "\nCijena: " << o._cijena;
	cout << "\nDostupno na stanju: " << o._dostupnoNaStanju << endl;
	return COUT;
}
bool operator==(Oprema& o1, Oprema& o2) {
	return o1._naziv == o2._naziv && *o1._proizvodjac == *o2._proizvodjac;
}

class Osoba abstract{
protected:
	string _imePrezime;
	char _spol;
	int _godiste;

public:
	Osoba():_imePrezime(""),_spol('M'),_godiste(2000){}
	Osoba(string imeP, char spol, int god) {
		if (!regex_match(imeP, regex("^[a-zA-Z\\s]{4,25}$")))
			throw exception("Ime i prezime za ucenika nije validno!");
		if (toupper(spol) != 'M' && toupper(spol) != 'Z')
			throw exception("Spol Ski Trenera nije validan!");
		if (god > 2019 || god < 1925)
			throw exception("Godiste nije validno!");
		_imePrezime = imeP;
		_spol = toupper(spol);
		_godiste = god;
	}
	Osoba(const Osoba& o):_imePrezime(o._imePrezime),_spol(o._spol),_godiste(o._godiste){}
	string GetImePrezime()const { return _imePrezime; }
	char GetSpol()const { return _spol; }
	int GetGodiste()const { return _godiste; }

	virtual void Info() {
		cout << crt;
		cout << "Ime i prezime: " << _imePrezime;
		cout << "\nSpol: ";
		if (_spol == 'M')
			cout << "Muski";
		else
			cout << "Zenski";
		cout << "\nGodiste: " << _godiste;
	}
	friend bool operator==(Osoba&, Osoba&);
};
bool operator==(Osoba& o1, Osoba& o2) { return o1._godiste == o2._godiste && o1._imePrezime == o1._imePrezime; }

class Ucenik:public Osoba {
	bool _posjedujeOpremu; 
	vector<Oprema>* _iznajmljenaOprema; //Trenutno iznajmljena oprema 
	Kolekcija<Uplata, string, 100> _uplate; //Drugi parametar tipa string predstavlja svrhu uplate: trening ili oprema

	int GetIndexOpreme(Oprema& o) {
		for (size_t i = 0; i < _iznajmljenaOprema->size(); i++)
			if ((*_iznajmljenaOprema)[i] == o)
				return i;
		return -1;
	}
public:
	Ucenik():_posjedujeOpremu(false){}
	Ucenik(string imePrezime,char spol, int godiste, bool posjedujeOpremu = false):Osoba(imePrezime,spol,godiste) {
		_posjedujeOpremu = posjedujeOpremu;
		_iznajmljenaOprema = new vector<Oprema>;
	}
	Ucenik(const Ucenik& u) :Osoba(u), _posjedujeOpremu(u._posjedujeOpremu),_uplate(u._uplate) {
		if (u._iznajmljenaOprema != nullptr)
			_iznajmljenaOprema = new vector<Oprema>(*u._iznajmljenaOprema);
		else
			_iznajmljenaOprema = new vector<Oprema>;
	}
	~Ucenik() {delete _iznajmljenaOprema; _iznajmljenaOprema = nullptr;}

	Ucenik& operator=(const Ucenik& u) {
		if (&u == this)
			return *this;

		delete _iznajmljenaOprema;
		_imePrezime = u._imePrezime;
		_godiste = u._godiste;
		_posjedujeOpremu = u._posjedujeOpremu;
		if (u._iznajmljenaOprema != nullptr)
			_iznajmljenaOprema = new vector<Oprema>(*u._iznajmljenaOprema);
		else
			_iznajmljenaOprema = new vector<Oprema>;
		_uplate = u._uplate;
		return *this;
	}

	Ucenik& operator+=(Oprema& o) {
		if (o.GetNaStanju() == 0) {
			cout << "Trazenu opremu trenutno nemamo na stanju..\n";
			return *this;
		}
		int index = GetIndexOpreme(o);
		if (index != -1) {
			cout << "Oprema vec iznajmljena!\n";
			return *this;
		}
		--o;
		_iznajmljenaOprema->push_back(o);
		return *this;
	}

	Ucenik& operator-=(Oprema& o) {
		int index = GetIndexOpreme(o);
		if (index == -1) {
			cout << "Oprema ne postoji na spisku iznajmljene opreme!\n";
			return *this;
		}
		vector<Oprema>::iterator it = _iznajmljenaOprema->begin() + (index + 1);
		++o;
		_iznajmljenaOprema->erase(it);

		return *this;
	}

	void DodajUplatu(Uplata& u,string svrha) {
		if (!regex_search(svrha, regex("(oprema|trening|Oprema|Trening){1}")))
			throw exception("Svrha opreme moze biti samo oprema ili trening!\n");
			_uplate.Dodaj(u, svrha);
	}
	void PrintUplate() {
		cout << crt << "::::IZVRSENE UPLATE::::" << crt;
		for (int i = 0; i < _uplate.GetTrenutno(); i++)
			cout << i + 1 << ". " << _uplate.GetElement1(i) << " - " << _uplate.GetElement2(i) << endl;
		cout << crt;
	}
	void PrintOpremu() {
		cout << crt << "--IZNAJMLJENA OPREMA TRENUTNO--" << crt;
		for (size_t i = 0; i < _iznajmljenaOprema->size(); i++)
			cout << i + 1 << ". " << (*_iznajmljenaOprema)[i].GetNaziv() << " - " << (*_iznajmljenaOprema)[i].GetProizvodjac() << endl;
		cout << crt;
	}
	double GetIznosSvihUplata(int x = 0) {
		if (x >= _uplate.GetTrenutno())
			return 0;
		return _uplate.GetElement1(x).GetIznos() + GetIznosSvihUplata(x + 1);
		
	}
	void Info() {
		cout << "::::PODACI O UCENIKU::::";
		Osoba::Info();
		cout << "\nPosjeduje opremu: ";
		if (_posjedujeOpremu)
			cout << "DA\n";
		else
			cout << "NE\n";

		cout << crt;
	}
	friend bool operator==(Ucenik&,Ucenik&);
	friend bool operator<(Ucenik&, Ucenik&);
};
bool operator<(Ucenik& u1, Ucenik& u2) { return u1._godiste < u2._godiste; }
bool operator==(Ucenik& u1,Ucenik& u2) { return *dynamic_cast<Osoba*>(&u1) == *dynamic_cast<Osoba*>(&u2); }
enum TipTreninga { Pocetnicki, Rekreativni, Profesionalni };
const char* TipTreningaChar[] = { "Pocetnicki","Rekreativni","Profesionalni" };

class SkiTrener:public Osoba {
bool _certificiran;
TipTreninga  _tipTreninga;

public: 
	SkiTrener():_certificiran(false),_tipTreninga(Pocetnicki){}
	SkiTrener(string imeprezime,int godiste, bool cert, char spol, TipTreninga tipTreninga):Osoba(imeprezime,spol,godiste),_certificiran(cert),
	_tipTreninga(tipTreninga){}

	bool GetCertificiran()const { return _certificiran; }
	TipTreninga GetTipTreninga()const { return _tipTreninga; }

	void Info() {
		cout << crt << ":::PODACI O TRENERU:::";
		Osoba::Info();
		cout << "\nCertificirani trener: ";
		if (_certificiran)
			cout << "DA\n";
		else
			cout << "NE\n";
		cout << "Obuka za trening: " << TipTreningaChar[_tipTreninga] << crt;
	}
	friend bool operator==(SkiTrener&, SkiTrener&);
	friend bool operator<(SkiTrener&, SkiTrener&);
};
bool operator==(SkiTrener& sk1, SkiTrener& sk2) { return *dynamic_cast<Osoba*>(&sk1) == *dynamic_cast<Osoba*>(&sk2); }
bool operator<(SkiTrener& sk1, SkiTrener& sk2) { return (int)sk1._tipTreninga < (int)sk2._tipTreninga; }


class Trening {
string* _datum;
SkiTrener* _trener; 
vector<Ucenik*> _ucenici;
int _brojCasova; 

public: 
	Trening():_datum(new string("")),_trener(new SkiTrener()),_brojCasova(0){}
	Trening(string datum, SkiTrener* trener, int brojcasova) {
		if (!regex_match(datum, regex("\\d{1,2}[.-/]{1}\\d{1,2}[./-]{1}\\d{4}")))
			throw exception("Datum treninga nije validan!");
		if (brojcasova <= 0)
			throw exception("Trening mora sadrzati jedan ili vise casova!");
		if (trener == nullptr)
			throw exception("Niste odredili trenera za trening!");
		_datum = new string(datum);
		_trener = trener;
		_brojCasova = brojcasova;
	}
	Trening(const Trening& t) :_datum(new string(*t._datum)), _trener(t._trener), _brojCasova(t._brojCasova) {
		for (size_t i = 0; i < t._ucenici.size(); i++)
			_ucenici.push_back(t._ucenici[i]);
	}
	~Trening() {
		delete _datum; _datum = nullptr;
		_trener = nullptr;
		for (size_t i = 0; i < _ucenici.size(); i++) 
			 _ucenici[i] = nullptr;
	}

	//Napomena: Atributi _trener i _ucenici čuvaju samo adrese objekata koji su pohranjeni u glavnoj klasi SkiKurs.
	Trening& operator=(const Trening& t) {
		if (&t == this)
			return *this;
		delete _datum;
		_datum = new string(*t._datum);
		_trener = t._trener;
		_brojCasova = t._brojCasova;
		_ucenici.clear();
		for (size_t i = 0; i < t._ucenici.size(); i++)
			_ucenici.push_back(t._ucenici[i]);
		return *this;
	}

	Trening& operator+=(Ucenik* u) {
		if (u == nullptr)
			throw exception("Morate proslijediti ucenika!");
		_ucenici.push_back(u);
		return *this;
	}
	string GetDatum()const { return *_datum; }
	SkiTrener& GetTrener()const { return *_trener; }
	int GetBrojCasova()const { return _brojCasova; }

	int GetUcenika(Osoba& o) {
		for (size_t i = 0; i < _ucenici.size(); i++)
			if (*dynamic_cast<Ucenik*>(&o) == *_ucenici[i])
				return i;
		return -1;
	}
	void PrintUcenike() {
		cout << "---SPISAK UCENIKA---" << crt;
		for (size_t i = 0; i < _ucenici.size(); i++) {
			cout << "-- " << i + 1 << ". --";
			_ucenici[i]->Info();
		}
	}
	friend ostream& operator<<(ostream&, Trening&);
	friend bool operator==(Trening&, Trening&);
	friend bool operator<(Trening&, Trening&);
};
ostream& operator<<(ostream& COUT, Trening& t) {
	cout << crt << "::::::::::::SKI TRENING:::::::::::::::" << crt;
	cout << "Datum odrzavanja treninga: " << *t._datum << "\nBroj casova: " << t._brojCasova;
	t._trener->Info();
	t.PrintUcenike();
	return COUT;

}
bool operator==(Trening& t1, Trening& t2) {
	return t1._brojCasova == t2._brojCasova && *t1._trener == *t1._trener && *t1._datum == *t2._datum;
}
bool operator<(Trening& t1, Trening& t2) { return t1._ucenici.size() < t2._ucenici.size(); }

mutex mut;
class SkiKurs {
	string _periodOdrzavanja;
	vector<SkiTrener> _treneri;
	vector<Ucenik> _polaznici;
	vector<Trening> _treninzi;
	void sorting() {
		sort(_treneri.begin(), _treneri.end(), [](SkiTrener& t1, SkiTrener& t2)->bool {return t2<t1; });
		sort(_polaznici.begin(), _polaznici.end(), [](Ucenik& u1, Ucenik& u2)->bool {return u1 < u2; });
		sort(_treninzi.begin(), _treninzi.end(),[](Trening& t1, Trening& t2)->bool {return t1 < t2; });
	}
public:
	SkiKurs() :_periodOdrzavanja("") {}
	SkiKurs(string periodOdrzavanja){
		if (!regex_match(periodOdrzavanja, regex("\\d{1,2}[.-/]{1}\\d{1,2}[.-/]{1}\\d{4}\\s?[-]{1}\\s?\\d{1,2}[.-/]{1}\\d{1,2}[.-/]{1}\\d{4}")))
			throw exception("Period odrzavanja nije validan!(Format: 01/01/2019 - 01/02/2019)");
		_periodOdrzavanja = periodOdrzavanja;
	}
   

	Ucenik* GetUcenik(int index) {
		if (index<0 || index>(int)_polaznici.size() - 1)
			throw exception("Index nije validan!");
		return &_polaznici[index];
	}
	SkiTrener* GetSkiTrener(int index) {
		if (index<0 || index>(int)_polaznici.size() - 1)
			throw exception("Index nije validan!");
		return &_treneri[index];
	}
//Funkciju za dodavanje trenera (bez dupliranja elemenata).
	void DodajTrenera(Osoba& o) {
		SkiTrener* trener = dynamic_cast<SkiTrener*>(&o);
		if (trener == nullptr)
			return;
		for (size_t i = 0; i < _treneri.size(); i++)
			if (_treneri[i] == *trener)
				throw exception("Trener vec postoji!");
		
		_treneri.push_back(*trener);
		trener = nullptr;
	}

//Funkciju za dodavanje polaznika (bez dupliranja elemenata).
	void DodajPolaznika(Osoba& o) {
		Ucenik* ucenik = dynamic_cast<Ucenik*>(&o);
		if (ucenik == nullptr)
			return;
		for (size_t i = 0; i < _polaznici.size(); i++)
			if (_polaznici[i] == *ucenik)
				throw exception("Polaznik vec dodat!");
		_polaznici.push_back(*ucenik);
		ucenik = nullptr;
		
	}

//Funkciju za dodavanje odrzanih treninga.
	void DodajTrening(Trening& t) {
		for (size_t i = 0; i < _treninzi.size(); i++)
			if (_treninzi[i] == t)
				throw exception("Trening vec postoji!");
		_treninzi.push_back(t);
	}

//Funkciju za dodavanje uplate uceniku za odredjeni trening (dakle, funkcija prima 3 parametra: uplata, ucenik, trening). 
//Prvenstveno je potrebno provjeriti da li je ucenik uopste evidentiran na datom treningu. 

//Cijena jednog casa zavisi od tipa treninga na kojem je ucenik bio: 
//Pocetnicki - 10KM
//Rekreativni - 15KM 
//Profesionalni - 20KM 

//Uplata treba da bude tacnog iznosa proracunatog na osnovu prethodne kategorizacije i broja casova odrzanog treninga. 
//U svim slucajevima kada je neko od ogranicenja naruseno, baciti izuzetak.
	void DodajUplatuUcenikuZaTrening(Uplata& u, Osoba& o, Trening& t) {
		Ucenik* ucenik = dynamic_cast<Ucenik*>(&o);
		if (ucenik == nullptr)
			return;
		int indexTreninga = -1;
		for (size_t i = 0; i < _treninzi.size(); i++)
			if (_treninzi[i] == t)
				indexTreninga = i;
		if (indexTreninga == -1)
			throw exception("Trening ne postoji!");

		int indexUcenika = _treninzi[indexTreninga].GetUcenika(*ucenik);

		if (indexUcenika==-1)
			throw exception("Ucenik nije deo ekipe navedenog treninga!");
		Trening* temp = &_treninzi[indexTreninga];

		double potrebniIznos;
		if (temp->GetTrener().GetTipTreninga() == Pocetnicki)
			potrebniIznos = 10;
		else if (temp->GetTrener().GetTipTreninga() == Rekreativni)
			potrebniIznos = 15;
		else if (temp->GetTrener().GetTipTreninga() == Profesionalni)
			potrebniIznos = 20;
		potrebniIznos *= temp->GetBrojCasova();

		if (potrebniIznos >u.GetIznos())
			throw exception("Neuspjela uplata! Netacan iznos uplate za trening..");

		_polaznici[indexUcenika].DodajUplatu(u, "trening");
		cout << "\nUplata uspjesna! Hvala na saradnji!\n\n";
		temp = nullptr;
		ucenik = nullptr;
   }

//Funkciju koja pronalazi ski trenera sa najvecim brojem odrzanih casova i ispisuje njegov ostvareni profit.
	void PrintNajangazovanijegSkiTrenera() {
		int max = (int)_treneri.size(), najbolji = 0, *brojacCasova = nullptr;
		if (max == 0)
			return;
		else if (max == 1) {
			brojacCasova = new int(0);
			for (size_t i = 0; i < _treninzi.size(); i++)
				(*brojacCasova) += _treninzi[i].GetBrojCasova();
		}
		else if (max > 1) {
			brojacCasova = new int[max] { 0 };
			for (int i = 0; i < max; i++)
				for (size_t j = 0; j < _treneri.size(); j++)
					if (_treninzi[i].GetTrener() == _treneri[j])
						brojacCasova[j] += _treninzi[i].GetBrojCasova();

			for (int i = 1; i < max; i++)
				if (brojacCasova[i] > brojacCasova[max])
					najbolji = i;
		}

		cout << crt << "::::PODACI O NAJANGOZOVANIJEM SKI TRENERU::::" << crt;
		_treneri[najbolji].Info();
		cout << "BROJ ODRZANIH CASOVA: ";
		if (max == 1) {
			cout << *brojacCasova << endl;
			delete brojacCasova;
		}
		else {
			cout << brojacCasova[najbolji] << endl;
			delete[] brojacCasova;
		}
		brojacCasova = nullptr;
	}

//Funkciju koja pronalazi ucenika od kojeg je ostvarena najveca zarada, ukljucujuci uplate za iznajmljenu opremu.
	Osoba* GetNajvecegPotrosaca() {
		int max = (int)_polaznici.size(), najvisePotrosio = 0;
		if (max == 0)
			return nullptr;
		else if (max > 1) {
			double* potrosnje = new double[max] {0};
			for (size_t i = 0; i < _polaznici.size(); i++)
				potrosnje[i] = _polaznici[i].GetIznosSvihUplata();
			for (int i = 1; i < max; i++)
				if (potrosnje[i] > potrosnje[najvisePotrosio])
					najvisePotrosio = i;
		}
		return &_polaznici[najvisePotrosio];
	}

//Funkciju koja sortira podatke o kompletnom kursu. Podaci trebaju biti sortirani na sljedeci nacin: 
//- treneri prema kategoriji 
//- treninzi prema broju ucenika koji su prisustvovali
//- polaznici prema godistu. 
//Ovu funkciju je potrebno pozvati kreiranjem posebnog thread-a.
	void sortKurs() {
		mut.lock();
		thread t(&SkiKurs::sorting, this);
		t.join();
		mut.unlock();

	}
//Funkciju koja vrsi ispis svih podataka po kategorijama i sortirano u skladu sa zahtijevima
	void Print() {
		sortKurs();
		cout << crt << ":::::SKI KURS " << _periodOdrzavanja << ":::::" << crt;
		cout << "---Treneri na kursu---\n";
		if ((int)_treneri.size() == 0)
			cout << "-----TRENUTNO NEMA TRENERA-----\n";
		else
			for (size_t i = 0; i < _treneri.size(); i++)
				_treneri[i].Info();
		
		cout <<crt<< "---INFORMACIJE O SVIM TRENINZIMA I POLAZNICIMA----" << crt;
		for (size_t i = 0; i < _treninzi.size(); i++){
			cout <<i+1<<". \n"<< _treninzi[i] << endl;
		}
		cout << crt;
	}
};
		

int main() {
	try {
		Ucenik u1("John John",'M', 1994, false), u2("Muhammed Salman", 'M', 1987, true), u3("John Newman", 'M', 1999, false);
		Ucenik test4(u1);


		Oprema o1("Skije GT", "Fischer", 40, 3), o2("Pantalone", "Colmar", 35, 4), o3("Naocare", "Bogner", 90, 2);
		u1 += o1;
		u1 += o2;
		u1.PrintOpremu();

		u1 -= o1;

		//NAKON BRISANJA 
		u1.PrintOpremu();

		u1 += o1;

		Uplata uplata1("01/01/2019", 106), uplata2("20/12/2018", 80), uplata3("18/01/2019", 42.5);
		u1.DodajUplatu(uplata1, "oprema");
		u1.DodajUplatu(uplata2, "Oprema");
		u1.DodajUplatu(uplata3, "oprema");
		
		u2.DodajUplatu(uplata1, "oprema");
		u2.DodajUplatu(uplata2, "oprema");
		u2.DodajUplatu(uplata3, "oprema");
		u2.DodajUplatu(uplata1, "oprema");

		Ucenik test5;
		test5 = u1;
		test5.Info();

		SkiTrener st1("Profesor Profesor", 1979, true, 'M', Pocetnicki), st2("Profesorica Profesorica", 1985, false, 'z', Rekreativni);
		SkiTrener testst1, testst2(st2);
		testst1 = st1;
		testst1.Info();
		testst2.Info();

		Trening trening1("01.02.2019", &st1, 8);

		trening1 += &u1;
		trening1 += &u2;
		trening1 += &u3;
		cout << trening1 << endl;

		Trening testTrening1(trening1);
		cout << testTrening1 << endl;

		Trening testTrening2;
		testTrening2 = testTrening1;
		cout << testTrening2 << endl;

		SkiKurs skiKurs("01/01/2019 - 17/01/2019");

		skiKurs.DodajTrenera(st1);
		skiKurs.DodajTrenera(st2);

		skiKurs.DodajPolaznika(u1);
		skiKurs.DodajPolaznika(u2);
		skiKurs.DodajPolaznika(u3);

		Trening trening2("05/01/2019", skiKurs.GetSkiTrener(0), 7);
		Trening trening3("8/01/2019", skiKurs.GetSkiTrener(1), 12);

		trening2 += skiKurs.GetUcenik(0);
		trening2 += skiKurs.GetUcenik(1);
		trening2 += skiKurs.GetUcenik(2);

		trening2.PrintUcenike();
		skiKurs.DodajTrening(trening2);

		trening3 += skiKurs.GetUcenik(1);
		trening3 += skiKurs.GetUcenik(2);

		
		skiKurs.DodajTrening(trening3);

		
		skiKurs.Print(); //---> ispisuje elemente sortirane DESC po zahtjevanim uslovima
		                 //USLOVI: 
		                 //- treneri prema kategoriji DESC
                         //- treninzi prema broju ucenika koji su prisustvovali DESC
                         //- polaznici prema godistu. DESC

		skiKurs.PrintNajangazovanijegSkiTrenera();

		Osoba* najvisePotrosio = skiKurs.GetNajvecegPotrosaca();
		cout << "\n\n--UCENIK KOJI JE POTROSIO NAJVISE NOVCA--\n";
		najvisePotrosio->Info();

		skiKurs.DodajUplatuUcenikuZaTrening(uplata1, u1, trening2);
		najvisePotrosio = nullptr;
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}
	system("pause"); 
	return 0;
		
}