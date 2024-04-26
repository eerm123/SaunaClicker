#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <thread>
#include "sauna.h"

using namespace std;

Clicker automaatClicker;
Clicker keriseRect;

int main(int argc, char *argv[]) {
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

    SDL_Window *aken = SDL_CreateWindow("Sauna Cookie Clicker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        AKNA_LAIUS, AKNA_KÕRGUS, SDL_WINDOW_SHOWN);
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


    automaatClicker.rect = {60, 50, 375, 50}; // Vasakule üles kast (automaatklikeri jaoks)
    automaatClicker.enabled = false;

    keriseRect.rect = {AKNA_LAIUS / 2 - 50, 380, 150, 300};

    SDL_Rect poodRect = {10, 10, 80, 40}; // Vasakule üles kast (Poe jaoks kast)


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
                if (kursorClickeril(keriseRect, hiirX, hiirY)) {
                    kraadid += 0.01f;
                    saunaraha += 1;
                    cout << "Kraadid: " << fixed << setprecision(2) << kraadid << endl;
                }
                if (kursorClickeril(automaatClicker, hiirX, hiirY) && automaatClicker.enabled) {
                    ostaAutomaatClicker();
                    if (automaatClickerOlemas) {
                        // Enne uue lõime loomist kontrollige, kas eelmine lõim on lõpetanud
                        if (automaatneKlikkThread.joinable()) {
                            automaatneKlikkThread.join();
                        }
                        std::thread uusAutomaatneKlikkThread(automaatneKlikk); // Luua uus lõim
                        automaatneKlikkThread = std::move(uusAutomaatneKlikkThread); // Kasuta uut lõime
                    }
                }
            }

        }

        //////////////////////////////////////////////////
        //       SDL-iga joonistame teksti ja kaste     //
        //////////////////////////////////////////////////
        SDL_SetRenderDrawColor(renderdaja, 0, 0, 0, 255);
        SDL_RenderClear(renderdaja);

        SDL_SetRenderDrawColor(renderdaja, 255, 255, 200, 2);
        SDL_RenderFillRect(renderdaja, &keriseRect.rect);

        /*ring(renderdaja, AKNA_LAIUS / 2, AKNA_KÕRGUS / 2, 50);
         */

        // Laen fonti
        TTF_Font *font = TTF_OpenFont("C:/Users/Kaspar/CLionProjects/SaunaClicker/DejaVuSans.ttf", 24);
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
            renderText(font, "Vee tilgutaja (+0.01 kraadi/s)", {65, 107, 223, 255}, automaatClicker.rect.x + 10,
                       automaatClicker.rect.y + 10);
        }

        // Näita poodi
        SDL_SetRenderDrawColor(renderdaja, 255, 255, 255, 255); // Valge värv
        SDL_RenderFillRect(renderdaja, &poodRect);
        renderText(font, "Pood:", {0, 0, 0, 255}, poodRect.x + 5, poodRect.y + 5);

        // Näita kraade ringi all
        stringstream skooriTekst;
        skooriTekst << "Kraadid: " << fixed << setprecision(2) << kraadid;
        renderText(font, skooriTekst.str(), {255, 255, 255, 255}, AKNA_LAIUS / 2 - 50, 685);

        // Näita saunaraha
        stringstream saunarahaTekst;
        saunarahaTekst << "Saunaraha $: " << saunaraha;
        renderText(font, saunarahaTekst.str(), {22, 156, 53, 255}, poodRect.x + 100, 15);

        SDL_RenderPresent(renderdaja);

        // Sulge font
        TTF_CloseFont(font);
        this_thread::sleep_for(chrono::milliseconds(50)); // Ootab 50 millisekundit enne puhkerežiimi minekut

        // Kontrollige, kas automaatClicker ostuaken peaks olema aktiivne
        automaatClicker.enabled = saunaraha >= automaatclickerihind;
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
