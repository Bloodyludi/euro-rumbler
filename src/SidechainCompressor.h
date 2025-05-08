#ifndef SIDECHAINCOMPRESSOR_H
#define SIDECHAINCOMPRESSOR_H

#include "daisysp.h"
using namespace daisysp;

class SidechainCompressor {

public:
    SidechainCompressor();
    void initialize(float sampleRate);
    void process(const float &dryL, const float &dryR, float *wetL, float *wetR, float &duckKey);
};
#endif
