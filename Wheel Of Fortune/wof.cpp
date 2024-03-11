#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <random>
#include <chrono>
#include <thread>

using namespace std;
ifstream fin("puzzle.in");
int wheel[] = {2500, 2500, 2500, 600, 600, 600, 700, 700, 700, 600, 600, 600, 650, 650, 650, 500, 500, 500, 700, 700, 700, 0, 666, 0, 600, 600,
               600, 550, 550, 550, 600, 600, 600, 0, 0, 0, 888, 888, 888, 1, 1, 1, 800, 800, 800, 500, 500, 500, 650, 650, 650, 900, 900, 900,
               0, 0, 0};

char used[100];

using namespace std::this_thread;
using namespace std::chrono;

struct Puzzle
{
    char ctg[100];
    char pzl[100];
    char secret[100];
    int fin = 0;
};

struct Jucator
{
    char nume[20];
    int id;
    int overall = 0;
    int suma;
};
struct Wof
{
    Puzzle puzzle1, puzzle2, puzzle3;
    Jucator p1, p2, p3;
    char winner[20];
} joc;

bool isLetter(char s)
{
    if (s != ',' && s != '.' && s != '?' && s != '!' && s != ' ' && s != '-')
        return true;
    return false;
}

bool isVocala(char s)
{
    if (s == 'A' || s == 'E' || s == 'I' || s == 'O' || s == 'U')
        return true;
    return false;
}

bool isConsoana(char s)
{
    if (!isVocala(s) && isupper(s))
        return true;
    return false;
}

bool isInPuzzle(char string[100], char c)
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == c)
            return true;
    }
    return false;
}

bool checkConsoane(char string1[100], char string2[100])
{
    for (int i = 0; i < strlen(string1); i++)
    {
        if (string1[i] == '@' && isConsoana(string2[i]))
            return true;
    }
    return false;
}
bool checkVocale(char string1[100], char string2[100])
{
    for (int i = 0; i < strlen(string1); i++)
    {
        if (string1[i] == '@' && isVocala(string2[i]))
            return true;
    }
    return false;
}

bool checkSame(char string1[100], char string2[100])
{
    for (int i = 0; i < strlen(string1); i++)
        if (string1[i] != string2[i])
            return false;
    return true;
}
bool checkUsed(char string[100], char s)
{
    for (int i = 0; i < strlen(string); i++)
    { // printf("Compar %c cu %c\n", string[i], s);
        if (string[i] == s)
            return true;
    }
    return false;
}
void intro(Wof &joc)
{
    cout << "Nume jucator 1:" << endl;
    cin >> joc.p1.nume;
    joc.p1.id = 1;
    cout << "Nume jucator 2:" << endl;
    cin >> joc.p2.nume;
    joc.p1.id = 2;
    cout << "Nume jucator 3:" << endl;
    cin >> joc.p3.nume;
    joc.p1.id = 3;
    printf("Buna seara %s, %s, %s!\nBine ati venit la Roata Norocului!!!!!\n\n", joc.p1.nume, joc.p2.nume, joc.p3.nume);
}

int spin()
{

    std::random_device rd;

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 56);

    int i = dis(gen);
    return wheel[i];
}

void preparePuzzle(Wof &joc)
{
    char file[1000];
    int i = 0;
    while (fin.getline(file, 1000))
    {
        i++;
        char *token = strtok(file, ":");
        switch (i)
        {
        case 1:
            strcpy(joc.puzzle1.ctg, token);
            token = strtok(NULL, "\n");
            strcpy(joc.puzzle1.pzl, token);
            for (int j = 0; j < strlen(joc.puzzle1.pzl); j++)
            {
                if (isLetter(joc.puzzle1.pzl[j]))
                    joc.puzzle1.secret[j] = '@';
                else
                    joc.puzzle1.secret[j] = joc.puzzle1.pzl[j];
            }
            break;
        case 2:
            strcpy(joc.puzzle2.ctg, token);
            token = strtok(NULL, "\n");
            strcpy(joc.puzzle2.pzl, token);
            for (int j = 0; j < strlen(joc.puzzle2.pzl); j++)
            {
                if (isLetter(joc.puzzle2.pzl[j]))
                    joc.puzzle2.secret[j] = '@';
                else
                    joc.puzzle2.secret[j] = joc.puzzle2.pzl[j];
            }
            break;
        case 3:
            strcpy(joc.puzzle3.ctg, token);
            token = strtok(NULL, "\n");
            strcpy(joc.puzzle3.pzl, token);
            for (int j = 0; j < strlen(joc.puzzle3.pzl); j++)
            {
                if (isLetter(joc.puzzle3.pzl[j]))
                    joc.puzzle3.secret[j] = '@';
                else
                    joc.puzzle3.secret[j] = joc.puzzle3.pzl[j];
            }
            break;
        }
    }
}

void play(Puzzle &puzzle, Jucator &p)
{
    int fail = 0;
    do
    {
        printf("Puzzle-ul este\n%s\n\n", puzzle.secret);
        // printf("%s: spin, solve sau vocala?\nIntrodu comanda mai jos:\n", p.nume);
        int valid = 0;
        char comanda[20];
        do
        {
            printf("Litere folosite: %s\n", used);
            printf("%s ai %d dolari. Spin, solve sau vocala?\nIntrodu comanda mai jos:\n", p.nume, p.suma);

            cin >> comanda;
            printf("\n\nComanda aleasa: %s\n", comanda);
            if (strcmp(comanda, "spin") != 0 && strcmp(comanda, "solve") != 0 && strcmp(comanda, "vocala") != 0)
                printf("Comanda aleasa nu este valida!\nIncearca din nou!\n\n");
            else if (strcmp(comanda, "spin") == 0 && checkConsoane(puzzle.secret, puzzle.pzl) == false)
                printf("Nu mai exista consoane!!\nIncearca sa rezolvi puzzle-ul sau sa cumperi vocale!\n\n");
            else
                valid = 1;
        } while (valid == 0);

        if (strcmp(comanda, "spin") == 0)
        {
            int val = spin();
            /*int val;
            printf("Introdu valoarea obtinuta la roata\n(BANKRUPT = 0)\n(LOSE A TURN = 1)\n");
            cin >> val;*/
            if (val == 1 || val == 0)
            {
                switch (val)
                {
                case 0:
                    fail = 1;
                    printf("BANKRUPT :(((((((\n\n");
                    p.suma = 0;
                    sleep_for(seconds(3));
                    break;
                case 1:
                    fail = 1;
                    printf("LOSE A TURN :(((((((((( \n\n");
                    sleep_for(seconds(3));
                }
            }
            else if (val == 888)
            {
                char litera;

                printf("FREE PLAY!! Poti alege orice litera. Daca gresesti NU pierzi tura!\n", val);

                int validcons = 0;
                do
                {

                    printf("Puzzle-ul este: %s\n\nAlegeti o consoana:\n", puzzle.secret);
                    printf("Litere folosite: %s\n", used);
                    cin >> litera;
                    if (!isLetter(litera))
                        printf(" %c nu este litera!\nIncearca din nou:\n\n", litera);
                    if (isLetter(litera) && checkUsed(used, litera))
                        printf("Litera %c a fost deja folosita!\nIncearca din nou:\n\n");
                    else
                        validcons = 1;
                } while (validcons == 0);
                used[strlen(used)] = litera;
                int counter = 0;
                for (int i = 0; i <= strlen(puzzle.secret); i++)
                {
                    if (litera == puzzle.pzl[i])
                    {
                        puzzle.secret[i] = litera;
                        counter++;
                    }
                }
                if (counter > 0)
                {
                    printf("Bravo! In puzzle sunt %d de %c!\n\n", counter, litera);
                    if (checkConsoane(puzzle.secret, puzzle.pzl) == false)
                        printf("Nu mai sunt consoane in puzzle!\n");
                    if (checkSame(puzzle.secret, puzzle.pzl) == true)
                    {
                        printf("Puzzle-ul a fost rezolvat!\n");
                        puzzle.fin = 1;
                        p.overall = p.suma + 1000;
                        strcpy(joc.winner, p.nume);
                    }
                }
                else
                {
                    printf("Niciun %c din pacate\n\n", litera);
                    fail = 0;
                }
            }
            else
            {
                char consoana;

                printf("\n\nWOW %d de dolari?? INSANE!\n", val);
                int validcons = 0;
                do
                {

                    printf("Puzzle-ul este: %s\nLitere folosite: %s\n\nAlegeti o consoana:\n", puzzle.secret, used);
                    cin >> consoana;
                    checkUsed(used, consoana);
                    if (!isConsoana(consoana))
                    {
                        printf("Litera %c nu este consoana!\nIncearca din nou:\n\n", consoana);
                        sleep_for(seconds(2));
                    }

                    else if (isConsoana(consoana) && checkUsed(used, consoana))
                    {
                        printf("Consoana %c a fost deja folosita!\nIncearca din nou:\n\n");
                        sleep_for(seconds(5));
                    }
                    else
                        validcons = 1;
                } while (validcons == 0);

                used[strlen(used)] = consoana;
                int counter = 0;
                for (int i = 0; i <= strlen(puzzle.secret); i++)
                {
                    if (consoana == puzzle.pzl[i])
                    {
                        puzzle.secret[i] = consoana;
                        counter++;
                    }
                }
                if (counter > 0)
                {
                    printf("Bravo! In puzzle sunt %d de %c!\n\n", counter, consoana);
                    p.suma = p.suma + counter * val;
                    sleep_for(seconds(3));
                    if (checkConsoane(puzzle.secret, puzzle.pzl) == false)
                        printf("Nu mai sunt consoane in puzzle!\n");
                    if (checkSame(puzzle.secret, puzzle.pzl) == true)
                    {
                        printf("Puzzle-ul a fost rezolvat!\n");
                        puzzle.fin = 1;
                        p.overall = p.suma + 1000;
                        strcpy(joc.winner, p.nume);
                    }
                }
                else
                {
                    printf("Niciun %c din pacate\n\n", consoana);
                    sleep_for(seconds(3));
                    fail = 1;
                }
            }
        }
        if (strcmp(comanda, "solve") == 0)
        {
            char guess[100];
            printf("Puzzle-ul este\n%s\n\n", puzzle.secret);
            printf("Rezolva puzzle-ul: \n\n");
            cin >> guess;
            int corect = 0;
            int curs = 0;
            for (int i = 0; i < strlen(puzzle.secret); i++)
            {
                if (puzzle.secret[i] == '@' && puzzle.pzl[i] == guess[curs])
                {
                    curs++;
                }
                else if (puzzle.secret[i] == '@' && puzzle.pzl[i] != guess[curs])
                    corect = 1;
            }

            if (corect == 0)
            {
                printf("Raspuns corect!!!\n");
                puzzle.fin = 1;
                strcpy(joc.winner, p.nume);
                p.overall = p.suma + 1000;
            }
            else if (corect == 1)
            {
                printf("Din pacate raspunsul este gresit...\n\n");
                sleep_for(seconds(3));
                fail = 1;
            }
        }
        if (strcmp(comanda, "vocala") == 0)
        {
            if (p.suma < 250)
                printf("Nu ai destui bani!\n");
            else if (checkVocale(puzzle.secret, puzzle.pzl) == false)
                printf("Nu mai exista vocale!\n");
            else
            {
                p.suma = p.suma - 250;
                char vocala;
                printf("Alege o vocala:\n");
                int validvoc = 0;
                do
                {

                    printf("Puzzle-ul este: %s\n\nAlegeti o vocala:\n", puzzle.secret);
                    printf("Litere folosite: %s\n", used);
                    cin >> vocala;
                    if (!isVocala(vocala))
                        printf("Litera %c nu este vocala!\nIncearca din nou:\n\n", vocala);
                    else if (isVocala(vocala) && checkUsed(used, vocala))
                        printf("Vocala %c a fost deja folosita!\nIncearca din nou:\n\n");
                    else
                        validvoc = 1;
                } while (validvoc == 0);
                used[strlen(used)] = vocala;
                int counter = 0;
                for (int i = 0; i <= strlen(puzzle.secret); i++)
                {
                    if (vocala == puzzle.pzl[i])
                    {
                        puzzle.secret[i] = vocala;
                        counter++;
                    }
                }
                if (counter > 0)
                {
                    printf("Bravo! In puzzle sunt %d de %c!\n\n", counter, vocala);
                    sleep_for(seconds(3));

                    if (checkVocale(puzzle.secret, puzzle.pzl) == false)
                        printf("Nu mai sunt vocale in puzzle!\n");
                    if (checkSame(puzzle.secret, puzzle.pzl) == true)
                    {
                        printf("Puzzle-ul a fost rezolvat!\n");
                        puzzle.fin = 1;
                        p.overall = p.suma + 1000;
                        strcpy(joc.winner, p.nume);
                    }
                }
                else
                {
                    printf("Niciun %c din pacate\n\n", vocala);
                    sleep_for(seconds(3));
                    fail = 1;
                }
            }
        }
    } while (fail == 0 && puzzle.fin == 0);
}
void firstPuzzle(Wof &joc)
{
    joc.p1.suma = 0;
    joc.p2.suma = 0;
    joc.p3.suma = 0;

    printf("Sa trecem la primul puzzle!\nCategoria este: %s\n\n", joc.puzzle1.ctg);
    sleep_for(seconds(3));
    do
    {
        play(joc.puzzle1, joc.p1);
        if (joc.puzzle1.fin == 0)
            play(joc.puzzle1, joc.p2);
        if (joc.puzzle1.fin == 0)
            play(joc.puzzle1, joc.p3);
    } while (joc.puzzle1.fin == 0);

    printf("Puzzle-ul este: %s\n", joc.puzzle1.pzl);
    printf("Runda 1 este castigata de %s!!!\n", joc.winner);
    printf("Scorurile dupa prima runda:\n%s: %d\n%s: %d\n%s: %d\n\n", joc.p1.nume, joc.p1.overall, joc.p2.nume, joc.p2.overall, joc.p3.nume, joc.p3.overall);
}
void secondPuzzle(Wof &joc)
{
    joc.p1.suma = 0;
    joc.p2.suma = 0;
    joc.p3.suma = 0;

    printf("Sa trecem la al doilea puzzle!\nCategoria este: %s\n\n", joc.puzzle2.ctg);
    do
    {
        play(joc.puzzle2, joc.p2);
        if (joc.puzzle2.fin == 0)
            play(joc.puzzle2, joc.p3);
        if (joc.puzzle2.fin == 0)
            play(joc.puzzle2, joc.p1);
    } while (joc.puzzle2.fin == 0);

    printf("Puzzle-ul este: %s\n", joc.puzzle2.pzl);
    printf("Runda 2 este castigata de %s!!!\n", joc.winner);
    printf("Scorurile dupa a doua runda:\n%s: %d\n%s: %d\n%s: %d\n\n", joc.p1.nume, joc.p1.overall, joc.p2.nume, joc.p2.overall, joc.p3.nume, joc.p3.overall);
}
void thirdPuzzle(Wof &joc)
{
    joc.p1.suma = 0;
    joc.p2.suma = 0;
    joc.p3.suma = 0;

    memset(used, 0, sizeof(used));

    printf("Sa trecem la al treilea puzzle!\nCategoria este: %s\n\n", joc.puzzle3.ctg);
    do
    {
        play(joc.puzzle3, joc.p3);
        if (joc.puzzle3.fin == 0)
            play(joc.puzzle3, joc.p1);
        if (joc.puzzle3.fin == 0)
            play(joc.puzzle3, joc.p2);
    } while (joc.puzzle3.fin == 0);

    printf("Puzzle-ul este: %s\n", joc.puzzle3.pzl);
    printf("Runda 3 este castigata de %s!!!\n", joc.winner);
    printf("Scorurile dupa a triea runda:\n%s: %d\n%s: %d\n%s: %d\n\n", joc.p1.nume, joc.p1.overall, joc.p2.nume, joc.p2.overall, joc.p3.nume, joc.p3.overall);
}
int main()
{

    intro(joc);
    preparePuzzle(joc);
    firstPuzzle(joc);
    memset(used, 0, sizeof(used));
    sleep_for(seconds(5));
    secondPuzzle(joc);
    memset(used, 0, sizeof(used));
    sleep_for(seconds(5));
    thirdPuzzle(joc);
}