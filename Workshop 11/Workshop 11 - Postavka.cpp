#include<iostream> 
using namespace std;


class NedozvoljenaOperacija : public exception { 
	int _linijaKoda;

public:
};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr) 
		return nullptr; 
	int vel = strlen(sadrzaj) + 1; 
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T> 
class Kolekcija {
T* _elementi;
int _trenutno;
public: 
	//Kolekciju dopuniti u skladu sa potrebama zadatka 
	Kolekcija() {
		_elementi = nullptr;
		_trenutno = 0; 
	} 
	
	~Kolekcija() { delete[] _elementi; _elementi = nullptr; }

	void AddElement(const T& el) {
		T* temp = new T[_trenutno + 1]; 

		for (int i = 0; i < _trenutno; i++) { 
			temp[i] = _elementi[i]; 
		}
		delete[] _elementi;
		_elementi = temp;
		_elementi[_trenutno++] = el;
	}
	int GetTrenutno()const { return _trenutno; }

	T& operator[](int index) {
		if (index < 0 || index >= _trenutno)
			throw NedozvoljenaOperacija("Indeks nije validan.", __LINE__);
		return _elementi[index];
	}

	friend ostream& operator<<(ostream&, const Kolekcija&);
};

template<class T>
ostream& operator<<(ostream& cout, const Kolekcija<T>& kol) {
	for (int i = 0; i < kol._trenutno; i++) {
		cout << kol._elementi[i] << endl;
	}
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
Proizvod(const char* sifra, const char* naziv, double cijena, int kolicina) { 
	//U slucaju nedozvoljenih vrijednosti za cijenu i kolicinu baciti izuzetak
	strncpy_s(_sifra, sifra, _TRUNCATE);
	_naziv = AlocirajNizKaraktera(naziv);
	_cijena = cijena;
	_kolicina = kolicina;
	_popust = 0;
}
~Proizvod() { delete[] _naziv; _naziv = nullptr; }
//Implementirati potrebne funkcije za rad sa klasom
};

//Implementirati nasljedivanje 
class Odjeca { 

Kolekcija<char*> _velicine;
Kolekcija<char*> _boje;
char _kategorija[50]; 

public: 
//Potrebne konstruktor i destruktor funkcije
//Implementirati potrebne funkcije za rad sa klasom
};

class Katalog {
	char* _naslov; 
	char* _vrijediDo; //Datum u formatu dd.MM.yyyy 
Kolekcija<Proizvod*> _proizvodi;

public:
	//Potrebne konstruktor i destruktor funkcije 
	Katalog(const char* naslov, char* vrijediDo) {
		//U slucaju da datum ne ispunjava zadati format baciti izuzetak
		_naslov = AlocirajNizKaraktera(naslov); 
		_vrijediDo = AlocirajNizKaraktera(vrijediDo);
	}
	~Katalog() {
		delete[] _naslov; _naslov = nullptr;
		delete[] _vrijediDo; _vrijediDo = nullptr;
		for (size_t i = 0; i < _proizvodi.GetTrenutno(); i++) { 
			delete _proizvodi[i];
			_proizvodi[i] = nullptr;
		}
	}
	//Kreirati funkciju za dodavanje proizvoda u katalog i pri tome provjeriti da li se radi
	//o objektu izvedene ili osnovne klase. 
	//Onemoguciti dodavanje identicnih proizvoda u jedan katalog (proizvode razlikovati samo na osnovu šifre).
	//U slucaju pokušaja dodavanja istih proizvoda baciti izuzetak.
	//Funkciju za uklanjanje proizvoda iz kataloga na osnovu šifre. 
	//Ukoliko se ne pronade traženi proizvod baciti izuzetak.

	//Preklopiti operator za ispis kataloške ponude
};
class Narudzba {
	int _broj; 
	char* _datumIsporuke; //Datum u formatu dd.MM.yyyy 
Kolekcija<Proizvod*> _naruceniProizvodi; 
bool otkazana;

public:
	    //Potrebne konstruktor i destruktor funkcije
	    //U slucaju da datum ne ispunjava zadati format baciti izuzetak.
        //Kreirati funkciju za dodavanje proizvoda narudžbe sa povratnom vrijednošcu tipa bool
		//Ukoliko proizvod više nije dostupan baciti izuzetak.
		//Dozvoliti dodavanje istog proizvoda više puta.
		//Funkciju za uklanjanje odredenog proizvoda iz narudžbe. Voditi racuna da narudžba može 
		//imati više puta isti proizvod.
		//Dodati funkciju za proracun ukupne cijene narudžbe.
		//Voditi racuna o mogucnosti dodavanja više istih proizvoda (kolicina). 
		//Uracunati popust u slucaju da je definisan.
		//Preklopiti operator za ispis podataka o narudžbi zajedno sa cijenom.
};
//U posljednjoj klasi samostalno prepoznati izuzetne situacije 
class Prodavac {
char* _imePrezime;
Kolekcija<Narudzba>* _narudzbe; 

public: 
	//Potrebne konstruktor i destruktor funkcije
//Omoguciti dodavanje narudžbi za prodavaca
//Omoguciti otkazivanje narudžbe na osnovu njenog broja.
//Ispis svih podataka o prodavacu i kratke informacije o
//svim narudžbama (datum, broj proizvoda i cijena). Posebno izdvojiti otkazane narudžbe.
//Implementirati metodu koja u svim aktivnim narudžbama pronalazi proizvod koji se najviše prodavao
//sa informacijom o tome da li se radi o odjeci ili osnovnom proizvodu.

};

int main() { 
	
	//Dodati minimalno 5 proizvoda u katalog.
	//Ispisati ponudu kupcu 
	//Omoguciti kreiranje narudžbe za jednog prodavaca na osnovu izbora kupca.

	system("pause");
	return 0;
}