#include "DaisyDuino.h"
#include "SaturationProcessor.h"
#include "ReverbProcessor.h"
#include "ClockedDelay.h"
#include "SidechainCompressor.h"

DaisyHardware hw;
size_t num_channels;
float sample_rate;

static float wetMix = 1;
static float pot1, pot2, pot3, pot4, cv1, cv2;

ReverbProcessor *reverb;
SaturationProcessor *saturation;
ClockedDelay *clockedDelay;
SidechainCompressor *sidechain;
Svf *filter;
Limiter *limiter;

float CtrlVal(uint8_t pin) {
    analogReadResolution(16);
    return constrain(analogRead(pin) / 65535.f, 0.f, 1.f);
}

void callback(float **in, float **out, size_t size) {
    float dryL = 0, dryR = 0, dryMono = 0, revL = 0, revR = 0, delL = 0, delR = 0, wetL = 0, wetR = 0;
    float lBuffer[2];

    for (size_t i = 0; i < size; i++) {
        dryL = in[0][i];
        dryR = in[1][i];

        // Apply the saturation to the dry signal
        saturation->process(dryL, dryR, &dryL, &dryR);

        clockedDelay->process(dryL, dryR, &delL, &delR);
        reverb->process(dryL, dryR, &revL, &revR);

        wetL = revL * .8f + delL * .6f;
        wetR = revR * .8f + delR * .6f;

        // // Lowpass wet signal at 1kHz
        filter->SetFreq(4000.f);
        filter->Process(wetL);
        wetL = filter->Low();
        filter->Process(wetR);
        wetR = filter->Low();

        // Compress the wet signal
        dryMono = (dryL + dryR) * 0.5f;
        sidechain->process(wetL, wetR, &wetL, &wetR, dryMono);

        // lBuffer[0] = dryL + wetL * wetMix;
        // lBuffer[1] = dryR + wetR * wetMix;
        // limiter->ProcessBlock(lBuffer, 2, 0.5f);
        // out[0][i] = lBuffer[0];
        // out[1][i] = lBuffer[1];

        out[0][i] = SoftLimit(dryL + wetL * wetMix);
        out[1][i] = SoftLimit(dryR + wetR * wetMix);
    }

}

void setup() {
    hw = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    num_channels = hw.num_channels;
    sample_rate = DAISY.get_samplerate();

    saturation = new SaturationProcessor();
    saturation->initialize(sample_rate);

    reverb = new ReverbProcessor();
    reverb->initialize(sample_rate);

    clockedDelay = new ClockedDelay();
    clockedDelay->initialize(sample_rate);

    filter = new Svf();
    filter->Init(sample_rate);

    sidechain = new SidechainCompressor();
    sidechain->initialize(sample_rate);

    limiter = new Limiter();
    limiter->Init();

    DAISY.begin(callback);
}

void loop() {
    pot1 = CtrlVal(A0);
    pot2 = CtrlVal(A1);
    pot3 = CtrlVal(A2);
    pot4 = CtrlVal(A3);
    cv1 = 1 - CtrlVal(A4);
    cv2 = 1 - CtrlVal(A5);

    wetMix = pot1;

    reverb->updateParameters(1.0f, pot2, 20000);
    clockedDelay->updateParameters(pot4, pot3, cv1, 0);
    saturation->updateParameters(pot1, .0f);
}