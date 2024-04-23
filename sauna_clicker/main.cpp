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
SDL_Renderer* renderdaja = nullptr;
float kraadid = 0.0f;
int saunaraha = 0;
bool automaatClickerOlemas = false;
bool automaatneKlikkLubatud = false;

//////////////////////////////////////////////////
//               Kliki struktuur                //
//////////////////////////////////////////////////
struct Clicker {
    SDL_Rect rect;
    bool enabled;
};

Clicker automaatClicker;

//////////////////////////////////////////////////
//         Funktsioon ringi joonistamiseks      //
//////////////////////////////////////////////////
void ring(SDL_Renderer* renderdaja, int x, int y, int raadius) {
    for (int dy = -raadius; dy <= raadius; dy++) {
        for (int dx = -raadius; dx <= raadius; dx++) {
            if (dx * dx + dy * dy <= raadius * raadius) {
                SDL_RenderDrawPoint(renderdaja, x + dx, y + dy);
            }
        }
    }
}

//////////////////////////////////////////////////
//         Funktsioon ostmise jaoks             //
//////////////////////////////////////////////////
void ostaAutomaatClicker() {
    if (saunaraha >= AUTOMAAT_CLICKER_HIND * 100) {
        saunaraha -= AUTOMAAT_CLICKER_HIND * 100;
        automaatClickerOlemas = true;
        automaatneKlikkLubatud = true;
        cout << "Ostsid automaat clickeri" << endl;
    } else {
        cout << "Sul ei ole piisavalt saunaraha, et osta automaat clicker" << endl;
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
        kraadid += 0.01f; // Lisatud kraadi
        saunaraha += 1; // Lisatud saunaraha
        this_thread::sleep_for(chrono::seconds(1));
    }
}


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

        if (automaatClicker.enabled) {
            SDL_SetRenderDrawColor(renderdaja, 0, 255, 0, 255); // Roheline värv
            SDL_RenderFillRect(renderdaja, &automaatClicker.rect);

            TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
            if (!font) {
                cerr << "Fonti ei toimi: " << TTF_GetError() << endl;
                TTF_Quit();
                SDL_Quit();
                return 1;
            }
            SDL_Color tekstiVärv = {255, 255, 255, 255};
            SDL_Surface* tekstiPind = TTF_RenderText_Solid(font, "Sauna mjees", tekstiVärv);
            if (!tekstiPind) {
                cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 1;
            }
            SDL_Texture* tekstiTekstuuri = SDL_CreateTextureFromSurface(renderdaja, tekstiPind);
            if (!tekstiTekstuuri) {
                cerr << "Teksti texture ei toimi: " << SDL_GetError() << endl;
                SDL_FreeSurface(tekstiPind);
                TTF_CloseFont(font);
                TTF_Quit();
                SDL_Quit();
                return 1;
            }
            SDL_Rect tekstiKast = {automaatClicker.rect.x + 10, automaatClicker.rect.y + 10, tekstiPind->w, tekstiPind->h};
            SDL_RenderCopy(renderdaja, tekstiTekstuuri, NULL, &tekstiKast);
            SDL_FreeSurface(tekstiPind);
            SDL_DestroyTexture(tekstiTekstuuri);
            TTF_CloseFont(font);
        }

        // Kuva poodi
        SDL_SetRenderDrawColor(renderdaja, 255, 255, 255, 255); // Valge värv
        SDL_RenderFillRect(renderdaja, &poodRect);

        TTF_Font* font2 = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        if (!font2) {
            cerr << "Fonti ei toimi: " << TTF_GetError() << endl;
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Color tekstiVärv2 = {0, 0, 0, 255}; // Must värv
        SDL_Surface* tekstiPind2 = TTF_RenderText_Solid(font2, "Pood:", tekstiVärv2);
        if (!tekstiPind2) {
            cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
            TTF_CloseFont(font2);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Texture* tekstiTekstuuri2 = SDL_CreateTextureFromSurface(renderdaja, tekstiPind2);
        if (!tekstiTekstuuri2) {
            cerr << "Teksti tekstuuri ei toimi: " << SDL_GetError() << endl;
            SDL_FreeSurface(tekstiPind2);
            TTF_CloseFont(font2);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Rect tekstiKast2 = {poodRect.x + 10, poodRect.y + 5, tekstiPind2->w, tekstiPind2->h};
        SDL_RenderCopy(renderdaja, tekstiTekstuuri2, NULL, &tekstiKast2);
        SDL_FreeSurface(tekstiPind2);
        SDL_DestroyTexture(tekstiTekstuuri2);
        TTF_CloseFont(font2);

        // Kuva skoor ringi all
        TTF_Font* font3 = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        if (!font3) {
            cerr << "Fonti ei toimi: " << TTF_GetError() << endl;
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Color tekstiVärv3 = {255, 255, 255, 255};
        stringstream skooriTekst;
        skooriTekst << "Kraadid: " << fixed << setprecision(2) << kraadid;
        string skoorStr = skooriTekst.str();
        SDL_Surface* tekstiPind3 = TTF_RenderText_Solid(font3, skoorStr.c_str(), tekstiVärv3);
        if (!tekstiPind3) {
            cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
            TTF_CloseFont(font3);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Texture* tekstiTexture3 = SDL_CreateTextureFromSurface(renderdaja, tekstiPind3);
        if (!tekstiTexture3) {
            cerr << "Teksti texture ei toimi: " << SDL_GetError() << endl;
            SDL_FreeSurface(tekstiPind3);
            TTF_CloseFont(font3);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Rect tekstiKast3 = {AKNA_LAIUS / 2 - 50, AKNA_KÕRGUS / 2 + 100, tekstiPind3->w, tekstiPind3->h};
        SDL_RenderCopy(renderdaja, tekstiTexture3, NULL, &tekstiKast3);
        SDL_FreeSurface(tekstiPind3);
        SDL_DestroyTexture(tekstiTexture3);
        TTF_CloseFont(font3);

        // Kuva saunaraha
        TTF_Font* font4 = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 24);
        if (!font4) {
            cerr << "Fonti ei toimi: " << TTF_GetError() << endl;
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Color tekstiVärv4 = {255, 255, 255, 255};
        stringstream saunarahaTekst;
        saunarahaTekst << "Saunaraha: " << saunaraha;
        string saunarahaStr = saunarahaTekst.str();
        SDL_Surface* tekstiPind4 = TTF_RenderText_Solid(font4, saunarahaStr.c_str(), tekstiVärv4);
        if (!tekstiPind4) {
            cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
            TTF_CloseFont(font4);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Texture* tekstiTexture4 = SDL_CreateTextureFromSurface(renderdaja, tekstiPind4);
        if (!tekstiTexture4) {
            cerr << "Teksti texture ei toimi: " << SDL_GetError() << endl;
            SDL_FreeSurface(tekstiPind4);
            TTF_CloseFont(font4);
            TTF_Quit();
            SDL_Quit();
            return 1;
        }
        SDL_Rect tekstiKast4 = {AKNA_LAIUS / 2 - 50, AKNA_KÕRGUS / 2 + 150, tekstiPind4->w, tekstiPind4->h};
        SDL_RenderCopy(renderdaja, tekstiTexture4, NULL, &tekstiKast4);
        SDL_FreeSurface(tekstiPind4);
        SDL_DestroyTexture(tekstiTexture4);
        TTF_CloseFont(font4);

        SDL_RenderPresent(renderdaja);
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
