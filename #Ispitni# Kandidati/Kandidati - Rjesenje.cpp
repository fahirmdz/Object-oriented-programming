#include"pch.h"
#include <iostream>
#include<cstring>
#include<string>
#include<exception>
#include<regex>
#include<thread>
#include<iterator>
#include<algorithm>
#include<ctime>
#include<vector>
#include<sstream>
#include<chrono>
using namespace std;

#pragma warning(disable:4996)
const char *crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char* razredChar[] = { "Prvi","Drugi","Treci","Cetvrti" };
const float minimalan_prosjek = 4.5;

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
	T1 * _elementi1;
	T2 * _elementi2;
	int * _trenutno;
	void copyElements(const Kolekcija& k) {
		if (*k._trenutno > 0) {
			_elementi1 = new T1[*k._trenutno];
			_elementi2 = new T2[*k._trenutno];
			for (int i = 0; i < *k._trenutno; i++) {
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
	Kolekcija():_trenutno(new int(0)) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& k) :_trenutno(new int(*k._trenutno)) {
		copyElements(k);
	}
	~Kolekcija() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
		delete _trenutno; _trenutno = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		*_trenutno = *k._trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		copyElements(k);
		return *this;
	}
	T1& getElement1(int lokacija) { 
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw exception("Index lokacije elementa nije validan!");
		return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) {
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw exception("Index lokacije elementa nije validan!");
		return _elementi2[lokacija]; }

	void AddElement(T1 t1, T2 t2) {
		T1* temp1 = new T1[*_trenutno + 1];
		T2* temp2 = new T2[*_trenutno + 1];

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

	void SortOpadajuciByT2() {
		for (int i = 0; i < *_trenutno - 1; i++) {
			int max = i;
			for (int j = i + 1; j < *_trenutno; j++)
				if (_elementi2[max] < _elementi2[j])
					max = j;
			if (max != i) {
				swap(_elementi1[i], _elementi1[max]);
				swap(_elementi2[i], _elementi2[max]);
			}
		}
	}
	int getTrenutno()const { return *_trenutno; }

	friend ostream& operator<<<> (ostream &,const Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
};

template<class T1,class T2>
ostream& operator<< (ostream &COUT,const Kolekcija<T1,T2>& obj) {
	for (int i = 0; i < *obj._trenutno; i++)
		COUT << obj._elementi1[i] << " -  " << obj._elementi2[i] << endl;
	return COUT;
}
template<class T1, class T2>
bool operator==(Kolekcija<T1, T2>& k1, Kolekcija<T1, T2>& k2) {
	if (*k1._trenutno == *k2._trenutno) {
		for (int i = 0; i < *k1._trenutno; i++)
			if (k1._elementi1[i] != k2._elementi1[i] || k1._elementi2[i] != k2._elementi2[i])
				return false;
		return true;
	}
	return false;
}

class DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;

	int GetDanaMjesec(int mjesec, int godina) {
		if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 10 || mjesec == 12)
			return 31;
		else if (mjesec == 2) {
			if ((godina % 100 == 0 && godina % 400 == 0) || godina % 4 == 0)
				return 29;
			else
				return 28;
		}
		else return 30;
	}

	bool validneVrijednosti(int dan, int mjesec, int godina, int sat, int minut) {
		if (godina < 1920 || mjesec < 1 || mjesec>12 || dan<1 || dan>GetDanaMjesec(mjesec, godina) || sat < 0 || sat>23 || minut < 0 || minut>59)
			return false;
		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		int trDan = timeinfo.tm_mday, trMjesec = timeinfo.tm_mon + 1, trGodina = timeinfo.tm_year + 1900, trSat = timeinfo.tm_hour, trMinut = timeinfo.tm_min;
		if (godina > trGodina)
			return false;
		if (godina == trGodina && mjesec > trMjesec)
			return false;
		else if (trMjesec == mjesec && dan > trDan)
			return false;
		else if (trDan == dan && sat > trSat)
			return false;
		else if (trSat == sat && minut > trMinut)
			return false;
		return true;
	}
	int compareDoMinuta(DatumVrijeme d) {
		if (*_godina < *d._godina)
			return 1;
		else if (*_godina > *d._godina)
			return -1;
		else if (*_mjesec < *d._mjesec)
			return 1;
		else if (*_mjesec > *d._mjesec)
			return -1;
		else if (*_dan < *d._dan)
			return 1;
		else if (*_dan > *d._dan)
			return -1;
		else if (*_sati < *d._sati)
			return 1;
		else if (*_sati > *d._sati)
			return -1;
		return 0;
	}
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		if (!validneVrijednosti(dan, mjesec, godina, sati, minuti))
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& d):_dan(new int(*d._dan)),_mjesec(new int(*d._mjesec)),_godina(new int(*d._godina)),_sati(new int(*d._sati)),
		_minuti(new int(*d._minuti)){}
	~DatumVrijeme() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	DatumVrijeme& operator=(const DatumVrijeme& d) {
		if (&d == this)
			return *this;
		delete _dan; _dan = new int(*d._dan);
		delete _mjesec; _mjesec = new int(*d._mjesec);
		delete _godina; _godina = new int(*d._godina);
		delete _sati; _sati = new int(*d._sati);
		delete _minuti; _minuti = new int(*d._minuti);
		return *this;
	}

	int compare(DatumVrijeme d) {
		int compareDoMin = compareDoMinuta(d);
		if (compareDoMin == 1)
			return 1;
		else if (compareDoMin == -1)
			return - 1;
		else if (*_minuti < *d._minuti)
			return 1;
		else if (*_minuti > *d._minuti)
			return -1;
		return 0;
	}
	bool prosaoMinut(DatumVrijeme d) {
		int compareDoMin = compareDoMinuta(d);

		if (compareDoMin == 1)
			return true;
		else if (compareDoMin == -1)
			return false;
		else if ((*d._minuti - *_minuti) >= 1)
			return true;
		return false;
	}
	char* ToCharArray() {
		stringstream x;
		x << *_dan << "/" << *_mjesec << "/" << *_godina << " " << *_sati << ":" << *_minuti;
		return AlocirajNizKaraktera(x.str().c_str());
	}

	friend ostream& operator<< (ostream&,const DatumVrijeme&);
	friend bool operator==(DatumVrijeme&, DatumVrijeme&);
	friend bool operator!=(DatumVrijeme&, DatumVrijeme&);
};
ostream& operator<< (ostream &COUT,const DatumVrijeme &obj) {
	COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
	return COUT;
}
bool operator==(DatumVrijeme& d1, DatumVrijeme& d2) {
	return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan && *d1._sati == *d2._sati && *d1._minuti == *d2._minuti;
}
bool operator!=(DatumVrijeme& d1, DatumVrijeme& d2) { 
if(d1==d2)
	return false;
return true;
}


class Predmet {
	char * _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char * naziv="NEMA VRIJEDNOST", int ocjena=5, string napomena = ""){
		if (ocjena < 1 || ocjena>5)
			throw exception("Ocjena za predmet nije validna! (1-5)");
		if (naziv == nullptr || (strcmp(naziv, "NEMA VRIJEDNOST") != 0 && !regex_match(naziv, regex("^[a-zA-Z\\s]{2,15}$"))))
			throw exception("Naziv predmeta nije validan!");
		_naziv = AlocirajNizKaraktera(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet& p):_naziv(AlocirajNizKaraktera(p._naziv)),_ocjena(p._ocjena),_napomena(p._napomena){}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	Predmet& operator=(const Predmet& p) {
		if (&p == this)
			return *this;

		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(p._naziv);
		_ocjena = p._ocjena;
		_napomena = p._napomena;
		return *this;
	}
	__declspec(property(get = GetOcjenu))int Ocjena;
	int GetOcjenu()const { return _ocjena; }

	__declspec(property(get = GetNaziv))char* Naziv;
	char * GetNaziv()const { return _naziv; }

	_declspec(property(get = GetNapomena,put=DodajNapmomenu))string Napomena;
	string GetNapomena()const { return _napomena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
	friend ostream& operator<< (ostream &, Predmet &);
	friend bool operator==(Predmet&, Predmet&);
	friend bool operator!=(Predmet&, Predmet&);

};
ostream& operator<< (ostream &COUT, Predmet &obj) {
	COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
	return COUT;
}
bool operator==(Predmet& p1, Predmet& p2) { return strcmp(p1._naziv, p2._naziv) == 0 && p1._napomena == p2._napomena && p1._ocjena == p2._ocjena; }
bool operator!=(Predmet& p1, Predmet& p2) { 
if(p1==p2)
	return false;
return true;
}

struct Uspjeh {
	eRazred _razred;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet*, DatumVrijeme> * _predmeti;

	void dealocirajPredmete() {
		for (int i = 0; i < _predmeti->getTrenutno(); i++) {
			delete _predmeti->getElement1(i); 
			_predmeti->getElement1(i) = nullptr;
		}
	}
	void copyKolekcija(const Kolekcija<Predmet*,DatumVrijeme>* k) {
		Kolekcija<Predmet*, DatumVrijeme>& temp = *const_cast<Kolekcija<Predmet*, DatumVrijeme>*>(k);
		for (int i = 0; i < k->getTrenutno(); i++)
			_predmeti->AddElement(new Predmet(*temp.getElement1(i)), temp.getElement2(i));
	}
public:
	Uspjeh(eRazred razred=PRVI):_razred(razred) {
		_predmeti = new Kolekcija<Predmet*, DatumVrijeme>;
	}
	Uspjeh(const Uspjeh& u) :_razred(u._razred) {
		_predmeti=new Kolekcija<Predmet*, DatumVrijeme>;
		copyKolekcija(u._predmeti);
	}
	~Uspjeh() {
		dealocirajPredmete();
		delete _predmeti; _predmeti = nullptr;
	}
	Uspjeh& operator=(const Uspjeh& u) {
		if (&u == this)
			return *this;
		_razred = u._razred;
		dealocirajPredmete();
		delete _predmeti;
		_predmeti = new Kolekcija<Predmet*, DatumVrijeme>;
		copyKolekcija(u._predmeti);
		return *this;
	}

	__declspec(property(get = GetRazred))eRazred Razred;
	eRazred GetRazred()const { return _razred; }

	Kolekcija<Predmet*, DatumVrijeme> * GetPredmeti() { return _predmeti; }
	int GetBrojPredmeta()const { return _predmeti->getTrenutno(); }
	
	bool predmetPostoji(const char* naziv) {
		for (int i = 0; i < _predmeti->getTrenutno(); i++)
			if (strcmp(_predmeti->getElement1(i)->Naziv,naziv)==0)
				return true;
		return false;
	}
	double GetProsjek() {
		double prosjek = 0;
		for (int i = 0; i < _predmeti->getTrenutno(); i++)
			prosjek += _predmeti->getElement1(i)->Ocjena;
		return (double)prosjek / _predmeti->getTrenutno();
	}
	void AddPredmet(Predmet& p, DatumVrijeme d) {
		_predmeti->AddElement(new Predmet(p), d);
	}
	friend ostream& operator<< (ostream &, const Uspjeh &);
	friend bool operator==(Uspjeh&, Uspjeh&);
	friend bool operator!=(Uspjeh&, Uspjeh&);

};
ostream& operator<< (ostream &COUT, const Uspjeh &obj) {
	COUT << crt << "Razred: " << razredChar[obj._razred - 1] << crt << "::::PREDMETI::::" << crt;
	for (int i = 0; i < obj._predmeti->getTrenutno(); i++)
		cout << *obj._predmeti->getElement1(i) << "(Evidentirano: " << obj._predmeti->getElement2(i) << crt;
	return COUT;
}
bool operator==(Uspjeh& u1, Uspjeh& u2) { 
		if ((u1._razred == u2._razred && u1._predmeti->getTrenutno() == u2._predmeti->getTrenutno())){
			for (int i = 0; i < u1._predmeti->getTrenutno(); i++)
				if (*u1._predmeti->getElement1(i) != *u2._predmeti->getElement1(i) || u1._predmeti->getElement2(i) != u2._predmeti->getElement2(i))
					return false;
			return true;
		}
		return false;
}
bool operator!=(Uspjeh& u1, Uspjeh& u2) { 
if(u1==u2)
    return false;
return true;
}

class Kandidat {
	char * _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh *> _uspjeh;

	void Dealociraj() {
		delete[] _imePrezime;
		for (int i = 0; i < _uspjeh.size(); i++) {
			delete _uspjeh[i];
			_uspjeh[i] = nullptr;
		}
	}

	DatumVrijeme* poslednjiDodan() {
		DatumVrijeme* zadnjeDodavanje = &_uspjeh[0]->GetPredmeti()->getElement2(_uspjeh[0]->GetPredmeti()->getTrenutno()-1);
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			int sizeTemp = _uspjeh[i]->GetPredmeti()->getTrenutno() - 1;
			if (_uspjeh[i]->GetPredmeti()->getElement2(sizeTemp).compare(*zadnjeDodavanje) == -1)
				zadnjeDodavanje = &_uspjeh[i]->GetPredmeti()->getElement2(sizeTemp);
		}
		return zadnjeDodavanje;
	}

	void sendEmail(eRazred razred) {
		cout << crt << ":::Salje se E-mail...(LOADING)" << crt;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "To: " << _emailAdresa << endl;
		cout << "\nPostovani " << _imePrezime << "\n\n" << "Uspjesno ste evidentirali uspjeh za " << razredChar[razred - 1] << " razred!\n\nLijep pozdrav,\nFIT Osoblje" << crt;
	}
	void sendSMS(double uspjeh) {
		cout << crt << ":::Salje se SMS poruka...(LOADING)" << crt;
		this_thread::sleep_for(chrono::seconds(2));
		cout << "To: " << _brojTelefona << endl;
		cout << "\nPostovani " << _imePrezime << "\n\n" << "Cesititamo na ostvarenom uspjehu od " << uspjeh;
		cout<< "\nSvaka cast!\n\nLijep pozdrav,\nFIT Osoblje" << crt;
	}
public:
	Kandidat(const char * imePrezime = "NEMA VRIJEDNOST", string emailAdresa = "", string brojTelefona = "") {
		if (imePrezime == nullptr || (strcmp(imePrezime, "NEMA VRIJEDNOST") != 0 && !regex_match(imePrezime, regex("[A-Z]{1}[a-z]{1,15}\\s{1,2}[A-Z]{1}[a-z]{1,15}$"))))
			throw exception("Ime i prezime kandidata nisu validni!");
		if (emailAdresa != "" && !regex_match(emailAdresa, regex("[a-z]{3,12}\\@(edu.fit.ba|live.com|hotmail.com|yahoo.com|gmail.com)")))
			throw exception("E-mail adresa za kandidate ne posjeduje validan format!");
		if (brojTelefona != "" && !regex_match(brojTelefona, regex("\\+?\\(?\\d{2,3}\\)?[-\\s/]?\\d{3}[-\\s/]?\\d{3}")))
			throw exception("Broj telefona za kandidate ne posjeduje validan format!");
		_emailAdresa = emailAdresa;
		_brojTelefona = brojTelefona;
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	Kandidat(const Kandidat& k) :_imePrezime(AlocirajNizKaraktera(k._imePrezime)), _emailAdresa(k._emailAdresa), _brojTelefona(k._brojTelefona) {
		for (size_t i = 0; i < k._uspjeh.size(); i++)
			_uspjeh.push_back(new Uspjeh(*k._uspjeh[i]));
	}
	~Kandidat() {
		Dealociraj();
	}
	Kandidat& operator=(const Kandidat& k) {
		if (&k == this)
			return *this;
		Dealociraj();
		_imePrezime = AlocirajNizKaraktera(k._imePrezime);
		for (size_t i = 0; i < k._uspjeh.size(); i++)
			_uspjeh.push_back(new Uspjeh(*k._uspjeh[i]));
		_emailAdresa = k._emailAdresa;
		_brojTelefona = k._brojTelefona;
		return *this;
	}

	__declspec(property(get = GetImePrezime))char* ImePrezime;
	__declspec(property(get = GetBrojTelefona))string BrojTelefona;
	__declspec(property(get = GetEmail))string Email;

	char* GetImePrezime()const { return _imePrezime; }
	string GetBrojTelefona()const { return _brojTelefona; }
	string GetEmail()const { return _emailAdresa; }

	vector<Uspjeh *> GetUspjeh() { return _uspjeh; }

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje vise od 6 predmeta za jedan razred
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
	razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	bool AddPredmet(eRazred razred,Predmet& p,DatumVrijeme date) {
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			if (_uspjeh[i]->Razred == razred && _uspjeh[i]->GetBrojPredmeta() == 6) 
				return false;
			if (_uspjeh[i]->Razred == razred &&  _uspjeh[i]->predmetPostoji(p.Naziv))
				return false;
		}
		if ((int)_uspjeh.size() > 0)
			if (!poslednjiDodan()->prosaoMinut(date))
				return false;
		int indexRazreda = -1;
		for (size_t i = 0; i < _uspjeh.size(); i++)
			if (_uspjeh[i]->Razred == razred)
				indexRazreda = i;
		if (indexRazreda == -1) {
			_uspjeh.push_back(new Uspjeh(razred));
			_uspjeh[(int)_uspjeh.size() - 1]->AddPredmet(p, date);
		}
		else {
			_uspjeh[indexRazreda]->AddPredmet(p, date);
			if (_uspjeh[indexRazreda]->GetBrojPredmeta() == 6) {
				thread tEmail(&Kandidat::sendEmail, this, razred);
				tEmail.join();
				double uspjeh = _uspjeh[indexRazreda]->GetProsjek();
				if (uspjeh > minimalan_prosjek) {
					thread tSMS(&Kandidat::sendSMS, this, uspjeh);
					tSMS.join();
				}

			}
		}
		return true;
	}
	Uspjeh* operator()(eRazred razred) {
		for (size_t i = 0; i < _uspjeh.size(); i++)
			if (_uspjeh[i]->Razred == razred)
				return _uspjeh[i];
		return nullptr;
	}
	friend ostream& operator<< (ostream &,const Kandidat &);
	friend bool operator==(Kandidat&, Kandidat&);
	friend bool operator!=(Kandidat&, Kandidat&);
};
ostream& operator<< (ostream &COUT,const Kandidat &obj) {
	COUT << crt << "Ime i prezime: " << obj._imePrezime << "\nE-mail:  " << obj._emailAdresa << "\nBroj telefona: " << obj._brojTelefona << crt;
	cout << ":::::Ostvareni uspjeh:::::" << crt;
	for (size_t i = 0; i < obj._uspjeh.size(); i++)
		COUT << *obj._uspjeh[i] << endl;
	cout << crt;
	return COUT;
}
bool operator==(Kandidat& k1, Kandidat& k2) {
	return strcmp(k1._imePrezime, k2._imePrezime) && k1._emailAdresa == k2._emailAdresa && k1._brojTelefona == k2._brojTelefona;
}
bool operator!=(Kandidat& k1, Kandidat& k2) {
	if(k1==k2)
		return false;
return true;
}
void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUÈAJU DA POSTOJI
	   ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/

	try {
		DatumVrijeme temp,
			datum19062018_1015(19, 6, 2018, 10, 15),
			datum20062018_1115(20, 6, 2018, 11, 15),
			datum30062018_1215(30, 6, 2018, 12, 15),
			datum05072018_1231(5, 7, 2018, 12, 31),
			datum20062018_1115_Copy(datum20062018_1115);
		//funkcija ToCharArray vraca datum i vrijeme kao char *
		cout << datum19062018_1015.ToCharArray() << endl;//treba ispisati: 19.6.2018 10:15
		temp = datum05072018_1231;
		cout << temp.ToCharArray() << endl;//treba ispisati: 5.7.2018 12:31
		cout << datum20062018_1115_Copy.ToCharArray() << endl;//treba ispisati: 20.6.2018 11:15

		const int kolekcijaTestSize = 10;
		Kolekcija<int, int> kolekcija1;
		for (size_t i = 0; i < kolekcijaTestSize; i++)
			kolekcija1.AddElement(i + 1, i * i);
		cout << kolekcija1 << endl;

		Kolekcija<int, int> kolekcija2 = kolekcija1;
		cout << kolekcija2 << crt;
		Kolekcija<int, int> kolekcija3;
		kolekcija3 = kolekcija1;
		kolekcija3.SortOpadajuciByT2(); //od najvece prema najmanjoj vrijednosti
		cout << kolekcija3 << crt;

		//napomena se moze dodati i prilikom kreiranja objekta
		Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
			Fizika("Fizika", 5),
			Hemija("Hemija", 5),
			Engleski("Engleski", 5),
			Fizicko("Fizicko", 5),
			Biologija("Biologija", 3),
			OTO("OTO", 4),
			Francuski("Francuski", 5),
			Istorija("Istorija", 5);
		Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh"); // dodaje novu napomenu zadrzavajuci ranije dodane
		cout << Matematika << endl;

		Kandidat jasmin("Jasmin Azemovic", "jasmin@gmail.com", "033 281 172");
		Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");

		/*
		uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
		tom prilikom onemoguciti:
		- dodavanje vise od 6 predmeta za jedan razred
		- dodavanje istoimenih predmeta na nivou jednog razreda,
		- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje minut).
		razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II
		razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
		*/
		if (jasmin.AddPredmet(DRUGI, Fizika, datum20062018_1115))
			cout << "Predmet uspjesno dodan!" << crt;
		if (jasmin.AddPredmet(DRUGI, Hemija, datum30062018_1215))
			cout << "Predmet uspjesno dodan!" << crt;

		if (jasmin.AddPredmet(DRUGI, Fizicko, DatumVrijeme(30,06,2018,12,16)))
			cout << "Predmet uspjesno dodan!" << crt; 
		if (jasmin.AddPredmet(DRUGI, Biologija, DatumVrijeme(30, 06, 2018, 12, 18)))
			cout << "Predmet uspjesno dodan!" << crt;
		if (jasmin.AddPredmet(DRUGI, Francuski, DatumVrijeme(30, 06, 2018, 12, 25)))
			cout << "Predmet uspjesno dodan!" << crt;
		if (jasmin.AddPredmet(DRUGI, Istorija, DatumVrijeme(30, 06, 2018, 12, 50)))
			cout << "Predmet uspjesno dodan!" << crt;

		if (jasmin.AddPredmet(PRVI, Engleski, datum19062018_1015))
			cout << "Predmet uspjesno dodan!" << crt;
		if (jasmin.AddPredmet(PRVI, Matematika, datum20062018_1115))
			cout << "Predmet uspjesno dodan!" << crt;
		//ne treba dodati Matematiku jer je vec dodana u prvom razredu
		if (jasmin.AddPredmet(PRVI, Matematika, datum05072018_1231))
			cout << "Predmet uspjesno dodan!" << crt;
		//ne treba dodati Fiziku jer nije prosao minut od dodavanja posljednjeg predmeta
		if (jasmin.AddPredmet(PRVI, Fizika, datum05072018_1231))
			cout << "Predmet uspjesno dodan!" << crt;

		/*pretpostavka je da na nivou jednog razreda kandidati imaju 6 predmeta.
		upravo zbog toga, nakon evidentiranja 6 predmeta na nivou jednog razreda kandidatu se salje mail
		sa porukom: "evidentirali ste uspjeh za X razred". ukoliko je prosjek na nivou tog razreda veci od minimalan_prosjek
		kandidatu se salje SMS sa porukom: "svaka cast za uspjeh 4.X". slanje poruka i emailova implemenitrati koristeci
		zasebne thread-ove.
		*/

		Uspjeh * U = jasmin(PRVI);//vraca uspjeh kandidata ostvaren u prvom razredu
		if (U != nullptr)
			cout << *U << endl;
		U = nullptr;
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}

	system("pause>0");
}
