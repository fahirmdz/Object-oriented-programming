//fahirmdz GitHub

#include "pch.h"
#include <iostream>
#include<cstring>
#include<cstdlib>
using namespace std;

/*    
Vježbe 1 - Principi OOP. klase, objekti. 
Zadatak: Na osnovu opisa sistema prepoznati osnovne klase i atribute, kao i njihove relacije,
te implementirati osnovne metode koje omogucavaju kreiranje i  ispis narudzbi klijentu. 
Obavezno implementirati Get i Set metode.

 Tema: e-Restoran 
 
 Namjena aplikacije e-Restoran jeste da omoguci kreiranje narudzbi klijentima restorana 
 putem standardnih PC i mobilnih uredjaja.   U procesu kreiranja narudzbe neophodno je 
 da klijent obavi registraciju gdje upisuje svoje osnovne podatke: ime, prezime, kontakt
 broj i adresu.  Nakon uspjesne registracije klijenti mogu da kreiraju narudzbu na osnovu
 online ponude odabranog restorana. Ponuda restorana se sastoji od jela i pica. 
 Uz svako jelo i pice se prikazuje kratak opis i cijena. Narudzba koju klijent kreira sadrzi
 najmanje jedno jelo ili pice, ukupnu cijenu narudzbe i status isporuke.

*/
const char *crt = "\n-------------------------------------\n";
int id = 0;
char *AlocirajNizKaraktera(const char *a) {
	if (a == nullptr)
		return nullptr;
	char *temp = new char[strlen(a) + 1];
	strcpy_s(temp, strlen(a) + 1, a);
	return temp;
}
template<class T>
T *expandArray(T *niz, int br) {
	T* temp = new T[br + 1];
	for (int i = 0; i < br; i++)
		temp[i] = niz[i];
	niz = nullptr;
	return temp;
}

class Klijent {
private:
	char *_ime, *_prezime, *_adresa, *_telefon;
	
public:
	Klijent() :_ime(nullptr), _prezime(nullptr), _adresa(nullptr), _telefon(nullptr) {}
	Klijent(const char* ime,const char* prezime,const char* adresa,const char* tel) {
		_ime = AlocirajNizKaraktera(ime);
		_prezime = AlocirajNizKaraktera(prezime);
		_adresa = AlocirajNizKaraktera(adresa);
		_telefon = AlocirajNizKaraktera(tel);
	}
	Klijent(const Klijent& k) {
		_ime = AlocirajNizKaraktera(k.getIme());
		_prezime = AlocirajNizKaraktera(k.getPrezime());
		_adresa = AlocirajNizKaraktera(k.getAdresa());
		_telefon = AlocirajNizKaraktera(k.getTelefon());
	}
	~Klijent() {
		delete[] _ime;
		delete[] _prezime;
		delete[] _adresa;
		delete[] _telefon;
		_ime = _prezime = _adresa = _telefon = nullptr;
	}
	Klijent& operator=(const Klijent& k) {
		delete[] _ime;
		delete[] _prezime;
		delete[] _adresa;
		delete[] _telefon;
		_ime = AlocirajNizKaraktera(k._ime);
		_prezime = AlocirajNizKaraktera(k._prezime);
		_adresa = AlocirajNizKaraktera(k._adresa);
		_telefon = AlocirajNizKaraktera(k._telefon);
		return *this;
	}
	char *getIme()const {
		char *temp = AlocirajNizKaraktera(_ime);
		return temp;
	}
	char *getPrezime()const {
		char *temp = AlocirajNizKaraktera(_prezime);
		return temp;
	}
	char *getAdresa()const {
		char *temp = AlocirajNizKaraktera(_adresa);
		return temp;
	}
	char *getTelefon()const {
		char *temp = AlocirajNizKaraktera(_telefon);
		return temp;
	}
	void setIme(const char* ime) {
			delete[] _ime;
		_ime = AlocirajNizKaraktera(ime);
	}
	void setPrezime(const char* prezime) {
			delete[] _prezime;
		_prezime = AlocirajNizKaraktera(prezime);
	}
	void setAdresa(const char* adresa) {
			delete[] _adresa;
		_adresa = AlocirajNizKaraktera(adresa);
	}
	void setTelefon(const char* tel) {
			delete[] _telefon;
		_telefon = AlocirajNizKaraktera(tel);
	}
	friend bool operator==(const Klijent&, const Klijent&);
	friend ostream& operator<<(ostream&, const Klijent&);
};
ostream& operator<<(ostream& COUT, const Klijent& k) {
	if (k._ime == nullptr || k._prezime == nullptr)
		return COUT;
	cout << "\nIme: " << k._ime << "\nPrezime: " << k._prezime;
	cout << "\nAdresa: ";
	if (k._adresa != nullptr)
		cout << k._adresa;
	else
		cout << " NEMA ";
	cout << "\nKontakt tel. : ";
	if (k._telefon != nullptr)
		cout << k._telefon << endl;
	else
		cout << " NEMA \n";
	return COUT;
}
bool operator==(const Klijent& k1, const Klijent& k2) {
	return (strcmp(k1._ime, k2._ime) == 0) && (strcmp(k1._prezime, k2._prezime) == 0) && (strcmp(k1._adresa, k2._adresa) == 0);
}

class Jelo {
private:
	char *_naziv;
	double _cijena;
	int _kolicina;
public:
	Jelo():_naziv(nullptr),_kolicina(1) {}
	Jelo(const char* naziv, double cijena):_cijena(cijena),_kolicina(1) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Jelo(const Jelo& j) :_cijena(j.getCijena()),_kolicina(j.getKolicina()) {
		_naziv = AlocirajNizKaraktera(j.getNaziv());
	}

	~Jelo() {
		delete[] _naziv;
		_naziv = nullptr;
	}
	Jelo& operator=(const Jelo& j) {
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(j._naziv);
		_cijena = j._cijena;
		_kolicina = j._kolicina;
		return *this;
	}
	Jelo& operator++() {
		++_kolicina;
		return *this;
	}
	char *getNaziv()const {
		if (_naziv == nullptr)
			return nullptr;
		char *temp = AlocirajNizKaraktera(_naziv);
		return temp;
	}
	int getKolicina()const { return _kolicina; }
	double getCijena()const { return _cijena; }
	void setNaziv(const char* naziv) {
			delete[] _naziv;
		_naziv = AlocirajNizKaraktera(naziv);
	}
	void setCijena(double c) { _cijena = c; }

	friend bool operator==(const Jelo&, const Jelo&);
	friend ostream& operator<<(ostream&, const Jelo&);
};
ostream& operator<<(ostream &COUT, const Jelo& j) {
	cout << "\nNaziv: " << j._naziv << "\nCijena: " << j._cijena << " KM\n";
	return COUT;
}
bool operator==(const Jelo& j1, const Jelo& j2) {
	return strcmp(j1._naziv, j2._naziv) == 0;
}

class Pice {
private:
	char *_naziv;
	double _cijena;
	int _kolicina;
public:
	Pice():_naziv(nullptr),_kolicina(1),_cijena(0) {}
	Pice(const char* naziv, double cijena):_cijena(cijena),_kolicina(1) {
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Pice(const Pice& j):_cijena(j.getCijena()),_kolicina(j.getKolicina()) {
		_naziv = AlocirajNizKaraktera(j.getNaziv());
	}
	~Pice() {
		delete[] _naziv;
		_naziv = nullptr;
	}
	Pice& operator=(const Pice& p){
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(p._naziv);
		_cijena=p._cijena;
		_kolicina = p._kolicina;
		return *this;
	}
	Pice& operator++() {
		++_kolicina;
		return *this;
	}
	char *getNaziv()const {
		if (_naziv == nullptr)
			return nullptr;
		char *temp = AlocirajNizKaraktera(_naziv);
		return temp;
	}
	int getKolicina()const { return _kolicina; }
	double getCijena()const { return _cijena; }
	void setNaziv(const char* naziv) {
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(naziv);
	}
	void setCijena(double c) { _cijena = c; }

	friend bool operator==(const Pice&, const Pice&);
	friend ostream& operator<<(ostream&, const Pice&);

};
ostream& operator<<(ostream& COUT, const Pice& p) {
	cout << "\nNaziv: " << p._naziv << "\nCijena: " << p._cijena << " KM\n";
	return COUT;
}
bool operator==(const Pice& p1, const Pice& p2) {
	return strcmp(p1._naziv, p2._naziv) == 0;
}

class Narudzba {
private:
	Klijent k;
	static int _brojNarudzbi;
	const int _idNarudzbe;
	Pice *pica;
	Jelo *jela;
	int brPica, brJela;
	double ukupno;
	char *statusIsporuke;
public:
	Narudzba() :_idNarudzbe(++_brojNarudzbi),pica(nullptr),jela(nullptr),ukupno(0),brPica(0),brJela(0),statusIsporuke(nullptr) {}
	Narudzba(Klijent k) :_idNarudzbe(++_brojNarudzbi), pica(nullptr), jela(nullptr), ukupno(0), brPica(0), brJela(0) {
		this->k = k;
		statusIsporuke = AlocirajNizKaraktera("U IZRADI");
	}
	~Narudzba() {
		delete[] pica;
		delete[] jela;
		pica = nullptr;
		jela = nullptr;
	}
	Narudzba(const Narudzba& n):_idNarudzbe(n.getID()),brPica(n.getBrojPica()),brJela(n.getBrojJela()),ukupno(n.getUkupno()){
		k = n.getKlijent();
		if (n.getBrojJela() > 0) {
			jela = n.getJela();
			brJela = n.getBrojJela();
		}
		if (n.getBrojPica() > 0) {
			pica = n.getPica();
			brPica = n.getBrojPica();
		}
		delete[] statusIsporuke;
		statusIsporuke = AlocirajNizKaraktera(n.getStatusIsporuke());
	}
	Klijent getKlijent()const { return k; }
	int getID()const { return _idNarudzbe; }
	Jelo *getJela()const {
		if (brJela == 0 || jela == nullptr)
			return nullptr;
		Jelo *temp = new Jelo[brJela];
		for (int i = 0; i < brJela; i++)
			temp[i] = jela[i];
		return temp;
	}
	Pice *getPica()const {
		if (brPica == 0 || pica == nullptr)
			return nullptr;
		Pice *temp = new Pice[brPica];
		for (int i = 0; i < brPica; i++)
			temp[i] = pica[i];
		return temp;
	}
	int getUkupno()const { return ukupno; }
	int getBrojPica()const { return brPica; }
	int getBrojJela()const { return brJela; }
	void setStatus(const char *status) {
		delete[] statusIsporuke;
		statusIsporuke = AlocirajNizKaraktera(status);
	}
	char *getStatusIsporuke()const {
		char *temp = AlocirajNizKaraktera(statusIsporuke);
		return temp;
	}
	Narudzba& operator+=(const Jelo& j) {
		if (brJela > 0) {
			for (int i = 0; i < brJela; i++)
				if (j == jela[i]) {
					ukupno += j.getCijena();

					++jela[i];
					return *this;
				}
		}
		jela = expandArray(jela, brJela);
		jela[brJela++] = j;
		ukupno += j.getCijena();
		return *this;
	}
	Narudzba& operator+=(const Pice& p) {
		if (brPica > 0)
		{
			for (int i = 0; i < brPica; i++)
				if (pica[i] == p) {

					++pica[i];
					ukupno += p.getCijena();
					return *this;
				}
		}
		pica = expandArray(pica, brPica);
		pica[brPica++] = p;
		ukupno += p.getCijena();
		return *this;
	}

	friend ostream& operator<<(ostream&, const Narudzba&);
};
ostream& operator<<(ostream& COUT, const Narudzba& n) {
	cout << "\n---Informacije o narudzbi---\n";
	cout << "ID narudzbe: " << n._idNarudzbe << endl;
	cout << "Status isporuke: " << n.statusIsporuke << endl;
	cout << "Klijent koji je izvrsio narudzbu ---> \n" << n.k;
	cout << "Broj narucenih pica: " << n.brPica << endl;
	cout << "Broj narucenih jela: " << n.brJela << endl;
	cout << "----Pica ---> \n";
	for (int i = 0; i < n.brPica; i++) {
		cout << "- " << n.pica[i];
		cout << "\tX " << n.pica[i].getKolicina() << " --> " << n.pica[i].getKolicina()*n.pica[i].getCijena() << " KM\n";
	}
	cout << endl;
	cout << "----Jela ---> \n";
	for (int i = 0; i < n.brJela; i++) {
		cout << "- "<<n.jela[i];
		cout << "\tX " << n.jela[i].getKolicina() << " --> " << n.jela[i].getCijena()*n.jela[i].getKolicina() << " KM\n";
	}
	cout << endl;
	cout << crt << "Ukupno za platiti: " << n.ukupno << " KM" << crt;
	return COUT;
}
int Narudzba::_brojNarudzbi = 0;
class Restoran {
private:
	char *ime;
	Klijent *klijenti;
	Jelo* jela;
	Pice *pica;
	Narudzba **narudzbe;
	int brKl, brJela, brPica, brN;
public:
	Restoran() :ime(nullptr), klijenti(nullptr), jela(nullptr), pica(nullptr), narudzbe(nullptr), brKl(0), brJela(0), brPica(0), brN(0) {}
	Restoran(const char* ime) :klijenti(nullptr), jela(nullptr), pica(nullptr), narudzbe(nullptr), brKl(0), brJela(0), brPica(0), brN(0) {
		this->ime = AlocirajNizKaraktera(ime);
	}
	~Restoran() {
		delete[] ime;
		ime = nullptr;
		delete[] klijenti;
		klijenti = nullptr;
		delete[] jela;
		jela = nullptr;
		delete[] pica;
		pica = nullptr;
		delete[] narudzbe;
		narudzbe = nullptr;
	}
	char *getIme()const {
		char *temp = AlocirajNizKaraktera(ime);
		return temp;
	}
	Klijent getZadnjegKlijenta()const {
		Klijent k(klijenti[brKl - 1]);
		return k;
	}
	Klijent *getKlijenti()const {
		if (brKl == 0 || klijenti == nullptr)
			return nullptr;
		Klijent *temp = new Klijent[brKl];
		for (int i = 0; i < brKl; i++)
			temp[i] = klijenti[i];
		return temp;
	}
	Jelo *getJela()const {
		if (brJela == 0 || jela == nullptr)
			return nullptr;
		Jelo *temp = new Jelo[brJela];
		for (int i = 0; i < brJela; i++)
			temp[i] = jela[i];
		return temp;
	}
	Pice *getPica()const {
		if (brPica == 0 || pica == nullptr)
			return nullptr;
		Pice *temp = new Pice[brPica];
		for (int i = 0; i < brPica; i++)
			temp[i] = pica[i];
		return temp;
	}
	Narudzba **getNarudzbe()const {
		if (brN == 0 || narudzbe == nullptr)
			return nullptr;
		Narudzba **temp = new Narudzba*[brN];
		for (int i = 0; i < brN; i++)
			temp[i] = narudzbe[i];
		return temp;
	}
	int getBrojKl()const { return brKl; }
	int getBrojJela()const { return brJela; }
	int getBrojPica()const { return brPica; }
	int getBrojNaurdzbi()const { return brN; }
	void setIme(const char* ime) {
		delete[] this->ime;
		this->ime = AlocirajNizKaraktera(ime);
	}
	Restoran& operator+=(const Klijent& k) {
		if (brKl > 0)
			for (int i = 0; i < brKl; i++)
				if (k == klijenti[i])
					return *this;
		klijenti = expandArray(klijenti, brKl);
		klijenti[brKl++] = k;
		return *this;
	}
	Restoran& operator+=(const Jelo& j) {
		if (brJela > 0)
			for (int i = 0; i < brJela; i++)
				if (j == jela[i])
					return *this;
		jela = expandArray(jela, brJela);
		jela[brJela++] = j;
		return *this;
	}
	Restoran& operator+=(const Pice& p) {
		if (brPica > 0)
			for (int i = 0; i < brPica; i++)
				if (pica[i] == p)
					return *this;
		pica = expandArray(pica, brPica);
		pica[brPica++] = p;
		return *this;
	}
	Restoran& operator+=(const Narudzba& n) {
		Narudzba **temp = new Narudzba*[brN + 1];
		if (brN > 0)
			for (int i = 0; i < brN; i++) {
				temp[i] = new Narudzba(*narudzbe[i]);
				delete narudzbe[i];
			}
		delete[] narudzbe;
		temp[brN++] = new Narudzba(n);
		narudzbe = temp;
		return *this;
	}
	void printJelovnik()const {
		if (brJela <= 0) {
			cout << "---Nema jela---\n";
			return;
		}
		cout <<crt<< "--Jelovnik--" << crt;
		for (int i = 0; i < brJela; i++) {
			cout << i + 1 << ". " << jela[i];
		}
	}
	void printKartaPica()const {
		if (brPica <= 0) {
			cout << "---Nema pica---\n";
			return;
		}
		cout << crt << "--Karta pica--" << crt;
		for (int i = 0; i < brPica; i++) {
			cout << i + 1 << ". " << pica[i];
		}
	}
	void printNarudzbe(const Klijent& k)const {
		if (brN <= 0)
			return;
		for (int i = 0; i < brN; i++)
			if (narudzbe[i]->getKlijent() == k) {
				system("cls");
				cout << *narudzbe[i] << endl << endl;
			}
	}
	bool napraviNarudzbu(const Klijent& k) {
		Narudzba n(k);
		cout << crt << "--> Kreiranje narudzbe <--" << crt;
		printKartaPica();
		int x;
		do {
			cout << "Unesite redni broj pica koje zelite naruciti (0 - EXIT): ";
			cin >> x;
			if (x == 0)
				break;
			else if (x<1 || x>brPica)
				cout << "Pogresan redni broj..\n";
			else {
				n += pica[x - 1];
			}
		} while ((x<1 || x>brPica) || x != 0);
		system("cls");
		printJelovnik();
		do {
			cout << "Unesite redni broj jela koje zelite naruciti (0 - EXIT): ";
			cin >> x;
			if (x == 0)
				break;
			else if (x<1 || x>brJela)
				cout << "Pogresan redni broj..\n";
			else {
				n += jela[x - 1];
			}
		} while ((x<1 || x>brJela) || x != 0);
		system("cls");
		if (n.getBrojJela() < 1 && n.getBrojPica() < 1)
			return false;
		*this += n;
	}
	void trazeniStatus(const Klijent& k) {
		if (brN <= 0) {
			cout << "Trenutno nema ni jedne narudzbe na listi..\n";
			return;
		}
		for (int i = 0; i < brN; i++)
			if (narudzbe[i]->getKlijent()==k)
				cout << narudzbe[i]->getID() << " - Status: " << narudzbe[i]->getStatusIsporuke() << endl;
	}
	friend bool loginRestoran(Restoran&);
	friend ostream& operator<<(ostream&, const Restoran&);
};
ostream& operator<<(ostream& COUT, const Restoran& r) {
	cout << crt << "=========== Restoran '" << r.getIme() << "' ==============" << crt;
	r.printKartaPica();
	r.printJelovnik();
	cout <<crt<<endl<< "DA BISTE NAPRAVILI NARUDZBU, PRVO SE MORATE REGISTRIRATI!" << crt;
	return COUT;
}
bool loginRestoran(Restoran &r) {
	cout << "---Restoran '" << r.getIme() << "'---" << crt;
	char* ime, *prezime, *adresa, *tel;
	ime = new char[15];
	prezime = new char[15];
	adresa = new char[50];
	tel = new char[10];
	int brojacLogina = 0;
	do {
		cout << "-----LOG IN-----\n";
		cout << "Ime: ";
		cin.getline(ime, 15);
		cout << "Prezime: ";
		cin.getline(prezime, 15);
		cout << "Adresa: ";
		cin.getline(adresa, 50);
		cout << "Kontakt telefon: ";
		cin.getline(tel, 10);
		if (brojacLogina < 3 && !(strlen(ime) > 1 && strlen(prezime) > 1 && strlen(adresa) > 2 && strlen(tel) > 7) && brojacLogina<3) {
			cout << "Registracija nije uspjela! Niste unijeli ispravne podatke..\nOstala su vam " << 3 - brojacLogina << "logina..\n Ponovite Log In!\n";
			brojacLogina++;
		}
		else if(brojacLogina>=3)
			cout << "Prekoracili ste broj pokusaja logiranja! Zao nam je..\n";
	} while (!(strlen(ime) > 1 && strlen(prezime) > 1 && strlen(adresa) > 2 && strlen(tel) > 7) && brojacLogina<3);
	r+=Klijent(ime, prezime, adresa, tel);
	cout <<crt<< "Registracija uspjesna!" << crt;
	return true;
}

int main() {
	Restoran r("SAZ");
	Jelo j1("Pasta Bologneze", 15.00), j2("Pizza Margherita", 9.00), j3("Biftek", 10.50), j4("Pileca salata", 12.50);
	Pice p1("Juice Frutti", 3.5), p2("San Pellegrino", 5.50), p3("Vino Vranac", 5.5), p4("Gusti sok", 4.5);;

	r += j1;
	r += j2;
	r += j3;
	r += j4;
	r += p1;
	r += p2;
	r += p3;
	r += p4;

	system("cls");
	cout << r << endl;


	if (loginRestoran(r)) {
		int x;
		do {
			do {
				cout << "--- Odaberite neku od usluga --> " << endl << endl;
				cout << "1 za prikaz svih vasih narudzbi";
				cout << "\n2 - za prikaz statusa odabrane narudzbe";
				cout << "\n3 - kreiraj novu narudzbu";
				cout << "\n0 - EXIT" << crt;
				cout << "Izbor: ";
				cin >> x;
				if (x < 0 || x>3)
					cout << "Trazena opcija ne postoji..\n";
				if (x == 0)
					cout << crt << "Hvala na posjeti! Ugodan dan zelimo.." << crt;
			} while ((x < 0 || x>3) && x != 0);
			if (x == 3) {
				if (r.napraviNarudzbu(r.getZadnjegKlijenta()))
					cout << "Uspjesno napravljena narudzba! Hvala na povjerenju.\n";
				else
					cout << "Narucivanje neuspjesno! Narudzba mora sadrzati minimum 1 pice ili 1 jelo..\n";
			}
			else if (x == 2) {
				r.trazeniStatus(r.getZadnjegKlijenta());
				system("PAUSE");
			}
			else if (x == 1)
				r.printNarudzbe(r.getZadnjegKlijenta());
		} while (x != 0);
	}
	system("PAUSE");

	return 0;
}