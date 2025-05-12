#include <Arduino.h>
#include "RmtPulseGenerator.h"


void setup() {
    // Setup the synchronised RMT peripheral on two GPIO pins
    RmtPulseGenerator rmt(GPIO_NUM_7, GPIO_NUM_21);

    // Define a 2 µs pulse at 100 kHz
    const std::vector<rmt_symbol_word_t> pulseSequenceA = {{
        { .duration0 = 2, .level0 = 1, .duration1 = 8, .level1 = 0 },
    }};

    const std::vector<rmt_symbol_word_t> pulseSequenceB = {{
    // Align two 1 µs pulses, separated by 3 µs, to the trailing edge of the above 2 µs pulse
        { .duration0 = 2, .level0 = 0, .duration1 = 1, .level1 = 1 },
        { .duration0 = 3, .level0 = 0, .duration1 = 1, .level1 = 1 },
    }};

    // Begin the pulse sequences on the above-spedified GPIO output pins
    rmt.begin(pulseSequenceA, pulseSequenceB);
}

void loop() {
}
