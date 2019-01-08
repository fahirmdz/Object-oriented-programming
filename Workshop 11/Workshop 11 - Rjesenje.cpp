#include"pch.h"
#include<iostream> 
#include<string>
#include<exception>
#include<cstring>

using namespace std;


class NedozvoljenaOperacija : public exception { 
	int _linijaKoda;
public:
	NedozvoljenaOperacija(const char* poruka, int codeLine) :_linijaKoda(codeLine), exception(poruka) {}
	void ErrorInfo() { cout << "ERROR: " << exception::what() << " - Line: " << _linijaKoda << endl; }
};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr) 
		return nullptr; 
	int vel = strlen(sadrzaj) + 1; 
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
const char* crt = "\n----------------------------------\n";
template<class T> 
class Kolekcija {
T* _elementi;
int _trenutno;
public: 
	//Kolekciju dopuniti u skladu sa potrebama zadatka 
	Kolekcija():_trenutno(0) {
		_elementi = nullptr;
	} 
	Kolekcija(const Kolekcija& k):_trenutno(k._trenutno) {
		if (_trenutno > 0) {
			_elementi = new T[_trenutno];
			for (int i = 0; i < _trenutno; i++) {
				_elementi[i] = k._elementi[i];
			}
		}
		else _elementi = nullptr;
	}
	~Kolekcija() { delete[] _elementi; _elementi = nullptr; }

	Kolekcija& operator-=(int index) {
		if (index<0 || index>_trenutno - 1)
			return *this;
		for (int i = index; i < _trenutno - 1; i++)
			_elementi[i] = _elementi[i + 1];
		return *this;
	}
	void AddElement(const T& el) {
		T* temp = new T[_trenutno + 1]; 

		for (int i = 0; i < _trenutno; i++) { 
			temp[i] = _elementi[i]; 
		}
		delete[] _elementi;
		_elementi = temp;
		_elementi[_trenutno++] = const_cast<T&>(el);
	}
	int GetTrenutno()const { return _trenutno; }

	T& operator[](int index) {
		if (index < 0 || index > _trenutno-1)
			throw NedozvoljenaOperacija("Indeks nije validan.", __LINE__);
		return _elementi[index];
	}

	friend ostream& operator<<<>(ostream&,Kolekcija&);
};

template<class T>
ostream& operator<<<>(ostream& cout,Kolekcija<T>& kol) {
	if (kol._elementi == nullptr || kol._trenutno <= 0)
		return cout;
	for (int i = 0; i < kol._trenutno; i++)
		cout << kol._elementi[i] << endl;
	return cout;
}

//Kataloška prodaja 
class Proizvod { 
	char _sifra[10]; 
	char* _naziv; 
	double _cijena;
	double _popust; //Izrazen procentualno
	int _kolicina;	//Dostupno proizvoda 

public:
	//Konstruktor i destruktor funkcije
	Proizvod():_naziv(nullptr),_cijena(0),_popust(0),_kolicina(0){}
    Proizvod(const char* sifra, const char* naziv, double cijena, int kolicina) { 
	//U slucaju nedozvoljenih vrijednosti za cijenu i kolicinu baciti izuzetak
		if (sifra == nullptr || naziv == nullptr || cijena <= 0 || kolicina <= 0)
			throw NedozvoljenaOperacija("Proslijedjeni parametri za proizvod nisu validni!", __LINE__);
	strncpy_s(_sifra, sifra, _TRUNCATE);
	_naziv = AlocirajNizKaraktera(naziv);
	_cijena = cijena;
	_kolicina = kolicina;
	_popust = 0;
}
	Proizvod(const Proizvod& p) :_kolicina(p._kolicina), _naziv(AlocirajNizKaraktera(p._naziv)), _cijena(p._cijena), _popust(p._popust) {
		strncpy_s(_sifra, p._sifra, _TRUNCATE);
	}
    virtual ~Proizvod() { delete[] _naziv; _naziv = nullptr; }
    
	Proizvod& operator=(const Proizvod& p) {
		if (this == &p)
			return *this;
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(p._naziv);
		strncpy_s(_sifra, p._sifra, _TRUNCATE);
		_cijena = p._cijena;
		_popust = p._popust;
		_kolicina = p._kolicina;
		return *this;
	}
	Proizvod& operator+=(int kol) {
		_kolicina += kol;
		return *this;
	}
	Proizvod& operator-=(int kol) { _kolicina -= kol;
	return *this;
	}

	char* GetSifru()const { return AlocirajNizKaraktera((char*)_sifra); }
	char* GetNaziv()const { return _naziv; }
	double GetCijenu()const { return _cijena - (_cijena*(_popust / 100)); } //ako je popust 0, cijena ce ostati ista
	double GetPopust()const {return _popust;}
	int GetKolicinu()const { return _kolicina; }

	virtual void setPopust(double p) { _popust = p; }
	friend bool operator==(Proizvod&, Proizvod&);
	friend ostream& operator<<(ostream&, Proizvod&);
};
ostream& operator<<(ostream& COUT, Proizvod& p) {
	if (p._naziv == nullptr)
		return COUT;
	cout <<crt<< "Naziv: " << p._naziv << endl;
	cout << "Sifra: " << p._sifra;
	cout << "\nCijena: " << p._cijena;
	cout << "KM\nPopust: " << p._popust;
	cout << "%\nKolicina: " << p._kolicina << endl;
	return COUT;
}
bool operator==(Proizvod& p1, Proizvod& p2) {
	return strcmp(p1._sifra, p2._sifra) == 0;
}

//Implementirati nasljedivanje 
class Odjeca:public Proizvod { 

Kolekcija<char*> _velicine;
Kolekcija<char*> _boje;
char _kategorija[50]; 


public: 
	Odjeca() {}
	Odjeca(const char* sifra, const char* naziv, double cijena, int kolicina, const char* kategorija):Proizvod(sifra,naziv,cijena,kolicina) {
		if (kategorija == nullptr)
			throw NedozvoljenaOperacija("Nije proslijedjena kategorija za odjecu!", __LINE__);
		strncpy_s(_kategorija, kategorija, _TRUNCATE);
	}
	Odjeca(const Odjeca& od) :Proizvod(od) {
		strncpy_s(_kategorija, od._kategorija, _TRUNCATE);
		Proizvod::operator=(od);
		for (int i = 0; i < od._velicine.GetTrenutno(); i++)
			_velicine.AddElement(AlocirajNizKaraktera(const_cast<Odjeca*>(&od)->_velicine[i]));

		for (int i = 0; i < od._boje.GetTrenutno(); i++)
			_boje.AddElement(AlocirajNizKaraktera(const_cast<Odjeca*>(&od)->_boje[i]));
	}
	~Odjeca() {
		for (int i = 0; i < _velicine.GetTrenutno(); i++) {
			delete[] _velicine[i];
			_velicine[i] = nullptr;
		}
		for (int i = 0; i < _boje.GetTrenutno(); i++) {
			delete[] _boje[i];
			_boje[i] = nullptr;
		}
	}

	Odjeca& operator=(Odjeca& od) {
		if (&od == this)
			return *this;

		Proizvod::operator=(od);
		strncpy_s(_kategorija, od._kategorija, _TRUNCATE);
		for (int i = 0; i < od._velicine.GetTrenutno(); i++)
			_velicine.AddElement(AlocirajNizKaraktera(od._velicine[i]));

		for (int i = 0; i < od._boje.GetTrenutno(); i++)
			_boje.AddElement(AlocirajNizKaraktera(od._boje[i]));
		return *this;
	}

	void DodajVelicinu(const char* vel) { _velicine.AddElement(AlocirajNizKaraktera(vel)); }
	void DodajBoju(const char* boja) { _boje.AddElement(AlocirajNizKaraktera(boja)); }

	char* GetVelicinu(int index) { return _velicine[index]; }
	char* GetBoju(int index) { return _boje[index]; }
	char* GetKategoriju()const { return AlocirajNizKaraktera((char*)_kategorija); }

	friend ostream& operator<<(ostream&T, Odjeca&);
};
ostream& operator<<(ostream& COUT, Odjeca& od) {
	cout << *dynamic_cast<Proizvod*>(&od) << endl;
	cout << "\nKategorija: " << od._kategorija << endl << endl;
	cout << "--Boje--\n";
	if (od._boje.GetTrenutno() > 0)
		cout << od._boje << endl;
	else
		cout << "--NEMA DOSTUPNIH BOJA--\n\n";

	cout << "--Velicine--\n";
	if (od._velicine.GetTrenutno() > 0)
		cout << od._velicine << endl;
	else
		cout << "--NEMA DOSTUPNIH VELICINA--\n\n";
	return COUT;
}

class Katalog {
	char* _naslov; 
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy 
    Kolekcija<Proizvod*> _proizvodi;

	bool ispravanFormat(const char* date) {
		if (date == nullptr || strlen(date) != 10)
			return false;
		for (int i = 0; i < 10; i++) {
			if ((i == 2 || i == 5) && date[i] != '.')
				return false;
			else if (date[i] != '.' && !isdigit(date[i]))
				return false;
		}
		return true;
	}

public:
	//Potrebne konstruktor i destruktor funkcije 
	Katalog():_vrijediDo(nullptr),_naslov(nullptr){}
	Katalog(const char* naslov, const char* vrijediDo) {
		//U slucaju da datum ne ispunjava zadati format baciti izuzetak
		if (!ispravanFormat(vrijediDo))
			throw NedozvoljenaOperacija("Proslijedjen neispravan format za rok vazenja kataloga!", __LINE__);
		_naslov = AlocirajNizKaraktera(naslov); 
		_vrijediDo = AlocirajNizKaraktera(vrijediDo);
	}
	Katalog(const Katalog& k) :_naslov(AlocirajNizKaraktera(k._naslov)), _vrijediDo(AlocirajNizKaraktera(k._vrijediDo)) {
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++) {
			if (dynamic_cast<Odjeca*>(const_cast<Katalog*>(&k)->_proizvodi[i]) != nullptr)
				_proizvodi.AddElement(new Odjeca(*dynamic_cast<Odjeca*>(const_cast<Katalog*>(&k)->_proizvodi[i])));
			else
				_proizvodi.AddElement(new Proizvod(*const_cast<Katalog*>(&k)->_proizvodi[i]));
		}
	}
	~Katalog() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _vrijediDo; _vrijediDo = nullptr;
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++) { 
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}

	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase. 
	//Onemoguciti dodavanje identicnih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
	//U slucaju pokušaja dodavanja istih proizvoda baciti izuzetak.
	void DodajProizvod(Proizvod& p) {
		for (int i = 0; i < _proizvodi.GetTrenutno(); i++)
			if (strcmp(_proizvodi[i]->GetSifru(),p.GetSifru())==0)
				throw NedozvoljenaOperacija("Ne mozete dodati iste proizvode u jedan katalog!", __LINE__);
				
		if (dynamic_cast<Odjeca*>(&p) != nullptr)
			_proizvodi.AddElement(new Odjeca(*dynamic_cast<Odjeca*>(&p)));
		else
			_proizvodi.AddElement(new Proizvod(p));
	}

	char* GetNaslov()const { return _naslov; }
	char* GetRok()const { return _vrijediDo; }
	Proizvod& GetProizvod(int index){ return *_proizvodi[index]; }

	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre. 
	//Ukoliko se ne pronade traženi proizvod baciti izuzetak.
	Katalog& operator-=(const char* sifra) {
		if (sifra == nullptr)
			return *this;
		bool nadjen = false;
		for(int i=0;i<_proizvodi.GetTrenutno();i++)
			if (strcmp(_proizvodi[i]->GetSifru(), sifra) == 0) {
				delete _proizvodi[i];
				_proizvodi -= i;
				nadjen = true;
			}
		if (!nadjen)
			throw NedozvoljenaOperacija("Proizvod sa datom sifrom nije pronadjen u katalogu! Brisanje nije uspjelo..", __LINE__);
		return *this;
	}

	//Preklopiti operator za ispis kataloške ponude
	friend ostream& operator<<(ostream&, Katalog&);
};
ostream& operator<<(ostream& COUT, Katalog& k) {
	if (k._naslov == nullptr || k._vrijediDo == nullptr)
		return COUT;
	cout << "Naslov: " << k._naslov;
	cout << "\nVrijedi do: " << k._vrijediDo << endl;
	cout << "\n-------PONUDA-------\n\n";
	for (int i = 0; i < k._proizvodi.GetTrenutno(); i++)
		cout <<i+1<<". "<< *k._proizvodi[i] << endl;
	return COUT;
}


class Narudzba {
	int _broj;
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy
	Kolekcija<Proizvod*> _naruceniProizvodi;
	bool otkazana;

	bool ispravanFormat(const char* date) {
		if (date == nullptr || strlen(date) != 10)
			return false;
		for (int i = 0; i < 10; i++) {
			if ((i == 2 || i == 5) && date[i] != '.')
				return false;
			else if (date[i] != '.' && !isdigit(date[i]))
				return false;
		}
		return true;
	}

public:
	//Potrebne konstruktor i destruktor funkcije
	//U slucaju da datum ne ispunjava zadati format baciti izuzetak.
	Narudzba():_broj(0),_datumIsporuke(nullptr),otkazana(false){}
	Narudzba(int broj, const char* datumIsporuke) {
		if (!ispravanFormat(datumIsporuke))
			throw NedozvoljenaOperacija("Pogresan format datuma isporuke!", __LINE__);
		_broj = broj;
		_datumIsporuke = AlocirajNizKaraktera(datumIsporuke);
		otkazana = false;
	}
	Narudzba(const Narudzba& n) :_broj(n._broj), _datumIsporuke(AlocirajNizKaraktera(n._datumIsporuke)), otkazana(n.otkazana) {
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++)	{
			if (dynamic_cast<Odjeca*>(const_cast<Narudzba*>(&n)->_naruceniProizvodi[i]) != nullptr)
				_naruceniProizvodi.AddElement(new Odjeca(*dynamic_cast<Odjeca*>(const_cast<Narudzba*>(&n)->_naruceniProizvodi[i])));
			else
				_naruceniProizvodi.AddElement(new Proizvod(*const_cast<Narudzba*>(&n)->_naruceniProizvodi[i]));
		}
	}
	~Narudzba() {
		delete[] _datumIsporuke; _datumIsporuke = nullptr;

		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			delete _naruceniProizvodi[i];
			_naruceniProizvodi[i] = nullptr;
		}
	}

	Narudzba& operator=(Narudzba& n) {
		if (this == &n)
			return *this;

		delete[] _datumIsporuke;
		_datumIsporuke = AlocirajNizKaraktera(n._datumIsporuke);
		otkazana = n.otkazana;
		_broj = n._broj;
		for (int i = 0; i < n._naruceniProizvodi.GetTrenutno(); i++) {
			if (dynamic_cast<Odjeca*>(const_cast<Narudzba*>(&n)->_naruceniProizvodi[i]) != nullptr)
				_naruceniProizvodi.AddElement(new Odjeca(*dynamic_cast<Odjeca*>(const_cast<Narudzba*>(&n)->_naruceniProizvodi[i])));
			else
				_naruceniProizvodi.AddElement(new Proizvod(*const_cast<Narudzba*>(&n)->_naruceniProizvodi[i]));
		}
		return *this;
	}

	void otkazi() { otkazana = true; }
	int GetBroj()const { return _broj; }
	char* GetDatumIsporuke()const { return _datumIsporuke; }
	bool GetOtkazana()const { return otkazana; }
	int GetBrojProizvoda()const { return _naruceniProizvodi.GetTrenutno(); }
	//Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednošcu tipa bool
	//Ukoliko proizvod više nije dostupan baciti izuzetak.
	//Dozvoliti dodavanje istog proizvoda više puta.
	bool DodajProizvod(Proizvod& p) {
		if (p.GetKolicinu() == 0)
			throw NedozvoljenaOperacija("Proizvod nema na stanju!", __LINE__);
		p -= 1;
		if (dynamic_cast<Odjeca*>(&p) != nullptr)
			_naruceniProizvodi.AddElement(new Odjeca(*dynamic_cast<Odjeca*>(&p)));
		else
			_naruceniProizvodi.AddElement(new Proizvod(p));
	}

	//Funkciju za uklanjanje odredenog proizvoda iz narudžbe. Voditi racuna da narudžba može 
	//imati više puta isti proizvod.

	Narudzba& operator-=(Proizvod& p) {
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			if (*_naruceniProizvodi[i] == p)
			{
				delete _naruceniProizvodi[i];
				_naruceniProizvodi[i] = nullptr;
				p += 1;
			}
		}
		return *this;
	}
	Proizvod& operator[](int index) {
		if (index<0 || index>_naruceniProizvodi.GetTrenutno() - 1)
			throw NedozvoljenaOperacija("Index koji ste proslijedili nije validan!", __LINE__);
		return *_naruceniProizvodi[index];
	}
	//Dodati funkciju za proracun ukupne cijene narudžbe.
	//Voditi racuna o mogucnosti dodavanja više istih proizvoda (kolicina). 
	//Uracunati popust u slucaju da je definisan.
	double ukupnaCijena() {
		double uk = 0;
		for (int i = 0; i < _naruceniProizvodi.GetTrenutno(); i++) {
			uk += _naruceniProizvodi[i]->GetCijenu();
		}
		return uk;
	}

	//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
	friend ostream& operator<<(ostream&, Narudzba&);
	friend bool operator==(Narudzba&, Narudzba&);
};
bool operator==(Narudzba& n1, Narudzba& n2) {
	return n1._broj == n2._broj;
}
ostream& operator<<(ostream& COUT, Narudzba& n) {
	cout << "\n----------INFORMACIJE O NARDUZBI------------\n";
	cout << "Broj narudzbe: " << n._broj << endl;
	if (n.otkazana)
		cout << "--NARUDZBA JE OTKAZANA!--\n\n";
	else {
		cout << "Datum isporuke: " << n._datumIsporuke;
		for (int i = 0; i < n._naruceniProizvodi.GetTrenutno(); i++) {
			Proizvod* temp = n._naruceniProizvodi[i];
			cout << crt << i + 1 << ". " << temp->GetNaziv();
			if (dynamic_cast<Odjeca*>(n._naruceniProizvodi[i]) != nullptr)
				cout << "\nKategorija: " << dynamic_cast<Odjeca*>(n._naruceniProizvodi[i])->GetKategoriju();
			cout<< "\nSifra: " << temp->GetSifru();
			cout << "\nCijena: " << temp->GetCijenu() << endl;
			if (temp->GetPopust() > 0)
				cout << "Popust: " << temp->GetPopust() << endl;
		}
	}
	return COUT;
	
}

//U posljednjoj klasi samostalno prepoznati izuzetne situacije 
class Prodavac {
char* _imePrezime;
Kolekcija<Narudzba>* _narudzbe; 

public: 
	//Potrebne konstruktor i destruktor funkcije
	Prodavac() :_imePrezime(nullptr) { _narudzbe = new Kolekcija<Narudzba>; }
	Prodavac(const char* imep):_imePrezime(AlocirajNizKaraktera(imep)){ _narudzbe = new Kolekcija<Narudzba>; }
	Prodavac(const Prodavac& p):_imePrezime(AlocirajNizKaraktera(p._imePrezime)) {
		_narudzbe = new Kolekcija<Narudzba>;
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++)
			_narudzbe->AddElement((*p._narudzbe)[i]);
	}
	~Prodavac(){ delete[] _imePrezime; _imePrezime = nullptr;
	delete _narudzbe;
	_narudzbe = nullptr;
	}

//Omoguciti dodavanje narudžbi za prodavaca
	void DodajNarudzbu(Narudzba& n) {
		_narudzbe->AddElement(n);
	}

//Omoguciti otkazivanje narudžbe na osnovu njenog broja.
	void otkaziNarudzbu(int broj) {
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++)
			if ((*_narudzbe)[i].GetBroj() == broj)
				(*_narudzbe)[i].otkazi();
	}

//Ispis svih podataka o prodavacu i kratke informacije o
//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.
	void Info() {
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << "-----NARUDZBE----->\n\n";
		for (int i = 0; i < _narudzbe->GetTrenutno(); i++) {
			cout << i + 1 << ". narudzba--\nBroj proizvoda: " << (*_narudzbe)[i].GetBrojProizvoda() << "\nDatum: " << (*_narudzbe)[i].GetDatumIsporuke();
			cout << "\nCijena: " << (*_narudzbe)[i].ukupnaCijena() << crt;
		}
	}

//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
//sa informacijom o tome da li se radi o odjeci ili osnovnom proizvodu.
	Proizvod& GetNajprodavaniji() {
		int max = (*_narudzbe)[0].GetBrojProizvoda();
		Proizvod** proizvodi = new Proizvod*[max];

		for (int i = 0; i < max; i++)
			proizvodi[i] = &(*_narudzbe)[0][i];
		bool flag = false;

		//-----> Pravljenje niza koji sadrzi sve ponudjene proizvode <------
		for (int i = 1; i < _narudzbe->GetTrenutno(); i++) {
			for (int j = 0; j < (*_narudzbe)[i].GetBrojProizvoda(); j++) {
				for (int z = 0; z < max; z++)
					if (*proizvodi[z] == (*_narudzbe)[i][j])
						flag = true;
				if (!flag) {
					Proizvod** temp = new Proizvod*[max + 1];
					for (int i = 0; i < max; i++) {
						temp[i] = proizvodi[i];
						proizvodi[i] = nullptr;
					}
					delete[] proizvodi;
					proizvodi = temp;
					temp = nullptr;
					proizvodi[max++] = &(*_narudzbe)[i][j];
				}
			}
		}
		//-----------------------------------------------------------------------

		int* brojac = new int[max] {0};

		for (int i = 1; i < _narudzbe->GetTrenutno(); i++)
			for (int j = 0; j < (*_narudzbe)[i].GetBrojProizvoda(); j++)
				for (int z = 0; z < max; z++)
					if (*proizvodi[z] == (*_narudzbe)[i][j])
						brojac[z]++;

		int naj = 0;
		for (int i = 1; i < max; i++)
			if (brojac[naj] < brojac[i])
				naj = i;
		Proizvod *p;
		if (dynamic_cast<Odjeca*>(proizvodi[naj]) != nullptr)
			p = new Odjeca(*dynamic_cast<Odjeca*>(proizvodi[naj]));
		else
			p = new Proizvod(*proizvodi[naj]);

		for (int i = 0; i < max; i++)
			proizvodi[i] = nullptr;
		delete[] proizvodi;
		delete[] brojac;
		proizvodi = nullptr;
		brojac = nullptr;
		return *p;
	}
};

int main() { 
	
	//Dodati minimalno 5 proizvoda u katalog.
	//Ispisati ponudu kupcu 
	//Omoguciti kreiranje narudžbe za jednog prodavaca na osnovu izbora kupca.
	try {
		Proizvod* proizvodi[5]{ nullptr };
		proizvodi[0] = new Proizvod("01223", "Bicikl", 65.5, 7);
		proizvodi[1] = new Odjeca("01224", "Jakna", 79.5, 9, "Zimska odjeca");
		proizvodi[2] = new Odjeca("01225", "Patike", 79.5, 9, "Obuca");
		proizvodi[3] = new Odjeca("01226", "Majica", 79.5, 9, "Letnja odjeca");
		proizvodi[4] = new Odjeca("01227", "Farmerke", 79.5, 9, "Donji dio");

		proizvodi[0]->setPopust(10);
		proizvodi[2]->setPopust(5);
		proizvodi[4]->setPopust(50);

		dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Bijela");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Crna");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Zuta");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Plava");

		dynamic_cast<Odjeca*>(proizvodi[1])->DodajVelicinu("M");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajVelicinu("L");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajVelicinu("XXL");
		dynamic_cast<Odjeca*>(proizvodi[1])->DodajVelicinu("XS");

		for (int i = 1; i < 5; i++) {
			dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Zuta");
			dynamic_cast<Odjeca*>(proizvodi[1])->DodajBoju("Plava");

			dynamic_cast<Odjeca*>(proizvodi[i])->DodajVelicinu("M");
			dynamic_cast<Odjeca*>(proizvodi[i])->DodajVelicinu("L");
		}
		Katalog cat("Katalog 2018/19", "01.07.2019");

		for (int i = 0; i < 5; i++)
			if (dynamic_cast<Odjeca*>(proizvodi[i]) != nullptr) {
				cat.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[i]));
				cout << *dynamic_cast<Odjeca*>(proizvodi[i]) << endl;
			}
			else {
				cat.DodajProizvod(*proizvodi[i]);
				cout << *proizvodi[i] << endl;
			}

		cout << cat << endl;
	
		Prodavac prodavac("Smith Vetson");
		
		Narudzba n1(101, "05.08.2019");
		Narudzba n2(103, "02.08.2019");
		Narudzba n3(104, "05.04.2019");
		Narudzba n4(105, "20.03.2019");

		n1.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[1]));
		n1.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[2]));
		n1.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[3]));
		n1.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[4]));

		n2.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[1]));
		n2.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[4]));
		n2.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[2]));

		n3.DodajProizvod(*dynamic_cast<Odjeca*>(proizvodi[2]));
		n3.DodajProizvod(*proizvodi[0]);

		cout << n1 << endl;

		prodavac.DodajNarudzbu(n1);
		prodavac.DodajNarudzbu(n2);
		prodavac.DodajNarudzbu(n3);
		prodavac.DodajNarudzbu(n4);

		prodavac.Info();

		cout <<crt<< "---Najprodavaniji proizvod---" << crt;
		Proizvod* najpr = &prodavac.GetNajprodavaniji();
		if (dynamic_cast<Odjeca*>(najpr) != nullptr)
			cout << *dynamic_cast<Odjeca*>(najpr) << endl;
		else
			cout << *najpr << endl;

		for (int i = 0; i < 5; i++) {
			delete proizvodi[i];
			proizvodi[i] = nullptr;
		}
	}
	catch (NedozvoljenaOperacija& obj) {
		obj.ErrorInfo();
	}
	system("pause");
	return 0;
}