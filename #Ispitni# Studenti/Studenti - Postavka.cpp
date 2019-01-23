

/*1. Ukoliko je potrebno, implementirati nasljeđivanje 
2. U okviru svih klasa implementirati potrebne (de)konstruktor funkcije 
3. U slučaju nedozvoljenih operacija ili prekoračenja, a koristedi kreiranu klasu GreskaUProgramu, 
   sve funkcije članice trebaju „baciti“ izuzetak sa odgovarajudom porukom tj. objekat tipa GreskaUProgramu
4. Ukoliko smatrate da je neophodno, ispitni zadatak možete proširiti dodatnim funkcionalnostima ili 
   modifikovati postavljene zahtjeve. Proširenje ili modifikacije moraju pratiti adekvatni komentari sa 
   kojima dete obrazložiti potrebu za implementiranim/modifikovanim funkcionalnostima. 
5. Prilikom definisanja funkcija voditi računa da uništavanjem jednog objekta ne utječemo na stanje drugog objekta. 
   Prilikom uništavanja objekata ili bilo kakvih modifikacija voditi računa o dealokaciji zauzete memorije. */



template<class T1, class T2, int max>
class Kolekcija {
T1 * _clan1[max]; 
T2 * _clan2[max];  
int * _trenutnoClanova;
public: 
	//1. Potrebne konstruktor/destruktor funkcije
	//2. AddElement :: omogucava dodavanje novih elementa u kolekciju (u clan1 i clan2).Onemoguciti
	//dodavanje dva identicna elementa u kolekciju 
	//3. RemoveElement :: uklanja element na zahtijevanoj lokaciji.Prilikom uklanjanja elementa
	//izvrsiti potrebnu dealokaciju, te onemoguciti pojavljivanje neinicijaliziranih elemenata niza.
	//4. operator -=  :: iz postojece kolekcije (clanovi 1 i 2) uklanja elemente koji su identicni
	//nekom od elemenata u kolekciji koja je primljena kao parametar.

}; 

class Osoba{ 
protected: 
char * _imePrezime;

public:  
	//1. Potrebne konstruktor i destruktor funkcije
	//2. Deklarisati kao apstraktnu klasu 
}; 

class Predmet {
	char * _naziv; 
char * _profesor; 

public: 
	//1. Potrebne konstruktor/destruktor funkcije
}; 

class Student {
const int _brojIndeksa; 
Kolekcija<Predmet, int, 50>  _polozeniPredmeti;
vector<char *> _seminarski;//naslovi seminarskih radova 

public:
	//1. Potrebne konstruktor/destruktor funkcije
	//2. DodajPolozeniPredmet :: omogucava dodavanje novopolozenog predmeta.Onemoguciti dodavanje dva
	//istoimena predmeta
	//3. UkloniPolozeniPredmet :: na osnovu naziva, uklanja predmet iz liste polozenih predmeta
	//4. DodajSeminarski :: omogucava dodavanje podataka o seminarskom radu. Onemoguciti dodavanja
	//dva istoimena seminarska rada. U zavisnosti od uspjesnosti obavljene operacije funkcija vraca
	//true/false. Koristiti iteratore. //5. UkloniSeminarski :: uklanja podatke o seminarskom radu. U zavisnosti od uspjesnosti
	//obavljene operacije funkcija vraca true/false. Koristiti iteratore.
	//6. operator <<  :: ispisuje sve dostupne podatke o studentu 

}; 

void main() {  

	//provjeriti validnost izvrsenja svih dostupnih funkcionalnosti 
	//ukratko pojasniti i demonstrirati koncepciju polimorfizma 
	//prilikom kodiranja i testiranja izbjegavajte koristiti vlastito ime i prezime 

}