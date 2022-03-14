#include "LesData2.h"
#include <cctype>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <fstream>

using namespace std;

/*
 *	Hovedklassen - Iskrem
 */

class Iskrem{
	private:
		string smak;
		int pris;
	
	public:
		Iskrem(ifstream & inn){			//Leser iskrem fra fil
			cin.ignore(); inn >> pris;
			getline(inn, smak);
		};					
		
		virtual void lesData(){			//Lese inn data	
			cout << "\nHva slags smak er det: "; getline(cin, smak); 
			cout << "\nHvor mye koster den: "; cin >> pris; }

		virtual void skrivData(){		//Skriver ut data
			cout << "\nIsen er av smaken: " << smak << " og koster: " << pris;
		}
		virtual void skrivTilFil();		//Skriver Iskrem data til fil

};

enum Type { Sorbe, Granite, Slush, Annet};

/*
 * Subklasser - Sorbet og Floteis
 */

class Sorbet : public Iskrem{
	private: 
		enum Type sorbeType;			//Yes sir
			
	public:

		Sorbet(ifstream & inn) : Iskrem(inn){		//Leser fra fil - Sorbet
			int temp; inn >> temp;
			
			switch(temp){
				case '1' : sorbeType = Sorbe;
				case '2' : sorbeType = Granite;
				case '3' : sorbeType = Slush;
			}
		}

		virtual void lesData(){			//Leser inn ny sorbet is
		int svar;

		Iskrem::lesData();				//Leser baseklasse data

		do{
			cout << "Hva slags type (1)Sorbet, (2)Granite, (3)Slush: ";
			cin >> svar;
		}while(svar != 1 && svar != 2 && svar != 3);

			switch(svar)
			{
				case 1 : sorbeType = Sorbe;
				case 2 : sorbeType = Granite;
				case 3 : sorbeType = Slush;
			}
		}

		virtual void SkrivData(){
			Iskrem::skrivData();		//Skriver baseklasse data;

			switch(sorbeType){
				case Sorbe : cout << "\nIsen er av type Sorbet\n";
				case Granite : cout << "\nIsen er av type Granite\n";
				case Slush : cout << "\nDet er en slush hihi :)\n";
				case Annet : cout << "\nFUck you u little shit\n";
			}
		}

		//Annen versjon som jeg også tror vil fungere //MORE, fordi da blir den lett å finnne tilbake til
		//senere
		/*
		template <typename T>

		virtual void skrivData(const list <T> liste) {
			Iskrem::skrivData();
		
			for (auto it = liste.begin(); it != liste.end; it++)
				cout << "\n\t" << it*:

			cout << "\n\n";
		}
		*/

		virtual void skrivTilFil();
};

class Floteis : public Iskrem{
	private:
		bool vegan;						//Om fløteisen er vegansk

	public:
		Floteis(ifstream & inn) : Iskrem(inn){
		bool temp;
			if(temp) vegan = true;
			else vegan = false;
		}

		virtual void lesData(){
		char svar;
			Iskrem::lesData();
			cout << "Er fløteisen vegansk? (J/n): "; cin >> svar;
			
			if(toupper(svar) == 'J') vegan = true;
			else vegan = false;
		}

		virtual void skrivData(){
			Iskrem::skrivData();
			if(vegan) cout << "\nFløteisen er vegansk\n";
			else cout << "Fløteisen er IKKE vegansk\n";
		}

	
		virtual void skrivTilFil();
	
};

/*
 * Ny Baseklasse - Isbil
 */

class Isbil{
	private: 
		string sted;
		list <Iskrem*> iskremList;			//Hver isbil har sine engne is

	public:	
		Isbil(ifstream & inn){ getline(inn, sted);}  //Leser sted for isbil
				
		~Isbil();							//Sletter iskrem fra liste
 
		virtual void skrivData()			//Sted samt antall is listet
		{	
			cout << "Isbilen finnes " << lokasjon() << endl;
			cout << "\tDen bilen inneholder :" << iskremList.size();
		}

/*
		virtual void nyIskrem(){			//sorbet eller Floteis //MORE
		int svar;
			
		//do{
svar = lesInt("Skal det være (1)Sorbet is og (2)Fløteis eller (0) nada", 0, 2);
		//}while(svar != 0 && svar != 1 && svar != 2);
		//Denne blir nok endret senere, char temp = lesChar(), do{}while();

		if(svar != 0)
		{
			//cout << "Svaret var ikke null :O" << endl;
			if(svar == 1)
			{ 
				Sorbet *isSorbet; isSorbet = new Sorbet();
				isSorbet->lesData();
				iskremList.push_back(isSorbet);
			}else
			{
				Floteis *isFloteis; isFloteis = new Floteis;
				isFloteis->lesData();
				iskremList.push_back(isFloteis);
			}
		}else
			cout << "\nDen er grei, ingen is opprettet\n"; 
		}
*/

		virtual void skrivDataIsene(){		//skrivData() + listen med isene
			cout << "\nSkriver ut data for alle isene som finnes i isbilen\n";
			
			skrivData();	//Skriver ut hva sted og ant is	
		
			cout << "\n\nBilen inneholder disse dataene\n";
			for(auto it = iskremList.begin(); it != iskremList.end(); it++)
				cout << "\t" << *it << endl;	//Hva iterator peker på	
		}
			
		virtual void skrivIsbilDataTilFil();//Skriv isbil med data til fili
		//Når data skal skrives til fil skal antall iser være med for
		//å kunne vite hvor mange som skal leses inn i listen

		virtual string lokasjon() const { return sted; }; //pure func??
};

vector <Isbil*> gIsbiler;

/*
 * Globale funksjoner
 */

void skrivMeny();											//DONE
void skrivAlleIsbiler();									//DONE 
void skrivBilOgEvtLeggInn(const bool LeggInn);				//DONE -tror jeg
void skrivTilFil();											//
void lesFraFil();											//lele
Isbil* finnIsbil(const string navn);		//Skal sende med parameter, brukes av LANG funksjon

int main(){

	lesFraFil();

	skrivMeny();
	char kommando = lesChar("\ntHvilken kommando vil du kjøre");
	while(kommando != 'Q'){
		switch(kommando)
		{	
			case 'A' : skrivAlleIsbiler();					break;
			case 'E' : skrivBilOgEvtLeggInn(false);			break;
			case 'L' : skrivBilOgEvtLeggInn(true);			break;
			default  : cout << "Ugyldig kommando!" << endl;	break;	
		}
		kommando = lesChar("\nHvilken kommando vil du kjøre");
	}

	skrivTilFil();
}

void skrivTilFil(){
	cout << "Hello All The Nice braincells that want to make this program amazing! :O\n\n";
}

void lesFraFil(){			//Kjøres i starten av programmet
	int ant; string sted; char type; Isbil* bil; list <Iskrem*> tempList;

	ifstream innfil("ISBIL.dta");

	// trenger å vite hva slags is som skal leses in;
	if(innfil){
		cout << "\nKlarer å lese av filen -ISBIL.dta-\n";
		
		innfil >> ant; 
			
		while(!innfil.eof()){
			innfil.ignore(); 
			gIsbiler.push_back(new Isbil(innfil));

			//Finne måte å legge det under inn i objektet i isbil vector

			for(int i = 0; i < ant; i++){

			innfil >> type;
				if(type == 'S')	tempList.push_back(new Sorbet(innfil));	
				else tempList.push_back(new Floteis(innfil));
			}
		}
	}
}
void skrivBilOgEvtLeggInn(const bool LeggInn){
	string navn; Isbil *temp;

	skrivAlleIsbiler();		//Oversikt om hvem som finne

	getline(cin, navn);
	temp = finnIsbil(navn); //Finner hvis allerde finnes
	if(temp){ 
		temp->skrivData();
		//if(LeggInn) temp->nyIskrem(); //MORE
	}else
		cout << "\nUgyldig navn innskrevet\n";		
	
}

Isbil* finnIsbil(const string navn){
	cout << "Isbil funnet Mr. Refsgaard, ZYZZZZZ\n";

	Isbil* hei; return hei;
}

void skrivAlleIsbiler(){
	
	if(!gIsbiler.empty()){
		cout << "\tUtskrift av alle isbiler\n";
		for(int i = 0; i < gIsbiler.size(); i++){
			cout << "\nIsbil nr: " << i+1 << endl;
			gIsbiler[i]->skrivData();
		}
		
	}else cout << "\nFinnes ingen isbiler enda!\n";
}


void skrivMeny(){

	cout << "Menyvalgene:" << endl;
	cout << "\tSkriv Alle Isbiler (A)" << endl;
	cout << "\tSkriver bil og legger til is (E)" << endl;
	cout << "\tGjøre noe mer med funksjoner pkt.8 (L)" << endl;
	cout << "\tAvlutte programmet (Q)" << endl;
}
