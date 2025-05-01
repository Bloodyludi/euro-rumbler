# Kick Rumble Chain

## Description

This effect is designed to add a rumble to a kick drum. It is intended to be used in parallel with the original kick drum signal.

## Signal chain

```
Signal
|
|-> DRY
|-> WET

DRY
|
-> MIX

WET
|
Reverb + Delay
|
Saturation
|
LPF
|
Mono
|
Sidechain
|
-> MIX

DRY + WET MIX
|
(light) Saturation
|
EQ
    200Hz dip
|
LIMITER
    -1dB ceiling
|
=> [OUT]
```
