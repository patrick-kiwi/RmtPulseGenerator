#include <Arduino.h>
#include "RmtPulseGenerator_4chan.h"


void setup() {
    // Setup the synchronised RMT peripheral on two GPIO pins
    RmtPulseGenerator rmt(GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13);

    
    const std::vector<rmt_symbol_word_t> pulseSequenceA = {{
        { .duration0 = 1, .level0 = 1, .duration1 = 9, .level1 = 0 },
    }};

    const std::vector<rmt_symbol_word_t> pulseSequenceB = {{
        { .duration0 = 1, .level0 = 1, .duration1 = 9, .level1 = 0 },
    }};

    const std::vector<rmt_symbol_word_t> pulseSequenceC = {{
        { .duration0 = 1, .level0 = 1, .duration1 = 9, .level1 = 0 },
    }};

    const std::vector<rmt_symbol_word_t> pulseSequenceD = {{
        { .duration0 = 1, .level0 = 1, .duration1 = 9, .level1 = 0 },
    }};


    // Begin the pulse sequences
    rmt.begin(pulseSequenceA, pulseSequenceB, pulseSequenceC, pulseSequenceD);
}

void loop() {
}
