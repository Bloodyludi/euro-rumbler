#ifndef REVERB_PROCESSOR_H
#define REVERB_PROCESSOR_H

#include "DaisyDuino.h"

class ReverbProcessor {
private:
    float wetMix;
    float reverbL, reverbR, reverbMono;

public:
    ReverbProcessor();
    void initialize(float sampleRate);
    void process(const float &dryL, const float &dryR, float *wetL, float *wetR);
    void updateParameters(float wet, float feedback, float lpFreq);
};

#endif