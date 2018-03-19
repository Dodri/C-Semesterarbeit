//Semesterarbeit WS 17/18 C++ Programmieren 1 - Karolin Gärtner & Dominik Sloboda

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//Funktionsdeklaration
void printHeader(int sum);
char getCommand();
void doCommand(char command);

//Klasse für Platten
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

//Array aus Platten-Objekten
Platte plattenArr[10];

//Zuweisung von Klassenattributen aus der XML-Datei
void zuweisen(string tagname, string taginhalt, int i){
    if(tagname == "Albumtitel")
    {
        plattenArr[i].Albumtitel = taginhalt;
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

//Einlesen der .txt Datei & Zuweisen der in der .txt gespeicherten Informationen
void parseTxt(){
//Input-Stream wird erstellt
    ifstream txtin;
    txtin.open("Musiker.txt");

    string line; //eingelesene Zeile

    while(getline(txtin, line)){
        string array[4]; //Array für die Attribute aus der .txt
        int arr_i = 0;

        istringstream iss(line); //Leerstellen-getrenntes Einlesen der Zeilen
        do {
               string subs;
               iss >> subs;
               array[arr_i] = subs;
               arr_i++;
        } while (iss);
        string id = array[0];
        int Bestand = stoi(array[1], nullptr);
        float Preis = stof(array[2]);
        for(int i = 0; i < 10; i++) { //Durchlaufen aller Platten-Objekte
            if(plattenArr[i].id.compare(id) == 0) { //gibt es einen passenden Datensatz in der .txt dann werden die Werte der Platte zugewiesen
                plattenArr[i].Bestand = Bestand;
                plattenArr[i].Preis = Preis;
            }
        }
    }


}
//Funktion zum Konvertieren (Zusammenführen) der beiden Eingabedateien -> Stufe 1
void konvertieren(){
//Platten-Objekte werden in XML-Formate ausgegeben
    ofstream outputFile;
    outputFile.open("Output.xml");

    outputFile << "<Plattenliste>" << endl;
//Platten werden durchgegangen und ausgegeben
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

//Ausgabe eines Plattenobjektes auf CLI-Ebene, referenziert nach Array-Index
void ausgabe(int index) {
    cout << "ID: " + plattenArr[index].id << endl;
    cout << "Albumtitel: " + plattenArr[index].Albumtitel << endl;
    cout << "Kuenstler: " + plattenArr[index].Kuenstler << endl;
    cout << "Ort: " + plattenArr[index].Ort << endl;
    cout << "Plattenladen: " + plattenArr[index].Plattenladen<< endl;
    cout << "Bestand: " + to_string(plattenArr[index].Bestand) << endl;
    cout << "Preis " + to_string(plattenArr[index].Preis) << endl;
}

//Suchfunktion, Menuepunkt (s) -> Stufe 2
void search(){
    string cat;
    string searchFor;

//Einlesen der Suchparameter
    cout << "Kategorie: ";
    cin >> cat;
    cout << "Suchen: ";
    cin >> searchFor;

    bool foundSth = false; //Flag: Suchergebnis gefunden
//Durchlaufen der Plattenobjekte
    for(int i = 0; i < 10; i++){
        if( //Liegt Kategorie-String in Attribut? UND liegt Suchbegriff in Attributwert? -> passenden Datensatz gefunden
                (cat.find('id') != string::npos && plattenArr[i].id.find(searchFor)!= string::npos)
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
//wenn Flag nicht getriggert wurde, dann Fehlermeldung ausgeben
    if(!foundSth) {
        cout << "Keine Platten gefunden" << endl;
    }
}

//Parsen der XML-Datei
void parseXML(){
    char zeichen;
    char puffer[1024]; //Zwischenspeicher
    int zaehler;
    int zustand;
    string tagname;
    char taginhalt[1024];
    int tagzaehler = 0; //zaehlt Zeichenlaenge der Tags
    int i = 0; //Zaehlvariable

    ifstream eingabe; //Eingabestream

    eingabe.open("Musiker.xml");

    bool isid = false;

    for (zaehler=0,zustand=0;;)
        {
        eingabe.get(zeichen); //zeichenweises Durchlaufen der XML-Datei
        if (eingabe.eof()) break;

       switch(zeichen) //Unterscheiden von XML-Steuerzeichen
          {
       case '<': //Tag wird geoeffnet
          taginhalt[tagzaehler]='\0'; //leeren des Taginhaltes

          zuweisen(tagname, taginhalt, i); //Schreiben des zuletzt gelesenen vollstaendigen Tags

          if(tagname == "/Platte"){ //Ende eines Platten-Objektes
              i++;
          }

          tagzaehler = 0; //Zaehler wird zurueckgesetzt
          zustand=1; //Zustand zeigt an, dass Tag offen ist
          zaehler=0;
          break;

       case '>': //Tag wird geschlossen
          puffer[zaehler] = '\0'; //Taginhalt String terminieren
          tagname = puffer;
          zustand=0; //Zustand zeigt an, dass Tag geschlossen ist
          break;

       case ' ': //Behandlung von Leerzeichen

          if (zustand == 1) //ist Tag offen
             {
             zustand = 2; //Beginn eines XML-Attributes
             puffer[zaehler] = '\0';
             zaehler=0;
             }
          else if (zustand == 4) //Attribut zu Ende gelesen
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
             zustand = 3; //Attributwert wird gelesen
             puffer[zaehler] = '\0';
             string compare = puffer;
             if(compare == "xml:id"){ //Attribut = id?
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

       case '"': //Anfang oder Ende des Attributwertes
          if (zustand == 3)
             {
             zustand = 4;
             zaehler = 0;
             }
          else if (zustand == 4)//Ende des Attributes erreicht? Dann Plattenobjekt der gelesenen ID zuweisen
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

       default: //Behandlung jedes anderen Zeichens als XML-Steuerzeichen
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

int Plattenanazahl(){ //Anzahl der Objekte in Plattenarray bestimmen
    return(sizeof(plattenArr)/sizeof(*plattenArr));

}

int main()
{
    char command;

    parseXML(); //einlesen der XML-Datei
    parseTxt(); //einlesen der TXT-Datei & Zusammenfuehren der Attribute

    printHeader(Plattenanazahl()); // Ausgabe des Menues

    do {
        command = getCommand(); //Endlosschleife zum Einlesen neuer Befehle
        doCommand(command);
    } while(command != 'e'); // Buchstabe e fuehrt zur Beenden der Endlosschleife

}

void printHeader(int sum) { //Menuführung
    cout << "Plattensammlung" << endl;
    cout << "Platten " << sum << endl;
    cout << endl;
    cout << "(k)onvertieren der Daten" << endl;
    cout << "(s)uchen" << endl;
    cout << "(e)nde" << endl;
}

char getCommand() { //Einlesen eines CLI- Befehles und Rueckgabe als Return-Value
    char menu_option;

    cout << "command:";
    cin >> menu_option;

    return menu_option;
}

void doCommand(char command) { // Ausfuehren eines uebergebenden CLI-Befehles, oder Ausgabe einer Fehlermeldung
    switch(command) {
        case 'k':
            konvertieren();
            break;
        case 's':
        search();

            break;

        case 'e':
            cout << "Tschuess!";
        break;

        default:
            cout << "Bitte geben Sie ein gueltiges Kommando ein!" << endl;
    }
}
