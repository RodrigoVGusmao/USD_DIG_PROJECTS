#ifndef FPSHAND_H
#define FPSHAND_H

#include<cstdint>

namespace sdlFps
{
class FPSHand
{
    public:
        uint16_t fps;
        uint32_t delta;

        FPSHand();
        void frameRateCalc();
        void capFPS(uint16_t);

    private:
        uint32_t startTime;
};
} //namespace fps

#endif // FPSHAND_H
