#include "sauna.h"

// Globaalsete muutujate deklaratsioon
SDL_Renderer* renderdaja = nullptr;
float kraadid = 0.0f;
int saunaraha = 0;
int automaatclickerihind = 10;
bool automaatClickerOlemas = false;
bool automaatneKlikkLubatud = false;
int automaatClickerCount = 0;

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
    if (saunaraha >= automaatclickerihind) {
        saunaraha -= automaatclickerihind;
        automaatClickerOlemas = true;
        automaatneKlikkLubatud = true;
        automaatclickerihind *= 2;
        automaatClickerCount++;

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
        kraadid += 0.02f * automaatClickerCount; // Lisatud kraadi
        saunaraha += 2 * automaatClickerCount; // Lisatud saunaraha
        this_thread::sleep_for(chrono::seconds(1));
    }
}

// Funktsioon teksti renderdamiseks
void renderText(TTF_Font* font, const string& text, const SDL_Color& color, int x, int y) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) {
        cerr << "Teksti ei toimi: " << TTF_GetError() << endl;
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_Quit();
        exit(1);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderdaja, surface);
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
