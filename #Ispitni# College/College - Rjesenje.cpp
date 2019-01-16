//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<cstring>
#include<exception>
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
class Kolekcija
{
	T* _elementi[max];//staticki niz pokazivaca
	int _trenutnoElemenata;
	bool _dozvoliDuple;
public:
	Kolekcija(bool dozvoliDuple = false) {
		_trenutnoElemenata = 0;
		for (int i = 0; i < max; i++)
			_elementi[i] = nullptr;
		_dozvoliDuple = dozvoliDuple;
	}

	Kolekcija(const Kolekcija<T, max>& k) {
		_trenutnoElemenata = k._trenutnoElemenata;
		for (int i = 0; i < max; i++)
			if (k._elementi[i] != nullptr)
				_elementi[i] =new T(*k._elementi[i]);
			else
				_elementi[i] = nullptr;
		_dozvoliDuple = k._dozvoliDuple;
	}
	~Kolekcija() {
		if (_trenutnoElemenata > 0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				delete _elementi[i]; _elementi[i] = nullptr;
			}
	}
	int postoji(const T& x)const {
		if (_trenutnoElemenata <= 0)
			return -1;
		for (int i = 0; i < _trenutnoElemenata; i++)
			if (x == *_elementi[i])
				return i;
		return -1;
	}
	T operator[](int index)const {
		if (index<0 || index>_trenutnoElemenata - 1)
			throw exception("Incompatile index\n");
		return *_elementi[index];
	}
	Kolekcija<T, max>& operator+=(const T& element) {
		if (max == _trenutnoElemenata) {
			cout << "Nema vise mjesta..\n";
			return *this;
		}
		if (_trenutnoElemenata > 0)
			if (!_dozvoliDuple)
				for (int i = 0; i < _trenutnoElemenata; i++)
					if (_elementi[i] != nullptr && *_elementi[i] == element)
						return *this;

		_elementi[_trenutnoElemenata++] = new T(element);
		return *this;
	}

	Kolekcija<T, max>& operator-=(int x) {
		if (_trenutnoElemenata <= 0 || x>_trenutnoElemenata-1)
			return *this;
		if (x == _trenutnoElemenata - 1) {
			delete _elementi[x];
			_elementi[x] = nullptr;
			_trenutnoElemenata--;
			return *this;
		}
		delete _elementi[x];
		_elementi[x] = nullptr;
		for (int i = x; i < _trenutnoElemenata - 1; i++)
			_elementi[i] = _elementi[i + 1];
		_trenutnoElemenata--;
		return *this;
	}

	Kolekcija<T, max>& operator=(const Kolekcija<T, max>& k) {
		if (this == &k)
			return *this;
		_trenutnoElemenata = k._trenutnoElemenata;
		_dozvoliDuple = k._dozvoliDuple;
		if (_trenutnoElemenata > 0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				delete[] _elementi[i];
				_elementi[i] =new T(*k._elementi[i]);
			}
		return *this;
	}
	int getTrenutnoElemenata()const { return _trenutnoElemenata; }
	bool moguDupli()const { return _dozvoliDuple; }

	friend ostream& operator<< <>(ostream&, Kolekcija<T, max>&);
	friend bool operator== <>(const Kolekcija<T,max>&, const Kolekcija<T,max>&);
};

template<class T,int max>
ostream& operator<< <>(ostream& COUT, Kolekcija<T, max>& k) {
	cout << "\nTrenutno elemenata: " << k._trenutnoElemenata << endl;
	if (k._trenutnoElemenata > 0)
		for (int i = 0; i < k._trenutnoElemenata; i++)
			cout << *k._elementi[i] << ", ";
	return COUT;
}

template<class T,int max>
bool operator== <>(const Kolekcija<T, max>& k1, const Kolekcija<T, max>& k2) {
	if (k1._trenutnoElemenata == k1._trenutnoElemenata && k1._dozvoliDuple==k2._dozvoliDuple) {
		if (k1._trenutnoElemenata > 0)
			for (int i = 0; i < k1._trenutnoElemenata; i++)
				if (!(*k1._elementi[i] == *k2._elementi[i]))
					return false;
		return true;
	}
	return false;
}

class Prisustvo
{
private:
	char* _brojIndeksa;
	int _brojSati;
public:
	Prisustvo() {
		_brojIndeksa = nullptr;
		_brojSati = 0;
	}
	Prisustvo(const char *brIndeksa,int brs):_brojIndeksa(AlocirajNizKaraktera(brIndeksa)),_brojSati(brs){}
	Prisustvo(const Prisustvo& p) {
		_brojIndeksa = AlocirajNizKaraktera(p._brojIndeksa);
		_brojSati = p._brojSati;
	}
	~Prisustvo() {
		delete[] _brojIndeksa; _brojIndeksa = nullptr;
	}

	Prisustvo& operator+=(int brs) {
		if (brs <= 0)
			return *this;
		_brojSati += brs;
		return *this;
	}
	Prisustvo& operator=(const Prisustvo& p) {
		if (this != &p) {
			delete[] _brojIndeksa;
			_brojIndeksa = AlocirajNizKaraktera(p._brojIndeksa);
			_brojSati = p._brojSati;
		}
		return *this;
	}
	bool valid()const { return !(_brojIndeksa == nullptr || _brojSati < 0); }
	char *getBrojIndeksa()const { return AlocirajNizKaraktera(_brojIndeksa); }
	int getBrojSati()const { return _brojSati; }

	friend ostream& operator<<(ostream&, const Prisustvo&);
	friend bool operator==(const Prisustvo&, const Prisustvo&);
};

ostream& operator<<(ostream& COUT, const Prisustvo& p1) {
	if (!p1.valid())
		return COUT;
	cout << "Broj indeksa: " << p1._brojIndeksa << " - Broj sati prisustva: " << p1._brojSati << endl;
	return COUT;
}
bool operator==(const Prisustvo& p1, const Prisustvo& p2) {
	if (!p1.valid() && !p2.valid())
		return false;
	return strcmp(p1._brojIndeksa, p2._brojIndeksa)==0 && p1._brojSati == p2._brojSati;
}


class Nastava
{
	char _datum[12];
	char _tipNastave; //P ili V
	int _odrzanoSati;
	Kolekcija<Prisustvo, 200> _prisutni;
	int _brojStudenata;
public:
	//Konstruktor sa osnovnim atributima klase
	Nastava() :_odrzanoSati(0), _brojStudenata(0) {}
	
	Nastava(const char *datum, char tipNastave, int sati) :_brojStudenata(0), _odrzanoSati(sati) {
		strncpy_s(_datum, datum, 11);
		if (tipNastave != 'P' && tipNastave != 'V')
			_tipNastave = 'P';
		else
			_tipNastave = tipNastave;
	} 
	Nastava(const Nastava& n) :_brojStudenata(n._brojStudenata), _odrzanoSati(n._odrzanoSati) {
		strncpy_s(_datum, n._datum, 11);
		if (n._tipNastave != 'P' && n._tipNastave != 'V')
			_tipNastave = 'P';
		else
			_tipNastave = n._tipNastave;
		if (_brojStudenata > 0)
			_prisutni = n._prisutni;
	}
	~Nastava() {}
	bool postoji(const Prisustvo& p)const {
		if (_prisutni.postoji(p)>=0)
			return true;
		return false;
	}
	int nadjiStudenta(const char *index) {
		if (index == nullptr || _brojStudenata <= 0)
			return -1;
		int trazeni = -1;
		for (int i = 0; i < _brojStudenata; i++)
			if (strcmp(_prisutni[i].getBrojIndeksa(),index)==0)
				trazeni = i;
		if (trazeni == -1)
			return -1;
		return trazeni;
	}
	Nastava& operator+=(int sati) {
		if (sati <= 0)
			return *this;
		_odrzanoSati += sati;
		return *this;
	}
	Nastava& operator=(const Nastava& n) {
		if (this == &n)
			return *this;
		_prisutni = n._prisutni;
		_brojStudenata = n._brojStudenata;
		_odrzanoSati = n._odrzanoSati;
		if (n._tipNastave != 'P' && n._tipNastave != 'V')
			_tipNastave = 'P';
		else
			_tipNastave = n._tipNastave;
		strncpy_s(_datum, n._datum, 11);
		return *this;
	}

	Nastava& operator+=(const Prisustvo& p) {
		if (_brojStudenata > 0 )
			for (int i = 0; i < _brojStudenata; i++)
				if (_prisutni.postoji(p)>=0)
					return *this;
		_prisutni += p;
		_brojStudenata++;
		return *this;
	}

	Nastava& operator-=(const char *index) {
		int student = nadjiStudenta(index);

		if (student == -1)
			return *this;
		_prisutni -= student;
		_brojStudenata--;
		return *this;
	}
	Nastava& operator-=(int index) {
		if (index<0 || index>_brojStudenata - 1)
			return *this;
		_prisutni -= index;
		_brojStudenata--;
		return *this;
	}
	Prisustvo* operator[](const char *index) {
		int student = nadjiStudenta(index);

		if (student == -1)
			return nullptr;
	
			return &_prisutni[student];
	}
	Nastava& operator()(const char *index, int brojSati) {
		int student = nadjiStudenta(index);

		if (student == -1)
			return *this;
		if (_prisutni[student].getBrojSati() == 0)
			_prisutni[student] += brojSati;

		return *this;
	}
	friend Nastava operator+(const Nastava&,const Nastava&);
	friend ostream& operator<<(ostream&, Nastava&);
	friend bool operator==(const Nastava&, const Nastava&);
	//Destruktor

	//Preklopljeni operator+= za dodavanje objekata tipa Prisustvo u niz _prisutni.
	//Nije moguce dodati više puta prisustvo za istog studenta (preklopiti operator== u klasi Prisustvo).
	//Elementi u svakom momentu treba da budu sortirani po broju indeksa studenta 
		//(preklopiti operator < u klasi Prisustvo).

	//Preklopljeni operator-= za uklanjanje prisustva za studenta sa proslijedjenim brojem indeksa. Nakon uklanjanja,
	//potrebno je sacuvati redoslijed dodavanja, odnosno podaci ostaju sortirani po broju indeksa.

	//Preklopljeni operator [] koji na osnovu broja indeksa provjerava da li je 
	//odredeni student prisustvovao nastavi i vraca njegove podatke o prisustvu.

	//Preklopljeni operator () koji pronalazi studenta sa proslijedenim brojem indeksa i uvecava mu broj sati prisustva
	//za vrijednost drugog parametra. Ako je studentu vec evidentiran odrzani broj sati nastave prekinuti funkciju.

	//Preklopljeni operator + koji zbraja dva objekta tipa Nastava na nacin da kreira novi objekat na osnovu lijevog operanda
	//i dodaje mu prisutne studente desnog operanda. Funkciju napustiti ukoliko objekti koji ucestvuju u operaciji sabiranja
	//nisu istog tipa nastave. Sprijeciti dodavanje prisustva za istog studenta više puta.
	//Operator implementirati kao globalnu funkciju.

};
bool operator==(const Nastava& n1, const Nastava& n2) {
	return (strcmp(n1._datum, n2._datum) == 0 && n1._brojStudenata == n2._brojStudenata && n1._odrzanoSati == n2._odrzanoSati && n1._tipNastave == n2._tipNastave && n1._prisutni == n2._prisutni);
}
ostream& operator<<(ostream& COUT,Nastava& n) {
	cout << "Datum: " << n._datum << endl;
	cout << "Broj studenata koji su prisustvovali: " << n._brojStudenata << endl;
	cout << "Sati ukupno: " << n._odrzanoSati << endl;
	cout << n._prisutni << endl;
	return COUT;
}
Nastava operator+ (const Nastava& n1,const Nastava& n2) {
	
	if (n1._tipNastave != n2._tipNastave)
		throw exception("Different types..\n");
	char temp[12];
	strncpy_s(temp, n1._datum, 11);
	Nastava novi(temp, n1._tipNastave, n1._odrzanoSati);
	
	novi = n1;
	novi += n2._odrzanoSati;
	int pomocna = 0, flag = 0, noviBrojStudenata = n1._brojStudenata;
	if (n2._brojStudenata > 0)
		for (int i = 0; i < n2._brojStudenata; i++)
			if (!novi.postoji(n2._prisutni[i])) {
				novi += n2._prisutni[i];
				novi._brojStudenata++;
			}
	return novi;
}

class Predmet
{
	char* _naziv;
	int _godinaStudija;
	Kolekcija<Nastava, 45> _odrzanaNastava;
public:
	Predmet():_naziv(nullptr),_godinaStudija(0){}
	Predmet(const char *naziv,int gods):_naziv(AlocirajNizKaraktera(naziv)),_godinaStudija(gods){}
	Predmet(const Predmet& p):_naziv(AlocirajNizKaraktera(p._naziv)),_godinaStudija(p._godinaStudija) {
		_odrzanaNastava = p._odrzanaNastava;
	}
	~Predmet() { delete _naziv; _naziv = nullptr;}

	Predmet& operator=(const Predmet& p) {
		if (this == &p)
			return *this;
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(p._naziv);
		_godinaStudija = p._godinaStudija;
		_odrzanaNastava = p._odrzanaNastava;
		return *this;
	}

	Predmet& operator+=(const Nastava& n) {
		if (_odrzanaNastava.postoji(n) < 0)
			_odrzanaNastava += n;
		return *this;
	}
	bool valid()const { return !(_naziv == nullptr || _godinaStudija > 5 || _godinaStudija < 1); }
	char *getNaziv()const { return AlocirajNizKaraktera(_naziv); }
	int getGodina()const { return _godinaStudija; }

	friend ostream& operator<<(ostream&, Predmet&);
	friend bool operator == (const Predmet&, const Predmet&);
};
ostream& operator<<(ostream& COUT,Predmet& p) {
	if (!p.valid())
		return COUT;
	cout << "Naziv predmeta: " << p._naziv << endl;
	cout << "Godina: " << p._godinaStudija << endl;
	cout << "--Odrzana nastava--\n\n" << p._odrzanaNastava << endl;
	return COUT;
}
bool operator==(const Predmet& p1, const Predmet& p2) {
	return strcmp(p1._naziv, p2._naziv) == 0 && p1._godinaStudija == p2._godinaStudija;
}

int main()
{
	Prisustvo p1("2255", 2);
	Prisustvo p2("2014", 3);
	Prisustvo p3("1211", 1);
	Prisustvo p4("1000", 4), p5("1212", 5), p6("1111", 6);

	Nastava n1("01/01/2019", 'V', 12), n2("05/01/2019", 'V', 37);
	n1 += p1;
	n1 += p2;
	n2 += p1;
	n2 += p3;
	n1 += p4;
	n2 += p6;
	n1 += p5;
	n2 += p5;
	Nastava n3 = n1 + n2;
	cout << n3 << endl;
	n3 -= 0;
	n3 -= 3;
	cout << "Nakon uklanjanja\n\n";
	cout << n3 << endl;


	Predmet pr1("Programiranje III", 2), pr2("Algoritmi i strukture podataka", 2), pr3("Statistika i vjerovatnoca", 2);
	pr1 += n1;
	pr1 += n2;
	pr1 += n3;

	cout << pr1 << endl;

	system("pause");
	return 0;
}

