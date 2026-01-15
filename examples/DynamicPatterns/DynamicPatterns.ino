#include <Arduino.h>
#include "RmtPulseGenerator_4chan.h"

// Create the RMT pulse generator (global so we can access from loop)
RmtPulseGenerator rmt(GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13);

// Pattern A: 5µs high, 15µs low (longest on-off sequence dictates frequency)
const std::vector<rmt_symbol_word_t> pulseSequenceA = {{
    { .duration0 = 5, .level0 = 1, .duration1 = 15, .level1 = 0 },
}};

// Pattern X: 1µs high (alternate for channel B)
const std::vector<rmt_symbol_word_t> pulseSequenceX = {{
    { .duration0 = 1, .level0 = 1, .duration1 = 1, .level1 = 0 },
}};

// Patterns B, C, D stay the same
const std::vector<rmt_symbol_word_t> pulseSequenceB = {{
    { .duration0 = 12, .level0 = 1, .duration1 = 6, .level1 = 0 },
}};

const std::vector<rmt_symbol_word_t> pulseSequenceC = {{
    { .duration0 = 15, .level0 = 0, .duration1 = 4, .level1 = 1 },
}};

const std::vector<rmt_symbol_word_t> pulseSequenceD = {{
    { .duration0 = 15, .level0 = 0, .duration1 = 2, .level1 = 1 },
}};

bool usePatternX = false;  // Track which pattern set we're using

void setup() {
    Serial.begin(115200);
    
    // Wait for serial to connect (useful for USB serial)
    delay(1000);
    
    Serial.println("RMT Dynamic Pattern Test");
    Serial.println("========================");
    Serial.println("Press 'a' + Enter to alternate between patterns A and X on channel 0");
    Serial.println("Currently using: Pattern A (1µs high, 9µs low)");
    Serial.println();

    // Start with original patterns A, B, C, D
    rmt.begin(pulseSequenceA, pulseSequenceB, pulseSequenceC, pulseSequenceD);
}

void loop() {
    // Check for serial input
    if (Serial.available() > 0) {
        char c = Serial.read();
        
        // Ignore newline/carriage return characters
        if (c == '\n' || c == '\r') {
            return;
        }
        
        if (c == 'a' || c == 'A') {
            // Toggle between pattern A and pattern X
            usePatternX = !usePatternX;
            
            if (usePatternX) {
                Serial.println("Switching to Pattern X (5µs high, 9µs low) on channel 0...");
                rmt.update(pulseSequenceA, pulseSequenceX, pulseSequenceC, pulseSequenceD);
                Serial.println("Done! Now using: A, X, C, D");
            } else {
                Serial.println("Switching to Pattern A (1µs high, 9µs low) on channel 0...");
                rmt.update(pulseSequenceA, pulseSequenceB, pulseSequenceC, pulseSequenceD);
                Serial.println("Done! Now using: A, B, C, D");
            }
            Serial.println();
        } else {
            Serial.print("Unknown command: '");
            Serial.print(c);
            Serial.println("' - Press 'a' to alternate patterns");
        }
    }
}
