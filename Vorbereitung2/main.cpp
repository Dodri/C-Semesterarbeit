/*#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void printHeader(int sum);
char getCommand();
void doCommand(char command);

void convertData(string source1, string source2);
void search(string keyword);
void help();

int main() {
    int sum = 0;
    char command;

    printHeader(sum);

    do {
        command = getCommand();
        doCommand(command);
    } while(command != 'e');

    return 0;
}

void printHeader(int sum) {
    cout << "**** stocktaking cli 2018 ****" << endl;
    cout << "******************************" << endl;
    cout << "€: " << sum << endl;
    cout << endl;
    cout << "(c)onvert data" << endl;
    cout << "(s)earch" << endl;
    cout << "(h)elp" << endl;
}

char getCommand() {
    char menu_option;

    cout << "command:";
    cin >> menu_option;

    return menu_option;
}

void doCommand(char command) {
    switch(command) {
        case 'c':
            convertData("", "");
            break;
        case 's':
            search("");
            break;
        case 'h':
            help();
            break;
        case 'e':
            cout << "Good bye.";
            break;
        default:
            cout << "Please insert an command from above or use 'h' for help" << endl;
    }
}

void convertData(string source1, string source2) {
    cout << "convert data" << endl;
}

void search(string keyword) {
    cout << "search" << endl;
}

void help() {
    cout << "help" << endl;
}
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

using namespace std;

struct Platte {
    string Platte;
    string id;
    string Albumtitel;
    string Kuenstler;
    string Ort;
    string Plattenladen;
    string Bestand;
    string Preis;
};

struct txt {
    int id;
    int Bestand;
    float Preis;
};

void zuweisen(string tagname, string taginhalt, Platte *platte){
    if(tagname == "Albumtitel")
    {
        platte->Albumtitel = taginhalt;
      //break;
    }

}

void parseTxt(){

    ifstream txtin;
    ifstream txtin2;

    txtin.open("Musiker.txt");

    txtin2.open("Musiker.txt");

    string line;
    int zaehler = 0;

    while(getline(txtin, line)){
        zaehler++;
    }

    txt List[zaehler];

    int zaehler2 = 0;

    while(getline(txtin2, line)){

        txt Item;
        string array[4];
        int i = 0;

        istringstream iss(line);
           do {
               string subs;
               iss >> subs;
               array[i] = subs;
               i++;
           }
        while (iss);

        Item.id = stoi(array[0], nullptr);
        Item.Bestand = stoi(array[1], nullptr);
        Item.Preis = stof(array[2]);

        List[zaehler2] = Item;
        zaehler2++;
    }

int i;
    for(i = 0; i < zaehler; i++){
        cout << List[i].id << endl;
    }
    }



int main()
{
char zeichen;
char puffer[100];
int zaehler;
int zustand;
string tagname;
char taginhalt[666];
int tagzaehler;

/* Dabei möge gelten:
   zustand = 0 : Wir sind "zwischen den Tags".
   zustand = 1 : Wir bearbeiten einen Tagnamen.
   zustand = 2 : Wir erwarten einen Attributnamen.
   zustand = 3 : Wir erwarten einen Attributwert.
   zustand = 4 : Wir verarbeiten einen Attributwert. */

ifstream eingabe;

eingabe.open("Musiker.xml");

Platte platte;

bool isid = false;
bool isplatte = false;
bool isalbumtitel = false;


for (zaehler=0,zustand=0;;)
    {
    eingabe.get(zeichen);
    if (eingabe.eof()) break;

   switch(zeichen)
      {
   case '<':
      taginhalt[tagzaehler]='\0';

      zuweisen(tagname, taginhalt, &platte);

      cout << taginhalt << endl;
      tagzaehler = 0;
      zustand=1;
      zaehler=0;
      break;

   case '>':
      puffer[zaehler] = '\0';
      tagname = puffer;
      zustand=0;
      break;

   case ' ':
// Wenn wir bisher einen Tagnamen gelesen haben, endet er.
      if (zustand == 1)
         {
         zustand = 2;
         puffer[zaehler] = '\0';

         cout << puffer << endl;
         zaehler=0;
         }
      else if (zustand == 4)
         {
         puffer[zaehler] = zeichen;
         zaehler++;
         }
      else if (zustand == 0){
          taginhalt[tagzaehler] = ' ';
          tagzaehler++;
      }
      break;

   case '=':
      if (zustand == 2)
         {
         zustand = 3;
         puffer[zaehler] = '\0';
         string compare = puffer;
         if(compare == "xml:id"){
             isid = true;
         }
         cout << puffer << endl;
         zaehler=0;
         }
      else if (zustand == 4)
         {
         puffer[zaehler] = zeichen;
         zaehler++;

         }
      else cout << "Fehlerhaftes Zeichen! '='" << endl;
      break;

   case '"':
      if (zustand == 3)
         {
         zustand = 4;
         zaehler = 0;
         }
      else if (zustand == 4)
         {
         zustand = 2;
         puffer[zaehler] = '\0';
         if(isid) {
             string compare = puffer;
             platte.id = compare;
         }
         cout << puffer << endl;
         zaehler=0;
         }
      else cout << "Fehlerhaftes Zeichen! '\"'" << endl;
      break;

   default:
      if (zustand != 0)
         {
         puffer[zaehler] = zeichen;
         zaehler++;
         }
      if(zustand == 0){
        taginhalt[tagzaehler] = zeichen;
        tagzaehler++;

      }
      break;
      }
   }
eingabe.close();

parseTxt();
}
