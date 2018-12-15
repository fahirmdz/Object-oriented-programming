//fahirmdz GitHub


#inckude"pch.h
#include<iostream>
#include<cstring>

using namespace std; 

char * AlocirajNizKaraktera(const char * sadrzaj) {
	
	if (sadrzaj == nullptr)  
		return nullptr; 
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel]; 
	strcpy_s(temp, vel, sadrzaj);
	return temp; 
}

template<class T, int max>
class Kolekcija {
T* _elementi[max]; 
int _trenutnoElemenata;

public: 

	//CONSTRUCTORS AND DESTRUCTOR
	Kolekcija():_trenutnoElemenata(0){
		for (int i = 0; i < max; i++)
			_elementi[i] = nullptr;
	}
	Kolekcija(const Kolekcija& k):_trenutnoElemenata(k._trenutnoElemenata){
		if (_trenutnoElemenata > 0) {
			for (int i = 0; i < _trenutnoElemenata; i++)
				if (k._elementi[i] != nullptr)
					_elementi[i] = new T(*k._elementi[i]);
			for (int i = _trenutnoElemenata; i < max; i++)
				_elementi[i] = nullptr;
		}
		else
			for (int i = 0; i < max; i++)
				_elementi[i] = nullptr;
	}
	Kolekcija(Kolekcija&& k) :_trenutnoElemenata(k._trenutnoElemenata) {
		if (_trenutnoElemenata > 0) 
			for (int i = 0; i < max; i++) {
				_elementi[i] = k._elementi[i]; 
				k._elementi[i] = nullptr;
			}
		cout << "\n---POZVAN MOVE CONSTRUCTOR--\n";
	}
	~Kolekcija() {
		if (_trenutnoElemenata > 0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				delete _elementi[i]; _elementi[i] = nullptr;
			}
	}

	//OPERATORS
	Kolekcija& operator=(const Kolekcija& k) {

		if (&k == this)
			return *this;

		_trenutnoElemenata = k._trenutnoElemenata;
		if (_trenutnoElemenata > 0) {
			for (int i = 0; i < _trenutnoElemenata; i++) {
				delete _elementi[i];
				if (k._elementi[i] != nullptr)
					_elementi[i] = new T(*k._elementi[i]);
			}
			for (int i = _trenutnoElemenata; i < max; i++)
				_elementi[i] = nullptr;
		}
		else
			for (int i = 0; i < max; i++)
				_elementi[i] = nullptr;
		return *this;
	}
	Kolekcija& operator+=(const T& el) {
		if(_trenutnoElemenata>0)
			for(int i=0;i<_trenutnoElemenata;i++)
				if (_elementi[i] != nullptr && *_elementi[i] == el) {
					cout << "Greska! Vec postoji..\n";
					return *this;
				}
		T** temp = new T*[_trenutnoElemenata + 1];
		if (_trenutnoElemenata > 0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				temp[i] = new T(*_elementi[i]);
				delete _elementi[i];
			}
		delete[] _elementi;
		_elementi = temp;
		_elementi[_trenutnoElemenata++] = el;
		cout << "Uspjesno dodan..\n";
		return *this;
	}
	T* operator[](int index) {
		if (index<0 || index>_trenutnoElemenata - 1 || _trenutnoElemenata <= 0)
			return nullptr;
		return _elementi[index];
	}
	
	//GET METHODS
	int getTrenutnoElemenata()const { return _trenutnoElemenata; }

	friend ostream& operator<< <>(ostream&, const Kolekcija&);
	friend bool operator==(const Kolekcija&, const Kolekcija&);
}; 

template<class T,int max>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T,max>& k) {
	if (k._trenutnoElemenata <= 0) {
		cout << "--Trenutno nema elemenata--\n\n";
		return COUT;
	}
	cout << "Broj elemenata: " << k._trenutnoElemenata << endl;
	for (int i = 0; i < k._trenutnoElemenata; i++)
		cout << i + 1 << ".  " << *_elementi[i] << endl << endl;
	return COUT;
}

template<class T,int max>
bool operator==(const Kolekcija<T, max>& k1, const Kolekcija<T, max>& k2) {
	if (k1._trenutnoElemenata == k2._trenutnoElemenata) {
		if (k1._trenutnoElemenata > 0)
			for (int i = 0; i < k1._trenutnoElemenata; i++)
				if (!(*k1._elementi[i] == *k2._elementi[i]))
					return false;
		return true;
	}
	return false;
}

class Prisustvo {
	char* _brojIndeksa;
	int _brojSati;

public:
	//CONSTRUCTORS AND DESTRUCTOR
	Prisustvo():_brojIndeksa(nullptr),_brojSati(0){}
	Prisustvo(const char* brojIndeksa, int brojSati) : _brojSati(brojSati), _brojIndeksa(AlocirajNizKaraktera(brojIndeksa)) {}
	Prisustvo(const Prisustvo& p):_brojSati(p._brojSati),_brojIndeksa(AlocirajNizKaraktera(p._brojIndeksa)){}
	Prisustvo(Prisustvo&& p):_brojSati(p._brojSati) {
		_brojIndeksa = p._brojIndeksa;
		p._brojIndeksa = nullptr;
	}
	~Prisustvo() { delete[] _brojIndeksa;   _brojIndeksa = nullptr; }


	//OPERATORS
	Prisustvo& operator=(const Prisustvo& p) {
		if (&p == this)
			return *this;
		delete[] _brojIndeksa;
		_brojIndeksa = AlocirajNizKaraktera(p._brojIndeksa);
		_brojSati = p._brojSati;
		return *this;
	}

	//Preklopljeni operator + i += za uvecavanje broja sati prisustva za vrijednost proslijedenog parametra 
	Prisustvo& operator+=(int brs) {
		if (brs > 0)
			_brojSati += brs;
		return *this;
	}
	Prisustvo operator+(int brs) {
		return Prisustvo(this->_brojIndeksa, _brojSati + brs);
	}
	bool operator<(Prisustvo& p) {
		int bri1 = atoi(_brojIndeksa), bri2 = atoi(p.getBrojIndeksa());
		return bri1 > bri2;
	}
	//GET METHODS
	int getBrojSati()const { return _brojSati; }
	char* getBrojIndeksa()const { return _brojIndeksa; }

	friend ostream& operator<<(ostream&, const Prisustvo&);
	friend bool operator==(const Prisustvo&, const Prisustvo&);
};

ostream& operator<<(ostream& COUT, const Prisustvo& p) {
	if (p._brojIndeksa == nullptr)
		return COUT;
	cout << "Broj indeksa: " << p._brojIndeksa << endl;
	cout << "Broj sati: " << p._brojSati << endl;
	return COUT;
}
bool operator==(const Prisustvo& p1, const Prisustvo& p2) {
	return strcmp(p1._brojIndeksa, p2._brojIndeksa) == 0 && p1._brojSati == p2._brojSati;
}


class Nastava {
	char _datum[12];
	char _tipNastave; //P ili V 
	int _odrzanoSati;
	Prisustvo* _prisutni;
	int _brojStudenata;

public:
	Nastava():_prisutni(nullptr),_brojStudenata(0),_odrzanoSati(0){}
	Nastava(const char* datum, char tip, int sati) :_tipNastave(tip), _odrzanoSati(sati), _brojStudenata(0), _prisutni(nullptr) {
		strncpy_s(_datum, datum, 11);
	}
	Nastava(const Nastava& n) :_tipNastave(n._tipNastave), _odrzanoSati(n._odrzanoSati), _brojStudenata(n._brojStudenata) {
		if (_brojStudenata > 0) {
			_prisutni = new Prisustvo[_brojStudenata];
			for (int i = 0; i < _brojStudenata; i++)
				_prisutni[i] = n._prisutni[i];
		}
		else _prisutni = nullptr;
	}
	Nastava(Nastava&& n):_brojStudenata(n._brojStudenata),_odrzanoSati(n._odrzanoSati),_tipNastave(n._tipNastave){
		_prisutni = n._prisutni;
		strncpy_s(_datum,n._datum, 11);
		n._prisutni = nullptr;
	}
	~Nastava() {
		delete[] _prisutni; _prisutni = nullptr;
	}

	int getBrojStudenata()const { return _brojStudenata; }
	Nastava& operator=(const Nastava& n) {
		if (&n == this)
			return *this;
		delete[] _prisutni;
		_brojStudenata = n._brojStudenata;
		_odrzanoSati = n._odrzanoSati;
		_tipNastave = n._tipNastave;
		if (_brojStudenata > 0)
		{
			_prisutni = new Prisustvo[_brojStudenata];
			for (int i = 0; i < _brojStudenata; i++)
				_prisutni[i] = n._prisutni[i];
		}
		else _prisutni = nullptr;
		strncpy_s(_datum, n._datum, 11);
		return *this;
	}

//Preklopljeni operator+= za dodavanje objekata tipa Prisustvo u niz _prisutni. 
		//Nije moguce dodati više puta prisustvo za istog studenta (preklopiti operator== u klasi Prisustvo).
		//Elementi u svakom momentu treba da budu sortirani po broju indeksa studenta (preklopiti operator < u klasi Prisustvo). 
	Nastava& operator+=(const Prisustvo& p) {
		if(_brojStudenata>0)
			for(int i=0;i<_brojStudenata;i++)
				if (p == _prisutni[i]) {
					cout << "Student vec postoji..\n";
					return *this;
				}
		Prisustvo* temp = new Prisustvo[_brojStudenata + 1];
		if (_brojStudenata > 0)
			for (int i = 0; i < _brojStudenata; i++)
				temp[i] = _prisutni[i];
		delete[] _prisutni;
		_prisutni = temp;
		_prisutni[_brojStudenata++] = p;
		sort();
		cout << "Student uspjesno dodan..\n";
		return *this;
	}


 //Preklopljeni operator-= za uklanjanje prisustva za studenta sa proslijedjenim brojem indeksa. Nakon uklanjanja,  
 //potrebno je sacuvati redoslijed dodavanja, odnosno podaci ostaju sortirani po broju indeksa. 
	Nastava& operator-=(const char* brIndeksa) {
		if (brIndeksa == nullptr)
			return *this;
		int indeks = getStudenta(brIndeksa);
		if (indeks == -1) {
			cout << "Student sa proslijedjenim brojem indeksa nije pronadjen..\n";
			return *this;
		}
		for (int i = indeks; i < _brojStudenata; i++)
			_prisutni[i] = _prisutni[i + 1];
		_brojStudenata--;
		cout << "Student uspjesno uklonjen..\n";
		return *this;
	}


 //Preklopljeni operator [] koji na osnovu broja indeksa provjerava da li je  
 //odredeni student prisustvovao nastavi i vraca njegove podatke o prisustvu. 
	Prisustvo* operator[](const char* indeks) {
		int index = getStudenta(indeks);
		if (index == -1)
			return false;
		return &_prisutni[index];
	}
	Prisustvo* operator[](int index) {
		if (index<0 || index>_brojStudenata || _brojStudenata <= 0)
			return nullptr;
		return &_prisutni[index];
	}
 //Preklopljeni operator () koji pronalazi studenta sa proslijedenim brojem indeksa i uvecava mu broj sati prisustva 
 //za vrijednost drugog parametra. Ako je studentu vec evidentiran odrzani broj sati nastave prekinuti funkciju. 
	Nastava& operator()(const char* indeks,int brs) {
		int index = getStudenta(indeks);
		if (index == -1) {
			cout << "Student sa prislijedjenim brojem indeksa nije pronadjen..\n";
			return *this;
		}
		if (_prisutni[index].getBrojSati() > 0) {
			cout << "Studentu vec evidentiran broj sati..\n";
			return *this;
		}
		_prisutni[index] += brs;
		cout << "Studentu uvecan broj sati..\n";
		return *this;
	}
 //Preklopljeni operator + koji zbraja dva objekta tipa Nastava na nacin da kreira novi objekat na osnovu lijevog operanda 
 //i dodaje mu prisutne studente desnog operanda. Funkciju napustiti ukoliko objekti koji ucestvuju u operaciji sabiranja 
 //nisu istog tipa nastave. Sprijeciti dodavanje prisustva za istog studenta više puta.  
 //Operator implementirati kao globalnu funkciju. 
	Nastava* operator+(Nastava& n) {
		if (this->_tipNastave != n._tipNastave)
			return nullptr;

		Nastava* temp = new Nastava(this->_datum, this->_tipNastave, this->_odrzanoSati);
		if (this->_brojStudenata > 0)
			for (int i = 0; i < this->_brojStudenata; i++)
				(*temp) += _prisutni[i];
		if (n._brojStudenata > 0)
			for (int i = 0; i < n._brojStudenata; i++)
				for (int j = 0; j < temp->getBrojStudenata(); j++)
					if (!(*n[i] == *(*temp)[j]))
						(*temp) += *n[i];
	}

	void sort() {
		if (_brojStudenata <= 1)
			return;
		for (int i = 0; i < _brojStudenata - 1; i++)
			for (int j = i + 1; j < _brojStudenata; j++)
				if (_prisutni[i] < _prisutni[j])
					swap(_prisutni[i], _prisutni[j]);
	}
	char* getDatum()const { return AlocirajNizKaraktera((char*)_datum); }
	char getTipNastave()const { return _tipNastave; }
	int getSate()const { return _odrzanoSati; }
	int getStudenta(const char* indeks) {
		if (_brojStudenata <= 0)
			return -1;
		for (int i = 0; i < _brojStudenata; i++)
			if (strcmp(indeks, _prisutni[i].getBrojIndeksa()) == 0)
				return i;
		return -1;
	}
};

	class Predmet {
	char* _naziv; 
	int _godinaStudija; 
	Kolekcija<Nastava, 45> _odrzanaNastava;

	public: 
		Predmet():_naziv(nullptr),_godinaStudija(0){}
		Predmet(const char* naziv, int godinaStudija) : _godinaStudija(godinaStudija), _naziv(AlocirajNizKaraktera(naziv)) {  }
		Predmet(const Predmet& p):_naziv(AlocirajNizKaraktera(p._naziv)),_godinaStudija(p._godinaStudija),_odrzanaNastava(p._odrzanaNastava) {}
		Predmet(Predmet&& p) :_godinaStudija(p._godinaStudija),_odrzanaNastava(p._odrzanaNastava) {
			_naziv = p._naziv;
			p._naziv = nullptr;
		}
	    ~Predmet() { delete[] _naziv;   _naziv = nullptr; }

		Predmet& operator=(const Predmet& p) {
			if (&p == this)
				return *this;
			delete[] _naziv;
			_naziv = AlocirajNizKaraktera(p._naziv);
			_odrzanaNastava = p._odrzanaNastava;
			_godinaStudija = p._godinaStudija;
			return *this;
		}

		//Funkciju za evidenciju održane nastave na predmetu. 
		Predmet& operator+=(const Nastava& n) {
			_odrzanaNastava += n;
			return *this;
		}
		Nastava* operator[](int index) {
			return _odrzanaNastava[index];
		}
		int getBrojNastava()const { return _odrzanaNastava.getTrenutnoElemenata(); }
		int getNastavu(const char* datum, char tip) {
			if (_odrzanaNastava.getTrenutnoElemenata() <= 0)
				return -1;
			for (int i = 0; i < _odrzanaNastava.getTrenutnoElemenata(); i++) {
				if (strcmp(_odrzanaNastava[i]->getDatum(), datum) == 0 && _odrzanaNastava[i]->getTipNastave() == tip)
					return i;
			}
			return -1;
		}
	/*Funkciju koja na osnovu datuma i tipa održane nastave dodaje prisustvo studentu 
	sa proslijedenim brojem indeksa i brojem sati.  Voditi racuna da broj sati prisustva
	ne može biti veci od broja sati održane nastave.*/
		Predmet& operator()(char tipNastave, const char* datum, int brs, const char *brIndeksa) {
			if (brIndeksa == nullptr || brs <= 0 || _odrzanaNastava.getTrenutnoElemenata())
				return *this;
			int index = getNastavu(datum, tipNastave);

			if (index != -1 && _odrzanaNastava[index]->getSate() >= brs)
				(*_odrzanaNastava[index]) += Prisustvo(brIndeksa, brs);

			return *this;
		}

	/*Funkciju koja na osnovu datuma i tipa održane nastave uklanja prisustvo studentu sa proslijedenim brojem indeksa.*/
		Predmet& operator()(const char* datum, char tip, const char* brIndeksa) {
			int index = getNastavu(datum,tip);
			if (index == -1)
				return *this;
			(*_odrzanaNastava[index]) -= brIndeksa;
			return *this;
		}

	/*Funkciju koja na osnovu broja indeksa studenta vraca sve podatke o njegovom prisustvu na nastavi*/
		Prisustvo getStudentaNaNastavi

	/*Funkciju koja na osnovu tipa nastave (P ili V) vrši ispis prisustva za sve studente (sumarno) u datom formatu:  IB140001 30/40 75%, gdje je prva kolona "broj indeksa", druga kolona "broj sati prisustva / ukupan broj sati održane nastave" i  treca kolona "procenat prisustva studenta na nastavi".*/

	friend ostream& operator<<(ostream&, const Predmet&);
	};

	ostream& operator<<(ostream& cout, const Predmet& p) {
		cout << "Predmet: " << p._naziv << endl;  cout << "Godina studija: " << p._godinaStudija << endl;

		return cout;

	}

	int main() {

		system("pause");  return 0;
	}