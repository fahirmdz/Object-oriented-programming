//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<cstring>
#include<regex>
#include<thread>
#include<algorithm>
#include<iterator>
#include<string>
#include<ctime>
#include<chrono>
#include<sstream>

using namespace std;

/*1. Ukoliko je potrebno, implementirati nasljeđivanje 
 2. U okviru svih klasa implementirati potrebne(de)konstruktor funkcije 
 3. U slučaju nedozvoljenih operacija ili prekoračenja, koristeći klasu exception, 
    sve funkcije članice trebaju  baciti izuzetak sa odgovarajućom porukom
 4. Ukoliko smatrate da je neophodno, ispitni zadatak možete proširiti dodatnim funkcionalnostima 
    ili modifikovati postavljene zahtjeve.Proširenje ili modifikacije moraju pratiti adekvatni komentari 
	sa kojima ćete obrazložiti potrebu za implementiranim / modifikovanim funkcionalnostima.  
 5. Prilikom definisanja funkcija voditi računa da uništavanjem jednog objekta ne utječemo na stanje drugog objekta.
    Prilikom uništavanja objekata ili bilo kakvih modifikacija voditi računa o dealokaciji zauzete memorije.
	*/
const char* crt = "\n----------------------------------------------------\n";
char* AlocirajNizKaraktera(const char* a) {
	if (a == nullptr)
		return nullptr;
	int v = strlen(a) + 1;
	char* temp = new char[v];
	strcpy_s(temp, v, a);
	return temp;
}

template<class T1, class T2, int max> 
class Kolekcija {
T1  * _elementi1[max];  
T2  * _elementi2[max];  
int * _trenutnoElemenata;
void copyElements(const Kolekcija& k) {
	for (int i = 0; i < *k._trenutnoElemenata; i++) {
		_elementi1[i] = new T1(*k._elementi1[i]);
		_elementi2[i] = new T2(*k._elementi2[i]);
	}
	for (int i = *k._trenutnoElemenata; i < max; i++) {
		_elementi1[i] = nullptr;
		_elementi2[i] = nullptr;
	}
}
void remove(int i) {
	delete _elementi1[i]; _elementi1[i] = nullptr;
	delete _elementi2[i]; _elementi2[i] = nullptr;
	for (int j = i; j < *_trenutnoElemenata - 1; j++) {
		swap(_elementi1[j], _elementi1[j + 1]);
		swap(_elementi2[j], _elementi2[j + 1]);
	}
	(*_trenutnoElemenata)--;
}
public:  
	Kolekcija():_trenutnoElemenata(new int(0)) {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	Kolekcija(const Kolekcija& k) :_trenutnoElemenata(new int(*k._trenutnoElemenata)) {
		copyElements(k);
	}
	~Kolekcija() {
		for (int i = 0; i < *_trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutnoElemenata; _trenutnoElemenata = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;
		*_trenutnoElemenata = *k._trenutnoElemenata;
		for (int i = 0; i < *_trenutnoElemenata; i++) {
			delete _elementi1[i];
			delete _elementi2[i];
		}
		copyElements(k);
		return *this;
	}

	int GetTrenutno()const { return *_trenutnoElemenata; }
	int GetMax()const { return max; }
	T1& GetElement1(int index) {
		if (index < 0 || index >= *_trenutnoElemenata)
			throw exception("Index lokacije elementa nije validan!");
		return *_elementi1[index];
	}
	T2& GetElement2(int index) {
		if (index < 0 || index >= *_trenutnoElemenata)
			throw exception("Index lokacije elementa nije validan!");
		return *_elementi2[index];
	}
	//1. AddElement :: omogucava dodavanje novog elementa u kolekciju. Onemoguciti ponavljanje
	//   elemenata 
	void AddElement(T1 t1, T2 t2) {
		if (*_trenutnoElemenata == max) {
			cout << "Dostigli ste maximalni kapacitet kolekcije!\n";
			return;
		}
		_elementi1[*_trenutnoElemenata] =new T1( t1);
		_elementi2[(*_trenutnoElemenata)++] =new T2( t2);
	}
	
	//2. RemoveElement :: na osnovu parametra tipa T1. Ukoliko je 
	//   potrebno,prilikom uklanjanja elementa izvrsiti dealokaciju. Onemoguciti pojavljivanje 
	//   neinicijaliziranih elemenata, te ocuvati redoslijed dodavanja elemenata 
	void RemoveElement(T1& t1) {
		for (int i = 0; i < *_trenutnoElemenata; i++)
			if (*_elementi1[i] == t1)
				remove(i);
	}
	void RemoveElement(int index) {
		if (index < 0 || index >= *_trenutnoElemenata)
			throw exception("Index lokacije elementa nije validan!");
		remove(index);
	}
	//3. PopFront :: uklanja prvi element iz kolekcije(iz oba niza). Ukoliko je kolekcija prazna 
	//   funkcija vraca false, a u suprotnom funkcija vraca true 
	bool PopFront() {
		if (*_trenutnoElemenata == 0)
			return false;
		remove(0);
		return true;
	}

	//4. operator -=  :: uklanja sve elemnte iz kolekcije koji su identicni onima koje posjeduje 
	//   drugi objekat tipa Kolekcija koji je primljen kao parametar. Funkcija vraca broj uklonjenih 
	//   elemenata 
	int operator-=(Kolekcija& k) {
		int brojac = 0;
		for(int i=0;i<*_trenutnoElemenata;i++)
			for(int j=0;j<*k._trenutnoElemenata;j++)
				if (*_elementi1[i] == *k._elementi1[j] && *_elementi2[i] == *k._elementi2[j]) {
					remove(i);
					brojac++;
				}
		return brojac;
	}

	friend ostream& operator<<<>(ostream&, const Kolekcija&);
	friend bool operator==<>(Kolekcija&, Kolekcija&);
};

template<class T1,class T2,int max>
ostream& operator<<<>(ostream& COUT, const Kolekcija<T1, T2, max>& k) {
	for (int i = 0; i < *k._trenutnoElemenata; i++)
		cout << *k._elementi1[i] << " - " << *k._elementi2[i] << endl;
	return COUT;
}
template<class T1, class T2, int max>
bool operator==<>(Kolekcija<T1, T2, max>& k1, Kolekcija<T1, T2, max>& k2) {
	if (*k1._trenutnoElemenata == *k2._trenutnoElemenata) {
		for (int i = 0; i < *k1._trenutnoElemenata; i++)
			if (*k1._elementi1[i] != *k2._elementi1[i] || *k1._elementi2[i] != *k2._elementi2[i])
				return false;
		return true;
	}
	return false;
}

class Datum{
	int * _dan, *_mjesec, *_godina; 

	int GetDanaUMjesecu(int mjesec, int godina) {
		if (mjesec == 1 || mjesec == 3 || mjesec == 5 || mjesec == 7 || mjesec == 8 || mjesec == 12 || mjesec == 10)
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
		if (dan<1 || dan>GetDanaUMjesecu(mjesec, godina) || mjesec < 1 || mjesec>12 || godina < 1920)
			return false;
		struct tm ft;
		time_t t = time(NULL);
		localtime_s(&ft, &t);
		
		int trDan = ft.tm_mday, trMjesec = ft.tm_mon + 1, trGodina = ft.tm_year + 1900;
		if (godina > trGodina)
			return false;
		if (godina == trGodina)
			if (mjesec > trMjesec)
				return false;
			else if (mjesec == trMjesec && dan > trDan)
				return false;
		return true;
	}

public:
	Datum(int dan=1,int mjesec=1,int godina=2000){
		if (!validneVrijednosti(dan, mjesec, godina))
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& d):_dan(new int(*d._dan)),_mjesec(new int(*d._mjesec)),_godina(new int(*d._godina)){}
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
	bool isStarijiOD(Datum d) {
		if (*_godina < *d._godina)
			return true;
		else if (*_godina > *d._godina)
			return false;
		else if (*_mjesec < *d._mjesec)
			return true;
		else if (*_mjesec > *d._mjesec)
			return false;
		else if (*_dan < *d._dan)
			return true;
		else if (*_dan > *d._dan)
			return false;
		return false;
	}
	string toString()const { return string(to_string(*_dan) + "/" + to_string(*_mjesec) + "/" + to_string(*_godina)); }

	friend ostream&  operator<<(ostream&, const Datum&);
	friend bool operator==(Datum&, Datum&);
	friend bool operator!=(Datum&, Datum&);
}; 
ostream& operator<<(ostream& COUT, const Datum& d) {
	cout << *d._dan << "/" << *d._mjesec << "/" << *d._godina;
	return COUT;			
}
bool operator==(Datum& d1, Datum &d2) {
	return *d1._godina == *d2._godina && *d1._mjesec == *d2._mjesec && *d1._dan == *d2._dan;
}
bool operator!=(Datum& d1, Datum &d2) { return !(d1 == d2); }

class Pregled{  
char * _dijagnoza; 
vector<string> _terapije; 
Datum _datumPregleda;

int nadjiTerapiju(string terapija) {
	transform(terapija.begin(), terapija.end(), terapija.begin(), tolower);
	for (size_t i = 0; i < _terapije.size(); i++){
		string temp = _terapije[i];
		transform(temp.begin(), temp.end(), temp.begin(), tolower);
		if (temp == terapija)
			return i;
	}
	return -1;
}
public:  
	Pregled():_dijagnoza(AlocirajNizKaraktera(" EMPTY ")){}
	Pregled(const char* dijagnoza,Datum datum):_dijagnoza(AlocirajNizKaraktera(dijagnoza)),_datumPregleda(datum){}
	Pregled(const Pregled& p):_dijagnoza(AlocirajNizKaraktera(p._dijagnoza)),_datumPregleda(p._datumPregleda),_terapije(p._terapije){}
	~Pregled() { delete[] _dijagnoza; _dijagnoza = nullptr; }

	Pregled& operator=(const Pregled& p) {
		if (&p == this)
			return *this;
		delete[] _dijagnoza;
		_dijagnoza = AlocirajNizKaraktera(p._dijagnoza);
		_datumPregleda = p._datumPregleda;
		_terapije = p._terapije;
		return *this;
	}

	char* GetDijagnozu()const { return _dijagnoza; }
	Datum GetDatum() { return _datumPregleda; }
	vector<string>& GetTerapije() { return _terapije; }

	//1. Info :: vraca string svih informacija o Pregled-u 
	string Info()const {
		stringstream obj;
		obj << crt << "Datum pregleda: ";
		obj<< _datumPregleda.toString()<< "\nDijagnoza: " << _dijagnoza;
		obj << "\n--Terapije--" << crt;
		int sizeT = (int)_terapije.size();
		if (sizeT > 0)
			for (size_t i = 0; i < sizeT; i++)
				obj << "- " << _terapije[i] << endl;
		else
			obj << "--NEMA PREPISANIH TERAPIJA--";
		return obj.str();
	}
	//2. AddTerapija :: dodaje novu terapiju. Onemoguciti ponavljanje terapija. Koristiti iteratore 
	bool AddTerapije(string terapija) {
		vector<string>::iterator it = _terapije.begin();
		for (it; it != _terapije.end(); it++)
			if (*it == terapija)
				return false;
		_terapije.push_back(terapija);
		return true;
	}

	//3. RemoveTerapija :: uklanja terapiju koja je primljena kao parametar. Koristiti iteratore
	bool RemoveTerapija(string terapija) {
		int indexTerapije = nadjiTerapiju(terapija);
		if (indexTerapije == -1)
			return false;
		_terapije.erase(_terapije.begin() + indexTerapije);
		return true;
	}
	bool posjedujeTerapiju(string terapija) {
		if (nadjiTerapiju(terapija) != -1)
			return true;
		return false;
	}
	friend ostream& operator<<(ostream&, const Pregled&);
	friend bool operator==(Pregled&, Pregled&);
	friend bool operator!=(Pregled&, Pregled&);

};
ostream& operator<<(ostream& COUT, const Pregled& p) {
	cout << p.Info();
	return COUT;
}
bool operator==(Pregled& p1, Pregled& p2) { return strcmp(p1._dijagnoza, p2._dijagnoza) == 0 && p1._datumPregleda == p2._datumPregleda && p1._terapije == p2._terapije; }
bool operator!=(Pregled& p1, Pregled& p2) { return !(p1 == p2); }

 class Pacijent{
 protected: 
	 static int generatorID;
	 const int _pacijentID;
	 char * _imePrezime; 
	 //tip bool čuva podatak o tome da li je zakazan kontrolni pregled 
	 //objekat tipa Pregled je za podatke o dijagnozi i terapijama 
     Kolekcija<Pregled *, bool, 20> * _preglediPacijenta; 
 
 public:   
	 Pacijent():_pacijentID(generatorID++),_imePrezime(AlocirajNizKaraktera(" NEMA VRIJEDNOST ")){
		 _preglediPacijenta = new Kolekcija<Pregled*, bool, 20>;
	 }
	 Pacijent(const char* imePrezime):_pacijentID(generatorID) {
		 if (!regex_match(imePrezime, regex("[A-Z]{1}[a-z]{1,15}\\s{1,2}[A-Z]{1}[a-z]{1,15}")))
			 throw exception("Ime i prezime pacijenta nisu validni!");
		 _imePrezime = AlocirajNizKaraktera(imePrezime);
		 _preglediPacijenta = new Kolekcija < Pregled*, bool, 20>;
		
	 }
	 Pacijent(const Pacijent& p):_pacijentID(p._pacijentID),_imePrezime(AlocirajNizKaraktera(p._imePrezime)) {
		 _preglediPacijenta = new Kolekcija < Pregled*, bool, 20>;
		 for (int i = 0; i < p._preglediPacijenta->GetTrenutno(); i++)
			 _preglediPacijenta->AddElement(new Pregled(*p._preglediPacijenta->GetElement1(i)), &p._preglediPacijenta->GetElement2(i));
	 }
	 ~Pacijent() {
		 delete[] _imePrezime; _imePrezime = nullptr;
		 for (int i = 0; i < _preglediPacijenta->GetTrenutno(); i++) {
			 delete _preglediPacijenta->GetElement1(i);
			 _preglediPacijenta->GetElement1(i) = nullptr;
		 }
		 delete _preglediPacijenta;
		 _preglediPacijenta = nullptr;
	 }

	 Pacijent& operator=(const Pacijent& p) {
		 if (&p == this)
			 return *this;
		 delete[] _imePrezime;
		 _imePrezime = AlocirajNizKaraktera(p._imePrezime);
		 for (int i = 0; i < _preglediPacijenta->GetTrenutno(); i++)
			 delete _preglediPacijenta->GetElement1(i);
		 delete _preglediPacijenta;
		 _preglediPacijenta = new Kolekcija<Pregled*, bool, 20>;
		 for (int i = 0; i < p._preglediPacijenta->GetTrenutno(); i++)
			 _preglediPacijenta->AddElement(new Pregled(*p._preglediPacijenta->GetElement1(i)),p._preglediPacijenta->GetElement2(i));
		 return *this;
	 }

	 //1. operator [] :: vraca Pregled kod koga je postavljena dijagnoza koja je proslijedjena kao   
	 //   parametar. Dakle, operatorska funkcija prima dijagnozu kao parametar 
	 Pregled* operator[](const char* dijagnoza) {
		 if (dijagnoza == nullptr)
			 return nullptr;
		 string dijagnozaMala = dijagnoza;
		 transform(dijagnozaMala.begin(), dijagnozaMala.end(), dijagnozaMala.begin(), tolower);
		 for (int i = 0; i < _preglediPacijenta->GetTrenutno(); i++) {
			 string tempDijagnoza = _preglediPacijenta->GetElement1(i)->GetDijagnozu();
			 transform(tempDijagnoza.begin(), tempDijagnoza.end(), tempDijagnoza.begin(), tolower);
			 if (tempDijagnoza==dijagnozaMala)
				 return _preglediPacijenta->GetElement1(i);
		 }
		 return nullptr;
	 }

	 //2. AddPregled :: dodaje podatke o novom pregledu pacijenta. Ukoliko je moguće, osigurati 
	 //   proširanje nize. U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false. 
	 bool AddPregled(Pregled& pregled, bool zakazanKontrolni = false) {
		 if (_preglediPacijenta->GetMax() == _preglediPacijenta->GetTrenutno())
			 return false;//popunjena lista pregleda
		 _preglediPacijenta->AddElement(new Pregled(pregled), zakazanKontrolni);
		 return true;
	 }
	 
	 //3. RemovePregled :: na osnovu primljenog parametra (dijagnoze) uklanja sve podatke o pregledu.
	 //   U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false  
	 bool RemovePregled(const char* dijagnoza) {
		 Pregled* trazeni = this->operator[](dijagnoza);
		 if (trazeni == nullptr)
			 return false;
		 int indexPregleda = 0;
		 for (int i = 1; i < _preglediPacijenta->GetTrenutno(); i++)
			 if (*trazeni == *_preglediPacijenta->GetElement1(i))
				 indexPregleda = i;
		 _preglediPacijenta->RemoveElement(indexPregleda);
		 trazeni = nullptr;
		 return true;
	 }

	 //4. RemovePregledByTerapija :: na osnovu primljenog parametra (terapije) uklanja podatke o svim 
	 //   pregledima kod koji je propisana trazena terapija, ali pod uslovom da je tokom tog pregleda 
	 //   zakazan novi pregled. U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false 
	 bool RemovePregledByTerapija(string terapija) {
		 int brojac = 0;
		 for (int i = 0; i < _preglediPacijenta->GetTrenutno(); i++)
			 if (_preglediPacijenta->GetElement1(i)->posjedujeTerapiju(terapija) && _preglediPacijenta->GetElement2(i)) {
				 _preglediPacijenta->RemoveElement(i);
				 brojac++;
			 }
		 if (brojac > 0)
			 return true;
		 return false;
	 }

	 //5. RemovePregledByDatum :: uklanja sve preglede koji su izvrseni nakon datuma primljenog kao  
	 //   parametar
	 void RemovePregledBydatum(Datum d) {
		 for (int i = 0; i < _preglediPacijenta->GetTrenutno(); i++)
			 if (d.isStarijiOD(_preglediPacijenta->GetElement1(i)->GetDatum()))
				 _preglediPacijenta->RemoveElement(i);
	 }

	 friend ostream& operator<<(ostream&, const Pacijent&);
	 friend bool operator==(Pacijent&, Pacijent&);
 }; 
 ostream& operator<<(ostream& COUT, const Pacijent& p) {
	 cout <<crt<< "::::PACIJENT INFO:::::"<<crt<<"Ime i prezime: " << p._imePrezime << endl;
	 cout << "ID Pacijent: " << p._pacijentID << endl;
	 cout << crt << "---LISTA PREGLEDA---\n";
	 int sizeP = p._preglediPacijenta->GetTrenutno();
	 if (sizeP > 0)
		 for (int i = 0; i < sizeP; i++)
			 cout << *p._preglediPacijenta->GetElement1(i) << crt;
	 else
		 cout << "---TRENUTNO NEMA EVIDENTIRANIH PREGLEDA---" << crt;
	 return COUT;
 }
 bool operator==(Pacijent& p1, Pacijent& p2) {
	 return strcmp(p1._imePrezime, p2._imePrezime) == 0 && p1._pacijentID == p2._pacijentID;
 }
 int Pacijent::generatorID = 1;
void main() { 

		//izbjegavajte koristenje vlastitog imena i prezimena. 
		//provjeriti validnost izvrsenja svih dostupnih funkcionalnosti na objektu tipa Pacijent
		//ukratko pojasniti i demonstrirati koncepciju polimorfizma
	try {
#pragma region TestiranjeKolekcija
		Kolekcija<int, int, 10> k1;
		for (int i = 0; i < 11; i++) {
			int j = i + 2;
			k1.AddElement(i, j);
		}
		cout << k1 << endl;

		Kolekcija<int, int, 10> k2(k1);
		cout << k2 << endl << endl;

		Kolekcija<int, int, 10> k3;
		k3 = k2;
		cout << k3 << endl;

		for (int i = 0; i < 5; i++)
			k3.PopFront();
		cout << k3 << endl;
		k1 -= k3;
		cout << k1 << endl;

#pragma endregion

#pragma region TestiranjeDatuma  
		//POKUSAJ KREIRANJA DATUMA KOJI JE U BUDUCNOSTI ILI DATUMA SA VRIJEDNOSTIMA KOJE NISU KARAKTERISTICNE ZA DATUM
		//PROUZROKOVACE BACANJE IZUZETKA U KONSTRUKTORU DATUMA
		try {
			Datum datumTest(5, 6, 2019); //PRIMJER DATUMA U BUDUCNOSTI
		}
		catch (exception& obj) {
			cout << "ERROR: " << obj.what() << endl;
		}

		Datum d1(1, 1, 2019), d2;
		cout << d1 << endl;
		d2 = d1;
		cout << d2 << endl;
		Datum d3(d2);
		cout << d3 << endl;
		if (d2 == d3)
			cout << "ISTI SU!\n";

#pragma endregion

#pragma region TestiranjePregleda
		Pregled p1("Uganuce zgloba sake", Datum(22, 1, 2019)), p2, p4("Istegnuce ligamenata", d1);
		cout << p1 << endl;
		p1.AddTerapije("Krema Zglobex 2xDnevno");
		p1.AddTerapije("Istezanje sake");
		cout << p1 << endl;
		p2 = p1;
		p2.RemoveTerapija("krema zglobex 2xdnevno"); //omogucen pronalazak terapije iako su unijeta mala slova
		cout << p2 << endl;
		Pregled p3(p2);
		cout << p3 << endl;
		if (p3 == p2)
			cout << "ISTI SU!\n";
#pragma endregion

#pragma region TestiranjePacijenta
		Pacijent pacijent1("John Newman"), pacijent2, pacijent4;
		pacijent1.AddPregled(p1,true);
		pacijent1.AddPregled(p4,true);
		cout << pacijent1 << endl;
		pacijent4 = pacijent1;

		pacijent2 = pacijent1;
		cout << pacijent2 << endl;
		pacijent2.RemovePregled("istegnuce ligamenata"); //omogucen pronalazak dijagnoze iako su unijeti mala slova
		Pacijent pacijent3;
		pacijent3 = pacijent2;
		cout << pacijent3 << endl;
		Pregled* trazeni = pacijent1["istegnuce ligamenata"];
		if (trazeni != nullptr)
			cout << *trazeni << endl;
		trazeni = nullptr;
		pacijent1.RemovePregledBydatum(Datum(10, 1, 2019)); //BRISE PREGLED KOJI JE BIO 22.1.2019 (p1), JER JE BIO NAKON DEFINISANOG DATUMA
		cout << pacijent1 << endl;
		pacijent4.RemovePregledByTerapija("krema zglobex 2xdnevno"); //BRISE PREGLED p1, JER POSJEDUJE OVU TERAPIJU I ZAKAZAN JE PONOVNI PREGLED
		cout << pacijent4 << endl;
#pragma endregion
	}
	catch (exception& obj) {
		cout << "ERROR: " << obj.what() << endl;
	}
} 