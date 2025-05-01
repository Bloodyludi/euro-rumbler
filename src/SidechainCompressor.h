#ifndef SIDECHAINCOMPRESSOR_H
#define SIDECHAINCOMPRESSOR_H

#include "DaisyDuino.h"

class SidechainCompressor {

public:
    SidechainCompressor();
    void initialize(float sampleRate);
    void process(const float &dryL, const float &dryR, float *wetL, float *wetR, float &duckKey);
};
#endif
