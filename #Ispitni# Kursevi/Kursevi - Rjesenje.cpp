//fahirmdz GitHub

#include"pch.h"
#include <iostream>
#include<exception>
#include<cstring>
#include<string>
#include<vector>
#include<regex>
#include<thread>
#include<ctime>
#include<algorithm>
#include<iterator>
using namespace std;

const char* crt = "\n------------------------------------------------------------------\n";
char* AlocirajNizKaraktera(const char* a) {
	if (a == nullptr)
		return nullptr;
	int v = strlen(a) + 1;
	char* temp = new char[v];
	strcpy_s(temp, v, a);
	return temp;
}
class Izuzetak;

class Datum {
	int *_dan, *_mjesec, *_godina;
	int DanaUMjesecu(int mjesec, int godina) {
		if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 10 || mjesec == 12)
			return 31;
		else if (mjesec == 2)
			if (godina % 400 == 0 && godina % 100 == 0 && godina % 4 == 0)
				return 29;
			else
				return 28;
		else
			return 30;
	}
	bool validneVrijednosti(int dan, int mjesec, int godina) {
		if (dan<1 || dan>DanaUMjesecu(mjesec, godina) || mjesec < 1 || mjesec>12 || godina < 1920)
			return false;
		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		int trDan = timeinfo.tm_mday, trMjesec = timeinfo.tm_mon + 1, trGodina = timeinfo.tm_year + 1900;
		if (godina > trGodina)
			return false;
		if (godina == trGodina) {
			if (mjesec > trMjesec)
				return false;
			else if (mjesec == trMjesec && dan > trDan)
				return false;
		}
		return true;
	}
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		if (!validneVrijednosti(dan, mjesec, godina))
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
		delete _dan; _dan = new int(*d._dan);
		delete _mjesec; _mjesec = new int(*d._mjesec);
		delete _godina; _godina = new int(*d._godina);
		return *this;
	}
	string toString()const {
		return string(to_string(*_dan) + "/" + to_string(*_mjesec) + "/" + to_string(*_godina));
	}

	bool isKolizija(Datum d) {
		if (*d._godina > *_godina)
			return false;
		else if (*d._godina < *_godina)
			return true;
		else if (*d._mjesec > *_mjesec)
			return false;
		else if (*d._mjesec < *_mjesec)
			return true;
		else if (*d._dan > *_dan)
			return false;
		else if (*d._dan < *_dan)
			return true;
		else
			return true;

	}
	friend ostream& operator<< (ostream &, const Datum &);
	friend bool operator==(Datum&, Datum&);
	friend bool operator!=(Datum&, Datum&);
};
ostream& operator<< (ostream &COUT, const Datum &obj) {
	COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
	return COUT;
}
bool operator==(Datum& d1, Datum& d2) {
	return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan;
}
bool operator!=(Datum& d1, Datum& d2) {return !(d1==d2);}


Datum GetCurrentDate() {
	struct tm tf;
	time_t t = time(NULL);
	localtime_s(&tf, &t);
	return Datum(tf.tm_mday, tf.tm_mon + 1, tf.tm_year + 1900);
}

class Izuzetak :public exception {
	Datum _datum;//KORISTITE DANASNJI DATUM
	string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
	int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK

public:
	Izuzetak(const char* msg, Datum date, string funk, int line) :exception(msg), _datum(date), _funkcija(funk), _linija(line) {}

	const char* what()const {
		string x =string("ERROR: ")+ exception::what() + string("\nDatum: ") + _datum.toString() + "\nFunkcija: " + _funkcija + "\nLinije koda: " + to_string(_linija);
		return AlocirajNizKaraktera(x.c_str());
	}
};
template<class T1, class T2, int max>
class Kolekcija{
	T1 _elementi1[max];
	T2 _elementi2[max];
	int * _trenutno;
	void copyElements(const Kolekcija& k) {
		for (int i = 0; i < *_trenutno; i++) {
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}
public:
	Kolekcija() :_trenutno(new int(0)) {}
	Kolekcija(const Kolekcija& k) :_trenutno(new int(*k._trenutno)) {
		copyElements(k);
	}
	~Kolekcija() {delete _trenutno; _trenutno = nullptr;}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		*_trenutno = *k._trenutno;
		copyElements(k);
		return *this;
	}
	void AddElement(T1 t1, T2 t2) {
		if (*_trenutno == max)
			throw Izuzetak("Dostigli ste maximum! Ne mozete dodati vise elemenata u kolekciju!", GetCurrentDate(), __FUNCTION__, __LINE__);
		_elementi1[*_trenutno] =t1;
		_elementi2[(*_trenutno)++] = t2;
	}

	int GetMax()const { return max; }
	int GetTrenutno()const { return *_trenutno; }
	T1& GetElement1(int index) {
		if (index < 0 || index >= *_trenutno)
			throw Izuzetak("Index lokacija elementa nije validna!", GetCurrentDate(), __FUNCTION__, __LINE__);
		return _elementi1[index];
	}
	T2& GetElement2(int index) {
		if (index < 0 || index >= *_trenutno)
			throw Izuzetak("Index lokacija elementa nije validna!", GetCurrentDate(), __FUNCTION__, __LINE__);
		return _elementi2[index];
	}
	friend ostream& operator<<<>(ostream&, const Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
};
template<class T1,class T2,int max>
ostream& operator<<<>(ostream& COUT, const Kolekcija<T1, T2, max>& k) {
	for (int i = 0; i < *k._trenutno; i++)
		cout << k._elementi1[i] << " - " <<k._elementi2[i] << endl;
	return COUT;
}
template<class T1,class T2,int max>
bool operator==(Kolekcija<T1, T2, max>& k1, Kolekcija<T1, T2,max>& k2) {
	if (*k1._trenutno == *k2._trenutno) {
		for (int i = 0; i < *k1._trenutno; i++)
			if (k1._elementi1[i] != k2._elementi1[i] || k1._elementi2[i] != k1._elementi2[i])
				return false;
		return true;
	}
	return false;
}





const char* kurseviChar[] = { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
class Kurs{
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
public:
	Kurs():_imePredavaca(AlocirajNizKaraktera("EMPTY")){}
	Kurs(enumKursevi kurs, const char* imePredavaca, Datum pocetak, Datum kraj) {
		if (imePredavaca == nullptr)
			throw Izuzetak("Greska! Morate proslijediti ime predavaca!", GetCurrentDate(), __FUNCTION__, __LINE__);
		if (!regex_match(imePredavaca, regex("^[A-Z]{1}[a-z]{1,15}\\s{1,2}[A-Z]{1}[a-z]{1,15}$")))
			throw Izuzetak("Ime i prezime nije validno!", GetCurrentDate(), __FUNCTION__, __LINE__);
		_kurs = kurs;
		_pocetak = pocetak;
		_kraj = kraj;
		_imePredavaca = AlocirajNizKaraktera(imePredavaca);
	}
	Kurs(const Kurs& k):_imePredavaca(AlocirajNizKaraktera(k._imePredavaca)),_pocetak(k._pocetak),_kraj(k._kraj),_kurs(k._kurs){}
	~Kurs() { delete[] _imePredavaca; _imePredavaca = nullptr; }

	Kurs& operator=(const Kurs& k) {
		if (&k == this)
			return *this;
		delete[] _imePredavaca; 
		_imePredavaca = AlocirajNizKaraktera(k._imePredavaca);
		_pocetak = k._pocetak;
		_kraj = k._kraj;
		_kurs = k._kurs;
		return *this;
	}

	enumKursevi GetKurs() { return _kurs; }
	Datum GetPocetakDatum() { return _pocetak; }
	Datum GetKrajDatum()const { return _kraj;}
	char* GetImePredavaca()const { return _imePredavaca; }

	friend bool operator==(Kurs&, Kurs&);
	friend ostream& operator<<(ostream&, const Kurs&);
	friend bool operator!=(Kurs&, Kurs&);
};
bool operator==(Kurs& k1, Kurs& k2) {
	return k1._kurs == k2._kurs && k1._pocetak == k2._pocetak && strcmp(k1._imePredavaca, k2._imePredavaca) == 0;
}
bool operator!=(Kurs& k1, Kurs& k2) {return !(k1==k2);}
ostream& operator<<(ostream& COUT, const Kurs& k) {
	cout << crt << "Kurs: " << kurseviChar[k._kurs];
	cout << "\nDatum pocetka: " << k._pocetak;
	cout << "\nDatum kraja: " << k._kraj;
	cout << "\nPredavac: " << k._imePredavaca << crt;
	return COUT;
}
bool ProvjeriFormatTelefona(string telefon) {
	return regex_match(telefon, regex("\\+?\\d{0,3}[-(]?\\d{2,3}[-)]?[-/\\s]?\\d{3}[-\\s]?\\d{3}"));
}
class Polaznik{
	static int ID;
	const int _polaznikID;
	//SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
	const char * _imePrezime;
	string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
	Kolekcija<Kurs *, int, 10> _uspjesnoOkoncaniKursevi;
	//INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
	Polaznik(const char * imePrezime, string telefon) : _polaznikID(ID++){
		if (!ProvjeriFormatTelefona(telefon))
			throw Izuzetak("Broj telefona nije validan!", GetCurrentDate(), __FUNCTION__, __LINE__);
		if (imePrezime==nullptr || !regex_match(imePrezime, regex("^[A-Z]{1}[a-z]{2,20}\\s{1,2}[A-Z]{1}[a-z]{2,20}$")))
			throw Izuzetak("Ime i prezime polaznika nisu validni", GetCurrentDate(), __FUNCTION__, __LINE__);
		_imePrezime = AlocirajNizKaraktera(imePrezime);
		_kontaktTelefon = telefon;	
	}
	Polaznik(const Polaznik& p):_polaznikID(p._polaznikID),_imePrezime(AlocirajNizKaraktera(p._imePrezime)),_kontaktTelefon(p._kontaktTelefon){
		for (int i = 0; i < p._uspjesnoOkoncaniKursevi.GetTrenutno(); i++) {
			Polaznik& temp = *const_cast<Polaznik*>(&p);
			_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*temp._uspjesnoOkoncaniKursevi.GetElement1(i)), temp._uspjesnoOkoncaniKursevi.GetElement2(i));
		}
	}
	~Polaznik(){
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++){
			delete _uspjesnoOkoncaniKursevi.GetElement1(i);
			_uspjesnoOkoncaniKursevi.GetElement1(i) = nullptr;
		}
	}
	Polaznik& operator=(const Polaznik& p) {
		if (&p == this)
			return *this;
		delete[] _imePrezime;
		_imePrezime = AlocirajNizKaraktera(p._imePrezime);
		_kontaktTelefon = p._kontaktTelefon;

		for (int i = 0; i < p._uspjesnoOkoncaniKursevi.GetTrenutno(); i++) {
			delete _uspjesnoOkoncaniKursevi.GetElement1(i);
			Polaznik& temp = *const_cast<Polaznik*>(&p);
			_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*temp._uspjesnoOkoncaniKursevi.GetElement1(i)), temp._uspjesnoOkoncaniKursevi.GetElement2(i));
		}
		return *this;
	}
	int GetID()const { return _polaznikID;}
	const char* GetImePrezime()const { return _imePrezime; }
	string GetKontaktTelefon()const { return _kontaktTelefon; }
	bool UspjesnoOkoncan(Kurs& k) {
		for (int i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
			if (*_uspjesnoOkoncaniKursevi.GetElement1(i) == k)
				return true;
		return false;
	}
	void DodajUspjehZaKurs(Kurs& k,int uspjeh) {
		_uspjesnoOkoncaniKursevi.AddElement(new Kurs(k), uspjeh);
	}
	friend ostream& operator<<(ostream&, const Polaznik&);
	friend bool operator==(Polaznik&, Polaznik&);
	friend bool operator!=(Polaznik&, Polaznik&);
};
ostream& operator<<(ostream& COUT, const Polaznik& p) {
	cout << crt << "ID: " << p._polaznikID;
	cout << "\nIme i prezime: " << p._imePrezime;
	cout << "\nKontakt telefon: " << p._kontaktTelefon << crt;
	return COUT;
}
bool operator==(Polaznik& p1, Polaznik& p2) {

	if (strcmp(p1._imePrezime, p2._imePrezime) == 0 && p1._uspjesnoOkoncaniKursevi.GetTrenutno() == p2._uspjesnoOkoncaniKursevi.GetTrenutno()) {
		for (int i = 0; i < p1._uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
			if (*p1._uspjesnoOkoncaniKursevi.GetElement1(i) != *p2._uspjesnoOkoncaniKursevi.GetElement1(i))
				return false;
		return true;
	}
	return false;
}
bool operator!=(Polaznik& p1, Polaznik& p2) {return !(p1==p2);}


int Polaznik::ID = 1;

class SkillsCentar
{
	string _nazivCentra;
	vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
	Kolekcija<Kurs, Polaznik *, 150> _aplikanti;

	int GetLokacijuKursa(Kurs& k) {
		for (size_t i = 0; i < _kursevi.size(); i++)
			if (k == _kursevi[i])
				return i;
		return -1;
	}
public:
	SkillsCentar(string naziv) { 
		if (!regex_match(naziv, regex("^[a-zA-Z0-9\\s\\-]{4,30}$")))
			throw Izuzetak("Naziv skills centra nije validan!", GetCurrentDate(), __FUNCTION__, __LINE__);
		_nazivCentra = naziv; 
	}
	SkillsCentar(const SkillsCentar & obj) :_kursevi(obj._kursevi){
		_nazivCentra = obj._nazivCentra;
		for (int i = 0; i < obj._aplikanti.GetTrenutno(); i++) {
			SkillsCentar& temp = *const_cast<SkillsCentar*>(&obj);
			_aplikanti.AddElement(temp._aplikanti.GetElement1(i), temp._aplikanti.GetElement2(i));
		}
	}
	~SkillsCentar() {
		for (int i = 0; i < _aplikanti.GetTrenutno(); i++) 
			_aplikanti.GetElement2(i) = nullptr;
		
	}

	SkillsCentar& operator=(SkillsCentar& sc) {
		if (&sc == this)
			return *this;
		_nazivCentra = sc._nazivCentra;
		_kursevi = sc._kursevi;
		for (int i = 0; i < sc._aplikanti.GetTrenutno(); i++)
			_aplikanti.AddElement(sc._aplikanti.GetElement1(i), sc._aplikanti.GetElement2(i));
		return *this;
	}

	bool ProvjeriKoliziju(Kurs& kurs){
		if ((int)_kursevi.size() == 0)
			return true;

		for (size_t i = 0; i < _kursevi.size(); i++)
			if (_kursevi[i].GetKrajDatum().isKolizija(kurs.GetPocetakDatum()))
				return false;
		return true;
	}
	void AddKurs(Kurs& k) {
		if (GetLokacijuKursa(k) != -1)
			throw Izuzetak("Ne mozete dodati isti kurs vise puta!", GetCurrentDate(), __FUNCTION__, __LINE__);
		_kursevi.push_back(k);
	}
	void AddAplikaciju(Kurs& kurs,Polaznik& polaznik) {
		int indexLokacijaKursa = GetLokacijuKursa(kurs);
		if (indexLokacijaKursa == -1)
			throw Izuzetak("Navedeni kurs ne postoji u nasem centru!", GetCurrentDate(), __FUNCTION__, __LINE__);
		for (int i = 0; i < _aplikanti.GetTrenutno(); i++)
			if (_aplikanti.GetElement1(i) == kurs && *_aplikanti.GetElement2(i) == polaznik) {
				cout << "Polaznik vec aplicirao za navedeni kurs!\n";
				return;
			}
		_aplikanti.AddElement(kurs, &polaznik);
	}
	void DodajUspjesnoOkoncanKurs(int id,Kurs& k,int uspjeh) {
		int idAplikacije = -1;
		for (int i = 0; i < _aplikanti.GetTrenutno(); i++)
			if (_aplikanti.GetElement1(i) == k && _aplikanti.GetElement2(i)->GetID() == id)
				idAplikacije = i;
		if (idAplikacije == -1) {
			cout << "Aplikant sa datim ID-om nije aplicirao ni na jedan kurs!";
			return;
		}
		if (uspjeh < 55)
			return;
		_aplikanti.GetElement2(idAplikacije)->DodajUspjehZaKurs(k, uspjeh);

	}
	// BROJ 1 OZNACAVA ID POLAZNIKA. FUNKCIJA JE ZADUZENA DA POLAZNIKU 
	// DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE 
	// POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%

	bool RemoveKurs(Kurs& kurs) {
		int indexKursa = GetLokacijuKursa(kurs);
		if (indexKursa == -1) 
			return false;
		
		bool apliciran = false;
		for (int i = 0; i < _aplikanti.GetTrenutno(); i++)
			if (kurs == _aplikanti.GetElement1(i))
				apliciran = true;
		if (apliciran) 
			return false;
		_kursevi.erase(_kursevi.begin() + indexKursa);
		return true;
	}

	vector<Polaznik> GetPolazniciByPredavac(const char* predavac,enumKursevi kurs) {
		vector<Polaznik> polaznici;
		int size = _aplikanti.GetTrenutno();
		if (size == 0)
			return polaznici;
		for (int i = 0; i < size; i++) {
			Kurs& kursTemp = _aplikanti.GetElement1(i);
			Polaznik* polaznikTemp = _aplikanti.GetElement2(i);
			if (kursTemp.GetKurs() == kurs && strcmp(kursTemp.GetImePredavaca(), predavac) == 0
				&& polaznikTemp->UspjesnoOkoncan(kursTemp))
				polaznici.push_back(*polaznikTemp);
			polaznikTemp = nullptr;
		}
		return polaznici;
	}
	friend ostream& operator<<(ostream&, const SkillsCentar&);
};
ostream& operator<<(ostream& COUT, const SkillsCentar& sc) {
	cout << crt << "::::::Skills centar '" << sc._nazivCentra << "'::::::::" << crt;
	cout << "-----Ponuda kurseva------" << crt;
	for (size_t i = 0; i < sc._kursevi.size(); i++)
		cout << i + 1 << ". " << sc._kursevi[i] << crt;
	return COUT;
}
void main(){
	try {
#pragma region TestiranjeDatuma
		Datum danas(21, 1, 2019);
		Datum sutra(danas);
		Datum prekosutra;
		prekosutra = danas;
		cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
		Kolekcija<int, int, 10> kolekcija1;
			for (int i = 0; i < kolekcija1.GetMax(); i++)
				kolekcija1.AddElement(i, i + 2);
			//FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
			cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
			//cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
			//FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
	
		

		cout << kolekcija1 << endl;
		Kolekcija<int, int, 10> kolekcija2;
		kolekcija2 = kolekcija1;
		cout << kolekcija2 << crt;
		if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
			cout << "ISTI BROJ ELEMENATA" << endl;
		Kolekcija<int, int, 10> kolekcija3(kolekcija2);
		cout << kolekcija3 << crt;
#pragma endregion

		#pragma region TestiranjeKursa
			Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
			cout << sef << endl;
			Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
			Kurs wsec(WindowsSecurity, "Zanin Vejzovic", Datum(16, 3, 2016), Datum(11, 4, 2016));

			//sef = msql;
			//if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
			//	cout << "ISTI SU KURSEVI" << endl;

			Kurs msql2(msql);
			cout << msql2 << endl;
		#pragma endregion

		#pragma region TestiranjePolaznika
			string telefon1 = "+387-683-111-111";
			string telefon2 = "(061)111222";
			string telefon3 = "(061)111-333";
		
			if (!ProvjeriFormatTelefona(telefon1))
				cout << "Broj " << telefon1 << " nije validan" << endl;
			else
				cout << "Broj telefona " << telefon1 << " je validan!\n";
			if (!ProvjeriFormatTelefona(telefon2))
				cout << "Broj " << telefon2 << " nije validan" << endl;
			else
				cout << "Broj telefona " << telefon2 << " je validan!\n";
			if (!ProvjeriFormatTelefona(telefon3))
				cout << "Broj " << telefon3 << " nije validan" << endl;
			else
				cout << "Broj telefona " << telefon3 << " je validan!\n";
		
		
			Polaznik denis("Denis Music", telefon1);
			Polaznik denis2("Denis Husic", telefon3);
			Polaznik adel("Adel Handzic", telefon2);
		
		#pragma endregion
		
		#pragma region TestiranjeCentra
			try{
				SkillsCentar mostar("Skills Center Mostar");
				if (mostar.ProvjeriKoliziju(sef)) //JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA I
					mostar.AddKurs(sef);          //KURSA SE NE SMIJE POKLAPAT
				if (mostar.ProvjeriKoliziju(msql))
					mostar.AddKurs(msql);
				if (mostar.ProvjeriKoliziju(msql2))
					mostar.AddKurs(msql2);
				if (mostar.ProvjeriKoliziju(wsec))
					mostar.AddKurs(wsec);
				else
					cout << "\nU zadanom vrijemenskom periodu se vec odrzava jedan kurs..\n";

				//ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
				mostar.AddAplikaciju(sef, denis);
				mostar.AddAplikaciju(sef, denis);

				//ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
				mostar.AddAplikaciju(msql, denis);
				mostar.AddAplikaciju(msql, denis);
				//ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
				cout << denis2 << endl;
				denis2 = denis;
				if (denis == denis2) //POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
					cout << "ISTI SU" << endl;
		
				mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);
				// BROJ 1 OZNACAVA ID POLAZNIKA. FUNKCIJA JE ZADUZENA DA POLAZNIKU 
				// DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE 
				// POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
				mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
				vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);
				cout << "\n\n****POLAZNICI KOJI SU USPJESNO ZAVRSILI " << kurseviChar[SoftwareEngeneeringFundamentals] << " KOD PROFESORA JASMINA AZEMOVICA******\n";
				copy(listaPolaznika.begin(), listaPolaznika.end(), ostream_iterator<Polaznik>(cout, "\n"));
				//VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
		
				cout << "\n\n----PONUDA KURSEVA PRIJE UKLANJANJA-----\n\n" << mostar << endl;
				if (mostar.RemoveKurs(wsec))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
					cout << "Kurs " << wsec << " uspjesno uklonjen iz centra " << mostar << endl;
		
				SkillsCentar sarajevo = mostar;
				cout << sarajevo << endl;
			}
			catch (exception& err)
			{
				cout << err.what() << endl;
			}
		#pragma endregion
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}
	system("pause");
}
