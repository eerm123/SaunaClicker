#ifndef SAUNA_H
#define SAUNA_H

#include <iostream>
#include <iomanip>
#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

//////////////////////////////////////////////////
//               Mängu propertied               //
//////////////////////////////////////////////////
const int AKNA_LAIUS = 1280;
const int AKNA_KÕRGUS = 720;
const float AUTOMAAT_CLICKER_KLIKIAEG_SEK = 0.02f;

//////////////////////////////////////////////////
//               Globaalsed muutujad            //
//////////////////////////////////////////////////
extern SDL_Renderer* renderdaja;
extern float kraadid;
extern float klikimisväärtus;
extern int saunaraha;
extern bool automaatClickerOlemas;
extern bool automaatneKlikkLubatud;
extern int automaatclickerihind;

extern bool automaatClickerOlemas2;
extern bool automaatneKlikkLubatud2;
extern int automaatclickerihind2;

extern bool automaatClickerOlemas3;
extern bool automaatneKlikkLubatud3;
extern int automaatclickerihind3;

extern bool automaatClickerOlemas4;
extern bool automaatneKlikkLubatud4;
extern int automaatclickerihind4;
extern bool kraadideLisamine;
//////////////////////////////////////////////////
//               Kliki struktuur                //
//////////////////////////////////////////////////
struct Clicker {
    SDL_Rect rect;
    bool enabled;
};

extern Clicker olukast;

void ring(SDL_Renderer* renderdaja, int x, int y, int raadius);
void ostaAutomaatClicker();
void ostaAutomaatClicker2();
void ostaAutomaatClicker3();
void ostaAutomaatClicker4();
bool kursorClickeril(Clicker clicker, int x, int y);
void automaatneKlikk();
void automaatneKlikk2();
void automaatneKlikk3();
void automaatneKlikk4();
void renderText(TTF_Font* font, const string& text, const SDL_Color& color, int x, int y);
void oluHullus();
void ajutineKraadid();

#endif
