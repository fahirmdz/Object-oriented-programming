
/*1. Ukoliko je potrebno, implementirati nasljeđivanje 
 2. U okviru svih klasa implementirati potrebne(de)konstruktor funkcije 
 3. U slučaju nedozvoljenih operacija ili prekoračenja, koristeći klasu exception, 
    sve funkcije članice trebaju  baciti izuzetak sa odgovarajućom porukom
 4. Ukoliko smatrate da je neophodno, ispitni zadatak možete proširiti dodatnim funkcionalnostima 
    ili modifikovati postavljene zahtjeve.Proširenje ili modifikacije moraju pratiti adekvatni komentari 
	sa kojima ćete obrazložiti potrebu za implementiranim / modifikovanim funkcionalnostima.  
 5. Prilikom definisanja funkcija voditi računa da uništavanjem jednog objekta ne utječemo na stanje drugog objekta.
    Prilikom uništavanja objekata ili bilo kakvih modifikacija voditi računa o dealokaciji zauzete memorije.
	*/
template<class T1, class T2, int max> 
class Kolekcija {
T1  * _elementi1[max];  
T2  * _elementi2[max];  
int * _trenutnoElemenata;

public:  
	//1. AddElement :: omogucava dodavanje novog elementa u kolekciju. Onemoguciti ponavljanje
	//   elemenata, te osigurati automatsko prosiranje kolekcije ukoliko broj elemenata dostigne  
	//   trenutni maksimum 
	//2. RemoveElement :: na osnovu parametara tipa T1 i T2 uklanja element iz kolekcije. Ukoliko je 
	//   potrebno,prilikom uklanjanja elementa izvrsiti dealokaciju. Onemoguciti pojavljivanje 
	//   neinicijaliziranih elemenata, te ocuvati redoslijed dodavanja elemenata 
	//3. PopFront :: uklanja prvi element iz kolekcije(iz oba niza). Ukoliko je kolekcija prazna 
	//   funkcija vraca false, a u suprotnom funkcija vraca true 
	//4. operator -=  :: uklanja sve elemnte iz kolekcije koji su identicni onima koje posjeduje 
	//   drugi objekat tipa Kolekcija koji je primljen kao parametar. Funkcija vraca broj uklonjenih 
	//   elemenata 
};

class Datum{
	int * _dan, *_mjesec, *_godina; 

	//1.potrebne funkcije

}; 

class Pregled{  
char * _dijagnoza; 
vector<string> _terapije; 
Datum _datumPregleda;

public:  
	//1. Info :: vraca string svih informacija o Pregled-u 
	//2. AddTerapija :: dodaje novu terapiju. Onemoguciti ponavljanje terapija. Koristiti iteratore 
	//3. RemoveTerapija :: uklanja terapiju koja je primljena kao parametar. Koristiti iteratore

};

 class Pacijent{
 protected: 
	 const int _pacijentID;
	 char * _imePrezime; 
	 //tip bool čuva podatak o tome da li je zakazan kontrolni pregled 
	 //objekat tipa Pregled je za podatke o dijagnozi i terapijama 
 Kolekcija<Pregled *, bool, 20> * _preglediPacijenta; 
 
 public:   
	 //1. operator [] :: vraca Pregled kod koga je postavljena dijagnoza koja je proslijedjena kao   
	 //   parametar. Dakle, operatorska funkcija prima dijagnozu kao parametar 
	 //2. AddPregled :: dodaje podatke o novom pregledu pacijenta. Ukoliko je moguće, osigurati 
	 //   proširanje nize. U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false. 
	 //3. RemovePregled :: na osnovu primljenog parametra (dijagnoze) uklanja sve podatke o pregledu.
	 //   U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false  
	 //4. RemovePregledByTerapija :: na osnovu primljenog parametra (terapije) uklanja podatke o svim 
	 //   pregledima kod koji je propisana trazena terapija, ali pod uslovom da je tokom tog pregleda 
	 //   zakazan novi pregled. U zavisnosti od uspjesnosti operacije, funkcija vraca true ili false 
	 //5. RemovePregledByDatum :: uklanja sve predmete koji su izvrseni nakon datuma primljenog kao  
	 //   parametar

 }; 

void main() { 

		//izbjegavajte koristenje vlastitog imena i prezimena. 
		//provjeriti validnost izvrsenja svih dostupnih funkcionalnosti na objektu tipa Pacijent
		//ukratko pojasniti i demonstrirati koncepciju polimorfizma
	
} 