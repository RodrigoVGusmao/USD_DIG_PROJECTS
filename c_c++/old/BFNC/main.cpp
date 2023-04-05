#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#endif // _WIN32_WINNT

#include<SDL.h>
#include<windows.h>
#include<stdio.h>
#include<time.h>

#define SCREEN_RADIUS 100
#define DEFAULT_TIME 5000
#define TIME_PER_CARACTER 5
#define MAX_TEST_CHARACTERS 36

int ThreadDelay(int Time)
{
    SDL_Delay(1);
    return Time + 1;
}

int main(int argc, char* argv[])
{
    bool quit, startBP, startTime;

    int Delay, maxString, currentString, maxCurrentString, ETime;

    clock_t start, finish;

    time_t now = time(0);

    printf("O programa começou a rodar em: %s\n", ctime(&now));

    KEYBDINPUT enterKey;
    enterKey.wVk = VK_RETURN;
    enterKey.wScan = 0;
    enterKey.dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();
    enterKey.dwFlags = 0;
    enterKey.time = 0;


    printf("configs:\nmax caracters:");
    scanf("%d", &maxString);

    ETime = 1;

    KEYBDINPUT kbi[maxString];

    for(currentString = 0; currentString < maxString; currentString++)
    {
        kbi[currentString].wVk = 0x2F;
        kbi[currentString].wScan = 0;
        kbi[currentString].dwFlags = 0;
        kbi[currentString].time = 0;
        kbi[currentString].dwExtraInfo = (ULONG_PTR) GetMessageExtraInfo();
        if(currentString > 0)
        {
            ETime *= MAX_TEST_CHARACTERS;
        }
    }

    if(ETime*TIME_PER_CARACTER < 1000)
    {
        printf("tempo estimado para completar o ciclo: %d ms\n%d\n", ETime*TIME_PER_CARACTER, ETime);
    }
    else
    {
        if(ETime*TIME_PER_CARACTER/1000 < 60)
        {
            printf("tempo estimado para completar o ciclo: %d s\n%d\n", ETime*TIME_PER_CARACTER/1000, ETime);
        }
        else
        {
            if(ETime*TIME_PER_CARACTER/6000 < 60)
            {
                printf("tempo estimado para completar o cilclo: %d min %d s\n", ETime*TIME_PER_CARACTER/6000, (ETime - ETime*TIME_PER_CARACTER/6000)*TIME_PER_CARACTER/1000);
            }
            else
            {
                printf("tempo estimado para completar o cilclo: %d h %d min %d s\n", ETime*TIME_PER_CARACTER/360000, (ETime - ETime*TIME_PER_CARACTER/360000)*TIME_PER_CARACTER/6000, (ETime - ETime*TIME_PER_CARACTER/6000)*TIME_PER_CARACTER/1000);
            }
        }
    }

    currentString = 0;

    INPUT input;
    input.type = INPUT_KEYBOARD;

    SDL_Window* window;

    SDL_Surface* ScreenSurface;

    SDL_Event event;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return 1;

    window = SDL_CreateWindow("BRNC", 0, 25, SCREEN_RADIUS, SCREEN_RADIUS, SDL_WINDOW_SHOWN);
    if(window == NULL)
        return 1;

    ScreenSurface = SDL_GetWindowSurface(window);

    quit = false;
    startBP = false;
    startTime = false;
    currentString = 0;
    maxCurrentString = 0;
    Delay = 0;

    while(quit == false)
    {
        if(startBP == false)
            SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0xFF, 0, 0));
        if(startTime == true)
            SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0xFF, 0xFF, 0));
        if(startBP == true)
            SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0, 0xFF, 0));

        if(startTime == true)
        {
            Delay = ThreadDelay(Delay);
            if(Delay == DEFAULT_TIME)
            {
                Delay = 0;
                startBP = true;
                startTime = false;
                start = clock();
            }
        }

        if(startBP == true)
        {
            Delay = ThreadDelay(Delay);

        }

        if(startBP == true && Delay == TIME_PER_CARACTER && quit == false)
        {
            for(currentString = 0; currentString <= maxCurrentString; currentString++)
            {
                input.ki = kbi[currentString];
                SendInput(1, &input, sizeof(INPUT));
            }

            kbi[0].wVk += 1;

            for(currentString = 0; currentString <= maxCurrentString; currentString++)
            {

                if(kbi[currentString].wVk > 0x39 && kbi[currentString].wVk < 0x40)
                {
                    kbi[currentString].wVk = 0x41;
                }

                if(kbi[currentString].wVk >= 0x5B)
                {
                    kbi[currentString].wVk = 0x30;
                    if(currentString < maxString)
                    {
                        kbi[currentString + 1].wVk += 1;
                    }
                    if(currentString == maxCurrentString && maxCurrentString < maxString - 1)
                    {
                        maxCurrentString++;
                    }
                    if(currentString == maxCurrentString && maxCurrentString == maxString - 1)
                    {
                        quit = true;
                    }
                }
            }
            input.ki = enterKey;
            SendInput(1, &input, sizeof(INPUT));
        }

        if(Delay >= TIME_PER_CARACTER && startBP == true)
        {
            Delay = 0;
        }

        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                startTime = true;
                SDL_FillRect(ScreenSurface, NULL, SDL_MapRGB(ScreenSurface->format, 0, 0xFF, 0));
            }
        }
        SDL_UpdateWindowSurface(window);
    }

    now = time(0);
    printf("O programa fechou em: %s\n", ctime(&now));

    finish = clock();
    if(finish - start < 1000)
        printf("o programa rodou por: %g ms\n", (double)(finish - start)*1000/CLOCKS_PER_SEC);
    else
    {
        if(finish - start < 60000)
            printf("o programa rodou por: %g s\n", (double)(finish - start)/CLOCKS_PER_SEC);
        else
        {
            if(finish - start < 3600000)
                printf("o programa rodou por: %.0g m %g s\n", (double)(finish - start) / 60 / CLOCKS_PER_SEC, (double)((finish - start)/ CLOCKS_PER_SEC) - ((int)((finish - start)/60/CLOCKS_PER_SEC)*60));
            else
                printf("o programa rodou por: %.0g h %g m %g s\n", (double)(finish - start) / 3600 / CLOCKS_PER_SEC, (double)((finish - start)/ CLOCKS_PER_SEC) - ((int)((finish - start)/60/CLOCKS_PER_SEC)*60), ((double)((finish - start)/CLOCKS_PER_SEC) - ((int)((finish - start)/3600/CLOCKS_PER_SEC))*3600));
        }
    }


    SDL_FreeSurface(ScreenSurface);
    ScreenSurface = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();

    return 0;
}
