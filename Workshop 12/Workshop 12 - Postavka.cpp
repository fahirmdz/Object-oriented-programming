#include<iostream>
#include<vector>
#include<string>
using namespace std;

class Term 
//Pojam 
{ char* _key; 
char* _value; 

public: 
//Konstruktor
//Destruktor
//Operator za ispis
//Operatori poredenja == i != (koristiti samo vrijednost atributa _key)
};

class Dictionary 
//Rijecnik pojmova 
{ string _title;
	int _publicationYear;
	//Kljucne rijeci bosanskog jezika u atribut _key 
	//Odgovarajuci prevod na engleskom jeziku u atribut _value 
vector<Term> _terms; 

public: 
//Konstruktor
//Operator () za dodavanje novih pojmova u rijecnik (u paru key-value).
//Sprijeciti dodavanje istih vrijednosti u kolekciju (baciti izuzetak). 
//Pojmovi u svakom momentu treba da budu sortirani abecedno 
//(koristiti funkciju sort iz biblioteke algorithm).
//Operator -= za uklanjanje pojma iz rijecnika na osnovu atributa _key.
//Funkciju koja vraca trenutni broj pojmova u rijecniku.
//operator[] koji vraca vrijednost na osnovu kljucne rijeci (povratna vrijednost je engleska rijec-value, 
//na osnovu bosanske-key)
//Funkciju koja vraca kolekciju kljuceva (bosanskih rijeci) generisanih slucajnim odabirom.
//Generisanje slucajnog broja izmedu min i max: int randNum = rand()%(max-min + 1) + min; 
//Funkcija kao parametar prima broj rijeci koje je potrebno generisati.
};
//Osmisliti i implementirati klasu Competitor (Takmicar) na nacin da se upotrebom objekta klase Dictionary 
//svakom takmicaru dodijele odredena pitanja (rijeci bosanskog jezika) - koristiti implementiranu funkciju klase Dictionary.
//Svakom pitanju je potrebno pridružiti odgovor takmicara (kolekcija _odgovori).
//Obavezno implementirati funkciju koja vrši kalkulaciju osvojenih bodova (1 tacan odgovor = 1 bod).
//Po potrebi dopuniti klasu dodatnim atributima. 

class Competitor 
//Takmicar
{ 
string _ime;
string _prezime;
vector<string> _pitanja;
vector<string*> _odgovori;

public:

};
int main() { 
	
	//Kreirati rijecnik pojmova. 
	//Kreirati niz objekata tipa Competitor i simulirati takmicenje u poznavanju rijeci engleskog jezika. 
	//Pronaci najboljeg medu takmicarima (najveci broj bodova).


	system("pause"); 
	return 0;
}