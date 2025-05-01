#ifndef CLOCKED_DELAY_H
#define CLOCKED_DELAY_H

#include "DaisyDuino.h"

class ClockedDelay {
    float dryL = 0, dryR = 0, delayOutL = 0, delayOutR = 0, targetDelayTimeL = 0, targetDelayTimeR = 0;
    float delayFeedback = 0.5f;
    float previousFbCVValue = 0;
    float delayChannelOffset = 1;


    bool isReceivingClockInput = false;
    bool hasReceivedClockInput = false;
    int numClocksReceived = 0;
    float clockDivisor = 1;
    float delayTimeSecs = 0.4f;
    float dividedTargetDelayTime = 0;
    const float maxFeedback = 0.95f;
    uint32_t previousTimestamp = 0;
    uint32_t clockTickDeltas[3] = {0, 0, 0};

public:
    ClockedDelay();
    void initialize(float sampleRate);
    void process(const float &dryL, const float &dryR, float *wetL, float *wetR);
    void updateFeedback(float fbPot, float fbCV);
    void updateClockDivisor(float divisorPot);
    void updateClock(float clockCV);
    void updateParameters(float fbPot, float divisorPot, float clockCV, float fbCV);
};
#endif
