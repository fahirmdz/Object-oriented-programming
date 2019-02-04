#include<iostream>
#include<cstring>
#include<string>
#include<regex>
#include<iterator>
#include<thread>
#include<vector>
#include<cctype>
#include<sstream>
#include<algorithm>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum GodinaStudija { PRVA = 1, DRUGA, TRECA };
const char* godinaStudijaChar[] = { "I","II","III" };
const char* godinaStudijaChar2[] = { "PRVA","DRUGA","TRECA" };

char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2>
class Dictionary {
	T1* _elementi1;
	T2* _elementi2;
	int* _trenutno;
	void dealociraj() {
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	void copyElements(const Dictionary& d) {
		if (*d._trenutno > 0) {
			_elementi1 = new T1[*d._trenutno];
			_elementi2 = new T2[*d._trenutno];
			for (int i = 0; i < *d._trenutno; i++) {
				_elementi1[i] = d._elementi1[i];
				_elementi2[i] = d._elementi2[i];
			}
		}
		else {
			_elementi1 = nullptr;
			_elementi2 = nullptr;
		}
	}
public:
	Dictionary() {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = new int(0);
	}
	Dictionary(const Dictionary& d) :_trenutno(new int(*d._trenutno)) {
		copyElements(d);
	}
	~Dictionary() {
		dealociraj();
		delete _trenutno; _trenutno = nullptr;
	}
	Dictionary& operator=(const Dictionary& d) {
		if (&d == this)
			return *this;
		dealociraj();
		*_trenutno = *d._trenutno;
		copyElements(d);
		return *this;
	}
	void AddElement(const T1& t1, const T2& t2) {
		T1* temp1 = new T1[*_trenutno + 1];
		T2* temp2 = new T2[*_trenutno + 2];

		for (int i = 0; i < *_trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		dealociraj();
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = nullptr;
		temp2 = nullptr;
		_elementi1[*_trenutno] = t1;
		_elementi2[(*_trenutno)++] = t2;
	}
	
	Dictionary getRange(int t1, int t2) {
		if (t2<t1 || t1 < 0 || t2 < 0 || t1 >= *_trenutno || t2 >= *_trenutno)
			throw exception("Lokacija elemenata nije validna!");
		Dictionary temp;
		temp.AddElement(_elementi1[t1], _elementi2[t2]);
		return temp;
	}
	T1 & getElement1(int lokacija)const {
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw exception("Lokacija elementa nije validna!\n");
		return _elementi1[lokacija];
	}
	T2 & getElement2(int lokacija)const {
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw exception("Lokacija elementa nije validna!\n");
		return _elementi2[lokacija];
	}

	int getTrenutno() { return *_trenutno; }
	friend ostream& operator<< <>(ostream&, const Dictionary&);
	friend bool operator== <>(const Dictionary&, const Dictionary&);
};
template<class T1,class T2>
ostream& operator<< <>(ostream& COUT, const Dictionary<T1,T2>& obj) {
	for (int i = 0; i < *obj._trenutno; i++)
		COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
	return COUT;
}
template<class T1, class T2>
bool operator==(const Dictionary<T1,T2>& d1, const Dictionary<T1,T2>& d2) {
	if (*d1._trenutno == *d2._trenutno) {
		for (int i = 0; i < *d1._trenutno; i++)
			if (d1._elementi1[i] != d2._elementi1[i] || d1._elementi2[i] != d2._elementi2[i])
				return false;
		return true;
	}
	return false;
}


class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
	int GetBrojDanaMjesec(int mjesec, int godina) {
		if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 10 || mjesec == 12)
			return 31;
		else if (mjesec == 2)
			if ((godina % 100 == 0 && godina % 400 == 0) || godina % 4 == 0)
				return 29;
			else
				return 28;
		return 30;
	}
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& d):_dan(new int(*d._dan)),_mjesec(new int(*d._mjesec)),_godina(new int(*d._godina)),
		_sati(new int(*d._sati)),_minuti(new int(*d._minuti)){}
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

	//treba ispisati: 19/06/2019 10:15
	char* ToCharArray() {
		stringstream a;
		if (*_dan < 10)
			a << "0";
		a << *_dan << "/";
		if (*_mjesec < 10)
			a<<"0";
		a << *_mjesec << "/" << *_godina << " ";
		if (*_sati < 10)
			a<<"0";
		a << *_sati << ":";
		if (*_minuti < 10)
			a<<"0";
		a << *_minuti;
		return Alociraj(a.str().c_str());

	}
	int compareDate(const DatumVrijeme& d) {
		if (*_godina < *d._godina)
			return 1;
		else if (*_godina > * d._godina)
			return -1;
		else if (*_mjesec < *d._mjesec)
			return 1;
		else if (*_mjesec > * d._mjesec)
			return -1;
		else if (*_dan < *d._dan)
			return 1;
		else if (*_dan > * d._dan)
			return -1;
		else if (*_sati < *d._sati)
			return 1;
		else if (*_sati > * d._sati)
			return -1;
		else if (*_minuti < *d._minuti)
			return 1;
		else if (*_minuti > * d._minuti)
			return -1;
		return 0;

	}
	bool ProsaoSat(const DatumVrijeme& d) {
		bool uslovPrelaskaSat = *d._sati == 0 && *_sati == 23;
		bool uslovPrelaskaMjesec = *d._dan == 1 && *_dan == GetBrojDanaMjesec(*_mjesec, *_godina);
		auto prosaoSat = [=]() {
			int prosloMinuta = *d._minuti + (60 - *_minuti);
			if (prosloMinuta < 60)
				return false;
			return true;
		};

		if (*d._godina < *_godina)
			return false;
		else if (*d._godina > * _godina) {
			if (*d._godina - 1 == *_godina && uslovPrelaskaMjesec && uslovPrelaskaSat)
				return prosaoSat();
		}
		else if (*d._godina > * _godina)
			return true;
		else if (*d._mjesec - 1 == *_mjesec && uslovPrelaskaMjesec && uslovPrelaskaSat)
			return prosaoSat();
		else if (*d._mjesec < *_mjesec)
			return false;
		else if (*d._mjesec > * _mjesec)
			return true;
		else if (*d._dan - 1 == *_dan && uslovPrelaskaSat)
			return prosaoSat();
		else if (*d._dan < *_dan)
			return false;
		else if (*d._dan > * _dan)
			return true;
		else if (*d._sati < *_sati)
			return false;
		else if (*d._sati > * _sati) {
			if (*d._sati - *_sati == 1)
				return prosaoSat();
			return true;
		}
		return false;
	}
	
	friend ostream& operator<< (ostream&, const DatumVrijeme&);
	friend bool operator==(const DatumVrijeme&, const DatumVrijeme&);
	friend bool operator!=(const DatumVrijeme&, const DatumVrijeme&);

};
ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
	COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
	return COUT;
}
bool operator==(const DatumVrijeme& d1, const DatumVrijeme& d2) {
	return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan && *d1._sati == *d2._sati && *d1._minuti == *d2._minuti;;
}
bool operator!=(const DatumVrijeme& d1, const DatumVrijeme& d2) {
	if (d1 == d2)
		return false;
	else 
		return true;
}


class Predmet {
	char* _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char* naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}
	Predmet(const Predmet& p):_naziv(Alociraj(p._naziv)),_ocjena(p._ocjena),_napomena(p._napomena){}
	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	Predmet& operator=(const Predmet& p) {
		if (&p == this)
			return *this;
		delete[] _naziv;
		_naziv = Alociraj(p._naziv);
		_ocjena = p._ocjena;
		_napomena = p._napomena;
		return *this;
	}

	string GetNapomena() { return _napomena; }
	char* GetNaziv()const { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}

	friend ostream& operator<< (ostream&,const Predmet&);
	friend bool operator==(const Predmet&, const Predmet&);
	friend bool operator!=(const Predmet&, const Predmet&);

};
ostream& operator<< (ostream& COUT,const Predmet& obj) {
	COUT << crt << "Naziv: " << obj._naziv << " - Ocjena(" << obj._ocjena << ") ";
	if (obj._napomena != "")
		cout << " --> NAPOMENA: " << obj._napomena;
	cout << crt;
	return COUT;
}
bool operator==(const Predmet& p1, const Predmet& p2) {
	return strcmp(p1._naziv, p2._naziv) == 0 && p1._napomena == p2._napomena && p1._ocjena == p2._ocjena;
}
bool operator!=(const Predmet& p1, const Predmet& p2) {
	if (p1 == p2)
		return false;
	else
		return true;
}

class Uspjeh {
	GodinaStudija* _godina;
	//datumvrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Dictionary<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(GodinaStudija godina=PRVA) {
		_godina = new GodinaStudija(godina);
	}
	Uspjeh(const Uspjeh& u) :_godina(new GodinaStudija(*u._godina)), _predmeti(u._predmeti){}
	~Uspjeh() { delete _godina; _godina = nullptr; }

	Uspjeh& operator=(const Uspjeh& u) {
		if (&u == this)
			return *this;
		*_godina = *u._godina;
		_predmeti = u._predmeti;
		return *this;
	}

	Dictionary<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
	GodinaStudija GetGodinaStudija() { return *_godina; }
	bool SadrziVec(const Predmet& p) {
		for (int i = 0; i < _predmeti.getTrenutno(); i++)
			if (_predmeti.getElement1(i) == p)
				return true;
		return false;
	}
	int GetBrojPredmeta(){ return _predmeti.getTrenutno(); }
	float GetProsjek() {
		float prosjek = 0;
		for (int i = 0; i < _predmeti.getTrenutno(); i++)
			prosjek += _predmeti.getElement1(i).GetOcjena();

		return (float)prosjek / _predmeti.getTrenutno();
	}
	friend ostream& operator<< (ostream&, const Uspjeh&);
	friend bool operator==(const Uspjeh&, const Uspjeh&);
	friend bool operator!=(const Uspjeh&, const Uspjeh&);

};
ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
	COUT <<"Godina: "<< godinaStudijaChar[*obj._godina-1] << " " << obj._predmeti << endl;
	return COUT;
}
bool operator==(const Uspjeh& u1, const Uspjeh& u2) {
	return *u1._godina == *u2._godina && u1._predmeti == u2._predmeti;
}
bool operator!=(const Uspjeh& u1, const Uspjeh& u2) {
	if (u1 == u2)
		return false;
	return true;
}


class Student {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
	void sendEmail(GodinaStudija g) {
		cout << crt << "Salje se e-mail...(LOADING)" << crt;
		this_thread::sleep_for(chrono::seconds(1));
		cout <<crt<< "FROM: info@fit.ba\nTO: " << _emailAdresa << endl;
		cout << "\nPostovani " << _imePrezime << ",\n\nEvidentirali ste uspjeh za " << godinaStudijaChar[g - 1] << " godinu studija.\n\nPozdrav.\nFIT Team." << crt;
	}
	void sendSMS(float prosjek, GodinaStudija g) {
		cout << crt << "Salje se SMS...(LOADING)" << crt;
		this_thread::sleep_for(chrono::seconds(1));
		cout << crt << "TO: " << _brojTelefona << "\n\nPostovani " << _imePrezime << ", \n\nSvaka cast za uspjeh " << prosjek << " ostvaren u " << godinaStudijaChar[g - 1];
		cout << " godinu studija.\n\nPozdrav.\nFIT Team" << crt;
	}
public:
	Student(const char* imePrezime="NEMA VRIJEDNOST", string emailAdresa="", string brojTelefona="") {
		if (!regex_match(emailAdresa, regex("[a-zA-Z]{3,8}\\-[a-zA-Z]{3,8}\\@(hotmail.com|outlook.com|fit.ba)")))
			emailAdresa = string("notSet@fit.ba");
		else		
			_emailAdresa = emailAdresa;

		_imePrezime = Alociraj(imePrezime);
		_brojTelefona = brojTelefona;
	}
	Student(const Student& s):_imePrezime(Alociraj(s._imePrezime)),_emailAdresa(s._emailAdresa),_brojTelefona(s._brojTelefona),_uspjeh(s._uspjeh){}
	~Student() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	int getUspjehGodina(GodinaStudija g) {
		for (size_t i = 0; i < _uspjeh.size(); i++)
			if (_uspjeh[i].GetGodinaStudija() == g)
				return i;
		return -1;
	}

	bool AddPredmet(const Predmet& p, GodinaStudija godina, const DatumVrijeme& d) {
		int uspjehIndex = getUspjehGodina(godina);
		if (uspjehIndex != -1) {
			if (_uspjeh[uspjehIndex].SadrziVec(p))
				return false;
			int brPr = _uspjeh[uspjehIndex].GetBrojPredmeta();
			if (brPr > 0 && !_uspjeh[uspjehIndex].GetPredmeti()->getElement2(brPr - 1).ProsaoSat(d))  //PROVJERA ZA PROTEKLO VRIJEME
				return false;
		}
		else {
			_uspjeh.push_back(Uspjeh(godina));
			uspjehIndex = (int)_uspjeh.size() - 1;
		}
		_uspjeh[uspjehIndex].GetPredmeti()->AddElement(p, d);
		thread tEmail(&Student::sendEmail, this, godina);
		tEmail.join();

		float prosjek = _uspjeh[uspjehIndex].GetProsjek();
		if (prosjek > 8) {
			thread tSMS(&Student::sendSMS, this, prosjek, godina);
			tSMS.join();
		}
		return true;
	}
	int BrojPonavljanjaRijeci(const char* rijec) {
		int brojPonavljanja = 0;
		regex reg(rijec);
		for (size_t i = 0; i < _uspjeh.size(); i++) {
			Dictionary<Predmet, DatumVrijeme>* predmeti = _uspjeh[i].GetPredmeti();
			for (int j = 0; j < predmeti->getTrenutno(); j++) {
				string napomena = predmeti->getElement1(j).GetNapomena();
				sregex_iterator it_poc(napomena.begin(), napomena.end(), reg);
				sregex_iterator it_kraj;
				while (it_poc != it_kraj) {
					brojPonavljanja++;
					it_poc++;
				}
			}
			predmeti = nullptr;
		}
		return brojPonavljanja;
	}

	vector<Predmet> operator()(DatumVrijeme* d1,DatumVrijeme* d2) {
		vector<Predmet> pr;
		for (size_t i = 0; i < _uspjeh.size(); i++){
			Dictionary<Predmet, DatumVrijeme>* predmeti = _uspjeh[i].GetPredmeti();
			for (int j = 0; j < predmeti->getTrenutno(); j++) {
				int poredjenje1 = predmeti->getElement2(j).compareDate(*d1);
				int poredjenje2 = predmeti->getElement2(j).compareDate(*d2);
				if ((poredjenje1 == 0 || poredjenje1 == -1) && (poredjenje2 == 0 || poredjenje2 == 1)) {
					pr.push_back(predmeti->getElement1(j));
				}
			}
			predmeti = nullptr;
		}
		return pr;
	}

	Uspjeh* operator[](const char* godina) {
		if (godina == nullptr)
			return nullptr;

		int godinaS = -1;
		for (int i = 0; i < 3; i++)
			if (strcmp(godina, godinaStudijaChar2[i]) == 0)
				godinaS = i;
		if (godinaS == -1)
			return nullptr;
		GodinaStudija g = GodinaStudija(godinaS + 1);

		int index = getUspjehGodina(g);
		if (index == -1)
			return nullptr;
		return &_uspjeh[index];

	}
	vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }

	friend ostream& operator<< (ostream&,const Student&);
	friend bool operator==(const Student&, const Student&);
};
ostream& operator<< (ostream& COUT,const Student& obj) {
	COUT << crt << "Ime i prezime: " << obj._imePrezime << "\nE-mail: " << obj._emailAdresa << "\nBroj telefona: " << obj._brojTelefona << crt;
	cout << ":::USPJEH:::" << crt;
	copy(obj._uspjeh.begin(), obj._uspjeh.end(), ostream_iterator<Uspjeh>(cout, "\n"));
	return COUT;
}
bool operator==(const Student& s1, const Student& s2) {
	return strcmp(s1._imePrezime, s2._imePrezime) == 0 && s1._emailAdresa == s2._emailAdresa && s1._brojTelefona == s2._brojTelefona && s1._uspjeh == s2._uspjeh;
}


void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/
	cout << "NA KRAJU ISPITA, RAD PREDAJTE U ODGOVARAJUCI FOLDER NA FTP SERVERU (INTEGRALNI)!" << endl;
	DatumVrijeme temp,
		datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);
	//funkcija ToCharArray vraca datum i vrijeme kao char *. konverziju izvrsiti koristeci stringstream objekat.
	//voditi racuna o tome da se izmedju datuma i vremena nalazi samo jedan razmak, te da su vrijednosti dana i mjeseca iskazane kao dvije cifre
	cout << datum19062019_1015.ToCharArray() << endl;//treba ispisati: 19/06/2019 10:15
	temp = datum05072019_1231;
	cout << temp.ToCharArray() << endl;//treba ispisati: 05/07/2019 12:31

	const int DictionaryTestSize = 9;
	Dictionary<int, int> Dictionary1;
	for (size_t i = 0; i < DictionaryTestSize; i++)
		Dictionary1.AddElement(i + 1, i * i);

	try {
		//vraca elemente kolekcije koji se nalaze na lokacijama definisanim vrijednostima parametara (npr. 2 - 7). 
		//funkcija baca izuzetak u slucaju da se zahtijeva lokacija koja ne postoji ili je vrijednost posljednje lokacije manja od pocetne
		Dictionary<int, int> opseg = Dictionary1.getRange(2, 7);
		cout << opseg << endl;
		
		Dictionary1.getRange(7, 11);
	}
	catch (exception & err) {
		cout << err.what() << endl;
	}
	cout << Dictionary1 << endl;

	Dictionary<int, int> Dictionary2 = Dictionary1;
	cout << Dictionary2 << crt;

	Dictionary<int, int> Dictionary3;
	Dictionary3 = Dictionary1;
	cout << Dictionary3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet MAT("Matematika", 7, "Ucesce na takmicenju"),
		UIT("Uvod u informacijske tehnologije", 9),
		RM("Racunarske mreze", 8),
		EN("Engleski jezik", 6),
		PRII("Programiranje II", 9),
		PRIII("Programiranje III", 7),
		BPI("Baze podataka I", 8);
	UIT.DodajNapomenu("Pohvala za ostvareni uspjeh"),
		
	cout << MAT << endl;

	/*
	email adresa mora biti u formatu: text-text@ nakon cega slijedi neka od sljedecih domena: hotmail.com ili outlook.com ili fit.ba. Pod text se podrazumijeva bilo koje slovo, malo ili veliko.
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@fit.ba
	za provjeru koristiti regex
	*/
	Student jasmin("Jasmin Azemovic", "jasmin.azemovic@hotmail.com", "033 281 172");
	Student adel("Adel Handzic", "adel.handzic@fit.ba", "033 281 170");
	Student emailNotValid("Ime Prezime", "korisnik@lazna.ba", "033 281 170");

	
	//uspjeh se dodaje za svaki predmet na nivou godine studija.
	//tom prilikom onemoguciti:
	//- dodavanje istoimenih predmeta na nivou jedne godine,
	//- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jedne godine, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 1 sat).
	//godine (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za drugu godinu, pa onda za prvu godinu i sl.).
	//Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	//
	if (jasmin.AddPredmet(UIT, DRUGA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(RM, DRUGA, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(EN, PRVA, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(MAT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati MAT jer je vec dodana u prvoj godini
	if (jasmin.AddPredmet(MAT, PRVA, datum05072019_1231))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati UIT jer nije prosao 1 sat od dodavanja posljednjeg predmeta
	if (jasmin.AddPredmet(UIT, PRVA, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if(!jasmin.AddPredmet(PRIII,DRUGA,DatumVrijeme(30,6,2019,13,14)))  //-->PREDMET SE NECE DODATI JER JE PROSLO 59 MINUTA, A  NE 60
		cout<<"Nije prosao sat od dodavanja posljednjeg predmeta na drugoj godini!\n";
	else
		cout<<"Predmet uspjesno dodan!"<<crt;

	if (jasmin.AddPredmet(PRIII, PRVA, DatumVrijeme(31,12,2019,23,30)))
		cout << "Predmet uspjesno dodan!" << crt;

	if (!jasmin.AddPredmet(PRII, PRVA, DatumVrijeme(1,1,2020,00,29)))  //-->PREDMET SE NECE DODATI JER JE PROSLO 59 MINUTA, A  NE 60 (PRELAZAK U NOVU GODINU)
		cout << "Nije prosao sat od dodavanja posljednjeg predmeta na drugoj godini!\n";
	else
		cout << "Predmet uspjesno dodan!" << crt;

	if (!jasmin.AddPredmet(PRII, DRUGA, DatumVrijeme(31, 8, 2019, 23, 35)))
		cout << "Predmet uspjesno dodan!" << crt;

	if (jasmin.AddPredmet(BPI, DRUGA, DatumVrijeme(1, 9, 2019, 0, 34))) //-->NECE SE DODATI, PROSLO 59 MINUTA (PRELAZAK U DRUGI MJESEC PRIMJER)
		cout << "Predmet uspjesno dodan!" << crt;
	else
		cout<< "Nije prosao sat od dodavanja posljednjeg predmeta na prvoj godini!\n";


	/*nakon evidentiranja uspjeha na bilo kojem predmetu tj. prilikom uspjesno izvrsene funkcije AddPredmet, Studentu se salje email sa sadrzajem:
	FROM:info@fit.ba
	TO: emailStudenta
	Postovani ime i prezime, evidentirali ste uspjeh za X godinu studija.
	Pozdrav.
	FIT Team.

	ukoliko je, nakon dodavanja predmeta, prosjek na nivou te godine veci od 8.0 Studentu se, pored email-a, salje i SMS sa sadrzajem: "Svaka cast za uspjeh X.X ostvaren u X godini studija".
	slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
	*/
	cout << "USPJEH ISPISATI KORISTEĆI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	//vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(new DatumVrijeme(18, 06, 2019, 10, 15), new DatumVrijeme(21, 06, 2019, 10, 10));
	for (Predmet u : jasminUspjeh)
		cout << u << endl;

	Uspjeh * uspjeh_I_godina = jasmin["PRVA"];//vraca uspjeh Studenta ostvaren u prvoj godini studija
	if (uspjeh_I_godina != nullptr)
		cout << *uspjeh_I_godina << endl;

	cin.get();
	system("pause>0");
}