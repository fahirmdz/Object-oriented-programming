#include"pch.h"
#include<iostream> 
#include<string>
#include<regex>
#include<cstring>
#include<exception>
#include<cctype>
#include<cstdlib>
using namespace std;

const char* crt = "\n---------------------------------------\n";
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
//Koristiti klasu NedozvoljenaOperacija za bacanje izuzetaka u cijelom programu 
class NedozvoljenaOperacija : public exception {
	int _linijaKoda;

public:
	NedozvoljenaOperacija(const char* poruka, int linijaKoda) : exception(poruka) { _linijaKoda = linijaKoda; }

	const char* what()const {
		string obj = (string)exception::what() +" : Line: "+ to_string(_linijaKoda);

		char* temp = new char[100];
		strcpy_s(temp, 100, obj.c_str());
		return temp;
	}
	void Info() {
		cout << "Greska u liniji koda " << _linijaKoda << " : " << exception::what() << endl;
	}
};
template<class T1, class T2> 
class FITKolekcija {
T1* _elementi1; 
T2* _elementi2;
int _trenutno; 

public:
	//Konstruktor, destruktor i ostale funkcije potrebne za rad sa klasom
	FITKolekcija() :_trenutno(0) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	FITKolekcija(const FITKolekcija& fk) :_trenutno(fk._trenutno) {
		if (_trenutno > 0) {
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (int i = 0; i < _trenutno; i++) {
				_elementi1[i] = fk._elementi1[i];
				_elementi2[i] = fk._elementi2[i];
			}
		}
		else {
			_elementi1 = nullptr;
			_elementi2 = nullptr;
		}
	}
	~FITKolekcija() {
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}

	FITKolekcija& operator=(const FITKolekcija& fk) {
		if (&fk == this)
			return *this;

		delete[] _elementi1;
		delete[] _elementi2;

		_trenutno = fk._trenutno;
		if (_trenutno > 0) {
			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];
			for (int i = 0; i < _trenutno; i++) {
				_elementi1[i] = fk._elementi1[i];
				_elementi2[i] = fk._elementi2[i];
			}
		}
		else {
			_elementi1 = nullptr;
			_elementi2 = nullptr;
		}
		return *this;
	}

	FITKolekcija& operator()(T1& el1, T2& el2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (int i = 0; i < _trenutno; i++) {
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno++] = el2;
		return *this;

	}
	int GetTrenutno()const { return _trenutno; }
	T1& GetT1(int index) {
		if (index<0 || index>_trenutno - 1)
			throw NedozvoljenaOperacija("Index za za element iz kolekcija nije validan!", __LINE__);
		return _elementi1[index];
	}
	T2& GetT2(int index) {
		if (index<0 || index>_trenutno - 1)
			throw NedozvoljenaOperacija("Index za za element iz kolekcija nije validan!", __LINE__);
		return _elementi2[index];
	}

	friend bool operator==<>(FITKolekcija&, FITKolekcija&);
};
template<class T1,class T2>
bool operator==<>(FITKolekcija<T1, T2>& fk1, FITKolekcija<T1, T2>& fk2) {
	if (fk1._trenutno == fk2._trenutno) {
		for (int i = 0; i < fk1._trenutno; i++)
			if (!(fk1._elementi1[i] == fk2._elementi1[i]) || !(fk1._elementi2[i] == fk2._elementi2[i]))
				return false;
		return true;
	}
	return false;

}

	
enum Kategorija { GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge };
const char* KategorijeChar[] = { "Game Challenge","Innovation Challenge","Mobile Challenge","SQL Challenge","Coding Challenge" };

//Klasu proglasiti apstraktnom 
class Osoba abstract {
protected:
	char* _ime;
	char* _prezime;
	int _godiste;

public: 

	Osoba():_ime(nullptr),_prezime(nullptr),_godiste(0){}
	Osoba(const char* ime, const char* prezime, int godiste) {
		if (ime == nullptr || prezime == nullptr || strlen(ime) < 2 || strlen(prezime) < 2 || godiste > 2019 || godiste < 1910 ||
			!regex_match((string)ime + string(prezime), regex("^[a-zA-Z]{4,30}$")))
			throw NedozvoljenaOperacija("Parametri za osobu nisu validni!", __LINE__);
		_ime = AlocirajNizKaraktera(ime);
		_prezime = AlocirajNizKaraktera(prezime);
		_godiste = godiste;
	}
	Osoba(const Osoba& o) :_ime(AlocirajNizKaraktera(o._ime)),_prezime(AlocirajNizKaraktera(o._prezime)),_godiste(o._godiste){}
	virtual ~Osoba() {
		delete[] _ime; _ime = nullptr;
		delete[] _prezime; _prezime = nullptr;
	}

	Osoba& operator=(const Osoba& o) {
		if (&o == this)
			return *this;

		delete[] _ime;
		delete[] _prezime;
		_ime = AlocirajNizKaraktera(o._ime);
		_prezime = AlocirajNizKaraktera(o._prezime);
		_godiste = o._godiste;
		return *this;
	}

	char* GetIme()const { return _ime; }
	char* GetPrezime()const { return _prezime; }
	int GetGodiste()const { return _godiste; }

	virtual void Info() {
		cout << "Ime i prezime: " << _ime << " " << _prezime << endl;
		cout << "Godiste: " << _godiste << endl;
	}
	friend bool operator==(Osoba&, Osoba&);
};
bool operator==(Osoba& o1, Osoba& o2) {
	return strcmp(o1._ime, o2._ime) == 0 && strcmp(o1._prezime, o2._prezime) == 0;
}


//Prepoznati nasljedjivanje izmedju odgovarajućih klasa
//I učesnici i članovi komisije trebaju imati podatke za pristup sistemu (email i password)
class Ucesnik :public Osoba {
protected:
	char* _email;
	string password; //Najmanje 5 karaktera, kombinacija brojeva i velikih slova (dopuniti sa regex-ima)
	string _obrazovnaInstitucija;
public:
	Ucesnik():_email(nullptr),password(""),_obrazovnaInstitucija(""){}
	Ucesnik(const char* ime, const char* prezime, int godiste,const char* email,string pw,string institucija):Osoba(ime,prezime,godiste){
		if (email == nullptr || !regex_match(string(email), regex("\\w*\.\\w*\\@(live|hotmail|yahoo|gmail).com")))
			throw NedozvoljenaOperacija("Email za ucesnika nije validan! format(xxxx.xxxx@live.com)", __LINE__);

		if(!regex_match(pw, regex("^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9]).{6,}$")))
			throw NedozvoljenaOperacija("Lozinka za ucesnika nije validna!", __LINE__);
		
		if (!regex_match(institucija, regex("^[a-zA-Z\\s]+$")))
			throw NedozvoljenaOperacija("Naziv obrazovne institucije nije validan!", __LINE__);
		_email = AlocirajNizKaraktera(email);
		password = pw;
		_obrazovnaInstitucija = institucija;
	}
	Ucesnik(const Ucesnik& u):_email(AlocirajNizKaraktera(u._email)),password(u.password),_obrazovnaInstitucija(u._obrazovnaInstitucija) {}
	~Ucesnik() { delete[] _email; _email = nullptr; }

	Ucesnik& operator=(const Ucesnik& u) {
		if (&u == this)
			return *this;
		Osoba::operator=(u);
		delete[] _email;
		_email = AlocirajNizKaraktera(u._email);
		password = u.password;
		_obrazovnaInstitucija = u._obrazovnaInstitucija;
		return *this;
	}

	char* GetEmail()const { return _email; }
	string GetPassword()const { return password; }
	string GetInstitucija()const { return _obrazovnaInstitucija; }
	virtual void Info() {
		Osoba::Info();
		cout << "Email: " << _email << endl;
	}
	friend bool operator==(Ucesnik&, Ucesnik&);

};
bool operator==(Ucesnik& u1, Ucesnik& u2) {
	return *dynamic_cast<Osoba*>(&u1) == *dynamic_cast<Osoba*>(&u2) && strcmp(u1._email, u2._email) == 0;
}



class ClanKomisije :public Ucesnik {
	char* _pozicija; //Radno mjesto u firmi 
	string _firma;

public:
	//Potrebne funkcije za rad sa klasom 
	ClanKomisije():_pozicija(nullptr),_firma(""){}
	ClanKomisije(const char* ime, const char* prezime, int godiste, const char* email, string pw, 
		string institucija,const char* pozicija,string firma):Ucesnik(ime,prezime,godiste,email,pw,institucija),
		_pozicija(AlocirajNizKaraktera(_pozicija)),_firma(firma){
		if (pozicija == nullptr || !regex_match(string(pozicija), regex("^[a-zA-Z\\s]+$")))
			throw NedozvoljenaOperacija("Naziv pozicije ne smije sadrzati nista osim slova!", __LINE__);
		if (!regex_match(firma, regex("^[a-zA-Z0-9\\s]+$")))
			throw NedozvoljenaOperacija("Naziv firma ne smije sadrzati nista osim slova i brojeva", __LINE__);
		_firma = firma;
		_pozicija = AlocirajNizKaraktera(pozicija);

	}
	ClanKomisije(const ClanKomisije& ck):Ucesnik(ck._ime,ck._prezime,ck._godiste,ck._email,ck.password,ck._obrazovnaInstitucija),
		_pozicija(AlocirajNizKaraktera(ck._pozicija)),_firma(ck._firma){}
	~ClanKomisije() {
		delete[] _pozicija; _pozicija = nullptr;
	}

	ClanKomisije& operator=(ClanKomisije& ck) {
		delete[] _pozicija;
		Ucesnik::operator=(ck);
		_firma = ck._firma;
		_pozicija = AlocirajNizKaraktera(ck._pozicija);

	}

	char* GetPozicija()const { return _pozicija; }
	string GetFirma()const { return _firma; }

	void Info() {
		Ucesnik::Info();
		cout << "Pozicija: " << _pozicija << endl;
		cout << "Firma: " << _firma << crt;
	}
	friend bool operator==(ClanKomisije&, ClanKomisije&);
};
bool operator==(ClanKomisije& ck1, ClanKomisije& ck2) {
	return *dynamic_cast<Ucesnik*>(&ck1) == *dynamic_cast<Ucesnik*>(&ck2) && strcmp(ck1._pozicija, ck2._pozicija) == 0;
}




//Prijave koje ne uključuju izradu i predaju projekata (kategorije: SQLChallenge i CodingChallenge)
class Prijava {
protected:
	string _datum; //Podržani format: dd.MM.yyyy (dopuniti sa regex-ima)
	Kategorija _kategorija;
	FITKolekcija<Ucesnik, bool> _tim; //Tim se sastoji od jednog ili više učesnika, drugi parametar označava predstavnika tima 

public:
	//Konstruktor
	Prijava():_datum(""){}
	Prijava(string datum, Kategorija category) {
		if (!regex_match(datum, regex("\\d{2}\\.\\d{2}\\.\\d{4}")))
			throw NedozvoljenaOperacija("Format datuma nije validan!", __LINE__);
		_datum = datum;
		_kategorija = category;
	}

	string GetDatum()const { return _datum; }
	Kategorija GetKategoriju()const { return _kategorija; }

	//Funkciju za dodavanje članova tima uz poštivanje ograničenja maksimalno dozvoljenog broja članova u timu. 
	//SQLChallenge i CodingChallenge: 2 
	//GameChallenge i MobileChallenge: 3 
	//InnovationChallenge: 4 
	//Onemogućiti dodavanje istog učesnika više puta. Tim može imati najviše jednog predstavnika.
		//GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge

	void DodajClana(Ucesnik& u, bool predstavnik) {
		if (((_kategorija == 3 || _kategorija == 4) && _tim.GetTrenutno() == 2) || ((_kategorija == 0 || _kategorija == 2) && _tim.GetTrenutno() == 3) ||
			((_kategorija == 1 && _tim.GetTrenutno() == 4)))
		{
			cout << "Maximalan broj clanova dodat! Ne mozete dodati vise clanova..\n";
			return;
		}

		if (predstavnik) {
			for (int i = 0; i < _tim.GetTrenutno(); i++)
				if (_tim.GetT2(i))
					throw NedozvoljenaOperacija("Vec postoji predstavnik tima!", __LINE__);
		}
		for (int i = 0; i < _tim.GetTrenutno(); i++)
			if (_tim.GetT1(i) == u)
				throw NedozvoljenaOperacija("Ne mozete dodati istog ucesnika vise puta!", __LINE__);
		_tim(u, predstavnik);
	}

	//Funkciju za promjenu predstavnika tima.
	void PromjenaPredstavnika(Ucesnik& u) {
		int ucesnik = -1;
		for (int i = 0; i < _tim.GetTrenutno(); i++)
			if (_tim.GetT1(i) == u)
				ucesnik = i;
			
		if (ucesnik==-1)
			throw NedozvoljenaOperacija("Ucesnik nije clan ovog tima!", __LINE__);

		bool imaPredstavnik = false;
		for (int i = 0; i < _tim.GetTrenutno(); i++)
			if (_tim.GetT2(i)) {
				_tim.GetT2(i) = false;
				imaPredstavnik = true;
			}

		if (!imaPredstavnik)
			throw NedozvoljenaOperacija("Ne postoji predstavnik u timu, jos uvijek!", __LINE__);

		_tim.GetT2(ucesnik) = true;
	}

	virtual void Info() {
		cout << crt << "::::PRIJAVA INFO::::" << crt;
		cout << "Datum: " << _datum << endl;
		cout << "Kategorija: " << KategorijeChar[_kategorija] << endl;
		cout << "--Sastav tima--" << crt;
		if (_tim.GetTrenutno() > 0) {
			for (int i = 0; i < _tim.GetTrenutno(); i++) {
				cout << i + 1 << ". clan ---> " << _tim.GetT1(i).GetIme() << " " << _tim.GetT1(i).GetPrezime();
				if (_tim.GetT2(i))
					cout << "\t\t **PREDSTAVNIK TIMA**\n";
				else cout << "\n";
			}
		}
		else
			cout << "--TRENUTNO NEMA CLANOVA--\n";
		cout << crt;
	}
	friend bool operator==(Prijava&, Prijava&);
};
bool operator==(Prijava& p1, Prijava& p2) {
	return p1._datum == p2._datum && p1._kategorija == p2._kategorija && p1._tim == p2._tim;
}


//Prijave koje uključuju izradu projekata (kategorije: GameChallenge, InnovationChallenge i MobileChallenge)
class PrijavaProjekta : public Prijava {
	char* _nazivProjekta; 
	string _opisProjekta; 
	bool _finalizirano; //Atribut označava da li je projekat završen i predan u predviđenom roku
    bool _prezentacija; //Projekti koji se prezentuju i ocjenjuju 

public: 
//Konstruktor i destruktor
	PrijavaProjekta():_nazivProjekta(nullptr),_finalizirano(false),_prezentacija(false),_opisProjekta(""){}
	PrijavaProjekta(string datum, Kategorija category,const char* naziv,string opis,bool finalizirano,bool prezentacija):Prijava(datum,category),
	 _finalizirano(finalizirano),_prezentacija(prezentacija){
		if (category == 3 || category == 4)
			throw NedozvoljenaOperacija("Kategorija nije podrzana!", __LINE__);
		if (naziv == nullptr || !regex_match(string(naziv), regex("^[a-zA-Z\\s]+$")))
			throw NedozvoljenaOperacija("Naziv projekta ne smije sadrzati nista osim slova", __LINE__);
		_nazivProjekta = AlocirajNizKaraktera(naziv);
		_kategorija = category;
	//	if(!regex_search(_opistProjekta),regex("[a-zA-z0-9\\s]+"))
	}
	PrijavaProjekta(const PrijavaProjekta& pp) :Prijava(pp._datum, pp._kategorija), _nazivProjekta(AlocirajNizKaraktera(pp._nazivProjekta)),
		_opisProjekta(pp._opisProjekta), _finalizirano(pp._finalizirano), _prezentacija(pp._prezentacija) {}
	~PrijavaProjekta() { delete[] _nazivProjekta; _nazivProjekta = nullptr; }

	PrijavaProjekta& operator=(const PrijavaProjekta& pp) {
		if (&pp == this)
			return *this;
		delete[] _nazivProjekta;
		_nazivProjekta = AlocirajNizKaraktera(pp._nazivProjekta);
		_opisProjekta = pp._opisProjekta;
		_finalizirano = pp._finalizirano;
		_prezentacija = pp._prezentacija;
		return *this;
	}

	void Info() {
		Prijava::Info();
		cout << "***DODATNE INFORMACIJE***\n";
		cout << "Naziv projekta: " << _nazivProjekta << endl;
		cout << "Opis projekta: " << _opisProjekta;
		cout << "\nFinalizirano: ";
		if (_finalizirano)
			cout << "DA\n";
		else
			cout << "NE\n";
		cout << "Prezentuje se: ";
		if (_prezentacija)
			cout << "DA\n";
		else
			cout << "NE\n";

	}
	friend bool operator==(PrijavaProjekta&, PrijavaProjekta&);
};
bool operator==(PrijavaProjekta& pp1, PrijavaProjekta& pp2) {
	return *dynamic_cast<Prijava*>(&pp1) == *dynamic_cast<Prijava*>(&pp2) && strcmp(pp1._nazivProjekta, pp2._nazivProjekta) == 0;
}


class FITCodingChallenge {
	int godina;
	string _prijaveDo; //Podržani format unosa: dd.MM.yyyy (dopuniti sa regex-ima)
vector<Prijava*> _prijave; 
FITKolekcija<ClanKomisije, Kategorija> _komisija;
FITKolekcija<Prijava*, FITKolekcija<ClanKomisije*, double>> _ocjene; 

public: 
//Konstruktor
//Operator += za dodavanje novih prijava. Isti učesnik ne može imati više prijava u sklopu iste kategorije.
//Poštovati ograničenje roka za prijave (_prijaveDo). 
//Voditi računa o alokaciji memorije za prijave (ne zadržavati adresu proslijeđenog objekta)


//Funkciju koja određenu prijavu označava kao finaliziranu i odabranu za prezentaciju ili ne.


//Operator () za dodavanje članova komisije po kategorijama. Spriječiti dodavanje istih članova u sklopu jedne kategorije.


//Funkciju za ispis svih prijava po kategorijama. Funkcija neobavezno prima parametar kategoriju. 
//Ispis treba da uključi i podatke o učesnicima, odnosno članovima tima. 
//Ukoliko kategorija nije proslijeđena, podaci treba da budu grupisani po kategorijama.


//Funkciju za ocjenu projekata (radova) od strane članova komisije (kolekcija _ocjene). 
//Kolekcija sadrzi samo pokazivače (bez alokacije nove memorije) na postojeće prijave i vec definisane članove komisije. 
//Voditi računa da se ne mogu ocijeniti projekti koji nisu finalizirani i nisu odabrani za prezentaciju, niti da ocjenu mogu dati članovi 
//komisije koji nisu pridruženi odgovarajućoj kategoriji prijave. Svaki član komisije daje ocjenu za svaki projekat, odnosno rad, u 
//kategoriji gdje je dodijeljen. Ocjene se kreću u rasponu od 1 do 10 za projekte, a za radove u kategoriji SQL ili CodingChallenge 
//u rasponu osvojenih bodova od 0 do 100. Pretpostaviti da su svi učesnici prijavljeni u kategorijama SQL i CodingChallenge predali radove.


//Funkcija koja na osnovu imena i prezimena člana komisije ispisuje podatke o njegovim ocjenama uključujući 
//prijave na koje se one odnose.


//Funkcija koja na osnovu imena i prezimena učesnika ispisuje podatke o njegovim prijavama na takmičenju, zajedno sa 
//ocjenama, ukoliko su one definisane za prijavu/e datog učesnika.


//Funkciju za rangiranje kandidata na osnovu kategorije proslijeđene kao parametar. 
//Funkcija treba da na osnovu ocjena članova komisije ispiše rang liste timova zajedno sa prosječnim ocjenama njihovih projekata (radova).
//Dakle, konačna rang lista uključuje nazive timova (u zavisnosti od kategorije i naziv projekta), te prosječnu ocjenu.


//Funkciju Info koja ispisuje sve podatke o takmičenju.
};

int main() { 
	
	//Kreirati objekat klase FITCodingChallenge, te demonstrirati sve implementirane funkcionalnosti.
	
	try {
		Osoba* ucesnici[5];
		ucesnici[0] = new Ucesnik("John", "Newman", 1998, "john.newman@gmail.com", "John1333", "Fakultet Informacijskih Tehnologija");
		ucesnici[1] = new Ucesnik("Mel", "Straus", 1992, "john.newman@live.com", "Mel44A", "Fakultet Informacijskih Tehnologija");
		ucesnici[2] = new Ucesnik("Nelly", "Newman", 1991, "john.newman@live.com", "Nels32", "Fakultet Informacijskih Tehnologija");
		ucesnici[3] = new Ucesnik("Ahmed", "Abdul", 1993, "john.newman@live.com", "Ambud1A2", "Fakultet Informacijskih Tehnologija");
		ucesnici[4] = new Ucesnik("Muhammed", "Sag", 1994, "john.newman@live.com", "Asd2cS...", "Fakultet Informacijskih Tehnologija");

		cout << crt << ":::UCESNICI::" << crt;
		for (int i = 0; i < 5; i++) {
			ucesnici[i]->Info();
			cout << crt;
		}


		Osoba* komisija[3];
		komisija[0] = new ClanKomisije("Adem", "Mert", 1975, "adem.mert@yahoo.com", "MertAd112", "Fakultet Informacijskih Tehnologija", "Profesor", "FIT");
		komisija[1] = new ClanKomisije("Sally", "Gourmet", 1975, "sally.gourmet@gmail.com", "Sal.12", "Fakultet Informacijskih Tehnologija", "Mentor", "Atlant");
		komisija[2] = new ClanKomisije("Salmin", "Brag", 1975, "salmin.b@live.com", "SBrag.12", "Fakultet Informacijskih Tehnologija", "Asistent", "Authority");

		cout << endl << crt << ":::CLANOVI KOMISIJE::" << crt;
		for (int i = 0; i < 3; i++)
			komisija[i]->Info();


		//GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge
		Prijava* prijave[4];
		prijave[0] = new PrijavaProjekta("01.02.2019", MobileChallenge, "Mobilna aplikacija", "Aplikacija za kontrolu kucnih uredjaja IoT", false, false);
		prijave[1] = new Prijava("05.07.2019", CodingChallenge);
		prijave[2] = new Prijava("21.09.2019", SQLChallenge);
		prijave[3] = new PrijavaProjekta("19.12.2019", GameChallenge, "Strategijska igrica", "Osvajanje kula i jacanje vojske", false, true);

		prijave[0]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[0]), false);
		prijave[0]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[1]), true);
		prijave[0]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[2]), false);

		/*Oznacava kao predstavnika ucesnika koji je proslijedjen kao parametar, ukoliko ga ima. Ukoliko ga nema ili nema predstavnika tima
		funkcija u oba slucaja baca izuzetak.*/
		prijave[0]->PromjenaPredstavnika(*dynamic_cast<Ucesnik*>(ucesnici[0]));

		prijave[1]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[1]), false);
		prijave[1]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[2]), true);
		prijave[1]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[3]), false); //NECE DOUPUSTITI, JER JE MAX 2 na CodingChallenge projektu
		prijave[1]->DodajClana(*dynamic_cast<Ucesnik*>(ucesnici[4]), false); //NECE DOPUSTITI, JER JE MAX 2 na CodingChallenge projektu


		for (int i = 0; i < 4; i++)
			prijave[i]->Info();



		//=========DEALOKACIJA UCESNIKA=============
		for (int i = 0; i < 5; i++) {
			delete ucesnici[i];
			ucesnici[i] = nullptr;
		}
		//=================================

		//===========DEALOKACIJA CLANOVA KOMISIJE============
		for (int i = 0; i < 3; i++) {
			delete komisija[i];
			komisija[i] = nullptr;
		}
		//==================================
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
	}

	cin.get();
	system("pause");
	return 0;
}