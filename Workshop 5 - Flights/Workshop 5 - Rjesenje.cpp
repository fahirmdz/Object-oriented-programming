#include"pch.h"
#include<iostream>
#include<iomanip>
#include<cstring>
using namespace std;

auto crt = "----------------------------------------------------\n";

char* AlocirajChar(const char *sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char *temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class Vrijeme {
	int* _sati;
	int* _minute;
	int* _sekunde;

public:
	Vrijeme(int sati = 0, int minute = 0, int sekunde = 0) :_sati(new int(sati)), _minute(new int(minute)), _sekunde(new int(sekunde)) {}
	Vrijeme(const Vrijeme& v) {
		_sati = new int(*v._sati);
		_minute = new int(*v._minute);
		_sekunde = new int(*v._sekunde);
	}
	Vrijeme(Vrijeme&& v) {
		_sati = v._sati;
		_minute = v._minute;
		_sekunde = v._sekunde;
		v._sati = v._minute = v._sekunde = nullptr;
	}
	~Vrijeme() {
		delete _sati; _sati = nullptr;
		delete _minute; _minute = nullptr;
		delete _sekunde; _sekunde = nullptr;
	}

	Vrijeme& operator=(const Vrijeme& v) {
		if (&v == this)
			return *this;
		delete _sati; 
		delete _minute; 
		delete _sekunde; 
		_sati = new int(*v._sati);
		_minute = new int(*v._minute);
		_sekunde = new int(*v._sekunde);
		return *this;
	}

	int getMin()const { return *_minute; }
	int getSat()const {return  *_sati;}
	int getSek()const { return *_sekunde; }

	long int sekunde()const { return *_sekunde + (*_minute * 60) + (*_sati * 60 * 60); }

	bool valid()const { return _sati != nullptr && _minute!=nullptr && _sekunde!=nullptr; }
	Vrijeme& operator+=(int m) {
		if (m <= 0)
			return *this;
		(*_minute) += m;
		if (((*_minute) / 60) > 0) {
			(*_sati) += ((*_minute) / 60);
			*_minute = ((*_minute) % 60);
			if ((*_sati) > 23)
				(*_sati) -= 24;
		}
		return *this;
	}
	friend ostream& operator<<(ostream&, const Vrijeme&);
	friend bool operator==(const Vrijeme&, const Vrijeme&);
};
ostream& operator<<(ostream& COUT, const Vrijeme& v) {
	auto f = [](int x) {
		int brc = 0;
		while (x != 0) {
			brc++;
			x /= 10;
		}
		return brc;
	};
	if (!v.valid())
		return COUT;
	if (f(*v._sati) <= 1)
		cout << "0";
	cout << *v._sati << ":";
	if (f(*v._minute) <= 1)
		cout << "0";
	cout << *v._minute << ":";
	if (f(*v._sekunde) <= 1)
		cout << "0";
	cout << *v._sekunde;
	return COUT;
}
bool operator==(const Vrijeme& v1, const Vrijeme& v2) {
	return *v1._sati == *v2._sati && *v2._minute == *v1._minute && *v1._sekunde == *v2._sekunde;
}

class Let {
	static const  int _oznakaSize = 10;
	char _oznaka[_oznakaSize];
	char* _odrediste;
	int _brIzlazneKapije;
	Vrijeme _vrijemePolijetanja; //sati i minute  
	int _trajanje; //izraženo u minutama 
	int _kasnjenje; //izraženo u minutama 

public:
	//Potrebne konstruktor i destruktor funkcije 
	Let():_odrediste(nullptr),_trajanje(0),_kasnjenje(0){}
	Let(const char* oznaka, const char* odrediste, int brKapije, Vrijeme vrPol, int trajanje, int kasnjenje) :_odrediste(AlocirajChar(odrediste)),
		_brIzlazneKapije(brKapije), _vrijemePolijetanja(vrPol), _trajanje(trajanje), _kasnjenje(kasnjenje) {
		strncpy_s(_oznaka, oznaka, 9);
	}
	Let(const Let& let) :_odrediste(AlocirajChar(let._odrediste)),
		_brIzlazneKapije(let._brIzlazneKapije), _vrijemePolijetanja(let._vrijemePolijetanja), _trajanje(let._trajanje), _kasnjenje(let._kasnjenje) {
		strncpy_s(_oznaka, let._oznaka, 9);
	}
	Let(Let&& let){
		strncpy_s(_oznaka, let._oznaka, 9);
		_odrediste = let._odrediste;
		let._odrediste = nullptr;
		_brIzlazneKapije = let._brIzlazneKapije;
		_vrijemePolijetanja = let._vrijemePolijetanja;
		_trajanje = let._trajanje;
		_kasnjenje = let._kasnjenje;
	}
	~Let() {
		delete[] _odrediste; _odrediste = nullptr;
	}

	Vrijeme getVrijemePolijetanja()const { return _vrijemePolijetanja; }
	char* getOznaku()const { return AlocirajChar((char*)_oznaka); }
	int getKasnjenje()const { return _kasnjenje; }
 //Funkciju sa jednim parametrom koja postavlja informaciju o eventualnom kašnjenju na vrijednost zadanu parametrom. 
	void setKasnjenje(int kasnjenje) { _kasnjenje = kasnjenje; }

 /*Preklopiti unarni operator "!" putem kojeg je moguce saznati da li odgovarajuci let kasni
 ili ne (vratiti logicku  vrijednost "true" u slucaju kašnjenja, u suprotnom vratiti "false").*/
	bool operator!() { return _kasnjenje > 0; }

 //Funkciju koja kao rezultat vraca trajanje leta.
	int getTrajanjeLeta()const { return _trajanje; }

	/*Preklopiti operator + na nacin da omogucava sabiranje objekata tipa "Let" i cijelog broja, pri
	cemu se kao rezultat dobiva novi objekat tipa "Let", u kojem je vrijeme polijetanja pomjereno
	unaprijed za iznos odreden drugim sabirkom (racunato u minutama).  Takoder preklopiti i operator
	"+=" koji osigurava da izraz oblika "X += Y uvijek ima isto znacenje kao i izraz  "X = X + Y" */
	Let& operator+(int polijetanje) {
		if (polijetanje <= 0)
			return *this;
		this->_vrijemePolijetanja +=polijetanje;
		return *this;
	}
	Let& operator+=(int pol) {

		this->operator+(pol);
		return *this;
	}

	/* Funkciju kojom je moguce saznati ocekivano vrijeme polijetanja kada se uracuna iznos kašnjenja
	u odnosu na predvideno vrijeme polijetanja (preklopiti operator + u klasi Vrijeme). */
	Vrijeme* vrijemeDolaska() {
		Vrijeme *dolazak;
		dolazak = new Vrijeme(this->_vrijemePolijetanja.getSat(), this->_vrijemePolijetanja.getMin(), this->_vrijemePolijetanja.getSek());
		(*dolazak) += this->_trajanje;

		if (_kasnjenje <= 0)
			(*dolazak) += this->_kasnjenje;
		return dolazak;
	}
	//Funkciju koja vraca ocekivano vrijeme slijetanja 

	/*Preklopiti operator "++" na nacin da pomijera vrijeme polaska za jedan sat unaprijed.
	Potrebno je podržati i prefiksnu i postfiksnu verziju ovog operatora.*/
	Let& operator++() {
		this->_vrijemePolijetanja += 60;
		return *this;
	}
	Let operator++(int v) {
		Let temp = *this;
		temp + v;
		return temp;
	}

	/*Preklopiti relacione operatore "<" i ">" koji ispituju koji let nastupa ranije, odnosno kasnije.
	Operator "<" vraca logicku vrijednost "true" ukoliko polijetanje leta sa lijeve strane nastupa
	prije polijetanje leta sa desne strane, a u suprotnom vraca logicku vrijednost "false".
	Analogno vrijedi za operator ">". Prilikom uporedivanja treba uzeti u obzir i ocekivano vrijeme kašnjenja,
	a ne samo planirano vrijeme polijetanja.*/
	bool operator<(Let& let) {
		Vrijeme temp(let._vrijemePolijetanja);
		if (let._kasnjenje > 0)
			temp += let._kasnjenje;
		Vrijeme temp1(this->_vrijemePolijetanja);
		if (this->_kasnjenje > 0)
			temp1 += this->_kasnjenje;

		return temp1.sekunde() < temp.sekunde();
	}
	bool operator>(Let let) {
		Vrijeme temp(let._vrijemePolijetanja);
		if (let._kasnjenje > 0)
			temp += let._kasnjenje;
		Vrijeme temp1(this->_vrijemePolijetanja);
		if (this->_kasnjenje > 0)
			temp1 += this->_kasnjenje;

		return temp1.sekunde() > temp.sekunde();
	}
	/*Preklopiti operator "<<" koji treba da podrži ispis objekata tipa "Let" na ekran. U slucaju da
	se radi o polijetanju bez kašnjenja, ispis bi trebao da izgleda kako slijedi:
	
	 JFK 156 Atalanta    12:50   19:30   5

	Podaci predstavljaju redom: oznaku leta, naziv odredišta, vrijeme polijetanja, ocekivano
	vrijeme slijetanja i broj izlazne kapije.

	U slucaju da se radi o letu koji kasni, ispis bi trebao izgledati kako slijedi:

	 ZGK 932 Zagreb    15:50 (Planirano 15:30, Kasni 20 min)*/
	friend ostream& operator<<(ostream&,Let&);
	friend  bool operator==(const Let&, const Let&);
};
ostream& operator<<(ostream& COUT,Let& let) {
	if (let._odrediste == nullptr)
		return COUT;
	Vrijeme a = let._vrijemePolijetanja;
	if (let._kasnjenje <= 0) {
		a += let._trajanje;
		cout << "\t" << let._oznaka << "\t" << let._odrediste << "\t" << a << "\t\t\t\t\t" << let._brIzlazneKapije << endl;
	}
	else
		cout << "\t" << let._oznaka << "\t" << let._odrediste << "\t(Planirano " << a << ", Kasni " << let._kasnjenje << " MIN)" << "\t" << let._brIzlazneKapije;

	return COUT;
}
bool operator==(const Let& l1, const Let& l2) {
	return strcmp(l1._oznaka, l2._oznaka) == 0;
}
class RasporedLetova {
	int _brojRegistrovanihLetova;
	const int _maksimalanBrojLetova;
	Let** _letovi;

public:
	RasporedLetova(int maksimalanBrojLetova=3) : _maksimalanBrojLetova(maksimalanBrojLetova),
		_letovi(new Let*[_maksimalanBrojLetova]), _brojRegistrovanihLetova(0) {
		for (int i = 0; i < _maksimalanBrojLetova; i++)
			_letovi[i] = nullptr;
	}
	RasporedLetova(const RasporedLetova& rl):_maksimalanBrojLetova(rl._maksimalanBrojLetova),
	_letovi(new Let*[rl._maksimalanBrojLetova]),_brojRegistrovanihLetova(rl._brojRegistrovanihLetova){
		if (_brojRegistrovanihLetova > 0) {
			for (int i = 0; i < _brojRegistrovanihLetova; i++)
				_letovi[i] = rl._letovi[i];
			if (_maksimalanBrojLetova - _brojRegistrovanihLetova > 0)
				for (int j = _brojRegistrovanihLetova; j < _maksimalanBrojLetova; j++)
					_letovi[j] = nullptr;
		}
		else
			for (int i = 0; i < _maksimalanBrojLetova; i++)
				_letovi[i] = nullptr;
	}
	RasporedLetova(RasporedLetova&& rl):_maksimalanBrojLetova(rl._maksimalanBrojLetova),_brojRegistrovanihLetova(rl._brojRegistrovanihLetova) {
		_letovi = rl._letovi;
		rl._letovi = nullptr;
	}
	~RasporedLetova() {
		if (_brojRegistrovanihLetova > 0)
			for (int i = 0; i < _brojRegistrovanihLetova; i++)
				delete _letovi[i];
		delete[] _letovi;
		_letovi = nullptr;
	}
	int getLetSaOznakom(const char* oznaka) {
		if (oznaka == nullptr || _brojRegistrovanihLetova <= 0)
			return -1;
		for (int i = 0; i < _brojRegistrovanihLetova; i++)
			if (_letovi[i] != nullptr)
				if (strcmp(_letovi[i]->getOznaku(), oznaka) == 0)
					return i;
		return -1;
	}
	void sortLetovi() {
		if (_brojRegistrovanihLetova > 1)
			for (int i = 0; i < _brojRegistrovanihLetova - 1; i++) {
				for (int j = i + 1; j < _brojRegistrovanihLetova; j++) {
					if ((*_letovi[i]) > (*_letovi[j]))
					{
						Let *temp = _letovi[i];
						_letovi[i] = _letovi[j];
						_letovi[j] = temp;
					}
				}
			}
	}
	/*Preklopiti operator "+=" na nacin da registruje novi let u raspored. Raspored letova u svakom momentu treba biti sortiran
	prema vremenu polijetanja. Voditi racuna o maksimalnom broju letova.  Za potrebe poredenja vremena polaska letova preklopiti
	odgovarajuce operatore u klasi Vrijeme.*/
	RasporedLetova& operator+=(const Let& let) {
		if (_brojRegistrovanihLetova >= _maksimalanBrojLetova) {
			cout << "Raspored letova je treutno pun.. Zao nam je..\n";
			return *this;
		}
		if (_brojRegistrovanihLetova > 0)
			for (int i = 0; i < _brojRegistrovanihLetova; i++)
				if(_letovi[i]!=nullptr)
					if (let == *_letovi[i]) {
						cout << "Let vec registrovan..\n";
						return *this;
					}
		_letovi[_brojRegistrovanihLetova++] = new Let(let);
		sortLetovi();
		cout << "Let uspjesno dodan..\n";
		return *this;
	}

	/*Preklopiti operator "-=" na nacin da uklanja registrovani let sa oznakom definisanom putem desnog operanda.
	Pri tome sacuvati redoslijed prethodno registrovanih letova. */
	RasporedLetova& operator-=(const char* oznaka) {
		if (oznaka == nullptr || _brojRegistrovanihLetova<=0)
			return *this;
		int flag = 0;
		for(int i=0;i<_brojRegistrovanihLetova;i++)
			if(_letovi[i]!=nullptr)
				if (strcmp(_letovi[i]->getOznaku(), oznaka) == 0) {
					delete _letovi[i];
					_letovi[i] = nullptr;
					for (int j = i; j < _brojRegistrovanihLetova - 1; j++)
						_letovi[j] = _letovi[j + 1];
					cout << "Let uspjesno uklonjen..\n";
					flag = 1;
					_brojRegistrovanihLetova--;
				}
		if (flag == 0)
			cout << "Let nije pronadjen..\n";
		return *this;
	}

	//Preklopiti operator "[]" na nacin da vraca objekat tipa Let na osnovu proslijedene pozicije u rasporedu (pozicije krecu od 1). 
	Let* operator[](int index) {
		if (index<1 || index>_brojRegistrovanihLetova || _brojRegistrovanihLetova <= 0)
			return nullptr;
		index -= 1;
		return _letovi[index];
	}

	/*Preklopiti operator "()" na nacin da letu sa definisanom oznakom pomjeri vrijeme polijetanja za vrijednost
	drugog parametra izraženog   u minutama. */
	RasporedLetova& operator()(const char* oznaka,int minutiPom) {
		int temp = getLetSaOznakom(oznaka);
		if (temp != -1) {
			(*_letovi[temp]) += minutiPom;
			sortLetovi();
		}

		return *this;
	}

	//Funkciju koja daje prosjecno trajanje svih pohranjenih letova. 
	double prosjecnoTrajanje() {
		if (_brojRegistrovanihLetova <= 0)
			return 0;
		double sum = 0;
		for (int i = 0; i < _brojRegistrovanihLetova; i++)
			if (_letovi[i] != nullptr)
				sum += _letovi[i]->getTrajanjeLeta();
		sum /= (double)_brojRegistrovanihLetova;
		return sum;
	}

	/*Preklopiti operator "<<" koji ispisuje kompletan spisak svih letova.*/
	friend ostream& operator<<(ostream&, const RasporedLetova&);
};
ostream& operator<<(ostream& COUT, const RasporedLetova& rl) {
	cout << "\n-------RASPORED LETOVA-------\n";
	if (rl._brojRegistrovanihLetova <= 0)
		cout << "\n--TRENUTNO NEMA REGISTROVANIH LETOVA--\n";
	else {
		for (int i = 0; i < rl._brojRegistrovanihLetova; i++)
			cout << i + 1 << ". LET -> " << *rl._letovi[i] << endl << endl;
	}
	return COUT;
}

int main() {
	Vrijeme v1(19, 45, 2), v2(20, 30, 30), v4(15, 20, 12);

	Let l1("JB 142", "London - Heathrow", 5, v1, 4.5 * 60, 7);
	Let l2("XS 111", "Washington DC", 9, v2, 11 * 60, 15);
	Let l3("RT 952", "Dubai", 1, v4, 7 * 60, 0);

	RasporedLetova flightsTable;
	flightsTable += l1;
	flightsTable += l1; //ERROR MESSAGE: Let vec registrovan..
	flightsTable += l2;
	flightsTable += l3;


	flightsTable -= "JB 100"; //ERROR MESSAGE: Let nije pronadjen..
	cout << "=====PRIJE POMIJERANJA VRIJEMENA POLIJETANJA LETA ZA LONDON========\n\n";
	cout << flightsTable << endl;

	flightsTable("JB 142", 60); //Let za  London pomijeren za 1h
    flightsTable("JB 142", 60); //Let za  London pomijeren za 1h

	cout << "=====POSLIJE POMIJERANJA VRIJEMENA POLIJETANJA LETA ZA LONDON========\n\n";
	cout << flightsTable << endl;

	if (!l3.operator!())
		cout << "Let za Dubai ne kasni..\n";
	else
		cout << "Let za Dubai kasni..\n";

	cout << "\nProsjecno trajanje svih letova koji se nalaze na rasporedu je " << flightsTable.prosjecnoTrajanje()/60 << " h..\n\n";
	system("pause");
	return 0;
}