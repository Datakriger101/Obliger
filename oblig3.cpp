#include "LesData2.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

/*
 *	Hovedklassen - Iskrem
 */

class Iskrem{
	private:
		string smak;
		int pris;
	
	public:
		Iskrem();						//Leser fra fil - Iskrem
		
		virtual void lesData();			//Lese inn data
		virtual void skrivData();		//Skriver ut data
		virtual void skrivTilFil();		//Skriver Iskrem data til fil

};

/*
 * Subklasser - Sorbet og Floteis
 */

class Sorbet{
	private: 
		enum Type { Sorbe, Granite, Slush};		//Yes sir
	
	public:
		Sorbet();						//Leser fra fil - Sorbet

		virtual void lesData();		
		virtual void SkrivData();
		virtual void skrivTilFil();
};

class Floteis{
	private:
		bool vegan;						//Om fløteisen er vegansk

	public:
		Floteis();

		virtual void lesData();
		virtual void skrivData();
		virtual void skrivTilFil();
	
};

/*
 * Ny Baseklasse - Isbil
 */

class Isbil{
	private: 
		string sted;
		list <

	public:	
		Isbil();							//Leser innhold fra fil
		~Isbil();							//Sletter iskrem fra liste

		virtual void skrivData()			//Sted samt antall is listet
		{ cout << "Isbilen er finnes på " << sted << endl; }

		virtual void nyIskrem();			//sorbet eller Floteis
		virtual void skrivDataIsene();		//skrivData() + listen med isene
		virtual void skrivIsbilDataTilFil();//Skriv isbil med data til fil
		virtual string loksjon() const { return sted; };
};

vector <Isbil*> gIsbiler;

/*
 * Globale funksjoner
 */

void skrivMeny();
void skrivAlleIsbiler();
void skrivBilOgEvtLeggInn(const bool LeggInn);
void skrivTilFil();
void lesFraFil();
Isbil* finnIsbil();		//Skal sende med parameter

int main(){

	skrivMeny();
	char kommando = lesChar("\ntHvilken kommando vil du kjøre");
	while(kommando != 'Q'){
		switch(kommando)
		{	
			case 'A' : skrivAlleIsbiler(); break;
			case 'E' : cout << "E" << endl; break;
			case 'L' : cout << "L" << endl; break;
			default  : cout << "Ugyldig kommando!" << endl; break;	
		}
		kommando = lesChar("\nHvilken kommando vil du kjøre");
	}
}

void skrivAlleIsbiler(){
	
	if(!gIsbiler.empty()){
		cout << "\tUtskrift av alle isbiler\n";
		for(int i = 0; i < gIsbiler.size(); i++){
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
