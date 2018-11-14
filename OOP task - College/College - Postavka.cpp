//fahirmdz GitHub

#include"pch.h"
#include<iostream>
#include<cstring>
#include<exception>
using namespace std;

char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

template<class T, int max>
class Kolekcija
{
	T* _elementi[max];//staticki niz pokazivaca
	int _trenutnoElemenata;
	bool _dozvoliDuple;
public:
};

class Prisustvo
{
private:
	char* _brojIndeksa;
	int _brojSati;
public:
	
};

class Nastava
{
	char _datum[12];
	char _tipNastave; //P ili V
	int _odrzanoSati;
	Kolekcija<Prisustvo, 200> _prisutni;
	int _brojStudenata;
public:
	
	//Destruktor

	//Preklopljeni operator+= za dodavanje objekata tipa Prisustvo u niz _prisutni.
	//Nije moguce dodati više puta prisustvo za istog studenta (preklopiti operator== u klasi Prisustvo).
	//Elementi u svakom momentu treba da budu sortirani po broju indeksa studenta 
		//(preklopiti operator < u klasi Prisustvo).

	//Preklopljeni operator-= za uklanjanje prisustva za studenta sa proslijedjenim brojem indeksa. Nakon uklanjanja,
	//potrebno je sacuvati redoslijed dodavanja, odnosno podaci ostaju sortirani po broju indeksa.

	//Preklopljeni operator [] koji na osnovu broja indeksa provjerava da li je 
	//odredeni student prisustvovao nastavi i vraca njegove podatke o prisustvu.

	//Preklopljeni operator () koji pronalazi studenta sa proslijedenim brojem indeksa i uvecava mu broj sati prisustva
	//za vrijednost drugog parametra. Ako je studentu vec evidentiran odrzani broj sati nastave prekinuti funkciju.

	//Preklopljeni operator + koji zbraja dva objekta tipa Nastava na nacin da kreira novi objekat na osnovu lijevog operanda
	//i dodaje mu prisutne studente desnog operanda. Funkciju napustiti ukoliko objekti koji ucestvuju u operaciji sabiranja
	//nisu istog tipa nastave. Sprijeciti dodavanje prisustva za istog studenta više puta.
	//Operator implementirati kao globalnu funkciju.

};


class Predmet
{
	char* _naziv;
	int _godinaStudija;
	Kolekcija<Nastava, 45> _odrzanaNastava;
public:
	
};

int main()
{
	Prisustvo p1("2255", 2);
	Prisustvo p2("2014", 3);
	Prisustvo p3("1211", 1);
	Prisustvo p4("1000", 4), p5("1212", 5), p6("1111", 6);

	Nastava n1("01/01/2019", 'V', 12), n2("05/01/2019", 'V', 37);
	n1 += p1;
	n1 += p2;
	n2 += p1;
	n2 += p3;
	n1 += p4;
	n2 += p6;
	n1 += p5;
	n2 += p5;
	Nastava n3 = n1 + n2;
	cout << n3 << endl;
	n3 -= 0;
	n3 -= 3;
	cout << "Nakon uklanjanja\n\n";
	cout << n3 << endl;


	Predmet pr1("Programiranje III", 2), pr2("Algoritmi i strukture podataka", 2), pr3("Statistika i vjerovatnoca", 2);
	pr1 += n1;
	pr1 += n2;
	pr1 += n3;

	cout << pr1 << endl;

	system("pause");
	return 0;
}

