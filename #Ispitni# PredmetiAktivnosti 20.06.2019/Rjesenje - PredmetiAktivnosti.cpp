//fahirmdz GitHub

#include<iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<iomanip>
#include<regex>
#include<exception>
#include<algorithm>
#include<windows.h>
#include<thread>
#include<vector>
#include<mutex>
#include<fstream>

using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";
const char* crt = "\n--------------------------------------------\n";
const char* stars = "\n********************************************\n";

enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
enum Razred { I = 1, II, III, IV };
const char* razredChar[] = { "I","II","III","IV" };
const char* predmetChar[] = { "Matematika", "Fizika", "Hemija", "Geografija", "NOT_SET" };

char* AlocirajNizKaraktera(const char* sadrzaj) {
	
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	if (sadrzaj == nullptr)
		strcpy_s(temp,vel, "NOT_SET");
	else
	  strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;

	void sort() {
		for (int i = 0; i < _trenutnoElemenata - 1; i++)
			for (int j = i + 1; j < _trenutnoElemenata; j++) {
				if (_sortiranje == ASC && (*_elementi1[i]) > (*_elementi1[j])) {
					swap(*_elementi1[i], *_elementi1[j]);
					swap(*_elementi2[i], *_elementi2[j]);
				}
				else if (_sortiranje == DESC && (*_elementi1[i]) < (*_elementi1[j])) {
					swap(*_elementi1[i], *_elementi1[j]);
					swap(*_elementi2[i], *_elementi2[j]);
				}
			}
	}

	void copyElements(const Kolekcija& k) {
		for (int i = 0; i < k._trenutnoElemenata; i++)
			if (k._elementi1[i] != nullptr && k._elementi2[i] != nullptr) {
				_elementi1[i] = new T1(*k._elementi1[i]);
				_elementi2[i] = new T2(*k._elementi2[i]);
			}

	}
public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	Kolekcija(const Kolekcija& k) :_trenutnoElemenata(k._trenutnoElemenata),_sortiranje(k._sortiranje),_dozvoliDupliranje(k._dozvoliDupliranje){
		copyElements(k);
	}

	~Kolekcija() {
		for (int i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	Kolekcija& operator=(Kolekcija& k) {
		if (&k == this)
			return *this;

		_trenutnoElemenata = k._trenutnoElemenata;
		for (int i = 0; i < max; i++) {
			delete _elementi1[i];
			delete _elementi2[i];
		}
		copyElements(k);
		return *this;
	}


	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}

	bool AddElement(const T1& ele1,const T2& ele2)
	{
		if (_trenutnoElemenata == max)
			throw exception("Dostignut maximum\n");
		_elementi1[_trenutnoElemenata] = new T1(ele1);
		_elementi2[_trenutnoElemenata++] = new T2(ele2);
		if (_sortiranje && _trenutnoElemenata > 1)
			sort();
		return true;
	}


	friend ostream& operator<< <>(ostream&, const Kolekcija&);
};

template<class T1,class T2,int max>
ostream& operator<< <>(ostream& COUT, const Kolekcija<T1,T2,max>& obj) {
	for (int i = 0; i < obj.GetTrenutno(); i++)
		COUT << obj.GetElement1(i) << " - " << obj.GetElement2(i) << endl;
	return COUT;
}


class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 1, string opis = not_set) {
		if (ocjena < 1 || ocjena>5)
			throw exception("Invalid format _ocjena in Aktivnost");
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}

	__declspec(property(get = GetOcjenu))const int Ocjena;
	__declspec(property(get = GetOpis))const string Opis;
	__declspec(property(get = GetRazred))const Razred razred;

	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }


	friend ostream& operator<<(ostream&, const Aktivnost&);
	friend bool operator==(const Aktivnost&, const Aktivnost&);
};
ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
	COUT <<"\nRazred: "<< razredChar[*obj._razred-1] << "\nOcjena: " << obj._ocjena << "\nOpis: " << obj._opis << endl;
	return COUT;
}
bool operator==(const Aktivnost& ak1, const Aktivnost& ak2) {
	return *ak1._razred == *ak2._razred && ak1._opis == ak2._opis && ak1._ocjena == ak2._ocjena;
}

class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik(string imePrezime = "NOT_SET", string brojTelefona = "NOT_SET") {
		if (imePrezime == "NOT_SET" || imePrezime == "" || brojTelefona == "" )
			throw exception("Invalid _imePrezime or _brojTelefona");
		regex reg("^\\+{1}\\d{3}[\\s\\(]{1}\\d{2}[\\s\)]{1}\\d{3}\\-{1}\\d{3}");
		if (!regex_match(brojTelefona, reg))
			brojTelefona = "NOT_SET";
		_imePrezime = AlocirajNizKaraktera(imePrezime.c_str());
		_brojTelefona = brojTelefona;
	}
	Polaznik(const Polaznik& p) :_imePrezime(AlocirajNizKaraktera(p._imePrezime)), _brojTelefona(p._brojTelefona) {}
	~Polaznik() { delete[] _imePrezime; }

	Polaznik& operator=(const Polaznik& p) {
		if(&p == this)
			return *this;
		delete[] _imePrezime;
		_imePrezime = AlocirajNizKaraktera(p._imePrezime);
		_brojTelefona = p._brojTelefona;
		return *this;
	}

	//---Getters---
	__declspec(property(get = GetImePrezime))const char* ImePrezime;
	__declspec(property(get = GetTelefon))string BrojTelefona;

	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _brojTelefona; }
	//------------

	virtual void PredstaviSe() = 0;
};


class Ucenik:public Polaznik  {
	Kolekcija<Predmet, Aktivnost, 16> * _aktivnosti;
public:
	Ucenik(string imePrezime = "NOT_SET", string brojTelefona = "NOT_SET") :Polaznik(imePrezime, brojTelefona) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
	}
	Ucenik(const Ucenik& u) :Polaznik(u._imePrezime, u._brojTelefona),_aktivnosti(new Kolekcija<Predmet,Aktivnost,16>(*u._aktivnosti)) {}

	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }

	Ucenik& operator=(const Ucenik& u) {
		if (&u == this)
			return *this;
		delete _aktivnosti;
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*u._aktivnosti);
		Polaznik::operator=(u);
		return *this;
	}

	Ucenik& operator()(Predmet predmet, const Aktivnost& ak) {
		_aktivnosti->AddElement(predmet, ak);
		return *this;
	}


	//---Getters---
	__declspec(property(get = GetAktivnosti))Kolekcija<Predmet, Aktivnost, 16> & Aktivnosti;
	Kolekcija<Predmet, Aktivnost, 16> & GetAktivnosti() { return *_aktivnosti; };
	//-------------
	
	double GetProsjecnuOcjenu() {
		double ocj = 0;
		for (int i = 0; i < _aktivnosti->GetTrenutno(); i++)
			ocj += _aktivnosti->GetElement2(i).Ocjena;
		return ocj / _aktivnosti->GetTrenutno();
	}

	virtual void PredstaviSe() {
		cout << "\nIme i prezime: " << Polaznik::_imePrezime;
		cout << "\nBroj telefona: " << Polaznik::_brojTelefona;
		cout << "\n--LISTA AKTIVNOSTI--"<<crt;
		if (_aktivnosti->GetTrenutno() == 0)
			cout << "-->TRENUTNO NEMA AKTIVNOSTI<--" << crt;
		else {
			for (int i = 0; i < _aktivnosti->GetTrenutno(); i++) {
				cout << "Predmet: " << predmetChar[_aktivnosti->GetElement1(i)];
				cout << _aktivnosti->GetElement2(i) << endl << endl;
			}
		}
	}
	friend ostream& operator<<(ostream&, Ucenik&);
};
ostream& operator<<(ostream& COUT, Ucenik& n){
	n.PredstaviSe();
	return COUT;
}

mutex mut;
class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;

	void sendSMS(string imePrezime, Razred razred, double ocj) {

		cout << crt << "----> SENDING SMS TO " << imePrezime << "..." << crt;
		this_thread::sleep_for(chrono::seconds(3));
		cout << "Uspjesno ste okoncali aktivnosti u okviru " << razredChar[razred - 1] << " razreda sa prosjecnom ocjenom " << setprecision(2)<<ocj << crt << "--FIT Mostar--" << crt;
		this_thread::sleep_for(chrono::seconds(2));

	}
public:
	Skola(const char* naziv = nullptr){
		if (naziv == nullptr || strcmp(naziv, " ") == 0)
			throw exception("Invalid school name");
		_naziv = AlocirajNizKaraktera(naziv);
	}
	Skola(const Skola& sk): _naziv(AlocirajNizKaraktera(sk._naziv)),_ucenici(sk._ucenici){}

	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}
	

	Skola& operator=(const Skola& sk) {
		if (&sk == this)
			return *this;
		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(sk._naziv);
		_ucenici = sk._ucenici;
		return *this;
	}

	Skola& operator()(string imePrezime,string brojTelefona) {
		for (auto&& x : _ucenici)
			if (x.ImePrezime == imePrezime)
				throw exception("Ucenik sa istim imenom i prezimenom vec postoji..");
			else if (x.BrojTelefona == brojTelefona)
				throw exception("Ucenik sa istim brojem telefona vec postoji..");

		_ucenici.push_back(Ucenik(imePrezime, brojTelefona));
		return *this;
	}

	//---Getters---
	__declspec(property(get = GetNaziv))const char* Naziv;
	__declspec(property(get = GetUcenici))vector<Ucenik>& Ucenici;

	vector<Ucenik>& GetUcenici() { return _ucenici; };
	char* GetNaziv()const { return _naziv; }
	//------------

	Ucenik* GetUcenik(string imePrezime) {
		for (size_t i = 0; i < _ucenici.size(); i++)
			if (_ucenici[i].ImePrezime == imePrezime)
				return &_ucenici[i];
		return nullptr;
	}

	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti,
	a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	bool DodajAktivnost(string imePrezime, Predmet predmet, const Aktivnost& aktivnost) {

		Ucenik& ucenik = *GetUcenik(imePrezime);
		double prosjecnaOcjena = 0;

		//--PROVJERE--
		if (GetUcenik(imePrezime) == nullptr)
			return false;

		int flagCounter = 0;
		for (int i = 0; i < ucenik.Aktivnosti.GetTrenutno(); i++) {
			if (aktivnost.razred == ucenik.Aktivnosti.GetElement2(i).razred) {
				flagCounter++;
				prosjecnaOcjena += ucenik.Aktivnosti.GetElement2(i).Ocjena;

				if (predmet == ucenik.Aktivnosti.GetElement1(i))
					return false;
			}
		}
		if (flagCounter == 4)
			return false;
		//------
		prosjecnaOcjena /= flagCounter;

		ucenik(predmet, aktivnost);

		if (flagCounter == 3 && prosjecnaOcjena > 1 && ucenik.BrojTelefona!="NOT_SET") {
			mut.lock();
			thread t(&Skola::sendSMS,this,imePrezime, aktivnost.razred, prosjecnaOcjena);
			t.join();
			mut.unlock();
		}

		return true;
	}

	pair<Polaznik*, float> GetNajboljegUcenika() {
		vector<double> ocjene;
		for (size_t i = 0; i < _ucenici.size(); i++)
			ocjene.push_back(_ucenici[i].GetProsjecnuOcjenu());
		int best = 0;
		for (size_t i = 1; i < ocjene.size(); i++)
			if (ocjene[i] > ocjene[best])
				best = i;

		return make_pair<Polaznik*, float>(&_ucenici[best], (float)ocjene[best]);
	}

	bool SpasiUFajl(string text, bool deleteContent=false) {
		ofstream write;
		if (deleteContent == false)
			write.open("Gimnazija.txt");
		else
			write.open("Gimnazija.txt",ios::out, ios::trunc);
		if (!write.fail()) {
			write << stars << "Skola: " << _naziv << "\n";
			write << "---> Ucenici: " << stars;
			for (size_t i = 0; i < _ucenici.size(); i++) {
				write << "\n" << i + 1 << ".  " << crt;
				write << "\nIme i prezime: " << _ucenici[i].ImePrezime;
				write << "\nBroj telefona: " << _ucenici[i].BrojTelefona;
				write << "\n--LISTA AKTIVNOSTI--" << crt;
				Kolekcija<Predmet, Aktivnost, 16> & Aktivnosti = _ucenici[i].Aktivnosti;
				if (Aktivnosti.GetTrenutno() == 0)
					write << "-->TRENUTNO NEMA AKTIVNOSTI<--" << crt;
				else {
					for (int i = 0; i < Aktivnosti.GetTrenutno(); i++) {
						write << "Predmet: " << predmetChar[Aktivnosti.GetElement1(i)];
						Aktivnost& tempAktivnost = Aktivnosti.GetElement2(i);
						write << "\nRazred: " << razredChar[tempAktivnost.razred - 1] << "\nOcjena: " << tempAktivnost.Ocjena << "\nOpis: " << tempAktivnost.Opis << endl;
					}
				}
			}

			write.close();
		}
		else {
			return false;
		}

		ifstream read("Gimnazija.txt", ios::_Nocreate);
		if (!read.fail()) {
			string STRING;
			while (!read.eof()) // To get you all the lines.
			{
				getline(read, STRING); // Saves the line in STRING.
				cout << STRING; // Prints our STRING.
			}
			read.close();
			return true;
		}
		return false;
	}

	friend ostream& operator<<(ostream&, Skola& );
};
ostream& operator<<(ostream& COUT, Skola& obj) {
	COUT << stars<<"Skola: " << obj._naziv << endl;
	COUT << "---> Ucenici: " << stars;
	for (size_t i = 0; i < obj._ucenici.size(); i++)
		COUT <<endl<<i+1<<".  "<<crt<< obj._ucenici[i] << endl;
	return COUT;
}

int main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);

	}
	catch (exception& ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (int i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());

	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception& ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 613333-444");
	cout << gimnazijaMostar << endl;

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti,
	a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda,
	te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti
	u okviru X razreda sa prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;

	/*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj 
	fajla prethodno biti pobrisan*/

	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;

	cin.get();
	system("pause>0");
	return 0;
}
