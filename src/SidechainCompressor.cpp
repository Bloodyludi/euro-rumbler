#include "SidechainCompressor.h"

#include <DaisySP-LGPL/Source/Dynamics/compressor.h>
#include <dev/sdram.h>

DSY_SDRAM_BSS Compressor ScComp;

SidechainCompressor::SidechainCompressor() {}

void SidechainCompressor::initialize(float sampleRate) {
    ScComp.Init(sampleRate);
    ScComp.SetAttack(0.01f);
    ScComp.SetRelease(0.05f);
    ScComp.SetThreshold(-30.0f);
    ScComp.SetRatio(16.0f);
    ScComp.AutoMakeup(true);
}

void SidechainCompressor::process(const float &dryL, const float &dryR, float *wetL, float *wetR, float &duckKey) {
  *wetL = ScComp.Process(dryL, duckKey);
  *wetR = ScComp.Process(dryR, duckKey);
}