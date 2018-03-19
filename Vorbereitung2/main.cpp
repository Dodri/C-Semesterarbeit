#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void printHeader(int sum);
char getCommand();
void doCommand(char command);


struct Platte {
    string Platte;
    string id;
    string Albumtitel;
    string Kuenstler;
    string Ort;
    string Plattenladen;
    int Bestand;
    float Preis;
};



Platte plattenArr[10];

void zuweisen(string tagname, string taginhalt, int i){
    if(tagname == "Albumtitel")
    {
        plattenArr[i].Albumtitel = taginhalt;
        //platte->Albumtitel = taginhalt;
      //break;
    } else if(tagname == "id") {
        plattenArr[i].id = taginhalt;
    } else if(tagname == "Kuenstler") {
        plattenArr[i].Kuenstler = taginhalt;
    } else if(tagname == "Ort") {
        plattenArr[i].Ort = taginhalt;
    } else if(tagname == "Plattenladen") {
        plattenArr[i].Plattenladen = taginhalt;
    }


}

void parseTxt(){

    ifstream txtin;
    txtin.open("Musiker.txt");

    string line;
    int zaehler = 0;
    int i;

    while(getline(txtin, line)){
        string array[4];
        int arr_i = 0;

        istringstream iss(line);
        do {
               string subs;
               iss >> subs;
               array[arr_i] = subs;
               arr_i++;
        } while (iss);
        string id = array[0];
        int Bestand = stoi(array[1], nullptr);
        float Preis = stof(array[2]);
        zaehler++;
        for(i = 0; i < 10; i++) {
            if(plattenArr[i].id.compare(id) == 0) {
                plattenArr[i].Bestand = Bestand;
                plattenArr[i].Preis = Preis;
            }
        }
    }


}

void konvertieren(){

    ofstream outputFile;
    outputFile.open("Output.xml");

    outputFile << "<Plattenliste>" << endl;

    int i = 0;
    for(i = 0; i < 10; i++){
        outputFile << "<Platte xml:id=\""+ plattenArr[i].id + "\">" << endl;
        outputFile << "\t<Albumtitel>" + plattenArr[i].Albumtitel + "</Albumtitel>" << endl;
        outputFile << "\t<Kuenstler>" + plattenArr[i].Kuenstler + "</Kuenstler>" << endl;
        outputFile << "\t<Ort>" + plattenArr[i].Ort + "</Ort>" << endl;
        outputFile << "\t<Plattenladen>" + plattenArr[i].Plattenladen + "Plattenladen" << endl;
        outputFile << "\t<Bestand>" + to_string(plattenArr[i].Bestand) + "</Bestand>" << endl;
        outputFile << "\t<Preis>" +  to_string(plattenArr[i].Preis) + "</Preis>" << endl;
        outputFile << "</Platte>" << endl;

    }

    outputFile << "</Plattenliste>" << endl;

    outputFile.close();

}

void ausgabe(int index) {
    cout << "ID: " + plattenArr[index].id << endl;
    cout << "ALbumtitel: " + plattenArr[index].Albumtitel << endl;
    cout << "Kuenstler: " + plattenArr[index].Kuenstler << endl;
    cout << "Ort: " + plattenArr[index].Ort << endl;
    cout << "Plattenladen: " + plattenArr[index].Plattenladen<< endl;
    cout << "Bestand: " + to_string(plattenArr[index].Bestand) << endl;
    cout << "Preis " + to_string(plattenArr[index].Preis) << endl;
}

void search(){
    string Category;
    string searchFor;

    cout << "Category: ";
    cin >> Category;
    cout << "Search for: ";
    cin >> searchFor;

    string cat = Category; //transform(Category.begin(), Category.end(), Category.begin(), tolower);

    bool foundSth = false;
    string Attribut;

    for(int i = 0; i < 10; i++){
        if(
            (cat.find('id') != string::npos && plattenArr[i].id.find(searchFor) != string::npos)
            || (cat.find("Albumtitel") != string::npos && plattenArr[i].Albumtitel.find(searchFor) != string::npos)
            || (cat.find("Kuenstler") != string::npos && plattenArr[i].Kuenstler.find(searchFor) != string::npos)
            || (cat.find("Ort") != string::npos && plattenArr[i].Ort.find(searchFor) != string::npos)
            || (cat.find("Plattenladen") != string::npos && plattenArr[i].Plattenladen.find(searchFor) != string::npos)
            || (cat.find("Bestand") != string::npos && to_string(plattenArr[i].Bestand).find(searchFor) != string::npos)
            || (cat.find("Preis") != string::npos && to_string(plattenArr[i].Preis).find(searchFor) != string::npos)
        ) {
            ausgabe(i);
            foundSth = true;
        }
    }

    if(!foundSth) {
        cout << "Keine Platten gefunden" << endl;
    }
}

void parseXML(){
    char zeichen;
    char puffer[100];
    int zaehler;
    int zustand;
    string tagname;
    char taginhalt[666];
    int tagzaehler = 0;
    int i = 0;

    /* Dabei möge gelten:
       zustand = 0 : Wir sind "zwischen den Tags".
       zustand = 1 : Wir bearbeiten einen Tagnamen.
       zustand = 2 : Wir erwarten einen Attributnamen.
       zustand = 3 : Wir erwarten einen Attributwert.
       zustand = 4 : Wir verarbeiten einen Attributwert. */

    ifstream eingabe;

    eingabe.open("Musiker.xml");

    Platte platte; //= new Platte();
    //platte =(struct Platte*) malloc(sizeof(Platte));


    bool isid = false;
   /* bool isplatte = false;
    bool isalbumtitel = false; */


    for (zaehler=0,zustand=0;;)
        {
        eingabe.get(zeichen);
        if (eingabe.eof()) break;

       switch(zeichen)
          {
       case '<':
          taginhalt[tagzaehler]='\0';

          zuweisen(tagname, taginhalt, i);

          if(tagname == "/Platte"){

              i++;
              platte = {};

          }

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

          if (zustand == 1)
             {
             zustand = 2;
             puffer[zaehler] = '\0';
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
             zaehler=0;
             }
          else if (zustand == 4)
             {
             puffer[zaehler] = zeichen;
             zaehler++;

             }
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
                 zuweisen("id", compare, i);
             }
             zaehler=0;
             }
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
}

int Plattenanazahl(){
    return(sizeof(plattenArr)/sizeof(*plattenArr));

}

int main()
{
    char command;

    parseXML();
    parseTxt();

    printHeader(Plattenanazahl());

    do {
        command = getCommand();
        doCommand(command);
    } while(command != 'e');

}

void printHeader(int sum) {
    cout << "Plattensammlung" << endl;
    cout << "Platten " << sum << endl;
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
            konvertieren();
            break;
        case 's':
        search();

            break;
        case 'h':

            break;
        case 'e':
            cout << "Good bye.";
            break;
        case 'p':

        default:
            cout << "Please insert an command from above or use 'h' for help" << endl;
    }
}
