#include"pch.h"
#include<iostream>
#include<vector>
#include<cstring>
#include<string>
#include<exception>

using namespace std;

string crt = "\n---------------------------------\n";
const char* poruke[] = { "Pitanje ne posjeduje niti jedan odgovor!" };

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1,class T2>
class Dictionary {
	T1* _niz1;
	T2* _niz2;
	int _trenutno;
	void Dealociraj() {
		delete[] _niz1; _niz1 = nullptr;
		delete[] _niz2; _niz2 = nullptr;
	}
	void expandArray() {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _niz1[i];
			temp2[i] = _niz2[i];
		}
		if (_trenutno > 0)
			Dealociraj();
		_niz1 = temp1;
		_niz2 = temp2;
	}
public:
	Dictionary():_niz1(nullptr),_niz2(nullptr),_trenutno(0){}
	~Dictionary() {
		Dealociraj();
	}
	Dictionary(const Dictionary& dc) :_trenutno(dc._trenutno),_niz1(nullptr),_niz2(nullptr) {
		if (_trenutno > 0) {
			_niz1 = new T1[_trenutno];
			_niz2 = new T2[_trenutno];
			for (int i = 0; i < _trenutno; i++)
			{
				_niz1[i] = dc._niz1[i];
				_niz2[i] = dc._niz2[i];
			}
		}
	}

	void Add(const T1& obj1, const T2& obj2) {
		expandArray();
		_niz1[_trenutno] = obj1;
		_niz2[_trenutno++] = obj2;
	}
	int getTrenutno()const { return _trenutno; }
	T1& GetT1(int index) { return _niz1[index]; }
	T2& GetT2(int index) { return _niz2[index]; }


	friend ostream& operator<< <>(ostream&, Dictionary&);
};

template<class T1,class T2>
ostream& operator<< <>(ostream& COUT, Dictionary<T1,T2>& dc) {
	if (dc._niz1 == nullptr || dc._niz2 == nullptr || dc._trenutno <= 0)
		return COUT;
	for (int i = 0; i < dc._trenutno; i++) 
		COUT << i + 1 << ". " << dc._niz1[i] << " - " << dc._niz2[i] << endl;
	return COUT;
}

class Pitanje {
	string _tekst;
	vector<string> _odgovori;
	vector<int> _tacni;
	int _bodova;

public: 
	Pitanje(string tekst = "", int bodovi=0):_bodova(bodovi),_tekst(tekst) {}


	vector<string> GetOdogovori() { return _odgovori; }
	vector<int> GetTacni() { return _tacni; }
	string GetTekst() { return _tekst; }
	int GetBodovi() { return _bodova; }

	int getBrojTacnih()const {
		int br = 0;
		for (int i = 0; i < _tacni.size(); i++)
			if (_tacni[i] == 1)
				br++;
		return br;
	}
	float getOsvojenoBodova(vector<int> odgStudenta) {
		if (odgStudenta.size() == 0 || _tacni.size() == 0)
			return 0;
		float osvojeno = 0, bodovaJedanOdgovor = _bodova / (float)getBrojTacnih();
		for (int i = 0; i < odgStudenta.size(); i++)
			if (_tacni[odgStudenta[i]] == 1)
				osvojeno += bodovaJedanOdgovor;
		return osvojeno;
	}
	void DodajOdgovor(string tekst,int tacan) {
		for (auto o : _odgovori) 
			if (o == tekst) {
				cout << "Odgovor vec postoji..\n";
				return;
			}
		_odgovori.push_back(tekst);
		_tacni.push_back(tacan);
		cout << "Odgovor uspjesno dodan..\n";
	}

	friend ostream& operator<<(ostream&, Pitanje&);
};
ostream& operator<<(ostream& COUT, Pitanje& p) {
	if (p._odgovori.size() == 0)
		throw exception(poruke[0]);
	cout << "Pitanje: " << p._tekst << "\nBodovi: " << p._bodova << "\n";
	for (int i = 0; i < p._odgovori.size(); i++)
		cout << i + 1 << ". " << p._odgovori[i] << endl;
	return COUT;
}

class Ispit {
	string _opis;
	Dictionary<Pitanje, vector<int>> _pitanjaOdgovori;

public:
	Ispit(string opis=""):_opis(opis){}
	Ispit(const Ispit& isp):_pitanjaOdgovori(isp._pitanjaOdgovori),_opis(isp._opis){}

	void DodajPitanje(Pitanje p) {
		_pitanjaOdgovori.Add(p,vector<int>());
	}

	float GetUspjeh() {
		if (_pitanjaOdgovori.getTrenutno() == 0)
			throw exception("Ispit ne posjeduje niti jedno pitanje, a ni odgovor!");
		float osvojeniBodovi = 0;
		int ukupnoBodova = 0;
		for (int i = 0; i < _pitanjaOdgovori.getTrenutno(); i++) {
			ukupnoBodova += _pitanjaOdgovori.GetT1(i).GetBodovi();
			osvojeniBodovi += _pitanjaOdgovori.GetT1(i).getOsvojenoBodova(_pitanjaOdgovori.GetT2(i));
		}
		return (osvojeniBodovi/ukupnoBodova)*100;
	}
	int GetBrojPitanja()const { return _pitanjaOdgovori.getTrenutno(); }
	void DodajOdgovorStudenta(int pitanje, int odgovor) {
		if (pitanje<1 || pitanje> _pitanjaOdgovori.getTrenutno() || odgovor < 1)
			return;
		else if (odgovor > _pitanjaOdgovori.GetT1(pitanje-1).GetTacni().size())
			return;
		_pitanjaOdgovori.GetT2(pitanje-1).push_back(odgovor-1);
	}
};


class Student {

	static int _generatorIndeksa;
	const int _indeks;
	string _imePrezime;
	vector<Ispit> _ispiti;

public:
	Student(string ip=""):_indeks(_generatorIndeksa++),_imePrezime(ip){}
	Student(const Student& s) :_indeks(s._indeks), _imePrezime(s._imePrezime), _ispiti(s._ispiti) {}

	void DodajIspit(Ispit& isp) {
		if (isp.GetBrojPitanja() == 0)
			throw exception("Ne mozete dodati ispit koji ne posjeduje pitanja!");
		_ispiti.push_back(isp);
		cout << "Uspjesno dodan ispit..\n";
	}

	string GetImePrezime()const { return _imePrezime; }

	Ispit& operator[](int index) {
		if (index<0 || index>_ispiti.size()-1)
			throw exception("Proslijedjen pogresan index!\n");
		return _ispiti[index];
	}
};

int Student::_generatorIndeksa = 180000;
void main() {
	try {
		Ispit prIII("PR III 21.01.2019 Redovni");

		Pitanje lokacijaFITa("U kojem gradu se nalazi FIT?", 5);
		lokacijaFITa.DodajOdgovor("Sarajevo", false);
		lokacijaFITa.DodajOdgovor("Mostar", true);
		lokacijaFITa.DodajOdgovor("Tuzla", false);
		lokacijaFITa.DodajOdgovor("Zenica", false);

		Pitanje lokacijaSFFa("U kojem gradu se odrzava SFF?", 5);
		lokacijaSFFa.DodajOdgovor("Sarajevo", true);
		lokacijaSFFa.DodajOdgovor("Mostar", true);
		lokacijaSFFa.DodajOdgovor("Tuzla", false);
		lokacijaSFFa.DodajOdgovor("Zenica", false);

		prIII.DodajPitanje(lokacijaFITa);
		prIII.DodajPitanje(lokacijaSFFa);

		prIII.DodajOdgovorStudenta(1, 2); // PRVI PARAMETAR - > INDEX PITANJA (>0 && <=brPitanja)    ,   DRUGI PARAMETAR - > INDEX ODGOVORA (>0 && <=brOdgovora)
		prIII.DodajOdgovorStudenta(2, 1);
		prIII.DodajOdgovorStudenta(2, 3);
		prIII.DodajOdgovorStudenta(2, 0); //NE MOZE INDEX ODGOVORA BITI 0

		Student s1("Joo Boh");
		s1.DodajIspit(prIII);
		cout << "Student "<<s1.GetImePrezime()<<" je ostvario uspjeh od " << s1[0].GetUspjeh() << " % \n\n";
	}
	catch (exception& obj) {
		cout << "\nERROR: " << obj.what() << endl;
	}

	cin.get();
}