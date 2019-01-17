//fahirmdz GitHub

#include"pch.h"
#include <iostream>
#include<cstring>
#include<string>
#include<exception>
#include<regex>
#include<thread>
#include<Windows.h>
#include<mutex>
#include<iomanip>
#include<chrono>
using namespace std;

const char* crt = "\n-------------------------------------------------------------\n";
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
class Datum
{
	int *_dan, *_mjesec, *_godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum &dat)
	{
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
	}
	~Datum()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}

	Datum& operator=(const Datum& d) {
		if (&d == this)
			return *this;

		delete _dan;
		_dan = new int(*d._dan);
		delete _mjesec;
		_mjesec = new int(*d._mjesec);
		delete _godina;
		_godina = new int(*d._godina);
		return *this;
	}
	int brojDanaIzmedju(Datum* d1) {
		long int d1d = 0, d2d = 0;
		auto isPrestupna = [](int g) {
			return g % 400 == 0 && g % 100 == 0 && g % 4 == 0;
		};
		if (isPrestupna(*d1->_godina))
			d1d += 366;
		else
			d1d += 365;

		if (isPrestupna(*_godina))
			d2d += 366;
		else
			d2d += 365;
		bool d1prestupna = isPrestupna(*d1->_godina), d2prestupna = isPrestupna(*_godina);

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
		d1d += brojDana(*d1->_mjesec, d1prestupna);
		d2d += brojDana(*_mjesec, d2prestupna);
		d1d += *d1->_dan;
		d2d += *_dan;
		return d1d - d2d;
	}
	friend ostream &operator<<(ostream&, const Datum &);
	friend bool operator==(Datum&, Datum&);
};
bool operator==(Datum& d1, Datum& d2) {
	return *d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina;
}
ostream &operator<<(ostream &COUT, const Datum &obj)
{
	COUT << *obj._dan << "/" << *obj._mjesec << "/" << *obj._godina;
	return COUT;
}

class Izuzetak : public exception {
	string _funkcija;

public:
	Izuzetak(const char* message, string funct) :exception(message), _funkcija(funct) {}

	const char* what()const {
		string x = string(exception::what()) + "  -  Funkcija: " + _funkcija;
		return AlocirajNizKaraktera(x.c_str());
	}
};

template <class T1, class T2>
class Kolekcija {
	T1 * _elementi1;
	T2 * _elementi2;
	int _trenutnoElemenata;
	bool _omoguciDupliranjeElemenata;
	void copyElements(const Kolekcija& k) {
		_elementi1 = new T1[k._trenutnoElemenata];
		_elementi2 = new T2[k._trenutnoElemenata];
		for (int i = 0; i < k._trenutnoElemenata; i++) {
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}

public:
	Kolekcija(bool omoguciDupliranjeElemenata = false) {
		_trenutnoElemenata = 0;
		_omoguciDupliranjeElemenata = omoguciDupliranjeElemenata;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
	}
	Kolekcija(const Kolekcija& k) :_trenutnoElemenata(k._trenutnoElemenata), _omoguciDupliranjeElemenata(k._omoguciDupliranjeElemenata) {
		if (_trenutnoElemenata > 0)
			copyElements(k);
	}
	~Kolekcija() {
		delete[]_elementi2; _elementi2 = nullptr;
		delete[]_elementi1; _elementi1 = nullptr;
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (&k == this)
			return *this;

		_trenutnoElemenata = k._trenutnoElemenata;
		delete[] _elementi1;
		delete[] _elementi2;
		if (_trenutnoElemenata > 0)
			copyElements(k);

		_omoguciDupliranjeElemenata = k._omoguciDupliranjeElemenata;
		return *this;
	}

	bool AddElement(T1 t1, T2 t2) {
		if (_omoguciDupliranjeElemenata && _elementi1 != nullptr)
			for (int i = 0; i < _trenutnoElemenata; i++)
				if (t1 == _elementi1[i])
					return false;
		T1* temp1 = new T1[_trenutnoElemenata + 1];
		T2* temp2 = new T2[_trenutnoElemenata + 1];
		if (_trenutnoElemenata > 0)
			for (int i = 0; i < _trenutnoElemenata; i++) {
				temp1[i] = _elementi1[i];
				temp2[i] = _elementi2[i];
			}
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		temp1 = nullptr;
		temp2 = nullptr;
		_elementi1[_trenutnoElemenata] = t1;
		_elementi2[_trenutnoElemenata++] = t2;
		return true;
	}

	bool RemoveElement(T1 t1) {
		int index = -1;
		for (int i = 0; i < _trenutnoElemenata; i++)
			if (_elementi1[i] == t1)
				index = i;
		if (index == -1)
			return false;
		for (int i = index; i < _trenutnoElemenata - 1; i++) {
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_trenutnoElemenata--;
		return true;
	}
	int GetTrenutno() const { return _trenutnoElemenata; }
	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi1[lokacija];
	}

	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw Izuzetak("Nepostojeca lokacija", __FUNCTION__);
		return _elementi2[lokacija];
	}

	friend ostream &operator<< <>(ostream &, const Kolekcija &);
};
template<class T1, class T2>
ostream &operator<< <>(ostream &COUT, const Kolekcija<T1, T2> &obj)
{
	for (int i = 0; i < obj.GetTrenutno(); i++)
		COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
	return COUT;
}


class Dogadjaj {
	Datum _datumOdrzavanja;
	Kolekcija<string, bool> * _obaveze; //cuva informaciju o obavezama koje je potrebno ispuniti prije samog dogadjaja, string se odnosi na 
										// opis, a bool na izvrsenje te obaveze (da li je zavrsena ili ne) 
	char *_naziv;
	int _notificirajPrije; //oznacava broj dana prije samog dogadjaja kada ce krenuti notifikacija/podsjetnik
	bool _rekurzivnaNotifikacija; //ako je vrijednost true onda se korisnik notificira svaki dan do _datumaOdrzavanja dogadjaja, 
								  //a pocevsi prije dogadjaja za _brojDanaZaNotifikaciju

public:
	Dogadjaj(Datum datumOdrzavanja, const char *naziv, int brojDana = 1,
		bool rekurzivnaNotifikacija = false) : _datumOdrzavanja(datumOdrzavanja) {
		if (!regex_match(string(naziv), regex("[a-zA-Z\\s]{5,30}")))
			throw Izuzetak("Pogresan format naziva dogadjaja!", __FUNCTION__);
		_naziv = AlocirajNizKaraktera(naziv);
		_notificirajPrije = brojDana;
		_rekurzivnaNotifikacija = rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>;
	}

	Dogadjaj(const Dogadjaj &obj) : _datumOdrzavanja(obj._datumOdrzavanja) {
		_naziv = AlocirajNizKaraktera(obj._naziv);
		_notificirajPrije = obj._notificirajPrije;
		_rekurzivnaNotifikacija = obj._rekurzivnaNotifikacija;
		_obaveze = new Kolekcija<string, bool>(*obj._obaveze);
	}
	~Dogadjaj()
	{
		delete[] _naziv;
		_naziv = nullptr;
		delete _obaveze;
		_obaveze = nullptr;
	}

	Dogadjaj& operator=(const Dogadjaj& d) {
		if (&d == this)
			return *this;
		_datumOdrzavanja = d._datumOdrzavanja;
		delete _obaveze;
		_obaveze = new Kolekcija<string, bool>(*d._obaveze);

		delete[] _naziv;
		_naziv = AlocirajNizKaraktera(d._naziv);
		_rekurzivnaNotifikacija = d._rekurzivnaNotifikacija;
		_notificirajPrije = d._notificirajPrije;
		return *this;
	}
	/*po vlasitom izboru definisati listu zabranjenih rijeci koje ce onemoguciti dodavanje odredjene obaveze.
	Prilikom provjere koristiti regex*/
	bool AddObavezu(const char* opis) {
		const char* zabranjeneRijeci[] = { "dodati bodove","pokloniti" };
		for (int i = 0; i < 2; i++)
			if (regex_search(opis, regex("(dodati|pokloniti|odloziti)")))
				throw Izuzetak("Opis obaveze sadrzi jednu ili vise zabranjenih rijeci!", __FUNCTION__);
		for (int i = 0; i < _obaveze->GetTrenutno(); i++)
			if (_obaveze->GetElement1(i) == opis)
				return false;
		_obaveze->AddElement(opis, false);
		return true;
	}

	bool oznaciZavrsenom(const char* opis) {
		for (int i = 0; i < _obaveze->GetTrenutno(); i++)
			if (_obaveze->GetElement1(i) == opis) {
				_obaveze->GetElement2(i) = true;
				return true;
			}
		return false;
	}
	char *GetNaziv() { return _naziv; }

	int& GetNotificirajPrije() { return _notificirajPrije; }
	bool GetRekurzivnaNotifikacija()const { return _rekurzivnaNotifikacija; }
	int GetBrojObaveza()const { return _obaveze->GetTrenutno(); }
	int GetBrojIzvrsenihObaveza()const {
		int x = 0;
		for (int i = 0; i < _obaveze->GetTrenutno(); i++)
			if (_obaveze->GetElement2(i))
				x++;

		return x;
	}
	Datum& GetDatumOdrzavanja() { return _datumOdrzavanja; }

	Kolekcija<string, bool> *GetObaveze() { return _obaveze; }
	friend bool operator==(Dogadjaj&, Dogadjaj&);
};
bool operator==(Dogadjaj& d1, Dogadjaj& d2) {
	return d1._datumOdrzavanja == d2._datumOdrzavanja && strcmp(d1._naziv, d2._naziv) == 0;
}

class Student
{
	int _indeks;
	string _imePrezime;
	vector<Dogadjaj> _dogadjaji;
public:
	Student() :_indeks(0), _imePrezime("") {}
	Student(int indeks, string imePrezime) :_indeks(indeks) {
		if (!regex_match(imePrezime, regex("^[a-zA-Z\\s]{4,25}$")))
			throw Izuzetak("Ime i prezime studenta nije validno!", __FUNCTION__);
		_imePrezime = imePrezime;
	}

	Dogadjaj& operator[](int index) {
		if (index < 0 || index >= (int)_dogadjaji.size())
			throw Izuzetak("Index nije validan!", __FUNCTION__);
		return _dogadjaji[index];
	}
	int GetIndeks() const { return _indeks; }
	string GetImePrezime() { return _imePrezime; }
	int GetBrojDogadjaja()const { return _dogadjaji.size(); }


	vector<Dogadjaj>& GetDogadjaji() { return _dogadjaji; }

	bool AddDogadjaj(Dogadjaj& d) {
		for (size_t i = 0; i < _dogadjaji.size(); i++)
			if (_dogadjaji[i] == d)
				return false;
		_dogadjaji.push_back(d);
		return true;
	}

	friend ostream &operator<<(ostream &, const Student &);
	friend bool operator==(Student&, Student&);
};
bool operator==(Student& s1, Student& s2) {
	return s1._imePrezime == s2._imePrezime && s1._indeks == s2._indeks;
}
ostream &operator<<(ostream &COUT, const Student &obj)
{
	COUT << obj._imePrezime << " (" << obj._indeks << ")" << endl;
	return COUT;
}

mutex mut;
class DLWMSReminder {
	vector<Student> _reminderList;
	/*-------------------------------------------------------------------------
		Postovani Jasmin Azemovic,
		Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
		1.Preraditi ispitne zadatke
		2.Samostalno vjezbati
		Predlazemo Vam da ispunite i ostale planirane obaveze.
		FIT Reminder
		-------------------------------------------------------------------------*/
	void sendNotification(string imePrezime, string obaveza, Dogadjaj& d, int brojDana) {
		double procenatIzvrsenihObaveza = ((double)d.GetBrojIzvrsenihObaveza() / d.GetBrojObaveza()) * 100;
		cout << crt << "Postovani " << imePrezime << ",\n Dogadjaj " << obaveza << " je zakazan za " << brojDana << " dana, a do sada ste obavili ";
		cout << setprecision(2) << procenatIzvrsenihObaveza << "% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su: \n";
		Kolekcija<string, bool>* temp = d.GetObaveze();
		int br = 0;
		for (int i = 0; i < temp->GetTrenutno(); i++)
			if (!temp->GetElement2(i)) {
				cout << (br++) + 1 << ". " << temp->GetElement1(i) << endl;
			}
		cout << "\nPredlazemo Vam da ispunite i ostale planirane obaveze.\nFIT Reminder" << crt;
		temp = nullptr;
		this_thread::sleep_for(chrono::seconds(1));
		d.GetNotificirajPrije()--;
	}
public:

	void AddStudent(Student& s) {
		for (size_t i = 0; i < _reminderList.size(); i++)
			if (_reminderList[i] == s)
				return;
		_reminderList.push_back(s);
	}

	//da bi bila oznacena kao zavrsena, obaveza mora postojati i mora biti oznacena kao nezavrsena (false)
	bool OznaciObavezuKaoZavrsenu(int index, const char* dogadjaj, const char* opisObaveze) {
		int student = -1;
		for (size_t i = 0; i < _reminderList.size(); i++)
			if (_reminderList[i].GetIndeks() == index)
				student = i;

		if (student == -1)
			return false;
		int dogadjajIndex = -1;

		for (int i = 0; i < _reminderList[student].GetBrojDogadjaja(); i++)
			if (strcmp(dogadjaj, _reminderList[student][i].GetNaziv()) == 0)
				dogadjajIndex = i;

		if (dogadjajIndex == -1)
			return false;
		return _reminderList[student][dogadjajIndex].oznaciZavrsenom(opisObaveze);
	}
	/*metodi PosaljiNotifikacije se salje trenutni datum na osnovu cega ona pretrazuje sve studente koje treba
	  podsjetiti/notoficirati o dogadjajima koji se priblizavaju.
	  Koristeci multithread-ing, svim studentima se salju notifikacije sa sljedecim sadrzajem:
	  -------------------------------------------------------------------------
	  Postovani Jasmin Azemovic,
	  Dogadjaj Ispit iz PRIII je zakazan za 3 dana, a do sada ste obavili 56% obaveza vezanih za ovaj dogadjaj. Neispunjene obaveze su:
	  1.Preraditi ispitne zadatke
	  2.Samostalno vjezbati
	  Predlazemo Vam da ispunite i ostale planirane obaveze.
	  FIT Reminder
	  -------------------------------------------------------------------------
	  Dakle, notifikacije ce biti poslane svim studentima koji su dodali dogadjaj za 30.01.2018. godine i oznacili da
	  zele da budu podsjecani ponovo/rekurzivno najmanje 2 dana prije samog dogadjaja (podaci se odnose
	  na konkretan dogadjaj: Ispit iz PRIII)
	  */
	int PosaljiNotifikacije(Datum& d) {
		cout << "\n::::::::REMINDER " << d << "::::::::::\n";
		int ukupnoPoslato = 0;
		for (size_t i = 0; i < _reminderList.size(); i++) {
			vector<Dogadjaj>& temp = _reminderList[i].GetDogadjaji();
			for (size_t j = 0; j < temp.size(); j++) {
				int ostaloDana = d.brojDanaIzmedju(&temp[j].GetDatumOdrzavanja());
				if (temp[j].GetRekurzivnaNotifikacija() && ostaloDana <= temp[j].GetNotificirajPrije()) {
					mut.lock();
					thread t(&DLWMSReminder::sendNotification, this, _reminderList[i].GetImePrezime(), string(temp[j].GetNaziv()), ref(temp[j]), ostaloDana);
					t.join();
					mut.unlock();
					ukupnoPoslato++;
				}
			}
		}
		return ukupnoPoslato;
	}
};

void main()
{
	/**************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA
	   KOJI UZROKUJU RUNTIME ERROR CE BITI OZNACENO KAO "RE"
	3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKODJER NALAZI U FAJLU CODE_ParcijalniII.TXT
	5. NAZIVI FUNKCIJA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U
	   TESTNOM CODE-U.OSTALE, POMOÆNE FUNKCIJE MOŽETE IMENOVATI PO ŽELJI.
****************************************************************************/
	try {
#pragma region Datum

		Datum danas(28, 1, 2018), sutra(29, 1, 2018);
		Datum datumIspitaPRIII(30, 1, 2018), datumIspitBPII(31, 1, 2018);
		Datum prekosutra(danas);
		prekosutra = danas;
		cout << danas << endl
			<< sutra << endl
			<< prekosutra << crt;

#pragma endregion

#pragma region Kolekcija

		/*
		AddElement :: omogucava dodavanje novog elementa u kolekciju. Ukoliko je moguce, osigurati automatsko
		prosiranje kolekcije prilikom dodavanja svakog novog elementa, te onemoguciti ponavljanje elemenata
		RemoveElement :: na osnovu parametra tipa T1 uklanja elemente iz kolekcije i ukoliko je moguce smanjuje
		velicinu niza/kolekcije. Prilikom uklanjanja elemenata ocuvati redoslijed njihovog dodavanja
		*/
		const int brElemenata = 10;
		Kolekcija<int, float> kolekcija1;
		for (int i = 0; i < brElemenata; i++)
			if (!kolekcija1.AddElement(i, i + (0.6 * i)))
				cout << "Elementi " << i << " i " << i + (0.6 * i) << " nisu dodati u kolekciju" << endl;

		cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
		cout << kolekcija1 << endl;

		kolekcija1.RemoveElement(1);

		Kolekcija<int, float> kolekcija2;
		kolekcija2 = kolekcija1;
		cout << kolekcija2 << crt;

		if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
			cout << "ISTI BROJ ELEMENATA" << endl;

		Kolekcija<int, float> kolekcija3(kolekcija2);
		cout << kolekcija3 << crt;

#pragma endregion

#pragma region Dogadjaj
		Dogadjaj ispitPRIII(datumIspitaPRIII, "Ispit iz PRIII", 5, true),
			ispitBPII(datumIspitBPII, "Ispit iz BPII", 7, true);

		if (ispitPRIII.AddObavezu("Preraditi PDF materijale")) cout << "Obaveza dodana!" << endl;
		//onemoguciti dupliranje obaveza
		if (!ispitPRIII.AddObavezu("Preraditi PDF materijale")) cout << "Obaveza nije dodana!" << endl;
		if (ispitPRIII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
		if (ispitPRIII.AddObavezu("Preraditi ispitne zadatke")) cout << "Obaveza dodana!" << endl;
		if (ispitPRIII.AddObavezu("Samostalno vjezbati")) cout << "Obaveza dodana!" << endl;

		if (ispitBPII.AddObavezu("Preraditi knjigu SQL za 24 h")) cout << "Obaveza dodana!" << endl;
		if (ispitBPII.AddObavezu("Pregledati video materijale")) cout << "Obaveza dodana!" << endl;
		if (ispitBPII.AddObavezu("Napraviti bazu za konkretnu aplikaciju")) cout << "Obaveza dodana!" << endl;

		Student jasmin(150051, "Jasmin Azemovic"), adel(160061, "Adel Handzic");

		if (jasmin.AddDogadjaj(ispitPRIII) && jasmin.AddDogadjaj(ispitBPII))
			cout << "Dogadjaj uspjesno dodan!" << endl;

		if (adel.AddDogadjaj(ispitPRIII) && adel.AddDogadjaj(ispitBPII))
			cout << "Dogadjaj uspjesno dodan!" << endl;
		//onemoguciti dupliranje dogadjaja
		if (!adel.AddDogadjaj(ispitPRIII))
			cout << "Dogadjaj nije uspjesno dodan!" << endl;

		DLWMSReminder reminder;

		try
		{
			reminder.AddStudent(jasmin);
			reminder.AddStudent(adel);
			//u slucaju dupliranja studenata funkcija baca izuzetak tipa Izuzetak
			reminder.AddStudent(jasmin);
		}
		catch (exception &err)
		{
			cout << err.what() << endl;
			delete[] err.what();
		}

		if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Pregledati video materijale"))
			cout << "Obaveza oznacena kao zavrsena" << endl;
		reminder.OznaciObavezuKaoZavrsenu(160061, "Ispit iz BPII", "Preraditi knjigu SQL za 24 h");

		int poslato = 0;
		//funkcija vraca broj poslatih podsjetnika/notifikacija
		poslato = reminder.PosaljiNotifikacije(danas);
		cout << "Za " << danas << " poslato ukupno " << poslato << " podsjetnika!" << endl;

		if (reminder.OznaciObavezuKaoZavrsenu(150051, "Ispit iz PRIII", "Preraditi PDF materijale"))
			cout << "Obaveza oznacena kao zavrsena" << endl;
		reminder.OznaciObavezuKaoZavrsenu(160061, "Ispit iz PRIII", "Preraditi PDF materijale");

		poslato = reminder.PosaljiNotifikacije(sutra);
		cout << "Za " << sutra << " poslato ukupno " << poslato << " podsjetnika!" << endl;
	}
	catch (exception& obj) {
		cout << obj.what() << endl;
		delete[] obj.what();
	}
#pragma endregion

	system("pause");
}
