#include "sauna.h"


// Globaalsete muutujate deklaratsioon
SDL_Renderer *renderdaja = nullptr;
float kraadid = 0.0f;
float klikimisväärtus = 0.01f;
int saunaraha = 0;

int automaatclickerihind = 25;
bool automaatClickerOlemas = false;
bool automaatneKlikkLubatud = false;
int automaatClickerCount = 0;

int automaatclickerihind2 = 125;
bool automaatClickerOlemas2 = false;
bool automaatneKlikkLubatud2 = false;
int automaatClickerCount2 = 0;

int automaatclickerihind3 = 600;
bool automaatClickerOlemas3 = false;
bool automaatneKlikkLubatud3 = false;
int automaatClickerCount3 = 0;

int automaatclickerihind4 = 1400;
bool automaatClickerOlemas4 = false;
bool automaatneKlikkLubatud4 = false;
int automaatClickerCount4 = 0;
bool kraadideLisamine = false;




Clicker olukast;

//////////////////////////////////////////////////
//         Funktsioon ringi joonistamiseks      //
//////////////////////////////////////////////////
void ring(SDL_Renderer *renderdaja, int x, int y, int raadius) {
    for (int dy = -raadius; dy <= raadius; dy++) {
        for (int dx = -raadius; dx <= raadius; dx++) {
            if (dx * dx + dy * dy <= raadius * raadius) {
                SDL_RenderDrawPoint(renderdaja, x + dx, y + dy);
            }
        }
    }
}

//////////////////////////////////////////////////
//         Funktsioonid ostmise jaoks           //
//////////////////////////////////////////////////
void ostaAutomaatClicker() { //vee tilgutaja
    if (saunaraha >= automaatclickerihind) {
        saunaraha -= automaatclickerihind;
        automaatClickerOlemas = true;
        automaatneKlikkLubatud = true;
        automaatclickerihind *= 2;
        automaatClickerCount++;

        cout << "Ostsid automaat clicker1(ehk vee tilgutaja)" << endl;

    } else {
        cout << "Sul ei ole piisavalt saunaraha, et osta automaat clicker1" << endl;
    }
}

void ostaAutomaatClicker2() { // Kuumkivi
    if (saunaraha >= automaatclickerihind2) {
        saunaraha -= automaatclickerihind2;
        automaatClickerOlemas2 = true;
        automaatneKlikkLubatud2 = true;
        automaatclickerihind2 *= 2;
        automaatClickerCount2++;

        cout << "Ostsid automaat clicker2 (ehk kuumkivi))" << endl;

    } else {
        cout << "Sul ei ole piisavalt saunaraha, et osta automaat clicker2" << endl;
    }
}

void ostaAutomaatClicker3() { // Süütetabletid
    if (saunaraha >= automaatclickerihind3) {
        saunaraha -= automaatclickerihind3;
        automaatClickerOlemas3 = true;
        automaatneKlikkLubatud3 = true;
        automaatclickerihind3 *= 3;
        automaatClickerCount3++;

        cout << "Ostsid automaat clicker3 (ehk Süütetabletid))" << endl;

    } else {
        cout << "Sul ei ole piisavalt saunaraha, et osta automaat clicker3" << endl;
    }
}

void ostaAutomaatClicker4() { // Pelletimasin
    if (saunaraha >= automaatclickerihind4) {
        saunaraha -= automaatclickerihind4;
        automaatClickerOlemas4 = true;
        automaatneKlikkLubatud4 = true;
        automaatclickerihind4 *= 4;
        automaatClickerCount4++;

        cout << "Ostsid automaat clicker3 (ehk pelletimasin))" << endl;

    } else {
        cout << "Sul ei ole piisavalt saunaraha, et osta automaat clicker4" << endl;
    }
}

// Funktsioon et checkida kas kursor on kasti peal
bool kursorClickeril(Clicker clicker, int x, int y) {
    return (x >= clicker.rect.x && x <= clicker.rect.x + clicker.rect.w &&
            y >= clicker.rect.y && y <= clicker.rect.y + clicker.rect.h);
}

// Funktsioon automaatklikimisele (iga sekund annab 0.02 kraadi juurde)
void automaatneKlikk() {
    while (automaatneKlikkLubatud) {
        this_thread::sleep_for(chrono::milliseconds (1750));
        kraadid += 0.02f * automaatClickerCount; // Lisatud kraadi
        saunaraha += 2 * automaatClickerCount; // Lisatud saunaraha
    }
}

void automaatneKlikk2() {
    while (automaatneKlikkLubatud2) {
        this_thread::sleep_for(chrono::milliseconds (1500));
        kraadid += 0.08f * automaatClickerCount2; // Lisatud kraadi
        saunaraha += 8 * automaatClickerCount2; // Lisatud saunaraha
    }
}

void automaatneKlikk3() {
    while (automaatneKlikkLubatud3) {
        this_thread::sleep_for(chrono::milliseconds(1250));
        kraadid += 0.2f * automaatClickerCount3; // Lisatud kraadi
        saunaraha += 20 * automaatClickerCount3; // Lisatud saunaraha
    }
}

void automaatneKlikk4() {
    while (automaatneKlikkLubatud4) {
        this_thread::sleep_for(chrono::milliseconds(1000));
        kraadid += 1.0f * automaatClickerCount4; // Lisatud kraadi
        saunaraha += 100 * automaatClickerCount4; // Lisatud saunaraha
    }
}

// Funktsioon teksti renderdamiseks
void renderText(TTF_Font *font, const string &text, const SDL_Color &color, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderdaja, surface);
    if (!texture) {
        cerr << "Teksti texture ei toimi: " << SDL_GetError() << endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderdaja, texture, NULL, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void oluHullus() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis_x(0, AKNA_LAIUS - olukast.rect.w);
    uniform_int_distribution<int> dis_y(0, AKNA_KÕRGUS - olukast.rect.h);

    while (true) {
        this_thread::sleep_for(chrono::seconds(rand() % 21 + 50));
        olukast.rect.x = dis_x(gen);
        olukast.rect.y = dis_y(gen);
        olukast.enabled = true;
        this_thread::sleep_for(chrono::seconds(5));
        olukast.enabled = false;
        this_thread::sleep_for(chrono::minutes(1));
    }
}

void ajutineKraadid() {
    this_thread::sleep_for(chrono::seconds(10));
    kraadideLisamine = false;
}