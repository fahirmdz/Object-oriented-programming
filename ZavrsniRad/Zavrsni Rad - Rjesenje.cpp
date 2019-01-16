//fahirmdz GitHub


#include"pch.h"
#include <iostream>
#include <string>
#include <vector>
#include<regex>
#include<thread>
#include<mutex>
#include<exception>
#include<cstring>
#include<cctype>
#include<algorithm>
/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

using namespace std;

const char *crt = "\n-----------------------------------------\n";
enum Oblast { SoftverskiInzinjering, KomunikacijskiSistemi, SigurnostInformacijskihSistema, InteligentniSistemi };
const char* oblast_txt[] = { "Softverski inzinjering", "Komunikacijski sistemi", "Sigurnost informacijskih sistema", "Inteligentni sistemi" };

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T1, class T2, int max>
class Kolekcija
{
	T1 _elementi1[max];
	T2 _elementi2[max];
	int _trenutno;
public:
	Kolekcija():_trenutno(0){}

	void AddElement(const T1& elem1, const T2& elem2)
	{
		if (_trenutno == max)
			throw exception("Dostigli ste maksimalan broj elemenata u kolekciji!");

		for (int i = 0; i < _trenutno; i++){
			if (_elementi1[i] == *const_cast<T1*>(&elem1) || _elementi2[i] == *const_cast<T2*>(&elem2))
				throw exception("Nije moguce dodati duple elemente u kolekciju!");
		}
		_elementi1[_trenutno] = elem1;
		_elementi2[_trenutno++] = elem2;
	}

	T1 * getElementi1Pok() { return _elementi1; }
	T2 * getElementi2Pok() { return _elementi2; }

	T1 & getElement1(int lokacija) {
		if (lokacija < 0 || lokacija >= _trenutno)
			throw exception("Lokacija elementa nije validna!");
		return _elementi1[lokacija]; }

	T2 & getElement2(int lokacija) { 
		if (lokacija < 0 || lokacija >= _trenutno)
			throw exception("Lokacija elementa nije validna!");
		return _elementi2[lokacija]; }

	int getTrenutno() { return _trenutno; }

	friend ostream & operator<<(ostream &, Kolekcija<T1, T2, max> &);
};

template<class T1,class T2,int max>
ostream & operator<<(ostream & COUT, Kolekcija<T1, T2, max> & obj)
{
	for (size_t i = 0; i < obj._trenutno; i++)
		COUT << obj._elementi1[i] << " " << obj._elementi2[i]<< endl;
	return COUT;
}
//Deklaracija klase Nastavnik omogucava njeno koristenje u klasi ZavrsniRad, a definicija je data naknadno 
class Nastavnik;

class ZavrsniRad
{
	char* _nazivTeme;
	Oblast* _oblastTeme;
	string _datumPrijave; //Format: dd.MM.gggg
	vector<Nastavnik*> _komisija; //U vector pohraniti samo adresu objekta tipa Nastavnik, dakle bez alokacije nove memorije
	string _datumOdbrane; //Format: dd.MM.gggg (najranije 2 mjeseca od datuma prijave)
	int _ocjena;

	bool PrijavaURoku(string datum,string datum2) {
		int godina1 = stoi(string(datum.begin() + 6, datum.end())), mjesec1 = stoi(string(datum.begin() + 3, datum.begin() + 5)),
			dan1 = stoi(string(datum.begin(), datum.begin() + 2));

		int godina2 = stoi(string(datum2.begin() + 6, datum2.end())), mjesec2 = stoi(string(datum2.begin() + 3, datum2.begin() + 5)),
			dan2 = stoi(string(datum2.begin(), datum2.begin() + 2));

		long int d1d = 0, d2d = 0;
		auto isPrestupna = [](int g) {
			return g % 400 == 0 && g % 100 == 0 && g % 4 == 0;
		};
		if (isPrestupna(godina1))
			d1d += 366;
		else
			d1d += 365;

		if (isPrestupna(godina2))
			d2d += 366;
		else
			d2d += 365;
		bool d1prestupna = isPrestupna(godina1), d2prestupna = isPrestupna(godina2);
		d1d *= godina1;
		d2d *= godina2;

		auto brojDana = [](int x, bool prestupna) {
			int brd = 0;

			for (int i = 1; i < x; i++)
				if (i == 3 || i == 1 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
					brd += 31;
				else if (i == 2) {
					if (prestupna)
						brd += 29;
					else
						brd += 28;
				}
				else
					brd += 30;
			return brd;
		};
		d1d += brojDana(mjesec1, d1prestupna);
		d2d += brojDana(mjesec2, d2prestupna);
		d1d += dan1;
		d2d += dan1;
		return (d1d - d2d) > 60;
	}
public:
	ZavrsniRad() :_nazivTeme(nullptr), _oblastTeme(nullptr), _datumPrijave("NEMA VRIJEDNOST"), _datumOdbrane("NEMA VRIJEDNOST"), _ocjena(5){ }

	ZavrsniRad(string nazivTeme, Oblast oblastTeme, string datumPrijave){
		
		if (!regex_match(nazivTeme, regex("^[a-zA-Z0-9\\s\\-šđčćžŠĐČĆŽ]*$")))
			throw exception("Naziv teme nije validan!");
		if (!regex_match(datumPrijave, regex("\\d{1,2}[.-/]{1}\\d{1,2}[./-]{1}\\d{4}")))
			throw exception("Datum odbrane nije validan!");
		_nazivTeme = AlocirajNizKaraktera(nazivTeme.c_str());
		_oblastTeme = new Oblast(oblastTeme);
		_datumPrijave = datumPrijave;
		_datumOdbrane = "NIJE ODREDJEN";
		_ocjena = 5;
	
	}
	ZavrsniRad(const ZavrsniRad& zr) :_nazivTeme(AlocirajNizKaraktera(zr._nazivTeme)), _oblastTeme(new Oblast(*zr._oblastTeme)),
		_datumPrijave(zr._datumPrijave), _datumOdbrane(zr._datumOdbrane), _ocjena(zr._ocjena) {
		for (size_t i = 0; i < zr._komisija.size(); i++)
			_komisija.push_back(zr._komisija[i]);
	}
	~ZavrsniRad()
	{
		delete[] _nazivTeme; _nazivTeme = nullptr;
		delete _oblastTeme;	_oblastTeme = nullptr;
		for (size_t i = 0; i < _komisija.size(); i++)
			_komisija[i] = nullptr;
	}

	ZavrsniRad& operator=(const ZavrsniRad& zr) {
		if (&zr == this)
			return *this;
		delete[] _nazivTeme;
		_nazivTeme = AlocirajNizKaraktera(zr._nazivTeme);
		delete _oblastTeme;
		_oblastTeme = new Oblast(*zr._oblastTeme);
		_datumPrijave = zr._datumPrijave;
		_datumOdbrane = zr._datumOdbrane;
		for (size_t i = 0; i < zr._komisija.size(); i++)
			_komisija.push_back(zr._komisija[i]);
		return *this;
	}
	char* GetNazivTeme()const { return _nazivTeme; }
	Oblast GetOblastTeme()const { return *_oblastTeme; }
	string GetDatumOdbrane() { return _datumOdbrane; }
	int GetOcjena() { return _ocjena; }
	bool GetPrijavaURoku(string datumOdbrane){ return PrijavaURoku(datumOdbrane, _datumPrijave); }
	vector<Nastavnik*>& GetKomisija() { return _komisija; };
	bool GetPrijavljen() { return _datumPrijave != "NEMA VRIJEDNOST"; }
	int GetBrojClanovaKomisije() { return _komisija.size(); }

	void SetOcjenu(int ocj) { _ocjena = ocj; }
	void SetDatumOdbrane(string datumOdbrane) { _datumOdbrane = datumOdbrane; }
	void SetKomisiju(vector<Nastavnik*> kom) {
		for (size_t i = 0; i < kom.size(); i++)
			_komisija.push_back(kom[i]);
	}
	friend ostream& operator<<(ostream&, ZavrsniRad&);
	friend bool operator==(ZavrsniRad&, ZavrsniRad&);
};
bool operator==(ZavrsniRad& zr1, ZavrsniRad& zr2) { return strcmp(zr1._nazivTeme, zr2._nazivTeme) == 0 && *zr1._oblastTeme == *zr2._oblastTeme; }
ostream& operator<<(ostream& COUT, ZavrsniRad& r)
{
	COUT << "Tema rada: " << r._nazivTeme << endl;
	COUT << "Oblast teme: " << oblast_txt[*r._oblastTeme] << endl;
	COUT << "Datum prijave rada: " << r._datumPrijave << endl;
	cout << "Datum odbrane rada: " << r._datumOdbrane << endl;
	cout << "Ocjena: " << r._ocjena << endl;
	//Podatke o nastavnicima nije moguce ispisati ovdje jer klasa jos nije definisana
	return COUT;
}
class Nastavnik{
	string _imePrezime;
	Oblast _oblastIzboraUZvanje;
	Kolekcija<string, ZavrsniRad, 10> _teme; 
	//Parametar string predstavlja broj indeksa studenta koji prijavljuje zavrsni rad kod odredjenog nastavnika
public:
	Nastavnik(string imePrezime, Oblast oblastIzboraUZvanje) {
		if (!regex_match(imePrezime, regex("[a-zA-Z\\s]{4,30}")))
			throw exception("Ime i prezime za nastavnika nisu validni!");
		_imePrezime = imePrezime;
		_oblastIzboraUZvanje = oblastIzboraUZvanje;
	}

	int GetBrojRadova() { return _teme.getTrenutno(); }
	string GetImePrezime() { return _imePrezime; }
	Oblast GetOblast() { return _oblastIzboraUZvanje; }
	double GetProsjecnuOcjenu() {
		double prosjecna = 0;
		int brGotovihRadova = 0;
		for (int i = 0; i < _teme.getTrenutno(); i++) {
			ZavrsniRad* temp = &_teme.getElement2(i);
			if (temp->GetDatumOdbrane() != "NEMA VRIJEDNOST"){
				prosjecna += temp->GetOcjena();
				brGotovihRadova++;
			}
			temp = nullptr;
		}

		if (brGotovihRadova == 0)
			return 5;
		return (double)prosjecna / brGotovihRadova;
	}
	string GetIndeksSaNajboljomOcjenom() {
		int max = 0;
		for (int i = 1; i < _teme.getTrenutno(); i++) {
			ZavrsniRad* temp = &_teme.getElement2(i);
			if (temp->GetDatumOdbrane() != "NEMA VRIJEDNOST" && temp->GetOcjena() > _teme.getElement2(max).GetOcjena())
				max = i;
			temp = nullptr;
		}
		return _teme.getElement1(max);
	}

	// Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za
		// dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije. U zavisnosti od 
		// uspjesnosti izvrsenja, funkcija vraca true ili false
	bool operator()(string index, int ocjena) {
		if (ocjena < 5 || ocjena>10)
			return false;
		int indexTeme = GetIndexTeme(index);

		if (indexTeme == -1)
			return false;
		ZavrsniRad* temp = &_teme.getElement2(indexTeme);

		if (temp->GetDatumOdbrane() != "NEMA VRIJEDNOST" && temp->GetBrojClanovaKomisije() > 1) {
			temp->SetOcjenu(ocjena);
			temp = nullptr;
			return true;
		}
		temp = nullptr;
		return false;
	}


	int GetIndexTeme(string brojIndeksa) {
		for (int i = 0; i < _teme.getTrenutno(); i++)
			if (_teme.getElement1(i) == brojIndeksa)
				return i;
		return -1;

	}
	Kolekcija<string, ZavrsniRad, 10>& GetTeme() { return _teme; };

	//Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. 
		//Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) 
		//samo radove iz oblasti za koju posjeduje izbor u zvanje.U slucaju da se nastavniku pokusa dodati 
		//rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom

		//indeks, naslov, oblast, datum prijave 
	void DodajZavrsniRad(string indeks,const char* naslov,Oblast o,string datumPrijave) {
		if (o != _oblastIzboraUZvanje)
			throw exception("Ne mozete dodati zavrsni rad iz oblasti za koju ovaj nastavnik ne posjeduje izbor u zvanje!");
		for (int i = 0; i < _teme.getTrenutno(); i++)
			if (strcmp(_teme.getElement2(i).GetNazivTeme(), naslov) == 0) {
				cout << "Ne mozete dodati rad sa istom temom! Rad sa tom temom vec postoji..\n";
				return;
			}
		_teme.AddElement(indeks, ZavrsniRad(string(naslov), o, datumPrijave));
	}

	    // Funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu 
		// zavrsnog rada sto podrazumijeva definisanje datuma odbrane i liste clanova komisije pred kojima ce 
		// student braniti zavrsni rad. Odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio. 
		// Komisiju trebaju ciniti najmanje 2 nastavnika koji imaju izbor u zvanje u oblasti kojoj pripada tema rada. 
		//  U slucaju da bilo koji od navedenih uslova nije ispunjen funkcija treba	da baci izuzetak
	void ZakaziOdbranuRada(string index,string datumOdbrane,vector<Nastavnik*> komisija) {
		int indexStudenta = GetIndexTeme(index);

		if (indexStudenta == -1)
			throw exception("Student sa datim brojem indeksa nema prijavljen rad!");
		ZavrsniRad* temp = &_teme.getElement2(indexStudenta);
		if (!temp->GetPrijavljen())
			throw exception("Rad nije prijavljen!");

		if (!temp->GetPrijavaURoku(datumOdbrane))
			throw exception("Zakasnili ste sa prijavom! Rade se mora prijaviti minimalno 2 mjeseca prije..");

		int brojNastavnikaOdgovarajucih = 0;
		for (size_t i = 0; i < komisija.size(); i++)
			if (komisija[i]->GetOblast() == temp->GetOblastTeme())
				brojNastavnikaOdgovarajucih++;
		if (brojNastavnikaOdgovarajucih < 2)
			throw exception("Ne mozete zakazati odbranu bez 2 nastavnika koji imaju izbor u zvanje oblasti kojoj pripada tema rada!");
		temp->SetDatumOdbrane(datumOdbrane);
		temp->SetKomisiju(komisija);
		temp = nullptr;
	}
	friend ostream& operator<<(ostream&, Nastavnik&);
};
ostream& operator<<(ostream& COUT, Nastavnik& n) {
	cout << crt << "Ime i prezime: " << n._imePrezime;
	cout << "\nOblast mentorstva: " << oblast_txt[n._oblastIzboraUZvanje] << endl;
	cout <<crt<< "--LISTA MENTOSRTAVA--" << crt;
	for (int i = 0; i < n._teme.getTrenutno(); i++) {
		cout << i + 1 << ". \nBroj indeksa: " << n._teme.getElement1(i) << endl;
		vector<Nastavnik*> temp = n._teme.getElement2(i).GetKomisija();
		cout << n._teme.getElement2(i) << endl;
		cout << "\n--SPISAK CLANOVA KOMISIJE--\n";
		for (size_t j = 0; j < temp.size(); j++)
			cout << j+1<< ". " << temp[j]->GetImePrezime() << endl;
		cout << crt;
	}
	return COUT;
}

//Funkcija PronadjiNajStudenta ima zadatak da pronadje broj indeksa studenta koji je na zavrsnom 
	//radu ostvario najvecu ocjenu kod nastavnika koji posjeduje najvecu prosjecnu ocjenu na zavrsnim radovima.
	//Ukoliko se kod nastavnika sa najvecom prosjecnom ocjenom pojavi vise studenata sa istom ocjenom, 
	//onda funkcija vraca broj indeksa prvog pronadjenog studenta. Svim studentima koji su odbranili 
	//rad kod nastavnika sa najvecom prosjecno ocjenom, u zasebnom thread.u, poslati email poruku 
	//(mail adresa: brojIndeksa@edu.fit.ba) sa sadrzajem da su svoj zavrsni rad uspjesno odbranili sa 
	//vecom ili manjom ocjenom od prosjecne. Ukoliko niti jedan od nastavnika ne posjeduje evidentirano
	//mentorstvo na zavrsnom radu, funkcija vraca tekst: NIJE PRONADJEN
string PronadjiNajStudenta(Nastavnik* nastavnici[],int max) {
	if(max<=0)
		return "NIJE PRONADJEN";
	if (max == 1)
		return nastavnici[0]->GetIndeksSaNajboljomOcjenom();

	bool flag = false;
	for (int i = 0; i < max; i++)
		if (nastavnici[i]->GetBrojRadova() > 0)
			flag = true;
	if (!flag)
		return "NIJE PRONADJEN";

	double *prosjecneNastavnici = new double[max] {0.0};
	for (int i = 0; i < max; i++) 
		prosjecneNastavnici[i] = nastavnici[i]->GetProsjecnuOcjenu();
	int najvecu = 0;

	for (int i = 1; i < max; i++)
		if (prosjecneNastavnici[i] > prosjecneNastavnici[najvecu])
			najvecu = i;
	delete[] prosjecneNastavnici;
	prosjecneNastavnici = nullptr;

	return nastavnici[najvecu]->GetIndeksSaNajboljomOcjenom();

}
void main()
{
	const int max = 5;
	Nastavnik* nastavnici[max];

	nastavnici[0] = new Nastavnik("Denis Music", SoftverskiInzinjering);
	nastavnici[1] = new Nastavnik("Zanin Vejzovic", KomunikacijskiSistemi);
	nastavnici[4] = new Nastavnik("Adil Joldic", KomunikacijskiSistemi);
	nastavnici[2] = new Nastavnik("Jasmin Azemovic", SigurnostInformacijskihSistema);
	nastavnici[3] = new Nastavnik("Emina Junuz", SoftverskiInzinjering);
	try
	{
		//Funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. 
		//Sprijeciti dodavanje zavrsnih radova sa istom temom vise puta. Nastavnik moze imati (mentorisati) 
		//samo radove iz oblasti za koju posjeduje izbor u zvanje.U slucaju da se nastavniku pokusa dodati 
		//rad koji nije iz njegove oblasti funkcija treba da baci izuzetak sa odgovarajucom porukom

		//indeks, naslov, oblast, datum prijave 
		nastavnici[0]->DodajZavrsniRad("IB130011", "Multimedijalni informacijski sistem za visoko-obrazovnu ustanovu", SoftverskiInzinjering, "01.04.2017");
		nastavnici[0]->DodajZavrsniRad("IB120051", "Sistem za podršku rada kablovskog operatera", SoftverskiInzinjering, "03.03.2017");

		nastavnici[1]->DodajZavrsniRad("IB140102", "Praktična analiza sigurnosti bežičnih računarskih mreža", KomunikacijskiSistemi, "22.08.2017");

		nastavnici[2]->DodajZavrsniRad("IB140002", "Primjena teorije informacija u procesu generisanja kriptografskih ključeva", SigurnostInformacijskihSistema, "10.09.2017");

		vector<Nastavnik*> komisija;//formira se komisija
		komisija.push_back(nastavnici[0]);
		komisija.push_back(nastavnici[1]);
		komisija.push_back(nastavnici[2]);
		komisija.push_back(nastavnici[3]);

		vector<Nastavnik*> komisija2;
		komisija2.push_back(nastavnici[1]);
		komisija2.push_back(nastavnici[4]);
		komisija2.push_back(nastavnici[2]);

		
		nastavnici[0]->ZakaziOdbranuRada("IB130011", "25.09.2017", komisija);
		nastavnici[0]->ZakaziOdbranuRada("IB120051", "25.09.2017", komisija);

		nastavnici[1]->DodajZavrsniRad("IB160262", "Razvoj 5G mreze", KomunikacijskiSistemi, "20.06.2017");
		nastavnici[1]->DodajZavrsniRad("IB160200", "Qualcomm", KomunikacijskiSistemi, "20.06.2017");

		nastavnici[1]->ZakaziOdbranuRada("IB160262", "25.08.2017",komisija2);
		nastavnici[1]->ZakaziOdbranuRada("IB160200", "25.08.2017",komisija2);
		nastavnici[1]->ZakaziOdbranuRada("IB140102", "25.01.2018", komisija2);


		if ((*nastavnici[1])("IB160262", 9))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl; 
		if ((*nastavnici[1])("IB160200", 10))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl; 
		if ((*nastavnici[1])("IB140102", 8))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;
		
		try {
			nastavnici[0]->ZakaziOdbranuRada("IB130111", "25.09.2017", komisija);//student sa brojem indeksa IB130111 
		}                                                                         //jos uvijek nije prijavio rad
		catch (exception& obj) {
			cout << "GRESKA -> " << obj.what() << endl;
		}														 

		// Studentu sa brojem indeksa IB130011 dodjeljuje ocjenu 8 na zavrsnom radu. Uslov za
		// dodjelu ocjene je da student posjeduje definisan datum odbrane i listu clanova komisije. U zavisnosti od 
		// uspjesnosti izvrsenja, funkcija vraca true ili false

		if ((*nastavnici[0])("IB130011", 6))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;
		if ((*nastavnici[0])("IB120051", 10))
			cout << "Uspjesno ste ocijenili zavrsni rad!" << endl;

		// Ispisuje sve podatke o nastavniku i njegovim mentorstvima. Za clanove komisije je dovoljno 
		// ispisati samo ime i prezime.
		cout << *nastavnici[0] << endl;

		cout << *nastavnici[1] << endl;

		cout << "Najsupjesniji student: " << PronadjiNajStudenta(nastavnici, max) << endl;
		try {
			//Baca izuzetak zbog neadekvatnog izbora u zvanje, odnosno oblasti
			nastavnici[2]->DodajZavrsniRad("IB150008", "Razvoj sistema autentifikacije na osnovu biometrije glasa", InteligentniSistemi, "15.05.2017");
		}
		catch (exception& obj) {
			cout << "GRESKA -> " << obj.what() << endl;
		}
	}
	catch (exception& ex)
	{
		cout << "GRESKA -> " << ex.what() << endl;
	}

	for (int i = 0; i < max; i++)
	{
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
}