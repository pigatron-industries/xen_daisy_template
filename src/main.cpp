#include "DaisyDuino.h"
#include "io/PitchInput.h"
#include "io/AnalogInput.h"

DaisyHardware hardware;

size_t num_channels;

Svf filter;
PitchInput pitchInput(A0);


#define LEFT 0
#define RIGHT 1

void AudioCallback(float **in, float **out, size_t size)
{
    filter.SetFreq(pitchInput.getFrequency());

    for (size_t i = 0; i < size; i++) {
        filter.Process(in[LEFT][i]);
        out[LEFT][i] = filter.Band();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Pigatron Industries");

    float sample_rate;
    // Initialize for Daisy pod at 48kHz
    hardware = DAISY.init(DAISY_SEED, AUDIO_SR_48K);
    num_channels = hardware.num_channels;
    sample_rate = DAISY.get_samplerate();

    // Initialize Filter, and set parameters.
    filter.Init(sample_rate);

    DAISY.begin(AudioCallback);
}

void loop() {
    pitchInput.update();
}
