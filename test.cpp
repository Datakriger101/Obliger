/*
*   Dette er obligg nr. 1 for objekt orientert programmering. 
*
*   @auther Eskil Refsgaard, NTNU Gjøvik
*   @file obligg1.cpp
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "LesData2.h"

using namespace std;

struct Rute {
    vector <string> stopp;
    int ruteNr,
        totMin;
};

vector <Rute*> gRuter;

const int ANTSTOPP = 11;

const vector <string> gBusstopp =   //Navn på alle busstoppene
 { "Skysstasjonen", "Fahlstroms plass", "Sykehuset",
 "Gjovik stadion", "Bergslia", "Overby", "Nybrua",
 "NTNU", "Kallerud", "Hunndalen", "Mustad fabrikker" };

const int gMinutter[ANTSTOPP][ANTSTOPP] = 
{ { 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Skysstasjonen = 0
 { 3, 0, 3, 0, 0, 0, 3, 0, 0, 0, 4}, // Fahlstrøms plass = 1
 { 0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0}, // Sykehuset = 2
 { 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0}, // Gjøvik stadion = 3
 { 0, 0, 0, 3, 0, 2, 0, 0, 0, 0, 0}, // Bergslia = 4
 { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0}, // Øverby = 5
 { 0, 3, 0, 0, 0, 0, 0, 2, 0, 0, 2}, // Nybrua = 6
 { 0, 0, 0, 0, 0, 0, 2, 0, 0, 4, 0}, // NTNU = 7
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Kallerud = 8
 { 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2}, // Hunndalen = 9
 { 0, 4, 0, 0, 0, 0, 2, 0, 0, 2, 0} };

void skrivMeny();
void nyRute();
void skrivStopp();
void skrivRuter();
void ruteSkrivData(const Rute rute);
void skrivNesteStoppesteder(const int stopp);
bool ruteLesData(Rute & rute);
void slettRute();
void slett();
void slett(const int nr);

/*
*   Hovedprogrammet
*/

int main(){

    char svar;
    cout << "Hei og velkommein til programmet mitt <3" << endl;

    skrivMeny();
    svar = lesChar("Skriv inn en kommando: ");
    while(svar != 'Q'){

        switch(svar){
            case 'N' : nyRute();        break;
            case 'A' : skrivRuter();    break;
            case 'B' : skrivStopp();    break;
            default : skrivMeny();      
        }   

        svar = lesChar("Skriv inn en kommando");
    }
}   

void nyRute(){

    Rute* nyRute;
    nyRute = new Rute;
    ruteLesData(*nyRute);

    gRuter.push_back(nyRute);
    ruteSkrivData(*nyRute);
}

void ruteSkrivData(const Rute rute){

    cout << "Ruten tok totalt " << rute.totMin << " minutter.\n\n";

    for(int i = 0; i < rute.stopp.size()-1; i++){
        cout << rute.stopp[i] << " --> " << rute.stopp[i+1] << endl;
    }

    cout << "\n\n";
}
/*
*   Funksjon som leser inn data for bestemt rute.
*   Sjekker ikke for om brukeren skriver inn feil busstopp.
*   
*   @param skrivStopp()
*   @param skrivNesteStoppeSteder()
*/

bool ruteLesData(Rute & rute){
    int stopp, temp = 0;

    do{
        if(stopp-1 == 8) cout << "\nKan ikke dra videre fra "
        << gBusstopp[stopp-1] << "\n\n";

        rute.ruteNr = gRuter.size() - 1;
        skrivStopp();

        stopp = lesInt("\nVelg et startsted", 1, 11);

    }while(stopp - 1 == 8);

    cout << "\nDu vil starte på stoppet " << gBusstopp[stopp-1] << endl;
    (rute.stopp).push_back(gBusstopp[stopp-1]);

    while(1){  //Kjøres til det returneres 0

        skrivNesteStoppesteder(stopp-1);    //Skriver aktuelle stopp
        cout << "\nHva skal det neste stoppet være: ";
        
        temp = stopp;   //Vite hvor bussen var tidligere
        cin >> stopp; cin.ignore(); 


        if(stopp == 0) return false;

        (rute.stopp).push_back(gBusstopp[stopp-1]);
        rute.totMin += gMinutter[temp-1][stopp-1];

        cout << "\n" << gBusstopp[stopp-1] << "\n";
    }
}

void skrivNesteStoppesteder(const int stopp){

    cout << "Neste stopp kan være: \n";
    
    for(int i = 0; i < ANTSTOPP; i++){

        if(gMinutter[stopp][i] != 0){
            cout << setw(2) << "nr. " << i+1 
            << "\t" << gBusstopp[i] << endl;
        }
    }
}

void skrivRuter(){
    cout << "\n\n**Det er registrert 2 bussruter**\n\n";

    for(int i = 1; i < gRuter.size(); i++){
        cout << "Rute nr. " << i+1 << endl;
        ruteSkrivData(*gRuter[i]);
    }
}

void skrivStopp(){
    for(int i = 0; i < ANTSTOPP; i++)
     cout << setw(2) << i+1 << ". " << gBusstopp[i] << endl;
}

void skrivMeny(){

    cout << "\nDette er menyen;"
    << "\n\tNy rute (N)" 
    << "\n\tSlett rute (S)"
    << "\n\tSkriv alle ruter (A)"
    << "\n\tSkriv alle busstopp (B)\n\n";
}
