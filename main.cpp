
#include <iostream>
#include <string>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <iomanip>
#include <chrono> 
#include <thread> 
#include <vector>
#include <cctype>


#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>



using namespace std;
using namespace sql;

//struct
struct HistoriaSprzedazy {
    string nazwa;
    double ilosc;
    double cenaSprzedazy;
};
struct WalutaUzytkownika {
    string nazwa;
    double ilosc;
    double cenaZakupu;
};
struct DaneUzytkownika {
    string login;
    string haslo;
    double saldo;
    vector<WalutaUzytkownika> waluty;
    
};

struct Waluta {
    string nazwa;
    double cena;
};


// Deklaracja funkcji
void Zalogowany(DaneUzytkownika& uzytkownik);
void zarejestrujKonto();
void wplata(DaneUzytkownika& uzytkownik);
void wyplata(DaneUzytkownika& uzytkownik);
void zapiszDane(DaneUzytkownika& uzytkownik);
void wczytajDane(DaneUzytkownika& uzytkownik);
void zainwestuj(DaneUzytkownika& uzytkownik);
void sprawdzInwestycje(DaneUzytkownika& uzytkownik);


unique_ptr<Connection> getConnection() {
    Driver* driver;
    unique_ptr<Connection> con(nullptr);

    driver = get_driver_instance();
    con.reset(driver->connect("tcp://127.0.0.1:3306", "root", "12345")); // Polaczenie z baza danych MySQL
    con->setSchema("demo");

    return con;
}



double generujZmianeCeny(double cena) {
    double zmianaProcentowa = ((double)rand() / RAND_MAX) * 0.06 - 0.03; // Losowa zmiana od -3% do +3%
    return cena * (1.0 + zmianaProcentowa);
}

void sprzedajInwestycje(DaneUzytkownika& uzytkownik) {
}


void zainwestuj(DaneUzytkownika& uzytkownik) {
    system("cls");
    double aktualneSaldo = uzytkownik.saldo;
    const int walutyNaStrone = 6;
    int aktualnaStrona = 0;
    const int iloscWalut = 29;
    bool przewinietoStrone = false;
    Waluta waluty[iloscWalut] = {
        {"USD - United States Dollar", 3.79}, {"EUR - Euro", 4.28}, {"GBP - British Pound Sterling", 5.20},
        {"AUD - Australian Dollar", 2.80}, {"CAD - Canadian Dollar", 3.10}, {"CHF - Swiss Franc", 4.15},
        {"NZD - New Zealand Dollar", 2.60}, {"SEK - Swedish Krona", 0.45}, {"NOK - Norwegian Krone", 0.43},
        {"DKK - Danish Krone", 0.59}, {"CNY - Chinese Yuan", 0.58}, {"CZK - Czech Republic Koruna", 0.16},
        {"MXN - Mexican Peso", 0.19}, {"BRL - Brazilian Real", 0.70}, {"MYR - Malaysian Ringgit", 0.88},
        {"SGD - Singapore Dollar", 2.80}, {"THB - Thai Baht", 0.12}, {"TRY - Turkish Lira", 0.50},
        {"ZAR - South African Rand", 0.26}, {"AED - Emirati Dirham", 1.03}, {"KWD - Kuwaiti Dinar", 12.45},
        {"BHD - Bahraini Dinar", 10.05}, {"JOD - Jordanian Dinar", 5.34}, {"LYD - Libyan Dinar", 2.85},
        {"TND - Tunisian Dinar", 1.39}, {"BSD - Bahamian Dollar", 3.79}, {"BZD - Belize Dollar", 1.89},
        {"TTD - Trinidad and Tobago Dollar", 0.56}, {"XCD - East Caribbean Dollar", 1.40}
    };

    while (true) {
        system("cls");

        cout << "-------------------------MENU-------------------------------" << endl;
        cout << left << setw(40) << "Waluta" << "Cena" << endl;
        cout << setfill('-') << setw(60) << "-" << setfill(' ') << endl;

        for (int i = 0; i < iloscWalut; ++i) {
            waluty[i].cena = generujZmianeCeny(waluty[i].cena);
        }

        for (int i = aktualnaStrona * walutyNaStrone; i < min((aktualnaStrona + 1) * walutyNaStrone, iloscWalut); ++i) {
            cout << left << setw(40) << waluty[i].nazwa << fixed << setprecision(2) << waluty[i].cena << endl;
        }

        if (aktualnaStrona > 0 || przewinietoStrone) {
            cout << "Nacisnij 8, aby wrocic do poprzedniej strony." << endl;
        }

        cout << "Nacisnij 6, aby zaladowac ponownie strone z walutami." << endl;
        cout << "Nacisnij 7, aby kupic walute." << endl;
        cout << "Nacisnij 9, aby zobaczyc kolejne waluty." << endl;
        cout << "Nacisnij 0, aby wrocic do poprzedniego menu." << endl;
        cout << "Aktualne saldo: " << aktualneSaldo << " PLN" << endl;

        char wybor;
        cin >> wybor;

        if (wybor == '9') {
            if ((aktualnaStrona + 1) * walutyNaStrone < iloscWalut) {
                ++aktualnaStrona;
                przewinietoStrone = true;
                system("cls");
            }
        }
        else if (wybor == '0') {
            system("cls");
            break;
        }
        else if (wybor == '8') {
            if ((aktualnaStrona - 1) >= 0) {
                --aktualnaStrona;
                system("cls");
            }
            else {
                cout << "Niepoprawny wybor. Nacisnij dowolny klawisz, aby kontynuowac...";
                cin.ignore();
                cin.get();
                system("cls");
            }

        }
        else if (wybor == '7') {
            system("cls");

            cout << "Wybierz walute do zakupu (numer od 1 do " << min((aktualnaStrona + 1) * walutyNaStrone, iloscWalut) - aktualnaStrona * walutyNaStrone << ")" << endl;

            for (int i = aktualnaStrona * walutyNaStrone; i < min((aktualnaStrona + 1) * walutyNaStrone, iloscWalut); ++i) {
                cout << left << setw(5) << i + 1 << setw(35) << waluty[i].nazwa << fixed << setprecision(2) << waluty[i].cena << endl;
            }
            if (wybor == '6') {
                przewinietoStrone = false;
                system("cls");
            }

            cout << "Nacisnij 0, aby sie cofnac." << endl;
            if (wybor == '0') {
                system("cls");
                break;
            }

            int wyborWaluty;
            cout << "Wybierz opcje: ";
            if (!(cin >> wyborWaluty)) {
                cout << "Niepoprawna wartosc. Wprowadz liczbe calkowita." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nacisnij dowolny klawisz, aby kontynuowac...";
                cin.get(); 
                continue;
            }

            if (wyborWaluty == 0) {
                system("cls");
                continue;
            }
            else if (wyborWaluty == 8) {
                if ((aktualnaStrona - 1) >= 0) {
                    --aktualnaStrona;
                }
                else {
                    cout << "Niepoprawny wybor. Nacisnij dowolny klawisz, aby kontynuowac...";
                    cin.ignore();
                    cin.get();
                    system("cls");
                }
            }
            else if (wyborWaluty == 9) {
                if ((aktualnaStrona + 1) * walutyNaStrone < iloscWalut) {
                    ++aktualnaStrona;
                    system("cls");
                }
            }
            else if (wyborWaluty < 1 || wyborWaluty > min((aktualnaStrona + 1) * walutyNaStrone, iloscWalut) - aktualnaStrona * walutyNaStrone) {
                cout << "Niepoprawny numer waluty. Wybierz numer z listy." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nacisnij dowolny klawisz, aby kontynuowac...";
                cin.get(); 
                continue;
            }

            // Indeks wybranej waluty w tablicy
            int indeksWybranejWaluty = (aktualnaStrona * walutyNaStrone) + wyborWaluty - 1;
            // Użytkownik wybrał walutę do zakupu
            Waluta wybranaWaluta = waluty[indeksWybranejWaluty];

            double iloscDoZakupu;
            cout << "Podaj ilosc " << wybranaWaluta.nazwa << " do zakupu: ";
            if (!(cin >> iloscDoZakupu)) {
                cout << "Niepoprawna wartosc. Wprowadz liczbe rzeczywista." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Nacisnij dowolny klawisz, aby kontynuowac...";
                cin.get(); // Zatrzymuje program, aby użytkownik mógł przeczytać komunikat
                continue;
            }

            if (iloscDoZakupu <= 0) {
                cout << "Niepoprawna ilosc waluty. Wprowadz dodatnia wartosc." << endl;
                continue;
            }

            double cenaZakupu = iloscDoZakupu * wybranaWaluta.cena;

            if (cenaZakupu > uzytkownik.saldo) {
                cout << "Nie masz wystarczajacych srodkow na koncie. Twoje aktualne saldo wynosi: " << uzytkownik.saldo << " PLN." << endl;
                cout << "Nacisnij dowolny klawisz, aby kontynuowac...";
                cin.ignore();
                cin.get();
            }

            else {
                // Odejmowanie kwoty zakupu od salda użytkownika
                uzytkownik.saldo -= cenaZakupu;
                cout << "Zakupiono " << iloscDoZakupu << " " << wybranaWaluta.nazwa << " za " << cenaZakupu << " PLN." << endl;

                // Dodanie zakupionej waluty do listy walut użytkownika
                WalutaUzytkownika zakupionaWaluta;
                zakupionaWaluta.nazwa = wybranaWaluta.nazwa;
                zakupionaWaluta.ilosc = iloscDoZakupu;
                zakupionaWaluta.cenaZakupu = cenaZakupu;
                uzytkownik.waluty.push_back(zakupionaWaluta);
                cout << "Kliknij dowolny klawisz, aby kontynuowac..." << endl;
                cin.ignore();
                cin.get();
                try {
                    unique_ptr<Connection> con = getConnection();
                    unique_ptr<Statement> stmt(con->createStatement());

                    string query = "INSERT INTO zakupione_waluty (login, nazwa, ilosc, cena_zakupu) VALUES ('" + uzytkownik.login + "', '" + wybranaWaluta.nazwa + "', " + to_string(iloscDoZakupu) + ", " + to_string(cenaZakupu) + ")";
                    stmt->execute(query);

                    cout << "Transakcja zakonczona pomyslnie." << endl;
                }
                catch (SQLException& e) {
                    cout << "Blad SQL:" << e.what() << endl;
                }
            }
        }
    }
}


void HistKupna(DaneUzytkownika& uzytkownik) {
    system("cls");
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM zakupione_waluty WHERE login = '" + uzytkownik.login + "'"));

        if (!res->next()) {
            cout << "Nie posiadasz zadnych walut." << endl;
            return;
        }

        // Nagłówki kolumn
        cout << setw(30) << left << "Waluta" << setw(20) << left << "Ilosc"
            << setw(25) << left << "Cena zakupu" << setw(20) << left << "Data zakupu" << endl;
        cout << setfill('-') << setw(120) << "-" << setfill(' ') << endl;

        // Wyświetlanie danych
        do {
            string nazwa = res->getString("nazwa");
            double ilosc = res->getDouble("ilosc");
            double cenaZakupu = res->getDouble("cena_zakupu");
            string dataZakupu = res->getString("data_zakupu");

            // Ustawienie precyzji dla ceny zakupu
            cout << setw(30) << left << nazwa << setw(20) << left << ilosc
                << setw(25) << left << fixed << setprecision(2) << cenaZakupu
                << setw(20) << right << dataZakupu << endl;
        } while (res->next());

        // Wyświetlanie obecnego salda
        cout << "        " << endl;
        cout << "Obecne saldo: " << uzytkownik.saldo << " PLN" << endl;
        cout << "        " << endl;
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
    }
}
void sprawdzSprzedaneWaluty(DaneUzytkownika& uzytkownik) {
    system("cls");
    
}

void sprawdzInwestycje(DaneUzytkownika& uzytkownik) {
    system("cls");

    int wybor;
    do {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Pokaz historie zakupu walut" << endl;
        cout << "2. Pokaz historie sprzedazy walut" << endl;
        cout << "0. Cofnij do menu" << endl;
        cout << "Twoj wybor: ";
        cin >> wybor;

        switch (wybor) {
        case 1:
            // Pokazuje kupione waluty
            HistKupna(uzytkownik);
            break;
        case 2:
            // Pokazuje sprzedane waluty
            sprawdzSprzedaneWaluty(uzytkownik);
            break;
        case 0:
            // Cofa do menu
            return;
        default:
            cout << "Niepoprawny wybor. Sprobuj ponownie." << endl;
        }
    } while (true);
}



string zaloguj() {
    system("cls");
    string login, haslo;
    bool znaleziono = false;

    cout << "Podaj login: ";
    cin >> login;
    cout << "Podaj haslo: ";
    cin >> haslo;

    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM uzytkownicy WHERE login = '" + login + "' AND haslo = '" + haslo + "'"));

        if (res->next()) {
            znaleziono = true;
        }
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
    }

    if (znaleziono) {
        cout << "Zalogowano uzytkownika." << endl;
        return login;
    }
    else {
        cout << "Nieprawidlowe dane logowania." << endl;
        return "";
    }
}

void Zalogowany(DaneUzytkownika& uzytkownik) {
    system("cls");
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM zakupione_waluty WHERE login = '" + uzytkownik.login + "'"));

        // Aktualizuj strukturę DaneUzytkownika
        while (res->next()) {
            WalutaUzytkownika waluta;
            waluta.nazwa = res->getString("nazwa");
            waluta.ilosc = res->getDouble("ilosc");
            waluta.cenaZakupu = res->getDouble("cena_zakupu");
            uzytkownik.waluty.push_back(waluta);
        }
    }
    catch (SQLException& e) {
        cout << "Błąd SQL: " << e.what() << endl;
    }



    int wybor;

    do {
        cout << "    -------------MENU--------------" << endl;
        cout << "Witaj, " << uzytkownik.login << "!" << endl;
        cout << "                                             \n\n";
        cout << "| 1.Sprawdz srodki na koncie            | " << endl;
        cout << "| 2.Wplac Srodki                        | " << endl;
        cout << "| 3.Wyplac Srodki                       | " << endl;
        cout << "| 4.Zainwestuj                          | " << endl;
        cout << "| 5.Sprzedaj waluty                     | " << endl;
        cout << "| 6.Sprawdz swoje inwestycje            | " << endl;
        cout << "| 7.Wyloguj sie                         | " << endl;
        cout << "| 8.Informacje                          | " << endl;
        cout << "| Wybierz opcje: ";
        cin >> wybor;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (wybor) {
        case 1:
            system("cls");
            cout << " " << endl;
            cout << " " << endl;
            cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^        " << endl;
            cout << "Saldo na koncie: " << fixed << setprecision(2) << uzytkownik.saldo << endl;
            cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^        " << endl;
            cout << " " << endl;
            cout << " " << endl;
            break;
        case 2:
            wplata(uzytkownik);
            break;
        case 3:
            wyplata(uzytkownik);
            break;
        case 4:
            zainwestuj(uzytkownik);
            break;
        case 5:
            sprzedajInwestycje(uzytkownik);
            system("cls");
            break;
        case 6:

            sprawdzInwestycje(uzytkownik);
            break;

        case 7:
            
            cout << "Wylogowano uzytkownika." << endl;
            
            break;
        case 8:
            system("cls");
            cout << "Informacje o dzialniu aplikacji:" << endl;
            cout << "Aktualnie aplikacja nie posiada funkcji sprzedazy walut " << endl;
            cout << "Bedzie ona dodana wraz z rozwojem aplikacji " << endl;
            cout << "Ceny walut sa losowe" << endl;
            break;

        default:
            system("cls");
            cout << "       ----        " << endl;
            cout << "Niepoprawny wybor. " << endl;
            cout << "       ----        " << endl;
            break;
        }
    } while (wybor != 7);
}

void zarejestrujKonto() {
    system("cls");
    string login, haslo;
    double saldo = 0.0;

    cout << "Podaj login: ";
    cin >> login;
    cout << "Podaj haslo: ";
    cin >> haslo;

    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());

        string query = "INSERT INTO uzytkownicy (login, haslo, saldo) VALUES ('" + login + "', '" + haslo + "', " + to_string(saldo) + ")";
        stmt->executeUpdate(query);

        cout << "Konto zarejestrowane pomyslnie." << endl;
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
    }
}

void wplata(DaneUzytkownika& uzytkownik) {
    system("cls");
    double kwota;

    cout << "Podaj kwote do wplacenia: ";

    // Sprawdzenie, czy użytkownik wprowadził liczbę
    while (!(cin >> kwota)) {
        cout << "Niepoprawne dane. Podaj kwote jeszcze raz: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (kwota <= 0) {
        cout << "Niepoprawna kwota." << endl;
        return;
    }
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());

        string query = "INSERT INTO historia_wplat (login, kwota, data) VALUES ('" + uzytkownik.login + "', " + to_string(kwota) + ", NOW())";
        stmt->executeUpdate(query);
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
        return;
    }

    // Sprawdź sumę wpłat dla danego użytkownika
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT SUM(kwota) AS suma_wplat FROM historia_wplat WHERE login = '" + uzytkownik.login + "'"));

        if (res->next()) {
            double sumaWplat = res->getDouble("suma_wplat");
            if (sumaWplat > 500000) {
                cout << "Przekroczono limit wplat dla tego uzytkownika." << endl;
                return;
            }
        }
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
        return;
    }

    // Dodaj kwotę do salda użytkownika
    uzytkownik.saldo += kwota;
    zapiszDane(uzytkownik);
    cout << "Pomyslnie wplacono srodki." << endl;
}

void wyplata(DaneUzytkownika& uzytkownik) {
    system("cls");
    double kwota;

    cout << "Podaj kwote do wyplaty: ";

    // Sprawdzenie, czy użytkownik wprowadził liczbę dodatnią
    while (!(cin >> kwota) || kwota <= 0) {
        cout << "Niepoprawne dane. Podaj dodatnia kwote jeszcze raz: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    if (kwota > uzytkownik.saldo) {
        cout << "Brak wystarczajacych srodkow." << endl;
        return;
    }

    // Dodaj wpis do historii wyplat
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());

        string query = "INSERT INTO historia_wyplat (login, kwota, data) VALUES ('" + uzytkownik.login + "', " + to_string(kwota) + ", NOW())";
        stmt->executeUpdate(query);
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
        return;
    }

    // Odjecie kwoty od salda
    uzytkownik.saldo -= kwota;
    zapiszDane(uzytkownik);
    cout << "Pomyslnie wyplacono srodki." << endl;
}

void zapiszDane(DaneUzytkownika& uzytkownik) {
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());

        string query = "UPDATE uzytkownicy SET saldo = " + to_string(uzytkownik.saldo) + " WHERE login = '" + uzytkownik.login + "'";
        stmt->execute(query);
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
    }
}


void wczytajDane(DaneUzytkownika& uzytkownik) {
    try {
        unique_ptr<Connection> con = getConnection();
        unique_ptr<Statement> stmt(con->createStatement());
        unique_ptr<ResultSet> res(stmt->executeQuery("SELECT * FROM uzytkownicy WHERE login = '" + uzytkownik.login + "'"));

        while (res->next()) {
            uzytkownik.saldo = res->getDouble("saldo");
        }
    }
    catch (SQLException& e) {
        cout << "Blad SQL: " << e.what() << endl;
    }
}

int main() {
    
    system("cls");
    int wybor;
    DaneUzytkownika uzytkownik;

    do {
        cout << "  \\\\\\          Witaj             ///" << endl;
        cout << "   \\\\\\    w panelu logowania    ///" << endl;
        cout << "    \\\\\\    trading-software    ///" << endl;
        cout << "                                             \n\n";
        cout << "    -------------MENU--------------" << endl;
        cout << "                                             \n\n";
        cout << "| 1.Zaloguj sie                | " << endl;
        cout << "| 2.Zarejestruj sie            | " << endl;
        cout << "| 3.Jak dziala apliakcja       | " <<  endl;
        cout << "| 4.Wyjscie                    | " << endl;
        cout << "| Wybierz opcje: ";
        cin >> wybor;

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (wybor) {
        case 1: {
            string login = zaloguj();
            if (!login.empty()) {
                uzytkownik.login = login;
                wczytajDane(uzytkownik);
                Zalogowany(uzytkownik);
            }
            break;
        }
        case 2:
            zarejestrujKonto();
            break;
        case 3: {
            system("cls");
            break;
        case 4:
            cout << "Nara" << endl;
            system("pause");
            return 0;
        default:
            system("cls");
            cout << "       ----        " << endl;
            cout << "Niepoprawny wybor. " << endl;
            cout << "       ----        " << endl;
            break;
        }
    }
        } while (wybor != 4);

    system("pause");
    return 0;
}