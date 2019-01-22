#include"pch.h"
#include <iostream>
#include<cstring>
#include<string>
#include<cctype>
#include<vector>
#include<exception>
#include<regex>
#include<future>
#include<ctime>
using namespace std;

const char *crt = "\n---------------------------------------\n";

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

class DatumVrijeme{
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;

	int GetBrojDanaMjesec(int m, int g) {
		int i = m;
		if (i == 3 || i == 1 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
			return 31;
		else if (i == 2) {
			if (g % 400 == 0 && g % 100 == 0 && g % 4 == 0)
				return  29;
			else
				return 28;
		}
		else
			return 30;
	}
	bool validneVrijednosti(int dan, int mjesec, int godina, int sat, int minut) {
		struct tm timeinfo;
		time_t t = time(NULL);
		localtime_s(&timeinfo, &t);
		int trenutniDan = timeinfo.tm_mday, trenutniMjesec = timeinfo.tm_mon + 1, trenutnaGodina = timeinfo.tm_year + 1900, trenutniSat = timeinfo.tm_hour,
			trenutniMinut = timeinfo.tm_min + 1;
		if (dan<1 || dan>GetBrojDanaMjesec(mjesec, godina))
			return false;
		else if (mjesec < 1 || mjesec>12)
			return false;
		else if (godina<1920 || godina>trenutnaGodina)
			return false;

		if (godina == trenutnaGodina)
			if (mjesec > trenutniMjesec)
				return false;
			else if (mjesec == trenutniMjesec && dan > trenutniDan)
				return false;
			else if (dan == trenutniDan && sat > trenutniSat)
				return false;
			else if (sat == trenutniSat && minut > trenutniMinut)
				return false;
		return true;
	}
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0){
		if (!validneVrijednosti(dan, mjesec, godina, sati, minuti))
			throw exception("Datum nije validan!");
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	DatumVrijeme(const DatumVrijeme& dv):_dan(new int(*dv._dan)),_mjesec(new int(*dv._mjesec)),_godina(new int(*dv._godina)),
		_minuti(new int(*dv._minuti)),_sati(new int(*dv._sati)){}
	~DatumVrijeme(){
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}

	DatumVrijeme& operator=(const DatumVrijeme& dv) {
		if (&dv == this)
			return *this;
		delete _dan; _dan = new int(*dv._dan);
		delete _mjesec; _mjesec = new int(*dv._mjesec);
		delete _godina; _godina = new int(*dv._godina);
		delete _sati; _sati = new int(*dv._sati);
		delete _minuti; _minuti = new int(*dv._minuti);
		return *this;
	}

	int dateEqual(DatumVrijeme& d) {
		if (*d._godina > *_godina)
			return 1;
		else if (*d._godina < *_godina)
			return -1;
		else if (*d._mjesec > *_mjesec)
			return 1;
		else if (*d._mjesec < *_mjesec)
			return -1;
		else if (*d._dan > *_dan)
			return 1;
		else if (*d._dan < *_dan)
			return -1;
		else if (*d._sati > *_sati)
			return 1;
		else if (*d._sati < *_sati)
			return -1;
		else
			return 0;
	}
	friend bool operator== (DatumVrijeme&, DatumVrijeme&);
	friend bool operator!= (DatumVrijeme&, DatumVrijeme&);
	friend ostream& operator<< (ostream&, DatumVrijeme&);
};
bool operator==(DatumVrijeme& dv1, DatumVrijeme& dv2) {
	return *dv1._godina == *dv2._godina && *dv1._mjesec == *dv2._mjesec && *dv1._dan == *dv2._dan && *dv1._sati == *dv2._sati && *dv1._minuti == *dv2._minuti;
}
bool operator!=(DatumVrijeme& dv1, DatumVrijeme& dv2) {return !(d1==d2);}

ostream& operator<< (ostream &COUT, DatumVrijeme &obj){
	COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
	return COUT;
}


template<class T1, class T2 = int>
class FITKolekcija{
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutno;

	void copyElements(const FITKolekcija& fk) {
		if (_trenutno > 0) {
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (int i = 0; i < fk._trenutno; i++) {
				_elementi1[i] = fk._elementi1[i];
				_elementi2[i] = fk._elementi2[i];
			}
		}
		else {
			_elementi1 = nullptr;
			_elementi2 = nullptr;
		}
	}
public:
	FITKolekcija(){
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& fk) :_trenutno(fk._trenutno) {
		copyElements(fk);
	}
	~FITKolekcija(){
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}

	FITKolekcija& operator=(const FITKolekcija& fk) {
		if (&fk == this)
			return *this;
		_trenutno = fk._trenutno;
		delete[] _elementi1;
		delete[] _elementi2;
		copyElements(fk);
		return *this;
	}
	void Dodaj(T1& t1, T2& t2) {
		T1* temp = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		if(_trenutno>0)
			for (int i = 0; i < _trenutno; i++){
				temp[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp;
		_elementi2 = temp2;
		_elementi1[_trenutno] = t1;
		_elementi2[_trenutno++] = t2;
		temp = nullptr;
		temp2 = nullptr;
	}
	void Ukloni(T2& t2) {
		for(int i=0;i<_trenutno;i++)
			if (t2 == _elementi2[i]) {
				for (int j = i; j < _trenutno - 1; j++) {
					_elementi1[j] = _elementi1[j + 1];
					_elementi2[j] = _elementi2[j + 1];
				}
				_trenutno--;
				return;
			}
	}
	void Sortiraj() {
		for (int i = 0; i < _trenutno - 1; i++) {
			int min = i;

			for (int j = i + 1; j < _trenutno; j++)
				if (_elementi1[j] < _elementi1[min])
					min = j;
			if (min != i) {
				swap(_elementi1[min], _elementi1[i]);
				swap(_elementi2[min], _elementi2[i]);
			}
		}
	}

	void DodajNaLokaciju(T1& t1,T2& t2,int index) {
		if (index < 0 || index >= _trenutno)
			throw exception("Index lokacije nije validan!");
		Dodaj(t1, t2);
		swap(_elementi1[_trenutno - 1], _elementi1[index]);
		swap(_elementi2[_trenutno - 1], _elementi2[index]);
	}

	FITKolekcija<int, int> * GetJedinstveni() {
		FITKolekcija<int, int>* unikatna = new FITKolekcija<int, int>;
		
		for (int i = 0; i < _trenutno; i++) {
			bool flag = false;
			for (int j = 0; j < unikatna->GetTrenutno(); j++)
				if (_elementi1[i] == unikatna->GetT1()[j] && _elementi2[i] == unikatna->GetT2()[j])
					flag = true;
			if (!flag)
				unikatna->Dodaj(_elementi1[i], _elementi2[i]);
		}
		return unikatna;
	}
	T1 * GetT1() { return _elementi1; }
	T2 * GetT2() { return _elementi2; }

	int GetTrenutno() { return _trenutno; }

	friend ostream& operator<< <>(ostream &, FITKolekcija &);
};

template<class T1,class T2=int>
ostream& operator<< <>(ostream &COUT, FITKolekcija<T1,T2> &obj)
{
	for (size_t i = 0; i < obj._trenutno; i++)
		COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
	return COUT;
}

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char* vrstaDokumentaChar[] = { "PDF","DOC","TXT","HTML" };

class Dokument{
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument():_naziv(""),_sadrzaj(""){ _brojStranica = 0; }
	Dokument(vrstaDokumenta vrsta, string naziv){
		if (!regex_match(naziv, regex("^[a-zA-Z.]{5,15}$")))
			throw exception("Naziv dokumenta nije validan!");
		_sadrzaj = "";
		_naziv = naziv;
		_vrsta = vrsta;
		_brojStranica = 0;
	}
	string GetNaziv() { return _naziv; };
	string GetNazivBezEkstenzije() {
		int x = 0;
		for (int i = 0; i < _naziv.length(); i++)
			if (_naziv[i] == '.')
				x = i;
		return string(_naziv.begin(), _naziv.begin() + x);
	}
	string GetEkstenzija() {
		int x = 0;
		for (int i = 0; i < _naziv.length(); i++)
			if (_naziv[i] == '.')
				x = i;
		string str(_naziv.begin() + x+1, _naziv.end());
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }

	//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO
	//DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
	void DodajSadrzaj(const char* sadrzaj) {
		if (sadrzaj == nullptr)
			return;
		_sadrzaj += "\n" + string(sadrzaj);
		double brs = (double)_sadrzaj.length() / 30;

		_brojStranica = brs;
		if (brs - (int)brs > 0)
			_brojStranica++;
	}
		

	friend ostream & operator<<(ostream&,Dokument&);
	friend bool operator==(Dokument&, Dokument&);
};
ostream & operator<<(ostream&COUT, Dokument & obj){
	COUT << crt << "Naziv: " << obj._naziv << "\nBroj stranica: " << obj._brojStranica <<crt<< obj._sadrzaj << crt;
	return COUT;
}
bool operator==(Dokument& d1, Dokument& d2) { return d1._brojStranica == d2._brojStranica && d1._naziv == d2._naziv && d1._sadrzaj == d2._sadrzaj; }
class Uredjaj abstract{
protected:
	char * _proizvodjac;
	char * _model;
public:
	Uredjaj(const char * proizvodjac,const char * model){
		regex rgx("^[a-zA-Z0-9\\s-.]{2,15}$");
		if (!regex_match(proizvodjac, rgx))
			throw exception("Naziv proizvodjaca nije validan!");
		if(!regex_match(model,rgx))
			throw exception("Naziv modela nije validan!");
		_proizvodjac = AlocirajNizKaraktera(proizvodjac);
		_model = AlocirajNizKaraktera(model);
	}
	Uredjaj(const Uredjaj& u):_proizvodjac(AlocirajNizKaraktera(u._proizvodjac)),_model(AlocirajNizKaraktera(u._model)){}
	~Uredjaj(){
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	Uredjaj& operator=(const Uredjaj& u) {
		if (&u == this)
			return *this;
		delete[] _proizvodjac; _proizvodjac = AlocirajNizKaraktera(u._proizvodjac);
		delete[] _model; _model = AlocirajNizKaraktera(u._model);
		return *this;
	}

	char* GetProizvodjac()const { return _proizvodjac; }
	char* GetModel()const { return _model; }

	friend ostream & operator<<(ostream&,Uredjaj&);
};

ostream & operator<<(ostream&COUT, Uredjaj & obj){
	COUT << crt << "Proizvodjac: " << obj._proizvodjac << "\nModel: " << obj._model << endl;
	return COUT;
}


class Printer:public Uredjaj{
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
	bool uslovPrintanja(Dokument& dokument) {
		string ekstenzija = dokument.GetEkstenzija();

		bool flag = false;
		for (int i = 0; i < 4; i++)
			if (ekstenzija == vrstaDokumentaChar[i])
				flag = true;
		if (!flag)
			return false;

		if (!regex_match(dokument.GetNazivBezEkstenzije(), regex("[a-zA-Z]{5,15}")))
			return false;

		//2. pravilo 
		for (size_t i = 0; i < _zabranjeneRijeci.size(); i++)
			if (regex_search(dokument.GetSadrzaj(), regex("(" + _zabranjeneRijeci[i] + ")")))
				return false;
		return true;
	}
public:
	Printer(const char * proizvodjac, const char * model):Uredjaj(proizvodjac,model){}


	FITKolekcija<DatumVrijeme, Dokument> & GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string> & GetZabranjeneRijeci() { return _zabranjeneRijeci; };

	void DodajZabranjenuRijec(const char* zabranjenaR) {
		if (zabranjenaR == nullptr)
			return;
		string zabranjena = zabranjenaR;
		transform(zabranjena.begin(), zabranjena.end(), zabranjena.begin(), ::toupper);
		for (size_t i = 0; i < _zabranjeneRijeci.size(); i++)
			if (_zabranjeneRijeci[i] == zabranjena) {
				cout << "Ne mozete dodati istu zabranjenu rijec vise puta!\n";
				return;
			}
		_zabranjeneRijeci.push_back(zabranjena);
	}
	// DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
			// 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
			// NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
			// BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
			// 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
			// UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
			// ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
			// UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
			// SADRŽAJ DOKUMENTA KOJI SE PRINTA

	void Printaj(DatumVrijeme& d,Dokument& dokument){
		//1. pravilo "PDF","DOC","TXT","HTML"
		_printaniDokumenti.Dodaj(d, dokument);
		if (uslovPrintanja(dokument)) {
			cout << crt << "Printam dokument " << dokument.GetNaziv() << ".. (LOADING)" << crt;
			this_thread::sleep_for(chrono::seconds(2));
			cout << dokument.GetSadrzaj() << crt;
		}
		else
			throw exception("Dokument se nece prinatati, jer nisu zadovoljeni uslovi printanja!\n");

	}

	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme& dv) {
		int brs = 0, brd = 0;
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
			if (uslovPrintanja(_printaniDokumenti.GetT2()[i]) && _printaniDokumenti.GetT1()[i].dateEqual(dv) == 0) {
				brs += _printaniDokumenti.GetT2()[i].GetBrojStranica();
				brd++;
			}

		double razlika = (double)brs / brd;
		if (razlika - (int)razlika > 0)
			return razlika + 1;
		else
			return razlika;
	}

	string GetTopZabranjenuRijec() {
		int brojRijeci = (int)_zabranjeneRijeci.size();
		int *brojac = new int[brojRijeci] {0};
		for (int i = 0; i < brojRijeci; i++) 
			for (int j = 0; j < _printaniDokumenti.GetTrenutno(); j++) {
				string tempSadrzaj = _printaniDokumenti.GetT2()[j].GetSadrzaj();
				transform(tempSadrzaj.begin(), tempSadrzaj.end(), tempSadrzaj.begin(), tolower);
				string tempZabranjena = _zabranjeneRijeci[i];
				transform(tempZabranjena.begin(), tempZabranjena.end(), tempZabranjena.begin(), tolower);
				size_t tempIndex = tempSadrzaj.find(tempZabranjena);
				if (tempIndex != string::npos)
					brojac[i]++;
				while (tempIndex!=string::npos) {
					tempIndex = tempSadrzaj.find(tempZabranjena);
					if (tempIndex != string::npos) {
						brojac[i]++;
						tempIndex = tempSadrzaj.find(tempZabranjena, tempIndex + tempZabranjena.size());
					}
				}
			}
		
		int topZabranjena = -1;
		bool svenule = true;
		for (int i = 1; i < brojRijeci; i++) {
			if (brojac[i] != 0)
				svenule = false;
			if (brojac[i] > brojac[topZabranjena])
				topZabranjena = i;
		}
		delete[] brojac;
		brojac = nullptr;
		if (svenule)
			return "NIJEDNA ZABRANJENA RIJEC NIJE KORISTENA!";
		return _zabranjeneRijeci[topZabranjena];
	}

	void UkloniDokumente() {
		for (int i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
			if (!uslovPrintanja(_printaniDokumenti.GetT2()[i]))
				_printaniDokumenti.Ukloni(_printaniDokumenti.GetT2()[i]);
	}

	friend ostream& operator<<(ostream&, Printer&);
};
ostream& operator<<(ostream& COUT, Printer& p) {
	cout << crt << "::::PRINTER INFO::::" << crt;
	cout << *dynamic_cast<Uredjaj*>(&p);
	cout << crt << "--Dokumenti--" << crt;
	cout << p._printaniDokumenti << endl;
	cout << crt << "--ZABRANJENE RIJECI--" << crt;
	for (size_t i = 0; i < p._zabranjeneRijeci.size(); i++)
		cout << i + 1 << ". " << p._zabranjeneRijeci[i] << endl;
	cout << crt;
	return COUT;
}


void main(){

	try {
		DatumVrijeme danas(19,1,2019,19,19); //obezbijedjena validacija da datum ne moze biti u buducnosti i ne moze sadrzati vrijednosti koje nisu validne
		DatumVrijeme sutra(danas);
		DatumVrijeme prekosutra;
		prekosutra = danas;
		cout << danas << endl << sutra << endl << prekosutra << crt;
	
		int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

		FITKolekcija<int, int> brojevi;
		brojevi.Dodaj(v196, v6);
		brojevi.Dodaj(v13, v32);
		brojevi.Dodaj(v98, v196);
		brojevi.Dodaj(v63, v13);
		brojevi.Dodaj(v98, v196);
		brojevi.Dodaj(v196, v6);

		cout << brojevi << crt;
		//SORTIRANJE ÈLANOVA KOLEKCIJE SE VRŠI U RASTUÆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
		brojevi.Sortiraj();
		cout << brojevi << crt;
		//BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
		brojevi.DodajNaLokaciju(v109, v6, 2);
		cout << brojevi << crt;
		brojevi.Sortiraj();
		cout << brojevi << crt;
		// METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE 
		// (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U 
		// OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.
		FITKolekcija<int, int> * jedinstveni = brojevi.GetJedinstveni();
		cout << *jedinstveni << crt;
		*jedinstveni = brojevi;
		cout << *jedinstveni << crt;

		Dokument ispitPRIII(DOC, "ispitPRIII.doc");
		//BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO
		//DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
		ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO ");
		cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
		Dokument ispitBaze(DOC, "ispitBaze.doc");
		ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SERAT TAKODJER NALAZI U FAJLU CODE.TXT");
		cout << "Broj stranica -> " << ispitBaze.GetBrojStranica() << endl;

		cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

		Printer hp3200("HP", "3200");
		//PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI
		hp3200.DodajZabranjenuRijec("RE");// :)
		hp3200.DodajZabranjenuRijec("RAT");
		hp3200.DodajZabranjenuRijec("UBITI");
		hp3200.DodajZabranjenuRijec("RE");// ONEMOGUÆITI PONAVLJANJE ZABRANJENIH RIJEÈI, KORISTITI ITERATORE
		hp3200.DodajZabranjenuRijec("MRZITI");
		try
		{
			// DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
			// 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
			// NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
			// BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
			// 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
			// UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
			// ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
			// UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
			// SADRŽAJ DOKUMENTA KOJI SE PRINTA
	
			hp3200.Printaj(danas, ispitPRIII);  //--> BACA IZUZETAK, JER SADRZI ZABRANJENE RIJECI
			hp3200.Printaj(danas, ispitBaze);
	
	
			// IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
			// VODITI RAÈUNA O DIJELJENJU ZAJEDNIÈKIH RESURSA
		}
		catch (exception& err)
		{
			cout << err.what() << endl;
		}
	
		//KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
		cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
		cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;
	
		Printer hp4000(hp3200);
		cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
		cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
		hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
		cout << hp4000 << crt;
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}

}
