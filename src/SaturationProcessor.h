#ifndef SATURATIONPROCESSOR_H
#define SATURATIONPROCESSOR_H

#include "daisysp.h"
using namespace daisysp;

class SaturationProcessor {
    Overdrive driveProcessorL;
    Overdrive driveProcessorR;
public:
    SaturationProcessor();
    void initialize(float sampleRate);
    void process(const float &inL, const float &inR, float *outL, float *outR);
    void updateParameters(float drive, float driveCV);
};

#endif
