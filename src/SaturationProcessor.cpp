#include "SaturationProcessor.h"

SaturationProcessor::SaturationProcessor() {
}

void SaturationProcessor::initialize(float sampleRate) {
    driveProcessorL.Init();
    driveProcessorR.Init();
}

void SaturationProcessor::process(const float &inL, const float &inR, float *outL, float *outR) {
    *outL = driveProcessorL.Process(inL);
    *outR = driveProcessorR.Process(inR);
}

void SaturationProcessor::updateParameters(float drive, float driveCV) {
    drive = 0.3f + std::clamp(drive + driveCV, 0.f, 1.f) * 0.2f;
    driveProcessorL.SetDrive(drive);
    driveProcessorR.SetDrive(drive);
}