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

struct Rute
{
    vector<string> stopp;
    int ruteNr,
        totMin;
};

vector<Rute *> gRuter;

const int ANTSTOPP = 11;

const vector<string> gBusstopp = // Navn på alle busstoppene
    {"Skysstasjonen", "Fahlstroms plass", "Sykehuset",
     "Gjovik stadion", "Bergslia", "Overby", "Nybrua",
     "NTNU", "Kallerud", "Hunndalen", "Mustad fabrikker"};

const int gMinutter[ANTSTOPP][ANTSTOPP] =
    {{0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Skysstasjonen = 0
     {3, 0, 3, 0, 0, 0, 3, 0, 0, 0, 4}, // Fahlstrøms plass = 1
     {0, 3, 0, 1, 0, 0, 0, 0, 0, 0, 0}, // Sykehuset = 2
     {0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0}, // Gjøvik stadion = 3
     {0, 0, 0, 3, 0, 2, 0, 0, 0, 0, 0}, // Bergslia = 4
     {0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0}, // Øverby = 5
     {0, 3, 0, 0, 0, 0, 0, 2, 0, 0, 2}, // Nybrua = 6
     {0, 0, 0, 0, 0, 0, 2, 0, 0, 4, 0}, // NTNU = 7
     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, // Kallerud = 8
     {0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2}, // Hunndalen = 9
     {0, 4, 0, 0, 0, 0, 2, 0, 0, 2, 0}};

void skrivMeny();
void nyRute();
void skrivStopp();
void skrivRuter();
void ruteSkrivData(const Rute rute);
void skrivNesteStoppesteder(const int stopp);
bool ruteLesData(Rute &rute);
void slettRute();
void slett();
void slett(const int nr);

/*
 *   Hovedprogrammet
 */

int main()
{

    char svar;
    cout << "Hei og velkommein til programmet mitt <3" << endl;

    skrivMeny();
    svar = lesChar("Skriv inn en kommando");
    while (svar != 'Q')
    {

        switch (svar)
        {
        case 'N':
            nyRute();       break;
        case 'A':
            skrivRuter();   break;
        case 'B':
            skrivStopp();   break;
        case 'S':
            slettRute();    break;
        default:
            skrivMeny();
        }
        svar = lesChar("Skriv inn en kommando");
    }
    slett();
}

/*
 *   Funksjon som kaller funksjoner som enten sletter en bestemt
 *   rute eller alle sammen.
 *
 *   @see lesInt() fra "lesData2.h"
 *   @see slett(const int nr)
 *   @see slett()
 */

void slettRute()
{
    if (gRuter.size() > 0)
    {
        cout << "(-1) slett alle, (0) ingen slettes." << endl;

        int temp = lesInt("Hvilken rute skal slettes ", -1, gRuter.size());

        if (temp == -1)
        {
            slett(); // Funksksjon som sletter alt
        }
        else if (temp > 0)
        {
            slett(temp); // Sletter en bestemt
        }
        else
            cout << "Ingen ruter ble slettet" << endl;
    }
    else
        cout << "Det er ingen registrerte ruter" << endl;
}

/*
 *   Sletter en bestemt rute
 */

void slett(const int nr)
{ // Sletter en enkelt

    delete gRuter[nr - 1];                      // Sletter vector struct variabel
    gRuter[nr - 1] = gRuter[gRuter.size() - 1]; // Setter bakerste til nr fjernet
    gRuter.pop_back();                          // Fjerner bakerste
}

/*
 *   Sletter alle ruter som har blitt registrert. Den blir
 *   tilkalt når bruker ønsker å slette rute nr (-1).
 */

void slett()
{
    while (!gRuter.empty())
    {                                     // Så lengde det er noe finnes
        delete gRuter[gRuter.size() - 1]; // Sletter bakerste
        gRuter.pop_back();                // Fjerner bakersteplass
    }
    cout << "Alt ble slettet" << endl;
}

/*
 *   Lager en ny bussrute. Den sjekker om rute består av flere
 *   enn et stopp. Hvis den gjør det, blir den lagret.
 *
 *   @see ruteLesData()
 *   @see ruteSkrivData()
 */

void nyRute()
{

    Rute *nyRute;      // Lager ny peker variabel
    nyRute = new Rute; // Allokerer plass for pekeren
                       // Hvis totMin < 0, slett allokert plass.
    if (ruteLesData(*nyRute) == false)
        delete nyRute;
    else
    {
        gRuter.push_back(nyRute); // Legger ny rute bakerst
        ruteSkrivData(*nyRute);   // Skriver ut data for nylig laget rute
    }
}

/*
 *   Skriver ruten som ble kjørt
 *   Får tilsendt struct variabel
 *
 *   @see skrivStopp()
 *   @see skrivNesteStoppeSteder()
 */

void ruteSkrivData(const Rute rute)
{

    cout << "Rute nr. " << rute.ruteNr << endl;
    cout << "Ruten tok totalt " << rute.totMin << " minutter.\n\n";

    cout << rute.stopp[0]; // Første stoppet
    for (int i = 0; i < rute.stopp.size() - 1; i++)
    {                                         // Alle rutene som finnes
        cout << " --> " << rute.stopp[i + 1]; // Resten av stoppene
    }
    cout << "\n\n";
}
/*
 *   Funksjon som leser inn data for ny rute. Den bruker
 *   refferanseoverføring for å hele tiden oppdatere ruten.
 *
 *   @param skrivStopp()
 *   @param skrivNesteStoppeSteder()
 */

bool ruteLesData(Rute &rute)
{
    int stopp, temp = 0;
    rute.totMin = 0;                           // Nullstiller
    rute.ruteNr = lesInt("Rute nr: ", 1, 500); // Bestemmer rutenr

    do
    {
        if (stopp - 1 == 8)
            cout << "\nKan ikke dra videre fra "
                 << gBusstopp[stopp - 1] << "\n\n";

        skrivStopp();

        stopp = lesInt("\nVelg et startsted", 1, 11);

    } while (stopp - 1 == 8); // Kan ikke starte på Kallerud, ingen vei videre

    cout << "\nDu vil starte på stoppet " << gBusstopp[stopp - 1] << endl;
    (rute.stopp).push_back(gBusstopp[stopp - 1]);

    while (1)
    { // Kjøres helt til noe returneres.

        skrivNesteStoppesteder(stopp - 1); // Skriver aktuelle stopp
        temp = stopp;                      // Vite hvor bussen var tidligere

        do
        { // Kan bare skrive inn lovlige verdier.
            stopp = lesInt("Hva skal det neste stoppet være: ", 0, 11);
        } while ((gMinutter[temp - 1][stopp - 1]) == 0 && stopp != 0);

        if (stopp == 0)
        { // 0 betyr at turen skal stoppes.
            if (rute.totMin != 0)
                return true;
            else
                return false;
        }

        (rute.stopp).push_back(gBusstopp[stopp - 1]);  // Setter busstoppet
        rute.totMin += gMinutter[temp - 1][stopp - 1]; // Legger til ant. min
    }
}

/*
 *   Skirver neste stopp, avhengig av hvilket stopp en befinner seg på.
 */

void skrivNesteStoppesteder(const int stopp)
{

    cout << "Neste stopp kan være: \n";

    for (int i = 0; i < ANTSTOPP; i++)
    {
        if (gMinutter[stopp][i] != 0)
        {                                         // Stopp som har vedrdi på linje
            cout << setw(2) << "nr. " << i + 1    // Hvilket nr
                 << "\t" << gBusstopp[i] << endl; // Selve stoppet
        }
    }
}

/*
 *   Funksjon som skirver ut alle rutene som er registrert. Bruker
 *   en annen funksjon for å skirve ut selve dataen.
 *
 *   @see ruteSkrivData()
 */

void skrivRuter()
{
    cout << "\n\n**Registrerte bussruter**\n\n";

    for (int i = 0; i < gRuter.size(); i++) // Alle rutene
        ruteSkrivData(*gRuter[i]);          // via peker
}

/*
 *   Skriver alle busstoppen som finnes
 */

void skrivStopp()
{
    for (int i = 0; i < ANTSTOPP; i++)
        cout << setw(2) << i + 1 << ". " << gBusstopp[i] << endl;
}

/*
 *   Skirver menyen for ulik funksjonalitet
 */

void skrivMeny()
{

    cout << "\nDette er menyen;"
         << "\n\tNy rute (N)"
         << "\n\tSlett rute (S)"
         << "\n\tSkriv alle ruter (A)"
         << "\n\tSkriv alle busstopp (B)\n\n";
}