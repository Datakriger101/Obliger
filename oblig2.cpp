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

        cout << gDagene.size() << endl;

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

    do{
        temp=lesChar("Aktivitetstype Jobb(J), Fritid(F), Skole(S) og Ikke angitt(I)");
    }while(temp != 'J' && temp != 'F' && temp!= 'S' && temp != 'I');
    cout << "Hva slags aktivitet: "; getline(cin, navn);

    switch(temp){       //setter ennum to verdi
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
        cout << "\nTime-start: "; cin >> startTime;
        cout << "\nMinutt-start: "; cin >> startMin;
    }while(klokkeslettOK(startTime, startMin));
    
    do{
        cout << "\nTime-Slutt: "; cin >> sluttTime;
        cout << "\nMinutt-Slutt: "; cin >> sluttMin;
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

    if((time > 0 && time < 23) && (min > 0 && min < 59))    //Gyldig klokkelsett
        return false; else return true;
}

/**
 *  Skriver ut ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::skrivData()
 */
void Tidsbegrenset::skrivData() const {         //  Skriver mor-klassens data.

    Aktivitet::skrivData();     //Henter skrivdata fra Aktivitet klasse

    cout << setfill('0');       //Fyller på med '0', tid 2 plasser
    cout << "\nAktitet start: " << setw(2) << startTime << " - " <<
    setw(2)<< startMin;
    cout << "\nAktivitet slutt: " << setw(2) << sluttTime << " - " <<
    setw(2) << sluttMin << endl;
}


/**
 *  Leser inn ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::lesData()
 */
void Heldags::lesData() {

    Aktivitet::lesData();           //Bruker klassefunksjon
    cout << "Beskrivelse aktivitet heldags: "; getline(cin, beskrivelse);
}


/**
 *  Skriver ut ALLE klassens data, inkludert morklassens data.
 *
 *  @see   Aktivitet::skrivData()
 */
void Heldags::skrivData() const {

    Aktivitet::skrivData();         //Klassefunksjon
    cout << "\nAktivitet beskrivelse: " << beskrivelse;
}


/**
 *  Destructor som sletter HELT begge vectorenes allokerte innhold.
 */
Dag :: ~ Dag() {

/*
    while(!tidsbegrensedeAktiviteter.empty()){
        delete tidsbegrensedeAktiviteter[tidsbegrensedeAktiviteter.size()-1];
        tidsbegrensedeAktiviteter.pop_back();
    }

    while(!heldagsAktiviteter.empty()){
        delete heldagsAktiviteter[heldagsAktiviteter.size()-1];
        tidsbegrensedeAktiviteter.pop_back();
    }
    cout << "\nSletter alt i aktivitets klassene.";
*/
    
    //Litt usikker på hvorfor funksjonen over ikke fungerer
    //Får en segmentation double dump (tcache)
    for(int i = 0; i < heldagsAktiviteter.size(); i++)  //Går gjennom alle
        delete heldagsAktiviteter[i];                   //Sletter

    heldagsAktiviteter.clear();                         //Fjerner hele

    for(int i = 0; i < tidsbegrensedeAktiviteter.size(); i++)
        delete tidsbegrensedeAktiviteter[i];

    tidsbegrensedeAktiviteter.clear();

    cout << "Slettet alle vector klasser" << endl;
}

/**
 *  Finner ut om selv er en gitt dato eller ei.
 *
 *  @param   dag     -  Dagen som skal sjekkes om er egen dag
 *  @param   maaned  -  MÃ¥neden som skal sjekkes om er egen mÃ¥ned
 *  @param   aar     -  Ã…ret som skal sjekkes om er eget Ã¥r
 *  @return  Om selv er en gitt dato (ut fra parametrene) eller ei
 */
                //Litt uryddig?
bool Dag::harDato(const int dag, const int maaned, const int aar) const {
    
    for(int i = 0; i < gDagene.size(); i++){
        if(gDagene[i]->dagNr == dag){
            if(gDagene[i]->maanedNr == maaned){
                if(gDagene[i]->aarNr == aar) return true;
            }
        }
    }
    return false;
}

/**
 *  Oppretter, leser og legger inn en ny aktivitet pÃ¥ dagen.
 *
 *  @see   Tidsbegrenset::lesData()
 *  @see   Heldags::lesData()
 */
void Dag::nyAktivitet()  {
    char temp;

    //Bestemmer type aktivitet
    temp = lesChar("Er aktiviteten (Heldags - H) (tidsbegrenset - T)");

    if (temp == 'H'){       //Hva slags aktiviet
        Heldags* aktivitetH; aktivitetH = new Heldags;  
        aktivitetH->lesData();                              //Leser data
        heldagsAktiviteter.push_back(aktivitetH);           //Legger bakerst
    }else{
        Tidsbegrenset* aktivitetT; aktivitetT = new Tidsbegrenset;
        aktivitetT->lesData();                              //Leser data
        tidsbegrensedeAktiviteter.push_back(aktivitetT);    //Legger bakerst
    }
}

/**
 *  Skriver ut ALLE aktiviteter pÃ¥ egen dato (og intet annet).
 *
 *  @see   Heldags::skrivData()
 *  @see   Tidsbegrenset::skrivData()
 */
void Dag::skrivAktiviteter() const {

    if(!heldagsAktiviteter.empty()){            //Finnes noe i vector
        cout << "Heldagsaktiviteter\n--------------------" << endl;
        for(int i = 0; i < heldagsAktiviteter.size(); i++){
            cout << "\n\n\tNr. " << i+1 << endl;
            heldagsAktiviteter[i]->skrivData(); //Skriver data
        }
    }

    if(!tidsbegrensedeAktiviteter.empty()){
        cout << "\n\nTidsbegrensede aktivitet\n--------------------\n\n";
        for(int i = 0; i < tidsbegrensedeAktiviteter.size(); i++){
            cout << "\tNr. " << i+1 << endl;
            tidsbegrensedeAktiviteter[i]->skrivData();  //Skriver data
        }
    }
}

/**
 *  Skriver KUN ut egen -dato.
 */
void Dag::skrivDato() const {
    cout << "\nAktiviteter for dato: " << dagNr << "." << maanedNr << "." 
    << aarNr << endl;
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
            if(aar >= 1990 && aar <= 2030) return true; //Hvis er gyldig
        }
    }
    return false;
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
            return gDagene[i];  //Hvis finnes, returner peker.
        else return nullptr;
    }
}


/**
 *  Frigir/sletter ALLE dagene og ALLE pekerne i 'gDagene'.
 */
void frigiAllokertMemory()  {
    /*
    while(!gDagene.empty()){                //Så lengde en dag fines
        delete gDagene[gDagene.size()-1];   //sletter bestemt dag
        gDagene.pop_back();                 //fjerner bakerste dag
    }
    */

   //Bruker denne i stedet
    for(int i = 0; i < gDagene.size(); i++) //Gjennom alle dagene
        delete gDagene[i];                  //Slettter
    gDagene.clear();                        //Fjerner alle ementer fra vector
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
    Dag* temp;                                  //Midlertidig peker

    for(int i = 0; i < gDagene.size(); i++){
        cout << "Dato " << i+1 << ": ";
        gDagene[i]->skrivDato();                //Skirver ut alle datoer
    }
        do{
            cout << "Skriv inn en ny dato:" << endl;
            cout << "Hvilket år: "; cin >> aar;
            cout << "Hvilken mnd: "; cin >> mnd;
            cout << "Hvilken dag: "; cin >> dag;
            cout << "\n\n";
        }while(!dagOK(dag, mnd, aar));          //Gyldig dato?

    if(!finnDag(dag, mnd, aar)){                //Hvis ikke funnet lage ny
        cout << "Ny dag" << endl;
        Dag* nyDag;
        nyDag = new Dag(dag, mnd, aar);
        nyDag->nyAktivitet(); 
        gDagene.push_back(nyDag);
    }else{                                      //Legger til eksiterende dag
        temp = finnDag(dag, mnd, aar);
        cout << "Dagen finnes allerede" << endl;
        temp->nyAktivitet();    //Legger ny aktivitet til eksiterende dag
    }
}

/**
 *  Skriver ut ALLE dagene (MED eller UTEN deres aktiviteter).
 *
 *  @param   inkludertAktiviteter - Utskrifti av ALLE aktivitetene ogsÃ¥, eller ei
 *  @see     Dag::skrivDato()
 *  @see     Dag::skrivAktiviteter()
 */
void skrivDager(const bool inkludertAktiviteter)  {
    if(!gDagene.empty()){
        for(int i = 0; i < gDagene.size(); i++){    //Skriver ut alle
            gDagene[i]->skrivDato();                //
            if(inkludertAktiviteter) gDagene[i]->skrivAktiviteter();
        }
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
    Dag *temp;
    int dag, mnd, aar;
    //skrivDager(!gDagene.empty());

    for(int i = 0; i < gDagene.size(); i++){
        cout << "Dato nr " << i+1 << ": ";
        gDagene[i]->skrivDato();
    }

    do{
        cout << "Hvilken dato vil du skrive ut" << endl;
            cout << "Hvilket år: "; cin >> aar;
            cout << "Hvilken mnd: "; cin >> mnd;
            cout << "Hvilken dag: "; cin >> dag;
    }while(!dagOK(dag, mnd, aar));

    if(temp = finnDag(dag, mnd, aar))
        temp->skrivAktiviteter();   //Skrier ut finner Aktivitet
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