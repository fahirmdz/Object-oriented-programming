#include<iostream> 
#include<string>
#include<regex>
using namespace std;

//Koristiti klasu NedozvoljenaOperacija za bacanje izuzetaka u cijelom programu 
class NedozvoljenaOperacija : public exception {
int _linijaKoda;

public: NedozvoljenaOperacija(const char* poruka, int linijaKoda) : exception(poruka) { _linijaKoda = linijaKoda; }

void Info() { 
	cout << "Greska u liniji koda " << _linijaKoda << ": " << exception::what() << endl;
} 
};
template<class T1, class T2> class FITKolekcija {
T1* _elementi1; 
T2* _elementi2;
int _trenutno; 

public:
	//Konstruktor, destruktor i ostale funkcije potrebne za rad sa klasom
};
	
enum Kategorija { GameChallenge, InnovationChallenge, MobileChallenge, SQLChallenge, CodingChallenge };

//Klasu proglasiti apstraktnom 
class Osoba { 
char* _ime;
char* _prezime; 
int _godiste; 

public: Osoba(const char* ime, const char* prezime, int godiste) {
	int vel = strlen(ime) + 1; 
	_ime = new char[vel];
	strcpy_s(_ime, vel, ime);

	vel = strlen(prezime) + 1; 
	_prezime = new char[vel]; 
	strcpy_s(_prezime, vel, prezime);

	_godiste = godiste;
}
~Osoba() {
	delete[] _ime; _ime = nullptr;
	delete[] _prezime; _prezime = nullptr;
}
};

//Prepoznati nasljedjivanje izmedju odgovarajucih klasa
//I ucesnici i clanovi komisije trebaju imati podatke za pristup sistemu (email i password)
class Ucesnik { 
	char* _email; 
	string password; //Najmanje 5 karaktera, kombinacija brojeva i velikih slova (dopuniti sa regex-ima)
    string _obrazovnaInstitucija;

public:
	//Potrebne funkcije za rad sa klasom

};
class ClanKomisije {
	char* _pozicija; //Radno mjesto u firmi 
string _firma; 

public:
	//Potrebne funkcije za rad sa klasom 
};

//Prijave koje ne ukljucuju izradu i predaju projekata (kategorije: SQLChallenge i CodingChallenge)
class Prijava {
	string _datum; //Podržani format: dd.MM.yyyy (dopuniti sa regex-ima)
	Kategorija _kategorija;
	FITKolekcija<Ucesnik, bool> _tim; //Tim se sastoji od jednog ili više ucesnika, drugi parametar oznacava predstavnika tima 
public: 
//Konstruktor
//Funkciju za dodavanje clanova tima uz poštivanje ogranicenja maksimalno dozvoljenog broja clanova u timu. 
//SQLChallenge i CodingChallenge: 2 
//GameChallenge i MobileChallenge: 3 
//InnovationChallenge: 4 
//Onemoguciti dodavanje istog ucesnika više puta. Tim može imati najviše jednog predstavnika.
//Funkciju za promjenu predstavnika tima.
};

//Prijave koje ukljucuju izradu projekata (kategorije: GameChallenge, InnovationChallenge i MobileChallenge)
class PrijavaProjekta : public Prijava {
	char* _nazivProjekta; 
	string _opisProjekta; 
	bool _finalizirano; //Atribut oznacava da li je projekat završen i predan u predvidenom roku
    bool _prezentacija; //Projekti koji se prezentuju i ocjenjuju 

public: 
//Konstruktor i destruktor
//Ostale potrebne funkcije za rad sa klasom
};
class FITCodingChallenge {
	int godina;
	string _prijaveDo; //Podržani format unosa: dd.MM.yyyy (dopuniti sa regex-ima)
vector<Prijava*> _prijave; 
FITKolekcija<ClanKomisije, Kategorija> _komisija;
FITKolekcija<Prijava*, FITKolekcija<ClanKomisije*, double>> _ocjene; 

public: 
//Konstruktor
//Operator += za dodavanje novih prijava. Isti ucesnik ne može imati više prijava u sklopu iste kategorije.
//Poštovati ogranicenje roka za prijave (_prijaveDo). 
//Voditi racuna o alokaciji memorije za prijave (ne zadržavati adresu proslijedenog objekta)


//Funkciju koja odredenu prijavu oznacava kao finaliziranu i odabranu za prezentaciju ili ne.


//Operator () za dodavanje clanova komisije po kategorijama. Sprijeciti dodavanje istih clanova u sklopu jedne kategorije.


//Funkciju za ispis svih prijava po kategorijama. Funkcija neobavezno prima parametar kategoriju. 
//Ispis treba da ukljuci i podatke o ucesnicima, odnosno clanovima tima. 
//Ukoliko kategorija nije proslijedena, podaci treba da budu grupisani po kategorijama.


//Funkciju za ocjenu projekata (radova) od strane clanova komisije (kolekcija _ocjene). 
//Kolekcija sadrzi samo pokazivace (bez alokacije nove memorije) na postojece prijave i vec definisane clanove komisije. 
//Voditi racuna da se ne mogu ocijeniti projekti koji nisu finalizirani i nisu odabrani za prezentaciju, niti da ocjenu mogu dati clanovi 
//komisije koji nisu pridruženi odgovarajucoj kategoriji prijave. Svaki clan komisije daje ocjenu za svaki projekat, odnosno rad, u 
//kategoriji gdje je dodijeljen. Ocjene se krecu u rasponu od 1 do 10 za projekte, a za radove u kategoriji SQL ili CodingChallenge 
//u rasponu osvojenih bodova od 0 do 100. Pretpostaviti da su svi ucesnici prijavljeni u kategorijama SQL i CodingChallenge predali radove.


//Funkcija koja na osnovu imena i prezimena clana komisije ispisuje podatke o njegovim ocjenama ukljucujuci 
//prijave na koje se one odnose.


//Funkcija koja na osnovu imena i prezimena ucesnika ispisuje podatke o njegovim prijavama na takmicenju, zajedno sa 
//ocjenama, ukoliko su one definisane za prijavu/e datog ucesnika.


//Funkciju za rangiranje kandidata na osnovu kategorije proslijedene kao parametar. 
//Funkcija treba da na osnovu ocjena clanova komisije ispiše rang liste timova zajedno sa prosjecnim ocjenama njihovih projekata (radova).
//Dakle, konacna rang lista ukljucuje nazive timova (u zavisnosti od kategorije i naziv projekta), te prosjecnu ocjenu.


//Funkciju Info koja ispisuje sve podatke o takmicenju.
};

int main() { 
	
	//Kreirati objekat klase FITCodingChallenge, te demonstrirati sve implementirane funkcionalnosti.
	system("pause");
	return 0;
}