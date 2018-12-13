#include<iostream> 

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
	Vrijeme(int sati, int minute, int sekunde = 0) { 
		_sati = new int(sati); 
	_minute = new int(minute); 
	_sekunde = new int(sekunde);
	}

Vrijeme(const Vrijeme& v) {
	_sati = new int(*v._sati); 
	_minute = new int(*v._minute);  
	_sekunde = new int(*v._sekunde);
}

//Preklopiti operator << za ispis objekata tipa Vrijeme 

};

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

 //Funkciju sa jednim parametrom koja postavlja informaciju o eventualnom kašnjenju na vrijednost zadanu parametrom. 

 /*Preklopiti unarni operator "!" putem kojeg je moguce saznati da li odgovarajuci let kasni
 ili ne (vratiti logicku  vrijednost "true" u slucaju kašnjenja, u suprotnom vratiti "false").*/

 //Funkciju koja kao rezultat vraca trajanje leta.

	/*Preklopiti operator + na nacin da omogucava sabiranje objekata tipa "Let" i cijelog broja, pri 
	cemu se kao rezultat dobiva novi objekat tipa "Let", u kojem je vrijeme polijetanja pomjereno 
	unaprijed za iznos odreden drugim sabirkom (racunato u minutama).  Takoder preklopiti i operator
	"+=" koji osigurava da izraz oblika "X += Y uvijek ima isto znacenje kao i izraz  "X = X + Y" */

 /* Funkciju kojom je moguce saznati ocekivano vrijeme polijetanja kada se uracuna iznos kašnjenja 
 u odnosu na predvideno vrijeme polijetanja (preklopiti operator + u klasi Vrijeme). */

 //Funkciju koja vraca ocekivano vrijeme slijetanja 

 /*Preklopiti operator "++" na nacin da pomijera vrijeme polaska za jedan sat unaprijed.   
 Potrebno je podržati i prefiksnu i postfiksnu verziju ovog operatora.*/

   /*Preklopiti relacione operatore "<" i ">" koji ispituju koji let nastupa ranije, odnosno kasnije.  
   Operator "<" vraca logicku vrijednost "true" ukoliko polijetanje leta sa lijeve strane nastupa  
   prije polijetanje leta sa desne strane, a u suprotnom vraca logicku vrijednost "false". 
   Analogno vrijedi za operator ">". Prilikom uporedivanja treba uzeti u obzir i ocekivano vrijeme kašnjenja,  
   a ne samo planirano vrijeme polijetanja.*/

  /*Preklopiti operator "<<" koji treba da podrži ispis objekata tipa "Let" na ekran. U slucaju da  
  se radi o polijetanju bez kašnjenja, ispis bi trebao da izgleda kako slijedi:

   JFK 156 Atalanta    12:50   19:30   5

  Podaci predstavljaju redom: oznaku leta, naziv odredišta, vrijeme polijetanja, ocekivano  
  vrijeme slijetanja i broj izlazne kapije.

  U slucaju da se radi o letu koji kasni, ispis bi trebao izgledati kako slijedi:

   ZGK 932 Zagreb    15:50 (Planirano 15:30, Kasni 20 min)*/
};


class RasporedLetova {
	int _brojRegistrovanihLetova; 
	const int _maksimalanBrojLetova; 
	Let** _letovi;

public: 
	RasporedLetova(int maksimalanBrojLetova) : _maksimalanBrojLetova(maksimalanBrojLetova),
		_letovi(new Let*[_maksimalanBrojLetova]), _brojRegistrovanihLetova(0) {

	}

	/*Preklopiti operator "+=" na nacin da registruje novi let u raspored. Raspored letova u svakom momentu treba biti sortiran  
	prema vremenu polijetanja. Voditi racuna o maksimalnom broju letova.  Za potrebe poredenja vremena polaska letova preklopiti
	odgovarajuce operatore u klasi Vrijeme.*/

	  /*Preklopiti operator "-=" na nacin da ukljanja registrovani let sa oznakom definisanom putem desnog operanda.  
	  Pri tome sacuvati redoslijed prethodno registrovanih letova. */

	   //Preklopiti operator "[]" na nacin da vraca objekat tipa Let na osnovu proslijedene pozicije u rasporedu (pozicije krecu od 1). 

	   /*Preklopiti operator "()" na nacin da letu sa definisanom oznakom pomjeri vrijeme polijetanja za vrijednost
	   drugog parametra izraženog   u minutama. */


	 //Funkciju koja daje prosjecno trajanje svih pohranjenih letova. 


	 /*Preklopiti operator "<<" koji ispisuje kompletan spisak svih letova, sortiran po ocekivanim vremenima polazaka.   
	 Za potrebe poredenja vremena polaska letova preklopiti odgovarajuce operatore u klasi Vrijeme. */

};

int main() { 
	

system("pause"); 
return 0;
}