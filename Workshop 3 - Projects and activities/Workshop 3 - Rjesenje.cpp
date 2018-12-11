//fahirmdz GitHub

#include"pch.h"
#include<iostream> 
using namespace std; 

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr) 
		return nullptr;
	int vel = strlen(sadrzaj) + 1; 
	char * temp = new char[vel]; 
	strcpy_s(temp, vel, sadrzaj);
	return temp; 
}

class Uposlenik {
	static int generatorSifre;
	const int _sifra; //Automatski uvecavati za 1 za svakog novog uposlenika. 
	char _ime[30];
	char _prezime[30];
	char* _radnoMjesto;

public:

	//Constructors and destructor 
	Uposlenik() :_radnoMjesto(nullptr), _sifra(generatorSifre++){}
	Uposlenik(const char* ime, const char* prezime, const char* radnoMjesto):_sifra(generatorSifre++),
		_radnoMjesto(AlocirajNizKaraktera(radnoMjesto)) {
		strncpy_s(_ime, ime, _TRUNCATE);
		strncpy_s(_prezime, prezime, _TRUNCATE);
	}
	Uposlenik(const Uposlenik& u) :_sifra(u._sifra), _radnoMjesto(AlocirajNizKaraktera(u._radnoMjesto)) {
		strncpy_s(_ime, u._ime, _TRUNCATE);
		strncpy_s(_prezime, u._prezime, _TRUNCATE);
	}
	Uposlenik(Uposlenik&& u):_sifra(u._sifra) {
		strncpy_s(_ime, u._ime, _TRUNCATE);
		strncpy_s(_prezime, u._prezime, _TRUNCATE);
		_radnoMjesto = u._radnoMjesto;
		u._radnoMjesto = nullptr;
	}
	~Uposlenik() { delete[] _radnoMjesto;   _radnoMjesto = NULL; }


	//Get methods
	int getSifru()const { return _sifra; }
	char* getIme()const { return AlocirajNizKaraktera(_ime); }
	char* getPrezime()const { return AlocirajNizKaraktera(_prezime); }
	char* getRadnoMjesto()const { return _radnoMjesto; }

	//Set methods
	void setIme(const char* ime) { strncpy_s(_ime, ime, _TRUNCATE); }
	void setPrezime(const char *prezime) { strncpy_s(_prezime, prezime, _TRUNCATE); }
	void setRadnoMjesto(const char *radnoMjesto) { delete[] _radnoMjesto; _radnoMjesto = AlocirajNizKaraktera(radnoMjesto); }


	friend ostream& operator<<(ostream&, const Uposlenik&);
	friend bool operator==(const Uposlenik&, const Uposlenik&);
};

ostream& operator<<(ostream& COUT, const Uposlenik& u) {
	if (u._radnoMjesto == nullptr)
		return COUT;
	cout << "Sifra: " << u._sifra << endl;
	cout << "Ime i prezime: " << u._ime << " " << u._prezime << endl;
	cout << "Radno mjesto: " << u._radnoMjesto << endl;
	return COUT;
}

bool operator==(const Uposlenik& u1, const Uposlenik& u2) {	return u1._sifra == u2._sifra; }

int Uposlenik::generatorSifre = 1;

class Aktivnost {

private:
	char* _naziv;
	bool _zavrsena;
	int* _trajanje; //Izrazeno u danima 

	Uposlenik* _uposlenici[5];

public:
	//Constructors and destructor
	Aktivnost() :_naziv(nullptr), _zavrsena(false), _trajanje(new int(0)) {
		for (int i = 0; i < 5; i++)
			_uposlenici[i] = nullptr;
	}
	Aktivnost(const char *naziv, bool zavrsena, int trajanje) :_naziv(AlocirajNizKaraktera(naziv)), _zavrsena(zavrsena), _trajanje(new int(trajanje)) {
		for (int i = 0; i < 5; i++)
			_uposlenici[i] = nullptr;
	}
	Aktivnost(const Aktivnost& a) :_naziv(AlocirajNizKaraktera(a._naziv)), _zavrsena(a._zavrsena), _trajanje(new int(*a._trajanje)) {
		for (int i = 0; i < 5; i++)
			if (a._uposlenici[i] != nullptr)
				_uposlenici[i] = new Uposlenik(*a._uposlenici[i]);
	}
	Aktivnost(Aktivnost&& a) :_zavrsena(a._zavrsena) {
		_naziv = a._naziv;
		_trajanje = a._trajanje;
		a._naziv = nullptr;
		a._trajanje = nullptr;
		for (int i = 0; i < 5; i++) {
			if (a._uposlenici[i] != nullptr)
				_uposlenici[i] = a._uposlenici[i];
			a._uposlenici[i] = nullptr;
		}
	}
	~Aktivnost() {
		delete[] _naziv; _naziv = nullptr;
		delete _trajanje; _trajanje = nullptr;
	}

	//Operators
	Aktivnost& operator=(const Aktivnost& a) {
		if (this == &a)
			return *this;

		delete[] _naziv;
		delete _trajanje;
		_naziv = AlocirajNizKaraktera(a._naziv);
		_trajanje = new int(*a._trajanje);
		_zavrsena = a._zavrsena;
		for (int i = 0; i < 5; i++) {
			delete[] _uposlenici[i];
			if (a._uposlenici[i] != nullptr)
				_uposlenici[i] = new Uposlenik(*a._uposlenici[i]);
		}
		return *this;
	}

	//Funkciju koja omogucava angazovanje uposlenika na projektnoj aktivnosti. Funkcija treba da vrati vrijednost tipa 
	//bool za uspjesno, odnosno neuspjesno dodavanje uposlenika. Nije moguce dodati istog uposlenika vise puta. Nije moguce  
	//angazovati uposlenike na vec zavrsenoj aktivnosti. 
	Aktivnost& operator+=(const Uposlenik& up) {
		if (_zavrsena) {
			cout << "Zao nam je, ali je aktivnost vec zavrsena..\n";
			return *this;
		}

		int i = 0;
		while (_uposlenici[i] != nullptr && i<5) {
			if (*_uposlenici[i] == up)
			{
				cout << "Uposlenik vec angazovan..\n";
				return *this;
			}
			i++;
		}
		if (i == 5) {
			cout << "Nije moguce dodati vise uposlenika..\n";
			return *this;
		}
		_uposlenici[i] = new Uposlenik(up);
		return *this;
	}

	//Funkciju koja uklanja uposlenika na aktivnosti sa sifrom proslijedjenom kao parametar. 
	Aktivnost& operator-=(int sifra) {
		int i = 0;
		while (_uposlenici[i] != nullptr && i < 5) {
			if (sifra == _uposlenici[i]->getSifru()) {
				delete _uposlenici[i];
				_uposlenici[i] = nullptr;
				for (int z = i; z < 4; z++)
					_uposlenici[z] = _uposlenici[z + 1];
				cout << "Uposlenik uspjesno uklonjen..\n";
				return *this;
			}
			i++;
		}
		cout << "Uposlenik sa unijetom sifrom ne postoji na ovoj aktivnosti..\n";
		return *this;
	}
	Uposlenik* operator[](int index) {
		if (index < 0 || index>4)
			return nullptr;
		return _uposlenici[index];
	}

	//Get methods
	char* getNaziv()const { return _naziv; }
	int getTrajanje()const { return *_trajanje; }
	bool getZavrsena()const { return _zavrsena; }

	//Set methods
	void setNaziv(const char* naziv) { delete[] _naziv; _naziv = AlocirajNizKaraktera(naziv); }
	void setTrajanje(int trajanje) { *_trajanje = trajanje; }
	void setZavrseno(bool zav) { _zavrsena = zav; }

	 //Funkciju koja uklanja sve uposlenike angazovane na aktivnosti. 
	void removeAllEmployees() {
		for (int i = 0; i < 5; i++)
			if (_uposlenici[i] != nullptr) {
				delete _uposlenici[i]; _uposlenici[i] = nullptr;
			}
	}
	friend ostream& operator<<(ostream&, const Aktivnost&);
	friend bool operator==(const Aktivnost&, const Aktivnost&);
};

ostream& operator<<(ostream& COUT, const Aktivnost& akt) {
	if (akt._naziv == nullptr || akt._trajanje == nullptr)
		return COUT;
	cout << "Naziv aktivnosti: " << akt._naziv << endl;
	cout << "Trajanje aktivnosti: " << *akt._trajanje <<" dana\n";
	cout << "Zavrsena: ";
	if (akt._zavrsena)
		cout << "DA\n";
	else
		cout << "NE\n";
	cout << "--Uposlenici angazovani na ovoj aktivnosti--\n";
	for (int i = 0; i < 5; i++)
		if (akt._uposlenici[i] != nullptr)
			cout << akt._uposlenici[i]->getIme()<<" "<<akt._uposlenici[i]->getPrezime() << " - "<<akt._uposlenici[i]->getRadnoMjesto()<< endl;
	return COUT;
}
bool operator==(const Aktivnost& akt1, const Aktivnost& akt2) {
	return strcmp(akt1._naziv, akt2._naziv) == 0;
}
class Projekat {
	char* _naziv;
	char* _oblast;
	int _trajanje; //Izrazeno u mjesecima 

	Aktivnost* _aktivnosti;
	int _brojAktivnosti;

public:

	//Constructors and destructor
	Projekat():_naziv(nullptr),_oblast(nullptr),_trajanje(0),_aktivnosti(nullptr),_brojAktivnosti(0){}

	Projekat(const char *naziv,const char *oblast,int trajanje):_naziv(AlocirajNizKaraktera(naziv)),_oblast(AlocirajNizKaraktera(oblast)),
		_trajanje(trajanje),_aktivnosti(nullptr),_brojAktivnosti(0){}

	Projekat(const Projekat& pr) :_naziv(AlocirajNizKaraktera(pr._naziv)), _oblast(AlocirajNizKaraktera(pr._oblast)), _trajanje(pr._trajanje),
		_brojAktivnosti(pr._brojAktivnosti) {
		if (_brojAktivnosti > 0) {
			_aktivnosti = new Aktivnost[_brojAktivnosti];
			for (int i = 0; i < _brojAktivnosti; i++)
				_aktivnosti[i] = pr._aktivnosti[i];
		}
		else _aktivnosti = nullptr;
	}
	Projekat(Projekat&& pr) :_trajanje(pr._trajanje), _brojAktivnosti(pr._brojAktivnosti) {
		_naziv = pr._naziv;
		_oblast = pr._oblast;
		_aktivnosti = pr._aktivnosti;
		pr._naziv = nullptr;
		pr._oblast = nullptr;
		pr._aktivnosti = nullptr;
	}
	//Operators
	Projekat& operator=(const Projekat& pr) {
		if (&pr == this)
			return *this;
		delete[] _naziv;
		delete[] _oblast;
		_naziv = AlocirajNizKaraktera(pr._naziv);
		_oblast = AlocirajNizKaraktera(pr._oblast);
		_trajanje = pr._trajanje;
		_brojAktivnosti = pr._brojAktivnosti;
		if (_brojAktivnosti > 0 && pr._aktivnosti != nullptr) {
			_aktivnosti = new Aktivnost[_brojAktivnosti];
			for (int i = 0; i < _brojAktivnosti; i++)
				_aktivnosti[i] = pr._aktivnosti[i];
		}
		else _aktivnosti = nullptr;
		return *this;
	}
	//Funkciju za dodavanje projektnih aktivnosti. Sprijeciti dodavanje vise aktivnosti sa istim nazivom. 
	//Za potrebe poredjenja aktivnosti kreirati funkciju unutar klase Aktivnosti. 
	Projekat& operator+=(const Aktivnost& akt) {
		if(_brojAktivnosti>0)
			for(int i=0;i<_brojAktivnosti;i++)
				if (_aktivnosti[i] == akt) {
					cout << "Aktivnost vec postoji..\n";
					return *this;
				}
		Aktivnost* temp = new Aktivnost[_brojAktivnosti + 1];
		if (_brojAktivnosti > 0)
			for (int i = 0; i < _brojAktivnosti; i++)
				temp[i] = _aktivnosti[i];
		delete[] _aktivnosti;
		_aktivnosti = temp;
		_aktivnosti[_brojAktivnosti++] = akt;
		cout << "Aktivnost uspjesno dodana..\n";
		return *this;
	}
	int nadjiAktivnost(const char *naziv) {
		if (naziv == nullptr || _brojAktivnosti <= 0)
			return -1;
		for (int i = 0; i < _brojAktivnosti; i++)
			if (strcmp(naziv, _aktivnosti[i].getNaziv()) == 0)
				return i;
		return -1;
	}
	//Funkciju koja treba da aktivnosti sa proslijedjenim nazivom dodijeli novog uposlenika. 
	Projekat& operator()(const char* nazivAktivnosti, const Uposlenik& up) {
		if (_brojAktivnosti <= 0)
			return *this;

		int trazena = nadjiAktivnost(nazivAktivnosti);
		if (trazena != -1) {
			cout << "Aktivnost pronadjena! Uposlenik dodan!\n";
			_aktivnosti[trazena] += up;
			return *this;
		}

		cout << "Aktivnost nije pronadjena!\n";
		return *this;
	}

	//Funkciju koja uklanja odredjenog uposlenika na aktivnosti. Ulazni parametri funkcije su naziv aktivnosti i sifra uposlenika. 
	Projekat& operator()(const char *nazivAktivnosti, int sifraUposlenika) {
		if (_brojAktivnosti <= 0)
			return *this;
		int trazena = nadjiAktivnost(nazivAktivnosti);
		if (trazena != -1) {
			cout << "Aktivnost pronadjena..\n";
			for (int i = 0; i < 5; i++)
				if (_aktivnosti[trazena][i]!=nullptr && _aktivnosti[trazena][i]->getSifru() == sifraUposlenika) {
					_aktivnosti[trazena] -= sifraUposlenika;
					cout << "Uposlenik uklonjen..\n";
					return *this;
				}
		}
		cout << "Aktivnost nije pronadjena - uposlenik nije uklonjen!\n";
		return *this;
	}

	//Funkciju koja uklanja sve uposlenike angazovane na aktivnosti sa nazivom proslijedjenim kao parametar. 
	Projekat& operator-=(const char *naziv) {

		int t = nadjiAktivnost(naziv);
		if (t == -1){
			cout << "Aktivnost ne postoji..\n";
			return *this;
		}
		_aktivnosti[t].removeAllEmployees();
		cout << "Uposlenici uspjesno uklonjeni!\n";
		return *this;
	}


	//Funkciju koja oznacava aktivnost kao zavrsenu. Funkcija kao parametar prima naziv aktivnosti. 
	void oznaciZavrsenom(int aktivnost) {
		if (aktivnost<0 || aktivnost>_brojAktivnosti)
			return;
		_aktivnosti[aktivnost].setZavrseno(true);
		cout << "Aktivnost oznacena kao zavrsena..\n";
	}

	//Funkciju koja treba da vrati sve uposlenike angazovane na odredjenoj aktivnosti. Ulazni parametar je naziv aktivnosti. 
	Uposlenik** angazovaniUposlenici(const char *nazivAktivnosti,int& br) {
		int t = nadjiAktivnost(nazivAktivnosti);

		if (t == -1) {
			cout << "Aktivnost sa datim nazivom ne postoji..\n";
			return nullptr;
		}
		Uposlenik** temp = new Uposlenik*[5];
		int k = 0, i = 0;
		while (i < 5) {
			if (_aktivnosti[t][i] != nullptr) {
				temp[k++] = new Uposlenik(*_aktivnosti[t][i]);
				br++;
			}
			i++;
		}
		return temp;
	}

	//Funkcija koja vraca sve uposlenika angazovane na svim aktivnostima (bez duplikata)
	Uposlenik** angazovaniUposleniciSveAktivnost(int &bru) {

		if (_brojAktivnosti <= 0)
			return nullptr;

		Uposlenik **temp = new Uposlenik*[100];
		for (int i = 0; i < 100; i++) 
			temp[i] = nullptr;
	
		auto provera = [=](Uposlenik& u,int x) {
			if (x <= 0)
				return true;
			for (int i = 0; i < x; i++)
				if (temp[i]!=nullptr && strcmp(temp[i]->getIme(), u.getIme()) == 0 && strcmp(temp[i]->getPrezime(), u.getPrezime()) == 0)
					return false;
			return true;
		};
		for (int i = 0; i < _brojAktivnosti; i++)
			for (int j = 0; j < 5; j++)
				if (_aktivnosti[i][j] != nullptr && provera(*_aktivnosti[i][j],bru))
					temp[bru++] = new Uposlenik(*_aktivnosti[i][j]);
		if (bru <= 0)
			return nullptr;
		return temp;
	}
	int *brojAngazmanaUposlenika(int &bru,int *brSatii,bool zavrsena=false) {
		int brSati = 0;
		Uposlenik **temp = angazovaniUposleniciSveAktivnost(bru);
		if (bru <= 0)
			return nullptr;
		auto indexUposlenika = [temp, bru](Uposlenik& u) {
			for (int i = 0; i < bru; i++)
				if (temp[i]!=nullptr && u == *temp[i])
					return i;

			return -1;
		};

		int *brojAngazmana = new int[bru] {0};

		for (int i = 0; i < _brojAktivnosti; i++)
			for (int j = 0; j < 5; j++)
				if (_aktivnosti[i][j] != nullptr) {
					int index = indexUposlenika(*_aktivnosti[i][j]);
					if (index != -1) {
						if (zavrsena && _aktivnosti[i].getZavrsena()) {
							brSati +=( _aktivnosti[i].getTrajanje() * 24);
							brojAngazmana[index]++;
						}
						else if (!zavrsena) {
							brSati += (_aktivnosti[i].getTrajanje() * 24);
							brojAngazmana[index]++;
						}

					}
				}
	
		*brSatii = brSati;
		return brojAngazmana;
	}
	//Funkciju koja pronalazi uposlenika sa najvecim brojem angazmana na aktivnostima. 
	Uposlenik* najveciBrojAngazmana(int *brSati,bool zavrsene=false) {
		if (_brojAktivnosti <= 0)
			return nullptr;
		int bru = 0;
		Uposlenik **temp = angazovaniUposleniciSveAktivnost(bru);
		int *brAngazmana;
		bru = 0;
		if (zavrsene)
			brAngazmana = brojAngazmanaUposlenika(bru, brSati, true);
		else
			brAngazmana = brojAngazmanaUposlenika(bru,brSati);
		
		int max = 0;
		for (int i = 1; i < bru; i++)
			if (brAngazmana[i] > brAngazmana[max])
				max = i;

		return temp[max];
	}

	//Funkciju koja pronalazi 3 uposlenika koja su angazovana na najvecem broju nezavrsenih aktivnosti. 
	Uposlenik** triUposlenikaSaNajviseAngzmana() {
		int bru = 0;
		Uposlenik **temp = angazovaniUposleniciSveAktivnost(bru);
		int brSati = 0;
		bru = 0;
		int *brAngazmana = brojAngazmanaUposlenika(bru,&brSati);
		if (bru <= 0)
			return nullptr;

		Uposlenik** triNaj = new Uposlenik*[3];
		int z = 0;
		while (z < 3) {
			int max = 0;
			for (int i = 1; i < bru; i++)
				if (brAngazmana[i] > brAngazmana[max] && brAngazmana[i] != -1)
					max = i;

			if (brAngazmana[max] != -1) {
				triNaj[z++] = new Uposlenik(*temp[max]);
				brAngazmana[max] = -1;
			}
		}
		return triNaj;
	}

	//Funkciju koja ispisuje sve zavrsene aktivnosti na projektu. 
	void Info() {
		if (_brojAktivnosti <= 0)
			return;
		for (int i = 0; i < _brojAktivnosti; i++)
			if (_aktivnosti[i].getZavrsena())
				cout << _aktivnosti[i] << endl;
	}
	friend ostream& operator<<(ostream&, const Projekat&);
};

ostream& operator<<(ostream& COUT, const Projekat& pr) {
	if (pr._naziv == nullptr || pr._oblast == nullptr || pr._trajanje == 0)
		return COUT;
	cout << "\n\nNaziv projekta: " << pr._naziv;
	cout << "\nOblast: " << pr._oblast << endl;
	cout << "Trajanje: " << pr._trajanje << " dana\n";
	cout << "Broj aktivnosti: " << pr._brojAktivnosti << endl;
	if (pr._brojAktivnosti > 0)
		for (int i = 0; i < pr._brojAktivnosti; i++)
			cout << i + 1 <<". "<< pr._aktivnosti[i].getNaziv() << endl << endl;
	else
		cout << "--TRENUTNO NEMA AKTIVNOSTI--\n\n";
	return COUT;
}

/*Kreirati globalnu funkciju koja pronalazi najuspjesnijeg uposlenika na projektima (vise projekata) proslijedjenim kao parametar. 
Uposlenik se smatra najuspjesnijim ako je angazovan na najvecem broju zavrsenih aktivnosti. Ako vise uposlenika ima isti
broj zavrsenih aktivnosti, uspjesnijim se smatra onaj cije su aktivnosti trajale duze. */
Uposlenik* najuspjesnijiUposlenik(Projekat* projects, int brPr) {
	if (brPr <=0 || projects == nullptr)
		return nullptr;

	int *nizOdradjenihSati = new int[brPr] {0};
	Uposlenik **nizUposlenika = new Uposlenik*[brPr];

	int counter = 0;
	for (int i = 0; i < brPr; i++) {
		Uposlenik* k = projects[i].najveciBrojAngazmana(&nizOdradjenihSati[i], true);
		if (k != nullptr)
			nizUposlenika[counter++] = new Uposlenik(*k);
	}
	Uposlenik **nizBezDuplikata = new Uposlenik*[brPr];
	int *nizOdradjeniSatiBezDuplikata = new int[brPr] {0};
	int *brojacProjekata = new int[brPr] {0};

	nizBezDuplikata[0] = new Uposlenik(*nizUposlenika[0]);
	nizOdradjeniSatiBezDuplikata[0] = nizOdradjenihSati[0];

	int brojac = 1;
	for (int i = 1; i < counter; i++) {
		int flag = 0;
		for (int j = 0; j < brojac; j++)
			if (*nizUposlenika[i] == *nizBezDuplikata[j]) {
				brojacProjekata[j]++;
				nizOdradjeniSatiBezDuplikata[j] += nizOdradjenihSati[i];
				flag = 1;
			}
		if (flag == 0) {
			brojacProjekata[brojac]++;
			nizOdradjeniSatiBezDuplikata[brojac] = nizOdradjenihSati[i];
			nizBezDuplikata[brojac++] = new Uposlenik(*nizUposlenika[i]);
		}
		else
			flag = 0;
	}

	int max = 0;
	for (int i = 1; i < brojac; i++)
		if (brojacProjekata[i] > brojacProjekata[max])
			max = i;
	for (int i = 0; i < brojac; i++)
		if (i != max && brojacProjekata[max] == brojacProjekata[i])
			if (nizOdradjeniSatiBezDuplikata[i] > nizOdradjeniSatiBezDuplikata[max])
				max = i;
	return nizBezDuplikata[max];
}

int main() {  
	
	//Testirati sve dostupne funkcionalnosti 
	Projekat *projects = new Projekat[3];

	projects[0]=Projekat("Izgradja Dzamije", "Arhitektura", 12);
	projects[1] = Projekat("Izgradnja stambenih jedinica","Arhitektura", 24);
	projects[2] = Projekat("Sakupljanje humanitarne pomoci", "Volontiranje", 2);

	Uposlenik up1("John", "Newman", "Arhitekta"), up2("Steve", "Jobs", "Gradjevinski inzinjer"), up3("Geerads", "Lersr", "Fizicki radnik");
	Aktivnost akt1("Postavljanje temelja", false, 13), akt2("Stolarija", false, 30), akt3("Krovna konstrukcija", false, 40);
	Uposlenik up4("Creed", "John", "Menadzer prodaje"), up5("Bart", "Simson", "Fizicki radnik");

	//--Testiranje funkcionalnosti klase Aktivnost--
	akt1 += up1;
	akt1 += up5;
	akt1 += up3;
	akt1 += up4;

	akt2 += up1;
	akt2 += up5;
	akt2 += up3;
	
	akt3 += up1;
	akt3 += up5;
	akt3 += up1; //ERROR MESSAGE: Uposlenik vec angazovan..
	akt3 += up1; //ERROR MESSAGE: Uposlenik vec angazovan..

	akt1 -= 0; //ERROR MESSAGE: Uposlenik sa unijetom sifrom ne postoji na ovoj aktivnosti
	akt1 -= 1; //MESSAGE: Uposlenik uspjesno uklonjen..

	cout <<endl<<endl<< akt1 << endl;
	//----------------------------------------------------------------------


	//--Testiranje funkcionalnosti klase Projekat--
	projects[0] += akt1; //MESSAGE: Aktivnost uspjesno dodana..
	projects[0] += akt2; //MESSAGE: Aktivnost uspjesno dodana..
	projects[0] += akt3; //MESSAGE: Aktivnost uspjesno dodana..

	projects[1] += akt1;
	projects[1] += akt2;

	projects[0] += akt1; //ERROR MESSAGE: Aktivnost vec postoji..

	projects[0]("Stolarija", Uposlenik("Jo", "Bohman", "Pomocni radnik"));  //MESSAGE: Aktivnost pronadjena - Uposlenik dodan!
	projects[0]("Keramika", Uposlenik()); //ERROR MESSAGE: Aktivnost nije pronadjena..

	projects[0]("Postavljanje temelja", 1); //MESSAGE: Uposlenik uklonjen..
	cout << projects[0] << endl;

	int brSati = 0;
	cout << "--Uposlenik koji je angazovan na najvecem broju aktivnosti--\n" << *projects[0].najveciBrojAngazmana(&brSati)<< endl;
	
	Uposlenik** triNajangazovanija = projects[0].triUposlenikaSaNajviseAngzmana();
	cout << "\n\n-------------TRI NAJANGAZOVANIJA UPOSLENIKA---------\n";
	for (int i = 0; i < 3; i++)
		cout << *triNajangazovanija[i] << endl;

	int br = 0;
	Uposlenik**angazovaniAktivnost = projects[0].angazovaniUposlenici("Postavljanje temelja",br);
	if (br > 0) {
		cout << "\n\n------Angazovani uposlenici za postavljanje temelja------\n\n";
		for (int i = 0; i < br; i++)
			cout << *angazovaniAktivnost[i] << endl;
	}
	else cout << "Trenutno nema angazovanih uposlenika za postavljanje temelja...\n\n";


	cout << "\n\n\nUposlenik koji je bio najuspjesniji u toku rada na projektima je --> " << *najuspjesnijiUposlenik(projects, 3);
	system("pause"); 
	return 0;
}