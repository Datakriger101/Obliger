/**
 *  Skjelett for obligatorisk oppgave nr 2 i PROG1003 - OOP.
 *
 *  Programmet er en kalender der man kan legge inn heldags og
 *  tidsbegrensede aktiviteter pÃ¥ spesifikke dager.
 *
 *  Hovedfunksjonalitet:
 *    - Inneholder klassen 'Aktivitet' og dens to subklasser
 *      'Tidsbegrenset' og 'Heldags'. Objekter av de to siste klassene legges
 *      inn for HVER ENKELT DAG inn i to ulike vectorer inni den selvlagede
 *      containerklassen: 'Dag'
 *    - De tre fÃ¸rste klassene inneholder alle constructorer og funksjoner
 *      for Ã¥ lese og skrive alle objektets data.
 *    - 'Dag' inneholder en del ulike funksjoner for Ã¥ hÃ¥ndtere datastrukturen
 *      inni seg. Det er disse medlemsfunksjonene som kalles i fra funksjonene
 *      som startes/kalles fra 'main' for EN gitt dag.
 *    - Den globale vectoren 'gDagene' inneholder ALLE DE ULIKE DAGENE
 *      med hver sine ulike aktiviteter.
 *
 *  @file OBLIG2.CPP
 *  @author Malin Foss, William Eide Seiner og Frode Haug, NTNU
 */


#include <iostream>                //  cout, cin
#include <string>                  //  string
#include <vector>                  //  vector
#include "LesData2.h"
#include <iomanip>
using namespace std;


/*
 *  Enum 'aktivitetsType' (med hva slags aktivitet dette er).
 */
enum aktivitetsType {Jobb, Fritid, Skole, ikkeAngitt};


/**
 *  Baseklassen 'Aktivitet' (med navn og aktivitetstype).
 */
class Aktivitet  {
  private:
    string navn;
    aktivitetsType kategori;
  public:
    Aktivitet()  {  navn = ""; kategori = ikkeAngitt; }
    void lesData();
    void skrivData() const;
};


/**
 *  Subklassen 'Tidsbegrenset' (med tidspunkter for start/stopp av aktivitet).
 */
class Tidsbegrenset : public Aktivitet  {
  private:
    int  startTime, startMin, sluttTime, sluttMin;
    bool klokkeslettOK(const int time, const int min) const;
  public:
    Tidsbegrenset()  { sluttMin = sluttTime = startTime = startMin = 0; };
    void lesData();
    void skrivData() const;
};


/**
 *  Subklassen 'Heldags' (med nÃ¦rmere beskrivelse av aktiviteten).
 */
class Heldags : public Aktivitet  {
  private:
    string beskrivelse;
  public:
    Heldags()  { beskrivelse = ""; };
    void lesData();
    void skrivData() const;
};


/**
 *  Selvlaget container-klasse 'Dag' (med dato og ulike aktiviteter).
 */
class Dag  {
  private:
    int dagNr, maanedNr, aarNr;
    vector <Tidsbegrenset*> tidsbegrensedeAktiviteter;
    vector <Heldags*> heldagsAktiviteter;

  public:
//  Dag()   { dagNr = 0; maanedNr = 0; aarNr = 0; };
    Dag(const int dag, const int maaned, const int aar) {
                           dagNr = dag;  maanedNr = maaned;  aarNr = aar; };
    ~ Dag();
    bool harDato(const int dag, const int maaned, const int aar) const;
    void nyAktivitet();
    void skrivAktiviteter() const;
    void skrivDato() const;
};


bool dagOK(const int dag, const int maaned, const int aar);
Dag* finnDag(const int dag, const int maaned, const int aar);
void frigiAllokertMemory();
void nyAktivitet(); 
void skrivDager(const bool inkludertAktiviteter);
void skrivEnDag();
void skrivMeny();


vector <Dag*> gDagene;           ///<  Dager med aktiviteter


/**
 *  Hovedprogrammet:
 */
int main ()  {
    char kommando;

    skrivMeny();
    kommando = lesChar("\nKommando");

    while (kommando != 'Q')  {
        switch (kommando)  {
          case 'N': nyAktivitet();      break;
          case 'A': skrivDager(true);   break;
          case 'S': skrivEnDag();       break;
          default:  skrivMeny();        break;
        }
        kommando = lesChar("\nKommando");
    }

    frigiAllokertMemory();

    return 0;
}


// ---------------------------------------------------------------------------
//                       DEFINISJON AV KLASSE-FUNKSJONER:
// ---------------------------------------------------------------------------

/**
 *  Leser inn ALLE klassens data.
 */
void Aktivitet::lesData() {
    char temp;
    cout << "Hva slags aktivitet: "; getline(cin, navn);
    do{
        temp=lesChar("Aktivitetstype Jobb(J), Fritid(F), Skole(S) og Ikke angitt(I)");
    }while(temp != 'J' && temp != 'F' && temp!= 'S' && temp != 'I');

    switch(temp){
        case 'J' : kategori = Jobb ; break;
        case 'F' : kategori = Fritid; break;
        case 'S' : kategori = Skole; break;
        case 'I' : kategori = ikkeAngitt; break;
    }
}


/**
 *  Skriver ut ALLE klassens data
 */
void Aktivitet::skrivData() const {

    cout << "\nAktivitet navn: " << navn;

    cout << "\nAktivitetstype: ";
    switch(kategori){
        case Jobb : cout << "Jobb aktivitet"; break;
        case Fritid : cout << "Fritids aktivitet"; break;
        case Skole : cout << "Skole aktivitet"; break;
        case ikkeAngitt : cout << "Ingen spesiell aktivitet"; break;
    }
}


/**
 *  Leser inn ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::lesData()
 *  @see   klokkeslettOK(...)
 */
void Tidsbegrenset::lesData() {

    Aktivitet::lesData();

    do{
        cout << "\nTime-start"; cin >> startTime;
        cout << "\nMinutt-start"; cin >> startMin;
    }while(klokkeslettOK(startTime, startMin));
    
    do{
        cout << "\nTime-start"; cin >> sluttTime;
        cout << "\nMinutt-start"; cin >> sluttMin;
    }while(klokkeslettOK(sluttTime, sluttMin));
}

/**
 *  Privat funksjon som finner ut om input er et lovlig klokkeslett.
 *
 *  @param   time    -  Timen som skal evalueres til mellom 0 og 23
 *  @param   minutt  -  Minuttet som skal evalueres til mellom 0 og 59
 *  @return  Om parametrene er et lovlig klokkeslett eller ei
 */
bool Tidsbegrenset::klokkeslettOK(const int time, const int min) const {

    if((time > 0 && time < 23) && (min > 0 && min < 59))
        return false; else return true;
}


/**
 *  Skriver ut ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::skrivData()
 */
void Tidsbegrenset::skrivData() const {         //  Skriver mor-klassens data.

    Aktivitet::skrivData();

    cout << setfill('0');
    cout << "Aktitet start" << setw(2) << startTime << " - " <<
    setw(2)<< startMin << endl;
    cout << "Aktivitet slutt" << setw(2) << sluttTime << " - " <<
    setw(2) << sluttMin << endl;
}


/**
 *  Leser inn ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::lesData()
 */
void Heldags::lesData() {

    Aktivitet::lesData();
    cout << "Beskrivelse aktivitet heldags: "; getline(cin, beskrivelse);
}


/**
 *  Skriver ut ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::skrivData()
 */
void Heldags::skrivData() const {

    Aktivitet::skrivData();
    cout << "\nAktivitet beskrivelse: " << beskrivelse;
}


/**
 *  Destructor som sletter HELT begge vectorenes allokerte innhold.
 */
Dag :: ~ Dag() {

    while(!tidsbegrensedeAktiviteter.empty()){
        delete tidsbegrensedeAktiviteter[tidsbegrensedeAktiviteter.size()-1];
        tidsbegrensedeAktiviteter.pop_back();
    }

    while(!heldagsAktiviteter.empty()){
        delete heldagsAktiviteter[heldagsAktiviteter.size()-1];
        tidsbegrensedeAktiviteter.pop_back();
    }
    cout << "\nSletter vectorer i dag klassen";
}


/**
 *  Finner ut om selv er en gitt dato eller ei.
 *
 *  @param   dag     -  Dagen som skal sjekkes om er egen dag
 *  @param   maaned  -  MÃ¥neden som skal sjekkes om er egen mÃ¥ned
 *  @param   aar     -  Ã…ret som skal sjekkes om er eget Ã¥r
 *  @return  Om selv er en gitt dato (ut fra parametrene) eller ei
 */
bool Dag::harDato(const int dag, const int maaned, const int aar) const {
    
    for(int i = 0; i < gDagene.size(); i++){
        if(gDagene[i]->dagNr == dag){
            if(gDagene[i]->maanedNr == maaned){
                if(gDagene[i]->aarNr == aar){
                    return false;
                }else return true;
            }else return true;
        }else return true;
    }
}

/**
 *  Oppretter, leser og legger inn en ny aktivitet pÃ¥ dagen.
 *
 *  @see   Tidsbegrenset::lesData()
 *  @see   Heldags::lesData()
 */
void Dag::nyAktivitet()  {
    char temp;

    temp = lesChar("Er aktiviteten (Heldags - H) (tidsbegrenset - T)");

    if (temp == 'H'){
        Heldags* aktivitetH; aktivitetH = new Heldags;
        aktivitetH->lesData();
        heldagsAktiviteter.push_back(aktivitetH);
        
    }else{
        Tidsbegrenset* aktivitetT; aktivitetT = new Tidsbegrenset;
        aktivitetT->lesData();
        tidsbegrensedeAktiviteter.push_back(aktivitetT);
    }
}

/**
 *  Skriver ut ALLE aktiviteter pÃ¥ egen dato (og intet annet).
 *
 *  @see   Heldags::skrivData()
 *  @see   Tidsbegrenset::skrivData()
 */
void Dag::skrivAktiviteter() const {
    cout << "Heldags aktiviteter\n------------------\n\n";
    for(int i = 0; i < heldagsAktiviteter.size(); i++)
        cout << "Aktivitet nr. " << i+1 << heldagsAktiviteter[i] << endl;

    cout << "Tidsbegrensede aktivitet\n--------------------\n\n";
    for(int i = 0; i < tidsbegrensedeAktiviteter.size(); i++)
        cout<<"\nAktivitet nr. " << i+1 << tidsbegrensedeAktiviteter[i]<<"\n";
}

/**
 *  Skriver KUN ut egen -dato.
 */
void Dag::skrivDato() const {
    cout << "Dagens dato: " << dagNr << "." << maanedNr << "." << aarNr;

}


// ---------------------------------------------------------------------------
//                       DEFINISJON AV ANDRE FUNKSJONER:
// ---------------------------------------------------------------------------

/**
 *  Returnerer om en dato er lovlig eller ei.
 *
 *  @param   dag     -  Dagen som skal sjekkes
 *  @param   maaned  -  MÃ¥neden som skal sjekkes
 *  @param   aar     -  Ã…ret som skal sjekkes
 *  @return  Om datoen er lovlig/OK eller ei
 */
bool dagOK(const int dag, const int maaned, const int aar)  {

    if(dag >= 1 && dag <= 31 ){
        if(maaned >= 1 << maaned <= 12){
            if(aar >= 1990 && aar <= 2030){
                cout << "Dato er gyldig" << endl;
                return true;
            }else return false;
        }else return false;
    }else return false;
}


/**
 *  Returnerer om mulig en peker til en 'Dag' med en gitt dato.
 *
 *  @param   dag     -  Dagen som skal bli funnet
 *  @param   maaned  -  MÃ¥neden som skal bli funnet
 *  @param   aar     -  Ã…ret som skal bli funnet
 *  @return  Peker til aktuell Dag (om funnet), ellers 'nullptr'
 *  @see     harDato(...)
 */
Dag* finnDag(const int dag, const int maaned, const int aar)  {

    for(int i = 0; i < gDagene.size(); i++){
        if(gDagene[i]->harDato(dag, maaned, aar))
            return gDagene[i];
        else return nullptr;
    }
    cout << "Ingen var like" << endl;
}


/**
 *  Frigir/sletter ALLE dagene og ALLE pekerne i 'gDagene'.
 */
void frigiAllokertMemory()  {

//  Lag innmaten
}


/**
 *  Legger inn en ny aktivitet pÃ¥ en (evt. ny) dag.
 *
 *  @see   skrivDager(...)
 *  @see   dagOK(...)
 *  @see   finnDag(...)
 *  @see   Dag::nyAktivitet()
 */
void nyAktivitet()  {
    int dag, mnd, aar;

    skrivDager(!gDagene.empty());

    do{
        do{
            cout << "Skriv inn en ny dato:" << endl;
            cout << "Hvilket år: "; cin >> aar;
            cout << "Hvilken mnd: "; cin >> mnd;
            cout << "Hvilken dag: "; cin >> dag;
            cout << "\n\n";
        }while(!dagOK(dag, mnd, aar));

    }while(!finnDag(dag, mnd, aar));

    Dag* nyDag;
    nyDag = new Dag(dag, mnd, aar);
    gDagene.push_back(nyDag);

    nyDag->nyAktivitet();       //Vet ikke om den oppdaterer den pakeren som er lagt bakerst
}


/**
 *  Skriver ut ALLE dagene (MED eller UTEN deres aktiviteter).
 *
 *  @param   inkludertAktiviteter - Utskrift av ALLE aktivitetene ogsÃ¥, eller ei
 *  @see     Dag::skrivDato()
 *  @see     Dag::skrivAktiviteter()
 */
void skrivDager(const bool inkludertAktiviteter)  {
    if(inkludertAktiviteter){
        for(int i = 0; i < gDagene.size(); i++)
            gDagene[i]->skrivAktiviteter();
    }else
        cout << "Ingen lagrede dager!" << endl;
}

/**
 *  Skriver ut ALLE data om EN gitt dag.
 *
 *  @see   skrivDager(...)
 *  @see   dagOK(...)
 *  @see   finnDag(...)
 *  @see   Dag::skrivAktiviteter()
 */
void skrivEnDag()  {
    int dag, mnd, aar;
    skrivDager(!gDagene.empty());

    do{
        cout << "Hvilken dato vil du skrive ut" << endl;
            cout << "Hvilket år: "; cin >> aar;
            cout << "Hvilken mnd: "; cin >> mnd;
            cout << "Hvilken dag: "; cin >> dag;
    }while(!dagOK(dag, mnd, aar));

    finnDag(dag, mnd, aar)->skrivDato();

}


/**
 *  Skriver programmets menyvalg/muligheter pÃ¥ skjermen.
 */
void skrivMeny()  {
   cout << "\nDisse kommandoene kan brukes:\n"
        << "\tN - Ny aktivitet\n"
        << "\tA - skriv ut Alle dager med aktiviteter\n"
        << "\tS - Skriv EN gitt dag og (alle) dens aktiviteter\n"
        << "\tQ - Quit / avslutt\n";
}