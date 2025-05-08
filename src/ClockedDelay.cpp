#include "ClockedDelay.h"

#include <system.h>
#include <dev/sdram.h>

using namespace daisy;

constexpr float CV_CHANGE_TOLERANCE = .01f;
constexpr float DELAY_SAMPLE_RATE = 48000;
constexpr size_t MAX_DELAY = static_cast<size_t>(DELAY_SAMPLE_RATE * 16.0f); // 16 Secs

DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS delayL;
DelayLine<float, MAX_DELAY> DSY_SDRAM_BSS delayR;

ClockedDelay::ClockedDelay(){}

void ClockedDelay::process(const float& dryL, const float& dryR, float* wetL, float* wetR)
{
    // Set the delay time (one-pole filter smooths out the changes)
    fonepole(targetDelayTimeL, dividedTargetDelayTime, .0001f);

    // Stereo effect: Offset the delay time of the Right channel
    fonepole(targetDelayTimeR, dividedTargetDelayTime * delayChannelOffset, .0001f);

    // Set delay and feedback
    delayL.SetDelay(targetDelayTimeL);
    delayR.SetDelay(targetDelayTimeR);
    delayOutL = delayL.Read();
    delayOutR = delayR.Read();
    delayL.Write(delayFeedback * delayOutL + dryL);
    delayR.Write(delayFeedback * delayOutR + dryR);

    // Create final dry/wet mix and send to the output with soft limiting
    *wetL = SoftLimit(delayOutL);
    *wetR = SoftLimit(delayOutR);
}

void ClockedDelay::initialize(float sampleRate)
{
    delayL.Init();
    delayR.Init();

    //TODO: Set the sample rate for the delay lines
}

void ClockedDelay::updateFeedback(float fbPot, float fbCV)
{
    // Ignore feedback CV changes smaller than the tolerance
    if (fabsf(fbCV - previousFbCVValue) > CV_CHANGE_TOLERANCE)
    {
        fonepole(previousFbCVValue, fbCV, 0.001f); // Smoother transition
    }

    delayFeedback = maxFeedback * std::clamp(previousFbCVValue + fbPot, 0.0f, 1.0f);
}

void ClockedDelay::updateClockDivisor(float divisorPot)
{
    if (divisorPot == 0.0f)
    {
        clockDivisor = 8;
    }
    else if (divisorPot <= 0.1f)
    {
        clockDivisor = 4;
    }
    else if (divisorPot <= 0.2f)
    {
        clockDivisor = 3; // half note triplets
    }
    else if (divisorPot <= 0.3f)
    {
        clockDivisor = 2;
    }
    else if (divisorPot <= 0.4f)
    {
        clockDivisor = 1.5; // whole note triplets
    }
    else if (divisorPot <= 0.5f)
    {
        clockDivisor = 1;
    }
    else if (divisorPot <= 0.6f)
    {
        clockDivisor = 0.5; // x2
    }
    else if (divisorPot <= 0.7f)
    {
        clockDivisor = 0.33; // x3
    }
    else if (divisorPot <= 0.8f)
    {
        clockDivisor = 0.25; // x4
    }
    else if (divisorPot <= 0.9f)
    {
        clockDivisor = 0.2; // x5
    }
    else
    {
        clockDivisor = 0.125; // x8
    }
}

void ClockedDelay::updateClock(float clockCV)
{
    // Check for clock input on CV1
    isReceivingClockInput = clockCV >= 0.5f;
    if (isReceivingClockInput && !hasReceivedClockInput) // rising edge of the incoming gate/trigger
    {
        numClocksReceived++;
        int maxClockTicks = std::size(clockTickDeltas);
        int index = numClocksReceived % maxClockTicks;

        // We have a previous timestamp, get the time between that and now
        if (previousTimestamp != 0)
        {
            // Add the time diff in Ms to an array/circular buffer (this is used to smooth out the bumps a little)
            clockTickDeltas[index] = System::GetNow() - previousTimestamp;

            // If we have >= maxTicks clock timings in our buffer, calculate the average time between the last three then update the delay time
            if (numClocksReceived >= maxClockTicks)
            {
                float sum = 0;
                for (const unsigned long dur : clockTickDeltas)
                {
                    sum += dur;
                }
                delayTimeSecs = sum / maxClockTicks / 1000.0f; // convert to seconds
                numClocksReceived = 0; // reset the counter
            }
        }

        previousTimestamp = System::GetNow();
    }
    hasReceivedClockInput = isReceivingClockInput;

    // Set the target delay time based on the divisor
    dividedTargetDelayTime = delayTimeSecs * DELAY_SAMPLE_RATE / clockDivisor;
}

void ClockedDelay::updateParameters(float fbPot, float divisorPot, float clockCV, float fbCV)
{
    updateFeedback(fbPot, fbCV);
    updateClockDivisor(divisorPot);
    updateClock(clockCV);

    //TODO: Update right channel offset:
    // delayChannelOffset = 1 - offset / 14;
}
