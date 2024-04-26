#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <thread>
#include "sauna.h"

using namespace std;

Clicker automaatClicker;

int main() {
    //////////////////////////////////////////////////
    //             SDL Installeerimine              //
    //////////////////////////////////////////////////
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL_Viga: " << SDL_GetError() << endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        cerr << "TTF_Init viga: " << TTF_GetError() << endl;
        return 1;
    }

    SDL_Window* aken = SDL_CreateWindow("Sauna Cookie Clicker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, AKNA_LAIUS, AKNA_KÕRGUS, SDL_WINDOW_SHOWN);
    if (!aken) {
        cerr << "Akent ei saanud luua, SDL_Viga: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    renderdaja = SDL_CreateRenderer(aken, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderdaja) {
        cerr << "Renderdajat ei saanud luua, SDL_Viga: " << SDL_GetError() << endl;
        SDL_DestroyWindow(aken);
        SDL_Quit();
        return 1;
    }


    automaatClicker.rect = {30, 50, 180, 50}; // Vasakule üles kast (automaatklikeri jaoks)
    automaatClicker.enabled = false;

    SDL_Rect poodRect = {10, 10, 100, 40}; // Vasakule üles kast (Poe jaoks kast)
    

    //////////////////////////////////////////////////
    //                Main Loop                     //
    //////////////////////////////////////////////////
    bool välju = false;
    thread automaatneKlikkThread;
    while (!välju) {
        SDL_Event sündmus;
        while (SDL_PollEvent(&sündmus)) {
            if (sündmus.type == SDL_QUIT) {
                välju = true;
            } else if (sündmus.type == SDL_MOUSEBUTTONDOWN) {
                int hiirX, hiirY;
                SDL_GetMouseState(&hiirX, &hiirY);
                int ringiX = AKNA_LAIUS / 2;
                int ringiY = AKNA_KÕRGUS / 2;
                int raadius = 50;
                if ((hiirX - ringiX) * (hiirX - ringiX) + (hiirY - ringiY) * (hiirY - ringiY) <= raadius * raadius) {
                    kraadid += 0.01f;
                    saunaraha += 1;
                    cout << "Kraadid: " << fixed << setprecision(2) << kraadid << endl;

                    if (kraadid >= AUTOMAAT_CLICKER_HIND && !automaatClickerOlemas) {
                        automaatClicker.enabled = true;
                    }
                } else if (kursorClickeril(automaatClicker, hiirX, hiirY) && automaatClicker.enabled) {
                    ostaAutomaatClicker();
                    automaatClicker.enabled = false;
                    if (automaatClickerOlemas && automaatneKlikkThread.joinable()) {
                        automaatneKlikkThread.join();
                    }
                    if (automaatClickerOlemas) {
                        automaatneKlikkThread = thread(automaatneKlikk);
                    }
                }
            }
        }
    
        //////////////////////////////////////////////////
        //       SDL-iga joonistame teksti ja kaste     //
        //////////////////////////////////////////////////   
        SDL_SetRenderDrawColor(renderdaja, 0, 0, 0, 255);
        SDL_RenderClear(renderdaja);

        SDL_SetRenderDrawColor(renderdaja, 255, 255, 255, 255);
        ring(renderdaja, AKNA_LAIUS / 2, AKNA_KÕRGUS / 2, 50);

        // Laen fonti
        TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        if (!font) {
            cerr << "Fonti ei toimi: " << TTF_GetError() << endl;
            TTF_Quit();
            SDL_Quit();
            exit(1);
        }

        // Renderdan tekste
        if (automaatClicker.enabled) {
            SDL_SetRenderDrawColor(renderdaja, 0, 255, 0, 255); // Roheline värv
            SDL_RenderFillRect(renderdaja, &automaatClicker.rect);
            renderText(font, "Sauna mjees", {255, 255, 255, 255}, automaatClicker.rect.x + 10, automaatClicker.rect.y + 10);
        }

        // Näita poodi
        SDL_SetRenderDrawColor(renderdaja, 255, 255, 255, 255); // Valge värv
        SDL_RenderFillRect(renderdaja, &poodRect);
        renderText(font, "Pood:", {0, 0, 0, 255}, poodRect.x + 10, poodRect.y + 5);

        // Näita kraade ringi all
        stringstream skooriTekst;
        skooriTekst << "Kraadid: " << fixed << setprecision(2) << kraadid;
        renderText(font, skooriTekst.str(), {255, 255, 255, 255}, AKNA_LAIUS / 2 - 50, AKNA_KÕRGUS / 2 + 100);

        // Näita saunaraha
        stringstream saunarahaTekst;
        saunarahaTekst << "Saunaraha: " << saunaraha;
        renderText(font, saunarahaTekst.str(), {255, 255, 255, 255}, AKNA_LAIUS / 2 - 50, AKNA_KÕRGUS / 2 + 150);

        SDL_RenderPresent(renderdaja);

        // Sulge font
        TTF_CloseFont(font);
        this_thread::sleep_for(chrono::milliseconds(50)); // Ootab 50 millisekundit enne puhkerežiimi minekut
    }

    //////////////////////////////////////////////////
    //                 Cleanup                      //
    //////////////////////////////////////////////////
    if (automaatneKlikkThread.joinable()) {
        automaatneKlikkThread.join();
    }

    TTF_Quit();
    SDL_DestroyRenderer(renderdaja);
    SDL_DestroyWindow(aken);
    SDL_Quit();
    return 0;
}
