//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<exception>
#include<vector>
#include<regex>
#include<thread>
#include<mutex>
#include<future>
#include<string>
#include<cstring>
#include<cctype>
#include<ctime>
#include<chrono>
using namespace std;

const char *crt = "\n-----------------------------------------------\n";

enum Status { ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA };
enum Sortiranje { RASTUCI, OPADAJUCI };
enum Drzava { SAD, AUSTRALIJA, ENGLESKA };
const char* StatusChar[] = { "ZAHTJEV PRIMLJEN","ODUSTAO","ODBIJENA","IZDATA","ISTEKLA","PONISTENA" };

const char* DrzavaChar[] = { "SAD","Australija","Engleska" };


char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Datum {
	int *_dan, *_mjesec, *_godina;


	bool validneVrijednosti(int a,int b,int c) {
		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		int trenutnaGodina = timeinfo.tm_year + 1900, trenutniMjesec = timeinfo.tm_mon + 1, trenutniDat = timeinfo.tm_mday;
		if (a<1 || a>GetBrojDanaMjesec(b,c))
			return false;
		else if (b < 1 || b>12)
			return false;
		else if (c<1920 || c>trenutnaGodina)
			return false;
		if (c == trenutnaGodina && (b > trenutniMjesec || a > trenutniDat))
			return false;
		return true;
	}
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		if (!validneVrijednosti(dan, mjesec, godina))  //OBEZBJEDJUJE DA NE MOZEMO DODATI DATUM KOJI NIJE VALIDAN, NITI DATUM U BUDUCNOSTI
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& d) :_dan(new int(*d._dan)), _mjesec(new int(*d._mjesec)), _godina(new int(*d._godina)) {}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
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
	int GetBrojDanaMjesec(int m,int g) {
		int i = m;
		if (i == 3 || i == 1 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			return 31;
		else if (i == 2) {
			if (g % 400 == 0 && g % 100 == 0 && g % 4 == 0)
				return  29;
			else
				return 28;
		}
		else
			return 30;
	}
	Datum& operator++() {
		(*_dan)++;
		if (*_dan > GetBrojDanaMjesec(*_mjesec,*_godina))
		{
			*_dan = 1;
			(*_mjesec)++;
		}
		if (*_mjesec > 12) {
			*_mjesec = 1;
			(*_godina)++;
		}
		return *this;
	}

	int isStariji(Datum d) {
		if (*_godina > *d._godina)
			return -1;
		else if (*_godina < *d._godina)
			return 1;
		else if (*_mjesec > *d._mjesec)
			return -1;
		else if (*_mjesec < *d._mjesec)
			return 1;
		else if (*_dan > *d._dan)
			return -1;
		else if (*_dan < *d._dan)
			return 1;
		else
			return 0;
	}
	bool StarijiOdDesetGodina() {

		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		Datum temp(timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1890);           //inace se dodaje 1900 za trenutnu godinu, a mi smo od toga oduzeli 10 godina
		return temp.isStariji(*this) <= 0; //gronja granica mladja ili jednaka datumu rodjenja          sto bi trebalaa biti granica gornja granica za datum rodjenja
	}
	friend ostream& operator<< (ostream &, Datum &);
	friend bool operator==(Datum&, Datum&);
	friend bool operator!=(Datum&, Datum&);

};


ostream& operator<< (ostream &COUT, Datum &obj) {
	COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
	return COUT;
}
bool operator==(Datum& d1, Datum& d2) {
	return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
}
bool operator!=(Datum& d1, Datum& d2) {return !(d1==d2);}

template<class T1, class T2, int max>
class FITKolekcija {
	T1 * _elementi1[max];
	T2 * _elementi2[max];
	int _trenutno;

	void copyElements(const FITKolekcija& fk) {
		for (int i = 0; i < _trenutno; i++) {
			_elementi1[i] = new T1(*fk._elementi1[i]);
			_elementi2[i] = new T2(*fk._elementi2[i]);
		}
		for (int i = _trenutno; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}

public:
	FITKolekcija() {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& fk) :_trenutno(fk._trenutno) {
		copyElements(fk);
	}
	~FITKolekcija() {
		for (int i = 0; i < max; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	FITKolekcija& operator=(const FITKolekcija& fk) {
		if (&fk == this)
			return *this;
		_trenutno = fk._trenutno;
		if (_trenutno > 0)
			for (int i = 0; i < _trenutno; i++) {
				delete _elementi1[i];
				delete _elementi2[i];
			}
		copyElements(fk);

		return *this;
	}
	void Dodaj(T1& t1, T2& t2) {
		if (max == _trenutno)
			throw exception("Dostigli ste maksimum, ne mozete dodati vise elemenata u kolekciju!");
		_elementi1[_trenutno] = new T1(t1);
		_elementi2[_trenutno++] = new T2(t2);
	}
	T1 ** GetT1() { return _elementi1; }
	T2 ** GetT2() { return _elementi2; }

	T1& GetElement1(int index) {
		if (index < 0 || index >= _trenutno)
			throw exception("Lokacija elementa nije validna!");
		return *_elementi1[index];
	}
	T2& GetElement2(int index) {
		if (index < 0 || index >= _trenutno)
			throw exception("Lokacija elementa nije validna!");
		return *_elementi2[index];
	}

	int GetTrenutno() { return _trenutno; }

	void Sortiraj(Sortiranje sortiranje) {
		if (_trenutno <= 1)
			return;

		for (int i = 0; i < _trenutno - 1; i++) {
			int flag = i;

			if (sortiranje == RASTUCI)
				for (int j = i + 1; j < _trenutno; j++)
					if (*_elementi1[j] < *_elementi1[flag])
						flag = j;

			if (sortiranje == OPADAJUCI)
				for (int j = i + 1; j < _trenutno; j++)
					if (*_elementi1[j] > *_elementi1[flag])
						flag = j;

			if (flag != i) {
				swap(*_elementi1[i], *_elementi1[flag]);
				swap(*_elementi2[i], *_elementi2[flag]);
			}
		}
	}
	friend ostream& operator<<<>(ostream &, FITKolekcija &);
	friend bool operator==<>(FITKolekcija&, FITKolekcija&);
};
template<class T1, class T2, int max>
ostream& operator<< <>(ostream &COUT, FITKolekcija<T1, T2, max> &obj) {
	for (int i = 0; i < obj._trenutno; i++)
		COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
	return COUT;
}

template<class T1, class T2, int max>
bool operator==<>(FITKolekcija<T1, T2, max>& fk1, FITKolekcija<T1, T2, max>& fk2) {
	if (fk1._trenutno == fk2._trenutno) {
		if (fk1._trenutno > 0)
			for (int i = 0; i < fk1._trenutno; i++)
				if (*fk1._elementi1[i] != *fk2._elementi1[i] || *fk1._elementi2[i] != *fk2._elementi2[i])
					return false;
		return true;
	}
	return false;
}

class Viza abstract {
protected:
	Drzava _drzava;
	Datum _vazenjeOD;
	Datum _vazenjeDO;
	char * _napomena;
	// string se odnosi na razlog promjene statusa, a moguæe je samo 10 izmjena statusa
	FITKolekcija<Status, string, 10> _statusi;
public:
	Viza() :_napomena(AlocirajNizKaraktera(" ")) {}
	Viza(Drzava drzava, Datum _OD, Datum _DO, const char * napomena = nullptr) :_drzava(drzava), _vazenjeDO(_DO), _vazenjeOD(_OD) {
		if (napomena == nullptr)
			_napomena = AlocirajNizKaraktera(" ");
		else
			_napomena = AlocirajNizKaraktera(napomena);
	}
	Viza(const Viza& v) :_drzava(v._drzava), _vazenjeDO(v._vazenjeDO), _vazenjeOD(v._vazenjeOD), _napomena(AlocirajNizKaraktera(v._napomena)),
	_statusi(v._statusi){}
	virtual ~Viza() { delete[] _napomena; _napomena = nullptr; }

	Viza& operator=(const Viza& v) {
		if (&v == this)
			return *this;
		delete _napomena;
		_napomena = AlocirajNizKaraktera(v._napomena);
		_drzava = v._drzava;
		_vazenjeDO = v._vazenjeDO;
		_vazenjeOD = v._vazenjeOD;
		_statusi = v._statusi;
		return *this;
	}

	Datum GetVazenjeOD() { return _vazenjeOD; }
	Datum GetVazenjeDO() { return _vazenjeDO; }
	char * GetNapomena() { return _napomena; }
	Drzava GetDrzava() { return _drzava; }

	void SetNapomena(const char* nap) {
		delete[] _napomena;
		_napomena = AlocirajNizKaraktera(nap);
	}
	Status GetPoslednjiStatus() { return _statusi.GetElement1(_statusi.GetTrenutno() - 1); }
	int GetBrojStatusa() { return _statusi.GetTrenutno(); }
	FITKolekcija<Status, string, 10>&  GetStatuse() { return _statusi; }

	//Drzava _drzava;
	//Datum _vazenjeOD;
	//Datum _vazenjeDO;
	//char * _napomena;
	//// string se odnosi na razlog promjene statusa, a moguæe je samo 10 izmjena statusa
	//FITKolekcija<Status, string, 10> _statusi;
	virtual void Info() {
		cout << ":::::INFO:::::" << crt;
		cout << "Drzava: " << DrzavaChar[_drzava];
		cout << "\nVazi od: " << _vazenjeOD << endl;
		cout << "Vazi do: " << _vazenjeDO << endl;
		cout << "Napomena: " << _napomena << endl;
		int brStatusa = _statusi.GetTrenutno();
		cout << "Status: ";
		if(brStatusa>0)
			cout<<StatusChar[_statusi.GetElement1(brStatusa - 1)] << crt;
		else
			cout<< StatusChar[_statusi.GetElement1(brStatusa)] << crt;
	}

	friend bool operator==(Viza&, Viza&);
	friend bool operator!=(Viza&, Viza&);
};
bool operator==(Viza& v1, Viza& v2) {
	return v1._drzava == v2._drzava && v1._vazenjeDO == v2._vazenjeDO && v1._vazenjeOD == v2._vazenjeOD;
}
bool operator!=(Viza& v1, Viza& v2) {return !(v1==v2);}

class PoslovnaViza :public Viza {
	string _kompanija; //naziv kompanije koja potvrðuje da posluje sa aplikantom
public:
	PoslovnaViza() :_kompanija("") {}
	PoslovnaViza(Drzava drzava, Datum _OD, Datum _DO, string kompanija, const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		if (!regex_match(kompanija, regex("[a-zA-Z0-9\\s,-]+")))
			throw exception("Naziv firme nije validan!");
		_kompanija = kompanija;
	}

	string GetKompanija() { return _kompanija; }

	void Info() {
		cout << crt << ":::::POSLOVNA VIZA:::::" << crt;
		Viza::Info();
		cout << "Naziv kompanije: " << _kompanija << crt;
	}

	friend ostream & operator <<(ostream&, PoslovnaViza&);
};
ostream & operator << (ostream & COUT, PoslovnaViza & obj) {
	obj.Info();
	return COUT;
}

class TuristickaViza : public Viza {
	string _adresa; //adresa hotele ili drugog smještaja na kojoj æe aplikant boraviti
public:
	TuristickaViza() :_adresa("") {}
	TuristickaViza(Drzava drzava, Datum _OD, Datum _DO, string adresa,const char * napomena = nullptr) : Viza(drzava, _OD, _DO, napomena) {
		if (!regex_match(adresa, regex("^[a-zA-Z0-9\\s,-]+$")))
			throw exception("Adresa za turisticku vizu nije validna!");
		_adresa = adresa;
	}

	string GetAdresa() { return _adresa; }
	void Info() {
		cout << crt << ":::::TURISTICKA VIZA:::::" << crt;
		Viza::Info();
		cout << "Adresa: " << _adresa << crt;
	}
	friend ostream & operator << (ostream &, TuristickaViza &);
};
ostream & operator << (ostream & COUT, TuristickaViza & obj) {
	obj.Info();
	return COUT;
}

class Aplikant {
	char * _imePrezime;
	Datum * _datumRodjenja;
	char * _brojPasosa;
	vector<Viza *> _vize;
	void print(Viza* viza) {
		int indexVize = GetIndexVize(*viza);
		if (indexVize == -1)
			return;

		TuristickaViza* tViza = dynamic_cast<TuristickaViza*>(_vize[indexVize]);
		PoslovnaViza* pViza = nullptr;
		if (tViza == nullptr)
			pViza = dynamic_cast<PoslovnaViza*>(_vize[indexVize]);
		cout << "\n\nPrintanje vize...(LOADING)\n\n";
		this_thread::sleep_for(chrono::seconds(2));
		if (tViza != nullptr)
			tViza->Info();
		else
			pViza->Info();
		tViza = nullptr;
		pViza = nullptr;
	}
public:
	Aplikant() :_imePrezime(AlocirajNizKaraktera("NEMA VRIJEDNOST")), _datumRodjenja(new Datum),
		_brojPasosa(AlocirajNizKaraktera("NEMA VRIJEDNOST")) {}
	Aplikant(const char * imePrezime, Datum datumRodjenja,const char * brPasosa) {
		if (!regex_match(imePrezime, regex("^[a-zA-Z\\s]{4,30}$")))
			throw exception("Ime i prezime aplikanta nisu validni!");
		if (!regex_match(brPasosa, regex("[A-Z0-9]{7,13}")))
			throw exception("Broj pasosa moze sadrzati od 7 do 13 brojki!");
		_datumRodjenja = new Datum(datumRodjenja);
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_brojPasosa = AlocirajNizKaraktera(brPasosa);
	}
	Aplikant(const Aplikant& ap) :_imePrezime(AlocirajNizKaraktera(ap._imePrezime)), _datumRodjenja(new Datum(*ap._datumRodjenja)),
		_brojPasosa(AlocirajNizKaraktera(ap._brojPasosa)) {
		for (size_t i = 0; i < ap._vize.size(); i++) {
			TuristickaViza* tViza = dynamic_cast<TuristickaViza*>(ap._vize[i]);
			if (tViza != nullptr)
				_vize.push_back(new TuristickaViza(*tViza));
			else
				_vize.push_back(new PoslovnaViza(*dynamic_cast<PoslovnaViza*>(ap._vize[i])));
			tViza = nullptr;
		}
	}
	~Aplikant() {
		delete[]_imePrezime; _imePrezime = nullptr;
		delete[]_brojPasosa; _brojPasosa = nullptr;
		delete _datumRodjenja; _datumRodjenja = nullptr;
		for (size_t i = 0; i < _vize.size(); i++) {
			delete _vize[i]; _vize[i] = nullptr;
		}
	}

	char* GetImePrezime()const { return _imePrezime; }
	int GetIndexVize(Viza& viza) {
		int indexVize = -1;

		TuristickaViza* tViza = dynamic_cast<TuristickaViza*>(&viza);
		PoslovnaViza* pViza = nullptr;
		if (tViza == nullptr)
			pViza = dynamic_cast<PoslovnaViza*>(&viza);

		for (size_t i = 0; i < _vize.size(); i++)
			if (pViza != nullptr && dynamic_cast<PoslovnaViza*>(_vize[i])!=nullptr && viza == *_vize[i]){
				tViza = nullptr;
				pViza = nullptr;
				return i;
			}
			else if (tViza != nullptr && dynamic_cast<TuristickaViza*>(_vize[i]) != nullptr && viza == *_vize[i]) {
				tViza = nullptr;
				pViza = nullptr;
				return i;
			}
		tViza = nullptr;
		pViza = nullptr;
		return -1;
	}
	/* NOVA VIZA SE NE MOŽE DODATI UKOLIKO:
   -JE APLIKANT MLAÐI OD 10 GODINA
    -U TOM PERIODU (ODNOSI SE NA PERIOD VIZE OD - DO) NE POSTOJI VAŽEÆA (POJAM VAŽEÆA PODRAZUMIJEVA DA JE POSLJEDNJI
	TATUS VIZE BIO IZDATA TJ. DA VIZA NIJE 	PONIŠTENA ILI ISTEKLA) VIZA ZA TU DRŽAVU, BEZ OBZIRA NA NJENU VRSTU.	
   -JE NEKA OD RANIJIH VIZA PONIŠTENA (OVO SE ODNOSI SAMO NA VIZE ZA DRŽAVU: SAD)*/
	bool DodajVizu(Viza* viza) {
		TuristickaViza* tViza = dynamic_cast<TuristickaViza*>(viza);
		PoslovnaViza* pViza = nullptr;
		if (tViza == nullptr) 
			pViza = dynamic_cast<PoslovnaViza*>(viza);

		
		if (!_datumRodjenja->StarijiOdDesetGodina())
			return false;

		for (size_t i = 0; i < _vize.size(); i++)
			if (viza->GetVazenjeOD().isStariji(_vize[i]->GetVazenjeDO()) == 1)
				if (_vize[i]->GetDrzava() == viza->GetDrzava() && (_vize[i]->GetPoslednjiStatus() != PONISTENA || _vize[i]->GetPoslednjiStatus() != ISTEKLA))
					return false;
				else if (_vize[i]->GetDrzava() == viza->GetDrzava() && _vize[i]->GetDrzava() == SAD && _vize[i]->GetPoslednjiStatus() == PONISTENA)
					return false;
		

		if (tViza != nullptr)
			_vize.push_back(new TuristickaViza(*tViza));
		else
			_vize.push_back(new PoslovnaViza(*pViza));

		tViza = nullptr;
		pViza = nullptr;
		return true;
		
	}
	     // STATUSI SE MOGU DODAVATI SAMO U REDOSLIJEDU U KOJEM SE NALAZE U ENUMERACIJI NPR.
		 // -	NIJE MOGUÆE ODBITI VIZU AKO PRETHODNO NIJE IMALA STATUS DA JE ZAHTJEV_ZAPRIMLJEN;
		 // -	NIJE MOGUÆE PONIŠTITI VIZU KOJA PRETHODNO NIJE IZDATA
	bool DodajStatus(Viza* viza,Status status,const char* napomena=nullptr) {
		int indexVize = GetIndexVize(*viza);
		if (indexVize == -1)
			return false;
		Status potrebanStatus = ZAHTJEV_ZAPRIMLJEN;
		if (status == ISTEKLA || status==PONISTENA)
			potrebanStatus = IZDATA;
		if (!(status == PONISTENA && _vize[indexVize]->GetPoslednjiStatus() == ZAHTJEV_ZAPRIMLJEN))
			if (status != ZAHTJEV_ZAPRIMLJEN && (_vize[indexVize]->GetBrojStatusa() > 0 && (_vize[indexVize]->GetPoslednjiStatus()
				!= potrebanStatus)))
				return false;
		string temp = "";
		if (napomena != nullptr)
			string temp = napomena;
		
		
		_vize[indexVize]->GetStatuse().Dodaj(status, temp);

		return true;
		//ZAHTJEV_ZAPRIMLJEN, ODUSTAO, ODBIJENA, IZDATA, ISTEKLA, PONISTENA
	}

	void PrintajVizu(Viza* viza) {
		thread t(&Aplikant::print, this, viza);
		t.join();
	}
	//UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
	void ZamijeniRijec(const char* trazeni,const char* zamjena) {
		for (size_t i = 0; i < _vize.size(); i++)
			_vize[i]->SetNapomena(string(regex_replace(string(_vize[i]->GetNapomena()), regex("(" + string(trazeni) + ")"), string(zamjena))).c_str());

	}
	vector<Viza*> GetVize() { return _vize; }
	vector<Viza*>GetVizeByDrzava(Drzava drzava,Status status) {
		vector<Viza*> vize;

		for (size_t i = 0; i < _vize.size(); i++)
			if (_vize[i]->GetDrzava() == drzava && _vize[i]->GetPoslednjiStatus() == status) 
				if (dynamic_cast<TuristickaViza*>(_vize[i]) != nullptr)
					vize.push_back(new TuristickaViza(*dynamic_cast<TuristickaViza*>(_vize[i])));
				else
					vize.push_back(new PoslovnaViza(*dynamic_cast<PoslovnaViza*>(_vize[i])));
		return vize;
	}
	friend ostream & operator << (ostream&, Aplikant&);
};
ostream & operator << (ostream & COUT, Aplikant & obj) {
	cout << crt << ":::APLIKANT INFO:::" << crt;
	COUT << "Ime i prezime: " << obj._imePrezime << "\nDatum rodjenja: " << obj._datumRodjenja << "\nBroj pasosa: " << obj._brojPasosa << crt;
	COUT << "---VIZE---> " << endl;
	for (size_t i = 0; i < obj._vize.size(); i++)
		COUT << obj._vize[i] << endl;
	return COUT;
}


void main()
{

	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME
	//    ERROR ÆE BITI OZNACENO KAO "RE"
	// 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	// 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U,
	//    OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI
	//    I DODAVATI PO ŽELJI.
	// 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/
	try {
		Datum danas(19, 1, 2019);
		Datum sutra(++danas); // inkrementiranjem se uvecava dan za 1
		Datum prekosutra;
		Datum dat15022017(15, 2, 2017), dat18082017(18, 8, 2017), dat11052018(11, 5, 2018), dat11122017(11, 12, 2017);


		prekosutra = danas;
		if (prekosutra == danas)
			cout << "Datumi su jednaki" << endl;
		cout << danas << endl << sutra << endl << prekosutra << crt;

		int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196, v19 = 19;

		FITKolekcija<int, int, 7> brojevi;
		brojevi.Dodaj(v196, v6);
		brojevi.Dodaj(v13, v32);
		brojevi.Dodaj(v98, v196);
		brojevi.Dodaj(v63, v13);
		brojevi.Dodaj(v98, v196);
		brojevi.Dodaj(v196, v6);
		brojevi.Dodaj(v13, v196);

		cout << brojevi << crt;
		//NA KOJI NAÈIN ÆE SE VRŠITI SORTIRANJE PRVENSTVENO ZAVISI OD VRIJEDNOSTI PARAMETRA, A VRŠI SE NA OSNOVU VRIJEDNOSTI TIPA T1
		brojevi.Sortiraj(RASTUCI);
		cout << brojevi << crt;
		brojevi.Sortiraj(OPADAJUCI);
		cout << brojevi << crt;

		try
		{
			//U SLUÈAJU PREKORAÈENJA DEFINISANE VELIÈINE KOLEKCIJE, POTREBNO JE BACITI IZUZETAK
			brojevi.Dodaj(v19, v196);

		}
		catch (exception & err)
		{
			cout << "Error -> " << err.what() << endl;
		}

		Aplikant jasmin("Jasmin Azemovic", Datum(19,1,2009), "BS6985645");
		PoslovnaViza poslovna(SAD, dat15022017, dat18082017, "Microsoft","nemaju informaciju");




		if (jasmin.DodajVizu(&poslovna))
			cout << "Podaci o vizi uspješno zaprimljeni" << endl;

		

		if (jasmin.DodajStatus(&poslovna, ZAHTJEV_ZAPRIMLJEN, "provjeriti navode sa predstavnicima kompanije Microsoft"))
			cout << "Status vize uspijesno promijenjen" << endl;
		else
			cout << "Aplikant " << jasmin.GetImePrezime() << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;


		if (jasmin.DodajStatus(&poslovna,PONISTENA, "predstavnicima kompanije microsoft nemaju informaciju o posjeti navedenog aplikanta"))
			cout << "Status vize uspijesno promijenjen" << endl;
		else
			cout << "Aplikant " << jasmin.GetImePrezime() << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

		TuristickaViza turisticka(ENGLESKA, dat18082017, dat11122017, "Whitehall Pl, London SW1A 2BD, UK", "nemaju informaciju");

		jasmin.DodajVizu(&turisticka);
		if (jasmin.DodajStatus(&turisticka, ZAHTJEV_ZAPRIMLJEN)) 
			cout << "Status vize uspijesno promijenjen" << endl;
		else  
			cout << "Aplikant " << jasmin.GetImePrezime() << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

		if (jasmin.DodajStatus(&turisticka, IZDATA))
		{
			cout << "Status vize uspijesno promijenjen" << endl;
			//U ZASEBNOJ NITI URADITI PRINTANJE VIZE...KORISTITI METODU SLEEP_FOR
			jasmin.PrintajVizu(&turisticka);
		}
		else
			cout << "Aplikant " << jasmin.GetImePrezime() << " ne posjeduje evidentiranu trazenu vizu ili status vize nema odgovarajucu vrijednost" << endl;

		//VRAÆA PODATKE O SVIM VIZAMA KOJE SU IZDATE ZA ODREÐENU DRŽAVU, A KOJE SU IMALE ODREÐENI STATUS
		vector<Viza *> jasminVize = jasmin.GetVizeByDrzava(SAD, PONISTENA);
		for (int i = 0; i < jasminVize.size(); i++)
			if (dynamic_cast<TuristickaViza*>(jasminVize[i]) != nullptr)
				dynamic_cast<TuristickaViza*>(jasminVize[i])->Info();
			else
				dynamic_cast<PoslovnaViza*>(jasminVize[i])->Info();

		////UNUTAR NAPOMENE MIJENJA SADRŽAJ "nemaju informaciju" SA "su upoznti sa" - KORISTITI REGEX
		jasmin.ZamijeniRijec("nemaju informaciju", "su upoznati sa");
		jasmin.PrintajVizu(&turisticka);
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}
}
