//fahirmdz GitHub

#include"pch.h"
#include <iostream>
#include<ctime>
#include<chrono>
#include<regex>
#include<vector>
#include<exception>
#include<thread>
#include<string>
#include<cstring>
#include<cctype>
using namespace std;

const char* crt = "\n-------------------------------------------------\n";
enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };
const char * ispisPredmeti[] = { "Matematika", "Historija", "Geografia", "Hemija", "Biologija", "Engleski", "Muzicko", "Fizicko" };
const char * ispisRazredi[] = { "I1", "I2", "I3", "II1", "II2", "II3", "III1", "III2", "III3", "IV1", "IV2", "IV3" };

ostream& operator<<(ostream& COUT, Predmeti& predmet) {
	COUT << ispisPredmeti[predmet];
	return COUT;
}
ostream& operator<<(ostream& COUT, Razredi& razred) {
	COUT << ispisRazredi[razred];
	return COUT;
}
char* AlocirajNizKaraktera(const char* a) {
	if (a == nullptr)
		return nullptr;
	char* temp = new char[strlen(a) + 1];
	strcpy_s(temp, strlen(a) + 1, a);
	return temp;
}
string GetRazred(int i) {
	string x = ispisRazredi[i];
	if (i >= 0 && i < 3)
		return string(x.begin(), x.begin() + 1);
	else if ((i >= 3 && i < 6) || (i >= 10 && i < 13))
		return string(x.begin(), x.begin() + 2);
	else if (i >= 6 && i < 10)
		return string(x.begin(), x.begin() + 3);
}
template<class T1, class T2>
class Kolekcija{
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;
	void copyElements(const Kolekcija& k) {
		if (k._trenutno > 0) {
			_elementi1 = new T1[k._trenutno];
			_elementi2 = new T2[k._trenutno];
			for (int i = 0; i < k._trenutno; i++) {
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
	Kolekcija(){
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	Kolekcija(const Kolekcija& k) :_trenutno(k._trenutno) {
		copyElements(k);
	}
	~Kolekcija(){
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		_trenutno = k._trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		copyElements(k);
		return *this;
	}

	void AddElement(T1 t1, T2 t2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		if(_trenutno>0)
			for (int i = 0; i < _trenutno; i++) {
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_elementi1[_trenutno] = t1;
		_elementi2[_trenutno++] = t2;
		temp1 = nullptr;
		temp2 = nullptr;
	}
	void RemoveElement(int index) {
		if (index < 0 || index >= _trenutno)
			return;
		for (int i = index; i < _trenutno - 1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutno--;
	}
	T1& GetElement1(int index) {
		if (index < 0 || index >= _trenutno)
			throw exception("Index lokacija elementa nije validna!");
		return _elementi1[index]; 
	}
	T2& GetElement2(int index) { 
		if (index < 0 || index >= _trenutno)
			throw exception("Index lokacija elementa nije validna!");
		return _elementi2[index];
	}

	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< <>(ostream&, Kolekcija&);
	friend bool operator== <>(Kolekcija&, Kolekcija&);
};
template<class T1,class T2>
ostream& operator<< <>(ostream& COUT, Kolekcija<T1, T2>& k) {
	for (int i = 0; i < k._trenutno; i++)
		cout << k._elementi1[i] << " - " << k._elementi2[i] << endl;
	return COUT;
}
template<class T1, class T2>
bool operator== <>(Kolekcija<T1, T2>& k1, Kolekcija<T1, T2>& k2) {
	if (k1._trenutno == k2._trenutno) {
		for (int i = 0; i < k1._trenutno; i++)
			if (k1._elementi1[i] != k2._elementi1[i] || k1._elementi2[i] != k2._elementi2[i])
				return false;
		return true;
	}
	return false;
}
class Datum
{
	int *_dan, *_mjesec, *_godina;

	int GetBrojDana(int m, int g) {
		if (m == 3 || m == 1 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12)
			return 31;
		else if (m == 2)
			if (g % 400 == 0 && g % 100 == 0 && g % 4 == 0)
				return 29;
			else return 28;
		else
			return 30;
	}
	bool validneVrijednosti(int dan, int mjesec, int godina) {
		if (dan<1 || dan>GetBrojDana(mjesec, godina))
			return false;
		else if (mjesec < 1 || mjesec>12)
			return false;
		else if (godina < 1920)
			return false;
		
		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		int trDan = timeinfo.tm_mday, trMjesec = timeinfo.tm_mon + 1, trGodina = timeinfo.tm_year + 1900;
		if (godina > trGodina)
			return false;
		if (godina == trGodina){
		   if(mjesec > trMjesec)
			return false;
		    else if (mjesec == trMjesec && dan > trDan)
			return false;
		}
		return true;
	}

public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000){
		if (!validneVrijednosti(dan, mjesec, godina))
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}

	Datum(const Datum&obj){
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}

	~Datum(){
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	Datum& operator=(const Datum& obj) {
		if (&obj == this)
			return *this;
		delete _dan; _dan = new int(*obj._dan);
		delete _mjesec; _mjesec = new int(*obj._mjesec);
		delete _godina; _godina = new int(*obj._godina);
		return *this;
	}

	friend ostream& operator<< (ostream&,const Datum&);
	friend bool operator==(Datum&, Datum&);
	friend bool operator!=(Datum&, Datum&);

};
ostream& operator<< (ostream &COUT, const Datum &obj){
	COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
	return COUT;
}
bool operator==(Datum& d1, Datum &d2) {return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan;}
bool operator!=(Datum& d1, Datum &d2) { return *d1._godina != *d2._godina || *d1._mjesec != *d2._mjesec || *d1._dan != *d2._dan; }

bool ProvjeriValidnostBrojaTelefona(string telefon) { return regex_match(telefon, regex("\\+?\\d{0,3}[-(]?\\d{2}[-)]?[/-]?\\d{3}[-\\s]?\\d{3}")); }

class Osoba{
protected:
	char * _imePrezime;
	Datum * _datumRodjenja;
	string _brojTelefona;
public:
	Osoba():_imePrezime(AlocirajNizKaraktera(" ")),_datumRodjenja(new Datum),_brojTelefona(""){}
	Osoba(const char * imePrezime, Datum datum){
		if (imePrezime == nullptr || !regex_match(imePrezime, regex("^[a-zA-Z\\s]{4,25}$")))
			throw exception("Ime i prezime nisu validni!");
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_datumRodjenja = new Datum(datum);
	}
	Osoba(const Osoba & obj) :_brojTelefona(obj._brojTelefona),_imePrezime(AlocirajNizKaraktera(obj._imePrezime)),
	_datumRodjenja(new Datum(*obj._datumRodjenja)){}
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
		_brojTelefona = o._brojTelefona;
		return *this;
	}

	char* GetImePrezime()const { return _imePrezime; }
	string GetBrojTelefona()const { return _brojTelefona; }
	Datum GetDatumRodjenja()const { return *_datumRodjenja; }

	bool setBrojTelefona(string brojTelefona){
		if (ProvjeriValidnostBrojaTelefona(brojTelefona)){
			_brojTelefona = brojTelefona;
			return true;
		}
		return false;
	}

	virtual void Info() = 0 {
		cout << crt << "Ime i prezime: " << _imePrezime;
		cout << "\nDatum rodjenja: " << *_datumRodjenja;
		cout << "\nBroj telefona: " << _brojTelefona << endl;
	}
	friend bool operator==(Osoba&, Osoba&);
};
bool operator==(Osoba& o1, Osoba& o2) {
	return o1._brojTelefona == o2._brojTelefona && *o1._datumRodjenja == *o2._datumRodjenja &&
		strcmp(o1._imePrezime, o2._imePrezime) == 0;
}
class Nastavnik :public Osoba{
	Kolekcija<Predmeti, Razredi> * _predmetiRazredi;
	
	int* GetBrojOdjeljenjaPredaje(Predmeti p) {
		int *brojOdjeljenja = new int[4]{ 0 };
		for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
			if (_predmetiRazredi->GetElement1(i) == p) {
				string razredString = GetRazred(_predmetiRazredi->GetElement2(i));
				if (razredString == "I")
					brojOdjeljenja[0]++;
				else if (razredString == "II")
					brojOdjeljenja[1]++;
				else if (razredString == "III")
					brojOdjeljenja[2]++;
				else if (razredString == "IV")
					brojOdjeljenja[3]++;
			}
		return brojOdjeljenja;
	}
public:
	Nastavnik(const char * imePrezime, Datum datumRodjenja):Osoba(imePrezime,datumRodjenja){
		_predmetiRazredi = new Kolekcija<Predmeti, Razredi>;
	}
	Nastavnik(const Nastavnik& n) :Osoba(n) {
		if (n._predmetiRazredi != nullptr && n._predmetiRazredi->GetTrenutno() > 0)
			_predmetiRazredi = new Kolekcija<Predmeti, Razredi>(*n._predmetiRazredi);
		else
			_predmetiRazredi = new Kolekcija<Predmeti, Razredi>;
	}
	~Nastavnik() {delete _predmetiRazredi; _predmetiRazredi = nullptr;}

	Nastavnik& operator=(const Nastavnik& n) {
		if (&n == this)
			return *this;
		Osoba::operator=(n);
		delete _predmetiRazredi;
		if (n._predmetiRazredi != nullptr && n._predmetiRazredi->GetTrenutno() > 0)
			_predmetiRazredi = new Kolekcija<Predmeti, Razredi>(*n._predmetiRazredi);
		else
			_predmetiRazredi = new Kolekcija<Predmeti, Razredi>;
		return *this;
	}

	Kolekcija<Predmeti, Razredi>& GetPredmetiRazredi() { return *_predmetiRazredi; }
	/*enum Predmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski, Muzicko, Fizicko };
	enum Razredi { I1, I2, I3, II1, II2, II3, III1, III2, III3, IV1, IV2, IV3 };*/

	/*nastavnik moze predavati isti predmet, ali to mora biti u razlicitim odjeljenjima i najvise u dva
			razreda (na dvije razlicite godine)*/
	bool AddPredmetRazred(Predmeti predmet,Razredi razred) {
		int* brojOdjeljenja = GetBrojOdjeljenjaPredaje(predmet);
		int brojacDvaURazredu = 0;
		for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
			if (brojOdjeljenja[i] == 2)
				brojacDvaURazredu++;
		if (brojacDvaURazredu == 2)
			return false;
		string razredString = GetRazred(razred);

		int indexRazreda = 0;
		if (razredString == "II")
			indexRazreda = 1;
		else if (razredString == "III")
			indexRazreda = 2;
		else if (razredString == "IV")
			indexRazreda = 3;


		if (brojOdjeljenja[indexRazreda] == 2)
			return false;
		delete[] brojOdjeljenja;
		brojOdjeljenja = nullptr;

		_predmetiRazredi->AddElement(predmet, razred);
		return true;

	}

	bool PredajeUTomRazredu(Razredi razred) {
		for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
			if (_predmetiRazredi->GetElement2(i) == razred)
				return true;
		return false;
	}
	bool PredajePredmetURazredu(Razredi razred, Predmeti predmet) {
		bool uslovRazred = PredajeUTomRazredu(razred);
		if (uslovRazred)
			for (int i = 0; i < _predmetiRazredi->GetTrenutno(); i++)
				if (_predmetiRazredi->GetElement1(i) == predmet)
					return true;
		return false;
	}
	void Info() {
		Osoba::Info();
		cout << crt << "----PREDMETI I RAZREDI-----\n\n";
		cout << *_predmetiRazredi << endl;
	}
	friend bool operator==(Nastavnik&, Nastavnik&);
	friend ostream& operator<<(ostream&, Nastavnik&);
};
ostream& operator<<(ostream& COUT, Nastavnik& n) {
	n.Info();
	return COUT;
}
bool operator==(Nastavnik& n1, Nastavnik& n2) {
	return *dynamic_cast<Osoba*>(&n1) == *dynamic_cast<Osoba*>(&n2) && *n1._predmetiRazredi == *n2._predmetiRazredi;
}
class Ucenik : public Osoba{
	int _brojUDnevniku;
	Razredi _razred;
	vector<Nastavnik*> _nastavnici;
	Kolekcija<Predmeti, int> _predmetiOcjene;
public:
	Ucenik():_brojUDnevniku(0){}
	Ucenik(const char * imePrezime, Datum datumRodjenja, int brDnevnik, Razredi razred) :Osoba(imePrezime, datumRodjenja) {
		if (brDnevnik <= 0)
			throw exception("Broj u dnevniku nije validan!");
		_brojUDnevniku = brDnevnik;
		_razred = razred;
	}
	Ucenik(const Ucenik& u):_brojUDnevniku(u._brojUDnevniku),_razred(u._razred),_predmetiOcjene(u._predmetiOcjene) {
		for (size_t i = 0; i < u._nastavnici.size(); i++)
			_nastavnici.push_back(u._nastavnici[i]);
	}
	~Ucenik() {
		for (size_t i = 0; i < _nastavnici.size(); i++)
			_nastavnici[i] = nullptr;
		_nastavnici.clear();
	}

	Ucenik& operator=(const Ucenik& u) {
		if (&u == this)
			return *this;
		_brojUDnevniku = u._brojUDnevniku;
		_razred = u._razred;
		for (size_t i = 0; i < _nastavnici.size(); i++)
			_nastavnici[i] = nullptr;
		_nastavnici.clear();
		for (size_t i = 0; i < u._nastavnici.size(); i++)
			_nastavnici.push_back(u._nastavnici[i]);
		_predmetiOcjene = u._predmetiOcjene;
		return *this;
	}

	bool AddNastavnik(Nastavnik& nastavnik) {
		if (!nastavnik.PredajeUTomRazredu(_razred))
			return false;
		for (size_t i = 0; i < _nastavnici.size(); i++)
			if (*_nastavnici[i] == nastavnik)
				return false;
		_nastavnici.push_back(&nastavnik);
		return true;
	}
	bool AddPredmetOcjenu(Predmeti predmet,int ocjena) {
		if (ocjena < 1 || ocjena>5)
			return false;
		bool flagPostojiNastavnik = false;
		for (size_t i = 0; i < _nastavnici.size(); i++)
			if (_nastavnici[i]->PredajePredmetURazredu(_razred,predmet))
				flagPostojiNastavnik = true;
		if (!flagPostojiNastavnik)
			return false;
		_predmetiOcjene.AddElement(predmet, ocjena);
		return true;
	}

	vector<Nastavnik*>& getNastavnici() { return _nastavnici; }
	Kolekcija<Predmeti, int>& getPredmetiOcjene() {
		Kolekcija<Predmeti, int>* polozeni = new Kolekcija<Predmeti, int>;
		for (int i = 0; i < _predmetiOcjene.GetTrenutno(); i++)
			if (_predmetiOcjene.GetElement2(i) > 1)
				polozeni->AddElement(_predmetiOcjene.GetElement1(i), _predmetiOcjene.GetElement2(i));
		
		return *polozeni; }

	int GetProsjekByNastavnik(string imePrezime) {
		int indexNastavnika = -1;
		for (size_t i = 0; i < _nastavnici.size(); i++)
			if (_nastavnici[i]->GetImePrezime() == imePrezime)
				indexNastavnika = i;
		if (indexNastavnika == -1)
			return 0;

		double ocj = 0;
		int brOcj = 0;
		for (int i = 0; i < _predmetiOcjene.GetTrenutno(); i++)
			if (_nastavnici[indexNastavnika]->PredajePredmetURazredu(_razred, _predmetiOcjene.GetElement1(i))) {
				ocj += _predmetiOcjene.GetElement2(i);
				brOcj++;
			}
		ocj /= (double)brOcj;
		if (ocj - (int)ocj > 0)
			return ocj + 1;
		else
			return ocj;
	}
	void Info() {
		Osoba::Info();
		cout << "Broj u dnevniku: " << _brojUDnevniku;
		cout << "\nRazred: " << ispisRazredi[_razred] << endl;
		cout << crt << "---PREDMETI I OCJENE----" << crt;
		cout << _predmetiOcjene << crt;
	}
	friend ostream& operator<<(ostream&, Ucenik&);
};
ostream& operator<<(ostream& COUT, Ucenik& u) {
	u.Info();
	return COUT;
}

void main()
{
	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
	// CE BITI OZNACENO KAO "RE"
	// 3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_.TXT
	// 5. NAZIV I LISTA PARAMETARA FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U.
	// 6. NIJE POTREBNO BACATI IZUZETKE. SVE FUNKCIJE VRACAJU TRUE ILI FALSE
	****************************************************************************/
	try {
#pragma region TestiranjeDatuma
		Datum danas(19, 1, 2019); //datum ne mozemo dodati u buducnosti, niti neke vrijednosti koje nisu karakteristicne za datum
		Datum sutra(danas);
		Datum prekosutra;
		prekosutra = danas;
		cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion

#pragma region TestiranjeKolekcije
		Kolekcija<int, int> kolekcija1;
		int brojElemenata = 15;
		for (size_t i = 0; i < brojElemenata; i++)
			kolekcija1.AddElement(i, i);

		kolekcija1.RemoveElement(0);//uklanja element na osnovu lokacije
		cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
		cout << kolekcija1 << endl;

		Kolekcija<int, int> kolekcija2;
		kolekcija2 = kolekcija1;
		cout << kolekcija2 << crt;
		if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
			cout << "ISTI BROJ ELEMENATA" << endl;
		Kolekcija<int, int> kolekcija3(kolekcija2);
		cout << kolekcija3 << crt;
#pragma endregion

#pragma region testiranjenastavnika
		Nastavnik jasmin("Jasmin Azemovic", Datum(15, 2, 1980));
		Nastavnik adel("Adel Handzic", Datum(15, 10, 1981));

		if (jasmin.AddPredmetRazred(Matematika, I1))
			cout << "predmeti dodan" << endl;
		if (jasmin.AddPredmetRazred(Historija, I1))
			cout << "predmeti dodan" << endl;

		if (jasmin.AddPredmetRazred(Geografia, I1))
			cout << "predmeti dodan" << endl;

		if (jasmin.AddPredmetRazred(Geografia, I2))
			cout << "predmeti dodan" << endl;
		if (jasmin.AddPredmetRazred(Geografia, II1))
			cout << "predmeti dodan" << endl;

		if (jasmin.AddPredmetRazred(Geografia, II2))
			cout << "predmeti dodan" << endl;

		if (!jasmin.AddPredmetRazred(Geografia, IV3)) //NECE DODATI, JER  VEC PREDAJE U 2 ODJELJENJA U 2 razlicita RAZREDA
			cout << "predmeti nije dodan" << endl;

		if (!jasmin.AddPredmetRazred(Geografia, I3)) //NECE DODATI, JER  VEC PREDAJE U 2 ODJELJENJA U TOM RAZREDU
			cout << "predmeti nije dodan" << endl;

		if (adel.AddPredmetRazred(Historija, III1))
			cout << "predmeti dodan" << endl;

		if (adel.AddPredmetRazred(Matematika, III1))
			cout << "predmeti dodan" << endl;


		Nastavnik goran(adel);
		if (goran == adel)
			cout << "nastavnici identicni" << endl;
		goran = jasmin;
		if (goran == jasmin)
			cout << "nastavnici identicni" << endl;

		Kolekcija<Predmeti, Razredi> & predmetirazredi = jasmin.GetPredmetiRazredi();

		if (!jasmin.setBrojTelefona("+38761111222"))
			cout << "broj telefona nije dodan!" << endl;
		if (!jasmin.setBrojTelefona("+387-61-111-222"))
			cout << "broj telefona nije dodan!" << endl;
		if (jasmin.setBrojTelefona("+387(61)-111-222"))
			cout << "broj telefona uspjesno dodan!" << endl;
#pragma endregion

#pragma region TestiranjeUcenika
		Ucenik denis("Denis Music", Datum(8, 10, 1990), 3, III1);
		Ucenik elmin("Elmin Sudic", Datum(15, 6, 1993), 1, I1);
		Ucenik adil("Adil Joldic", Datum(13, 8, 1992), 2, II1);
		/*VODITI RACUNA DA SE DODAJU REFERENCE NA NASTAVNIKE KAKO BI SVI UCENICI DIJELILI ISTE INFORMACIJE O
		NASTAVNICIMA U SLUCAJU DA SE NASTAVNIKU DODA NEKI PREDMET*/
		/*ZA USPJESNO DODAVANJE, NASTAVNIK MORA POSJEDOVATI (PREDAVATI) NAJMANJE JEDAN PREDMET U TOM RAZREDU */
		if (denis.AddNastavnik(jasmin))
			cout << "Nastavnik uspjesno dodan" << endl;
		/*ONEMOGUCITI DODAVANJE ISTIH NASTAVNIKA; POJAM ISTI PODRAZUMIJEVA IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA*/
		if (denis.AddNastavnik(adel))
			cout << "Nastavnik uspjesno dodan" << endl;
		/*DA BI SE DODALA OCJENA ZA ODREDJENI PREDMET UCENIK MORA POSJEDOVATI NASTAVNIKA KOJI PREDAJE TAJ PREDMET*/
		if (denis.AddPredmetOcjenu(Matematika, 2))
			cout << "Ocjena uspjesno dodana" << endl;
		if (denis.AddPredmetOcjenu(Historija, 4))
			cout << "Ocjena uspjesno dodana" << endl;

		vector<Nastavnik*> & nastavnici = denis.getNastavnici();
		cout << "Ucenik Denis ima dodijeljena " << nastavnici.size() << " nastavnika" << endl;

		Kolekcija<Predmeti, int> & uspjeh = denis.getPredmetiOcjene();
		cout << "Denis ima " << uspjeh.GetTrenutno() << " polozenih predmeta" << endl;

		int prosjecnaJasmin = denis.GetProsjekByNastavnik("Jasmin Azemovic");
		int prosjecnaAdel = denis.GetProsjekByNastavnik("Adel Handzic");

		if (prosjecnaJasmin > 0)
			cout << "Prosjecna ocjena kod nastavnika Jasmin Azemovic je " << denis.GetProsjekByNastavnik("Jasmin Azemovic") << endl;
		else cout << "Nastavnik Jasmin Azemovic nije ocjenivao niti jednog ucenika!\n";
		if (prosjecnaAdel > 0)
			cout << "Prosjecna ocjena kod nastavnika Adel Handzic je " << denis.GetProsjekByNastavnik("Adel Handzic") << endl;
		else cout << "Nastavnik Jasmin Azemovic nije ocjenivao niti jednog ucenika!\n";

		Ucenik denis2(denis);
		cout << denis2 << endl;

		cout << jasmin << endl;
#pragma endregion
	}
	catch (exception& obj) {
		cout << "ERROR: " << obj.what() << endl;
	}
	system("pause");
}
