#include "ReverbProcessor.h"

#include <DaisySP-LGPL/Source/Effects/reverbsc.h>
#include <dev/sdram.h>

using namespace daisysp;

// Declare the ReverbSc object as a global variable with DSY_SDRAM_BSS
DSY_SDRAM_BSS ReverbSc ReverbProcessorVerb;

ReverbProcessor::ReverbProcessor() : wetMix(0.0f), reverbL(0.0f), reverbR(0.0f), reverbMono(0.0f) {}

void ReverbProcessor::initialize(float sampleRate) {
    ReverbProcessorVerb.Init(sampleRate);
    ReverbProcessorVerb.SetFeedback(0.95f);
    ReverbProcessorVerb.SetLpFreq(16000.0f);
}

void ReverbProcessor::process(const float &dryL, const float &dryR, float *wetL, float *wetR) {
    ReverbProcessorVerb.Process(dryL, dryR, wetL, wetR);

    // Make reverb mono by averaging left and right channels
    // TODO: Decide if we want to use stereo or mono reverb
    // reverbMono = (reverbL + reverbR) * 0.5f;

    // *wetL = dryL * (1 - wetMix) + reverbMono * wetMix;
    // *wetR = dryR * (1 - wetMix) + reverbMono * wetMix;
}

// feedback = cv1 + CtrlVal(A2)
// lpFreq = cv2 + CtrlVal(A3)
void ReverbProcessor::updateParameters(float wet, float feedback, float lpFreq) {
    ReverbProcessorVerb.SetFeedback(0.2f + std::clamp(feedback, 0.f, 1.f) * .4f);
    // ReverbProcessorVerb.SetLpFreq(std::clamp(lpFreq, 0.f, 1.f) * 20000.0f);

    // wetMix = std::clamp(wet, 0.f, 1.f);
}
