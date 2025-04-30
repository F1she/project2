#include<iostream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<vector>

using namespace std;

enum KolorKarty {Czerwony,Czarny};

struct Karta {
	string kolor;
	string figura;
	int wartosc;
	KolorKarty kolorKarty;
	bool widzialnosc;

};

vector<Karta> talia;
vector<string> kolory = {"h","d","c","s"};
vector<string> figury = { "2","3","4","5","6","7","8","9","1","Q","K","J","A"};
vector<vector<Karta>> kolumny(12);

void generowanieTalii() {
    for (const auto& kolor : kolory) {
        for (int i = 0; i < figury.size(); ++i) {
            Karta karta;
            karta.kolor = kolor;
            karta.figura = figury[i];
            karta.widzialnosc = 0;

            if (figury[i] == "J") karta.wartosc = 11;
            else if (figury[i] == "Q") karta.wartosc = 12;
            else if (figury[i] == "K") karta.wartosc = 13;
            else if (figury[i] == "A") karta.wartosc = 1;
            else karta.wartosc = stoi(figury[i]);

            if (kolor == "h" || kolor == "d")
                karta.kolorKarty = Czerwony;
            else
                karta.kolorKarty = Czarny;

            talia.push_back(karta);
        }
    }
}
void tasowanieKart() {
    srand(time(0));
    int n = talia.size();
    for (int i = 0; i < n; ++i) {
        int k = rand() % n;
        swap(talia[i], talia[k]);
    }
}

void poleStart() {
    for (int i = 0; i < 7; i++) {
        int ilosckart = i + 1;
        for (int j = 0; j < ilosckart; j++) {
            if (j == ilosckart - 1) {
                talia[0].widzialnosc = 1;
            }
            kolumny[i].push_back(talia.front());
            talia.erase(talia.begin());
        }
    }
}
void wypiszPlansze() {
    cout << "||";
    for (int y = 2; y >= 0; y--) {
        if (y < kolumny[11].size()) {
            cout << kolumny[11][y].figura << kolumny[11][y].kolor << "||";
        }
        else {
            cout << "  ||";
        }
    }
    for (int x = 7; x < 11; x++) {
        if (kolumny[x].empty()) {
            cout << "--||";
        }
        else {
            cout << kolumny[x].back().figura << kolumny[x].back().kolor << "||";
        }
    }
    cout << endl;
    for (int k = 0; k < 30; k++) {
        cout << '-';
    }
    cout << endl;
    for (int i = 0; i < 10; i++) {
        cout << "||";
        for (int j = 0; j < 7; j++) {
            if (i < kolumny[j].size()) {
                if (kolumny[j][i].widzialnosc == 1) {
                    cout << kolumny[j][i].figura << kolumny[j][i].kolor << "||";
                }
                else {
                    cout << "==||";
                }
            }
            else {
                cout << "  ||";
            }
        }
        cout << endl;
    }
}
int czyJestKartaWKol(int numerkol, string nazwa) {
    numerkol--;
    if (numerkol < 0 || numerkol >= kolumny.size()) {
        return 100;
    }

    int rozmiar = kolumny[numerkol].size();
    string figura = nazwa.substr(0, nazwa.length() - 1);
    string kolor = nazwa.substr(nazwa.length() - 1);

    for (int i = 0; i < rozmiar; i++) {
        if (kolumny[numerkol][i].widzialnosc == 1 &&
            kolumny[numerkol][i].figura == figura &&
            kolumny[numerkol][i].kolor == kolor) {
            return i;
        }
    }
    return 100;
}

bool czyDozRuch(int zKolumna, int doKolumna, string nazwa) {
    if (zKolumna < 1 || zKolumna > 12 || doKolumna < 0 || doKolumna > 11)
        return false;

    int n = czyJestKartaWKol(zKolumna, nazwa);
    if (n == 100) return false;

    if (kolumny[doKolumna].empty()) {
        return true;
    }

    if (kolumny[zKolumna - 1][n].kolorKarty != kolumny[doKolumna].back().kolorKarty &&
        kolumny[zKolumna - 1][n].wartosc == kolumny[doKolumna].back().wartosc - 1) {
        return true;
    }
    return false;
}

bool czyDozRuch2(int zKolumna, int doKolumna, string nazwa) {
    if (zKolumna < 1 || zKolumna > 12 || doKolumna < 0 || doKolumna > 11)
        return false;

    int n = czyJestKartaWKol(zKolumna, nazwa);
    if (n == 100) return false;

    if (kolumny[doKolumna].empty()) {
        return true;
    }

    if (kolumny[zKolumna - 1][n].kolor == kolumny[doKolumna].back().kolor &&
        kolumny[zKolumna - 1][n].wartosc == kolumny[doKolumna].back().wartosc + 1 &&
        n == kolumny[zKolumna - 1].size() - 1) {
        return true;
    }
    return false;
}
void przenies(int zKolumna, int doKolumna, string nazwa) {
    if (zKolumna < 1 || zKolumna > 12 || doKolumna < 1 || doKolumna > 12) {
        cout << "Nieprawidlowe kolumny" << endl;
        return;
    }

    int x = czyJestKartaWKol(zKolumna, nazwa);
    if (x == 100) {
        cout << "Brak karty w kolumnie" << endl;
        return;
    }

    if (doKolumna < 8) {
        if (!czyDozRuch(zKolumna, doKolumna, nazwa)) {
            cout << "Nie mozna wykonac tego ruchu" << endl;
            return;
        }
    }
    else {
        if (!czyDozRuch2(zKolumna, doKolumna, nazwa)) {
            cout << "Nie mozna wykonac tego ruchu" << endl;
            return;
        }
    }

    --zKolumna;
    --doKolumna;

    if (kolumny[zKolumna].size() + kolumny[doKolumna].size() - x > 10 && doKolumna > 6) {
        cout << "Brak miejsca" << endl;
        return;
    }

    vector<Karta> kartyDoPrzeniesienia(kolumny[zKolumna].begin() + x, kolumny[zKolumna].end());
    kolumny[doKolumna].insert(kolumny[doKolumna].end(), kartyDoPrzeniesienia.begin(), kartyDoPrzeniesienia.end());
    kolumny[zKolumna].erase(kolumny[zKolumna].begin() + x, kolumny[zKolumna].end());

    if (!kolumny[zKolumna].empty()) {
        kolumny[zKolumna].back().widzialnosc = 1;
    }
}

void wybieranie() {
    int n = min(3, (int)talia.size());

    for (int i = 0; i < n; ++i) {
        talia[i].widzialnosc = 1;
        kolumny[11].push_back(talia[i]);
    }
    talia.erase(talia.begin(), talia.begin() + n);
}

void przewinStos() {
    for (int i = kolumny[11].size() - 1; i >= 0; --i) {
        talia.push_back(kolumny[11][i]);
    }
    kolumny[11].clear();
    wybieranie();
    wypiszPlansze();
}

void przenies2(int zKolumna, int doKolumna, string nazwa) {
    if (zKolumna < 1 || zKolumna > 12 || doKolumna < 1 || doKolumna > 12) {
        cout << "Nieprawidlowe kolumny" << endl;
        return;
    }

    int n = czyJestKartaWKol(zKolumna, nazwa);
    if (n == 100) {
        cout << "Brak karty" << endl;
        return;
    }

    --doKolumna;
    --zKolumna;

    if (kolumny[doKolumna].size() == 10) {
        cout << "Brak miejsca" << endl;
        return;
    }

    if (!czyDozRuch(zKolumna + 1, doKolumna, nazwa)) {
        cout << "Nie mozna wykonac tego ruchu" << endl;
        return;
    }

    kolumny[doKolumna].push_back(kolumny[zKolumna][n]);
    kolumny[zKolumna].erase(kolumny[zKolumna].begin() + n);
}
bool czyWygrana() {
    for (int i = 7; i < 11; ++i) {
        if (kolumny[i].size() != 13) return false;
        string kolorPierwszej = kolumny[i][0].kolor;
        for (const auto& karta : kolumny[i]) {
            if (karta.kolor != kolorPierwszej) return false;
        }
        for (int j = 0; j < 13; ++j) {
            if (kolumny[i][j].wartosc != j + 1) {
                return false;
            }
        }
    }
    return true;
}

void ruch() {
    string karta;
    int x, y;
    cout << "Podaj z kolumny (1-7, 0 = stos, 99 = przewin stos): ";
    cin >> x;

    if (x == 99) {
        przewinStos();
        return;
    }

    cout << "Do kolumny (1-11): ";
    cin >> y;
    cout << "Podaj karte (np. 7h): ";
    cin >> karta;

    if (y == 0 || y > 11) {
        cout << "Brak ruchu" << endl;
        return;
    }

    if (x == 0) {
        przenies2(12, y, karta);
    }
    else {
        przenies(x, y, karta);
    }

    wypiszPlansze();

    if (czyWygrana()) {
        cout << "Koniec gry" << endl;
        exit(0);
    }

}

int main()
{
    generowanieTalii();
    tasowanieKart();
    poleStart();
    wybieranie();
    wypiszPlansze();

    while (true) {
        ruch();
    }

    return 0;
}

