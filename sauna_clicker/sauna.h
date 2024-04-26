#ifndef SAUNA_H
#define SAUNA_H

#include <iostream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <thread>

using namespace std;

//////////////////////////////////////////////////
//               Mängu propertied               //
//////////////////////////////////////////////////
const int AKNA_LAIUS = 1280;
const int AKNA_KÕRGUS = 720;
const float AUTOMAAT_CLICKER_HIND = 0.99f;
const float AUTOMAAT_CLICKER_KLIKIAEG_SEK = 0.02f;

//////////////////////////////////////////////////
//               Globaalsed muutujad            //
//////////////////////////////////////////////////
extern SDL_Renderer* renderdaja;
extern float kraadid;
extern int saunaraha;
extern bool automaatClickerOlemas;
extern bool automaatneKlikkLubatud;


//////////////////////////////////////////////////
//               Kliki struktuur                //
//////////////////////////////////////////////////
struct Clicker {
    SDL_Rect rect;
    bool enabled;
};

void ring(SDL_Renderer* renderdaja, int x, int y, int raadius);
void ostaAutomaatClicker();
bool kursorClickeril(Clicker clicker, int x, int y);
void automaatneKlikk();
void renderText(TTF_Font* font, const string& text, const SDL_Color& color, int x, int y);

#endif
