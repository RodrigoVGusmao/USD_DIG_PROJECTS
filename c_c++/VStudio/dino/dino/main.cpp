#include<cstdint>
#include<exception>
#include<iostream>
#include<iterator>
#include<math.h>
#include<string>

#include<SDL.h>

#include"constants.h"
#include"extFiles.h"
#include"FPSHand.h"
#include"TextureHand.h"
#include"windowHand.h"
#include"2DSide.h"

template<class T>
void setValue(T*, uint8_t, int32_t, int32_t, int32_t, int32_t, int32_t, const SDL_Rect*, uint8_t);
template<class T>
int16_t speedCalc(T*, uint8_t, int32_t, int32_t, int32_t, int32_t, uint16_t, int16_t, int16_t);
template<class T>
int8_t detectCollision(T*, uint8_t, side::element);
side::element moveCollisionArea(side::element, side::element);

int main(int argc, char** argv)
{
    bool quit = false;
    bool cactusCollision = false;
    bool onAir = true;
    int8_t xSpeedMultiplier = 1;
    int8_t gravityMultiplier = 1;
    int16_t ySpeed = 0;
    int16_t outScreen = -1;
    uint32_t finalScore = 0;
    uint64_t highScoreValue = 0;
    float points = 0;
    float speed = static_cast<float>(movementSpeed);

    SDL_Event event;

    const side::element cactusMoveCollision = {{characterClip[0].w/10, 0, -characterClip[0].w/5, -characterClip[0].h/10},{0, 0},{0, 0}};
    const side::element groundMoveCollision = {{0, 0, 0, 0},{0, -groundClip.h-2},{0, 0}};
    SDL_Point newPos;

    try
    {
        sdlWindow::WindowHand mainWindow("main window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight);
        mainWindow.setWinColor(255);

        sdlTexture::TextureHand sprites(mainWindow.renderer, "sprites/sprites.png", &colorKey[0], &colorKey[1], &colorKey[2]);
        sprites.setCollisionBits(colorKey[0], colorKey[1], colorKey[2]);
        sprites.freeSurface();

        for (uint32_t i = 0; i < sprites.bitCollisionMap.size()-1; i++)
            std::cout << sprites.bitCollisionMap[i] << " ";

        sdlFps::FPSHand fps;

        file::extFiles highScoreFile("HScore.dtx");
        highScoreFile.readFile(&highScoreValue, 1);
        std::cout << static_cast<unsigned long>(highScoreValue) << std::endl;

        side::staticBackgroundObject* framerate = new side::staticBackgroundObject[fpsDigitNum];
        setValue(framerate, fpsDigitNum, scoreOffsetX-numbers[0].w+digitDistance, numbers[0].w+digitDistance, winHeight-scoreOffsetY, 1, 1, NULL, 0);

        side::staticBackgroundObject* score = new side::staticBackgroundObject[scoreDigitNum];
        setValue(score, scoreDigitNum, scoreOffsetX-numbers[0].w+digitDistance, numbers[0].w+digitDistance, scoreOffsetY, 1, 1, NULL, 0);

        side::staticBackgroundObject* HScore = new side::staticBackgroundObject[scoreDigitNum+2];
        setValue(HScore, scoreDigitNum+2, 2*scoreOffsetX-winWidth-2*(numbers[0].w+digitDistance), numbers[0].w+digitDistance, scoreOffsetY, 1, 1, NULL, 0);
        HScore[0].image.clip = highScore[0];
        HScore[1].image.clip = highScore[1];

        side::staticBackgroundObject* clouds = new side::staticBackgroundObject[cloudNum];
        setValue(clouds, cloudNum, winWidth, cloudClip.w + 20, winHeight/3-25, winWidth/4, winHeight/3, &cloudClip, 1);

        side::staticObject* ground = new side::staticObject[groundNum];
        setValue(ground, groundNum, -groundClip.w, groundClip.w, baseYPosition, 1, 1, &groundClip, 1);

        side::staticObject* cactus = new side::staticObject[cactusNum];
        setValue(cactus, cactusNum, winWidth, winWidth/3, baseYPosition, 3*winWidth/4, 1, cactusClip, std::end(cactusClip)-std::begin(cactusClip));

        side::character player(3, 5, characterClip);
        player.setAnimation(0, 0, 2, walkFrequency);
        player.setAnimation(1, 3, 2, walkFrequency);
        player.setAnimation(2, 2, 1, 1);
        setValue(&player, 1, playerXPosition, 0, baseYPosition-characterClip[0].h-100, 1, 1, &characterClip[0], 1);

        while(!quit)
        {
            if(SDL_PollEvent(&event))
                if(event.type == SDL_QUIT)
                    quit = true;
            const uint8_t* keyStates = SDL_GetKeyboardState(NULL);

            //calculations
            speedCalc(clouds, cloudNum,  cloudClip.w + 20, winHeight/3-25, 4*cloudClip.w, winHeight/3, fps.fps, backgroundSpeed*xSpeedMultiplier, 0);
            speedCalc(ground, groundNum, groundClip.w, baseYPosition, 1, 1, fps.fps, static_cast<int>(speed*xSpeedMultiplier), 0);
            speedCalc(&player, 1, 0, 0, 1, 1, fps.fps, 0, ySpeed);

            if(detectCollision(ground, groundNum, moveCollisionArea(groundMoveCollision, player.image)) >= 0)
            {
                ySpeed = 0;
                onAir = false;
                //newPos = {ground[i].collPoint.x-moveCollision.x, ground[i].collPoint.y-moveCollision.y};
                newPos = {playerXPosition, baseYPosition-characterClip[0].h+2};
                player.setPosition(newPos);
            }

            outScreen = speedCalc(cactus, cactusNum, winWidth/3, baseYPosition, 3*winWidth/4, 1, fps.fps, static_cast<int>(speed)*xSpeedMultiplier, 0);
            if(outScreen != -1)
            {
                cactus[outScreen].image.clip = cactusClip[rand()%(std::end(cactusClip)-std::begin(cactusClip))];
                cactus[outScreen].setPosition(cactus[outScreen].image.position.x, baseYPosition-cactus[outScreen].image.clip.h);
            }
            if(detectCollision(cactus, cactusNum, moveCollisionArea(cactusMoveCollision, player.image)) >= 0)
            {
                cactusCollision = true;
                xSpeedMultiplier = 0;
            }
            if(!cactusCollision)
            {
                if((keyStates[SDL_SCANCODE_SPACE] || keyStates[SDL_SCANCODE_UP]) && !onAir)
                {
                    ySpeed = jumpSpeed;
                    onAir = true;
                }
                if(keyStates[SDL_SCANCODE_DOWN])
                {
                    player.playAnimation(1, fps.fps);
                    gravityMultiplier = 2;
                    ySpeed = (ySpeed > 0) ? ySpeed:0;
                }
                else
                {
                    player.playAnimation(0, fps.fps);
                    gravityMultiplier = 1;
                }

                speed -= 4.0f/static_cast<float>(fps.fps);

                player.setAnimation(0, 0, 2, static_cast<int>(std::ceil(walkFrequency*speed/movementSpeed)));
                player.setAnimation(1, 3, 2, static_cast<int>(std::ceil(walkFrequency*speed/movementSpeed)));

                points += 1000.f/(pointFrequency*movementSpeed/speed)/fps.fps;
            }
            else
            {
                finalScore = static_cast<uint32_t>(std::floor(points));
                player.playAnimation(2, fps.fps);
                if(finalScore > highScoreValue)
                {
                    highScoreFile.refreshFile();
                    highScoreFile.writeFile(static_cast<void*>(&finalScore), FILE_UNSIGNED_INT, FILE_UNSIGNED_INT);
                }
            }
            if(fps.fps != 0)
            ySpeed += gravity*gravityMultiplier/fps.fps;

            uint32_t overScore = 0;
            for(uint8_t i=0; i<scoreDigitNum; i++)
            {
                uint8_t clip = static_cast<int>(points/pow(10,(scoreDigitNum-1)-i));
                overScore = overScore*10;
                if(clip>9)
                    clip -= overScore;
                overScore += clip;
                score[i].image.clip = numbers[clip];
            }

            overScore = 0;
            for(uint8_t i=0; i<fpsDigitNum; i++)
            {
                uint8_t clip = static_cast<int>(fps.fps/pow(10,(fpsDigitNum-1)-i));
                overScore = overScore*10;
                if(clip>9)
                    clip -= overScore;
                overScore += clip;
                framerate[i].image.clip = numbers[clip];
            }

            overScore = 0;
            for(uint8_t i=0; i<scoreDigitNum; i++)
            {
                uint8_t clip = static_cast<int>(highScoreValue/pow(10,(scoreDigitNum-1)-i));
                overScore = overScore*10;
                if(clip>9)
                    clip -= overScore;
                overScore += clip;
                HScore[i+2].image.clip = numbers[clip];
            }

            std::cout << speed << std::endl << points << std::endl << "points/frame: " << 1000.f/(pointFrequency*movementSpeed/speed)/fps.fps << std::endl;
            std::cout << "px/f*f: " << 4.0f/static_cast<float>(fps.fps) << std::endl;
            std::cout << "default point frequency: " << static_cast<int>(pointFrequency) << std::endl << "point frequency: " << pointFrequency*movementSpeed/speed << std::endl;

            //rendering
            SDL_RenderClear(mainWindow.renderer);

            for(uint8_t i=0; i<scoreDigitNum; i++)
                mainWindow.blitSurface(sprites.texture, &score[i].image.clip, &score[i].image.position);
            for(uint8_t i=0; i<scoreDigitNum+2; i++)
                mainWindow.blitSurface(sprites.texture, &HScore[i].image.clip, &HScore[i].image.position);
            for(uint8_t i=0; i<fpsDigitNum; i++)
                mainWindow.blitSurface(sprites.texture, &framerate[i].image.clip, &framerate[i].image.position);
            for(uint8_t i=0; i<cloudNum; i++)
                mainWindow.blitSurface(sprites.texture, &clouds[i].image.clip, &clouds[i].image.position);
            for(uint8_t i=0; i<groundNum; i++)
                mainWindow.blitSurface(sprites.texture, &ground[i].image.clip, &ground[i].image.position);
            for(uint8_t i=0; i<cactusNum; i++)
                mainWindow.blitSurface(sprites.texture, &cactus[i].image.clip, &cactus[i].image.position);
            mainWindow.blitSurface(sprites.texture, &player.image.clip, &player.image.position);

            SDL_RenderPresent(mainWindow.renderer);

            fps.capFPS(fpsCap);
            fps.frameRateCalc();
        }
        delete[] clouds;
        delete[] ground;
        delete[] cactus;

    }
    catch(std::exception&)
    {
        return 1;
    }
    catch(uint8_t error)
    {
        return error;
    }
    SDL_Quit();

    return 0;
}

template<class T>
void setValue(T* objects, uint8_t elementsNum, int32_t xPointPositionStart, int32_t minOffsetX, int32_t minOffsetY, int32_t maxOffsetX, int32_t maxOffsetY, const SDL_Rect* clip, uint8_t clipsNum)
{
    int clipHeight = 0;

    for(uint8_t i=0; i<elementsNum; i++)
    {
        if(clip != NULL)
        {
            objects[i].image.clip = clip[rand() % clipsNum];
            clipHeight = objects[i].image.clip.h;
        }
        if(i != 0)
        {
            objects[i].setPosition(objects[i-1].image.position.x + minOffsetX + rand() % maxOffsetX, -clipHeight + minOffsetY + rand() % maxOffsetY);
        }
        else
            objects[i].setPosition(xPointPositionStart + minOffsetX + rand() % maxOffsetX, -clipHeight + minOffsetY + rand() % maxOffsetY);
    }
}

template<class T>
int16_t speedCalc(T* objects, uint8_t elementsNum, int32_t minOffsetX, int32_t minOffsetY, int32_t maxOffsetX, int32_t maxOffsetY, uint16_t fps, int16_t xSpeed, int16_t ySpeed)
{
    int16_t outScreen = -1;
    side::velocity vel = {xSpeed, ySpeed};

    for(uint8_t i=0; i<elementsNum; i++)
    {
        objects[i].speedCalculation(fps, vel);
        if(objects[i].image.position.x+objects[i].image.clip.w < 0)
        {
            if(i != 0)
                objects[i].setPosition(objects[i-1].image.position.x + minOffsetX + rand() % maxOffsetX, -objects[i].image.clip.h + minOffsetY + rand() % maxOffsetY);
            else
            {
                objects[i].setPosition(objects[elementsNum-1].image.position.x + minOffsetX + rand() % maxOffsetX, -objects[i].image.clip.h + minOffsetY + rand() % maxOffsetY);
                objects[i].speedCalculation(fps, vel);
            }
            outScreen = i;
        }
    }
    return outScreen;
}

template<class T>
int8_t detectCollision(T* objects, uint8_t elementsNum, side::element element)
{
    for(uint8_t i=0; i<elementsNum; i++)
    {
        objects[i].getCollisionState(element);
        if(objects[i].collision)
            return i;
    }
    return -1;
}

side::element moveCollisionArea(side::element moveArea, side::element element)
{
    side::element result;
    result.position.x = element.position.x + moveArea.position.x;
    result.position.y = element.position.y + moveArea.position.y;
    result.oldPosition.x = element.oldPosition.x + moveArea.position.x;
    result.oldPosition.y = element.oldPosition.y + moveArea.position.y;
    result.clip.x = element.clip.x + moveArea.clip.x;
    result.clip.y = element.clip.y + moveArea.clip.y;
    result.clip.w = element.clip.w + moveArea.clip.w;
    result.clip.h = element.clip.h + moveArea.clip.h;

    return result;
}
