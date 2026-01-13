#pragma once


#include <driver/rmt_tx.h>
#include <vector>
#include <utility> // for std::pair

// Utility to count total ticks in a pulse pattern
inline uint32_t total_ticks(const rmt_symbol_word_t *pattern, size_t len) {
    uint32_t total = 0;
    for (size_t i = 0; i < len; ++i) {
        total += pattern[i].duration0 + pattern[i].duration1;
    }
    return total;
}

// Utility to pad a pattern with dummy ticks to equalize timing
inline std::vector<rmt_symbol_word_t> pad_pattern(const rmt_symbol_word_t *pattern, size_t len, uint32_t pad_ticks) {
    std::vector<rmt_symbol_word_t> result(pattern, pattern + len);
    if (pad_ticks > 0) {
        result.push_back({ .duration0 = pad_ticks, .level0 = 0, .duration1 = 0, .level1 = 0 });
    }
    return result;
}

// Utility to synchronize both patterns to the longer one
inline std::pair<std::vector<rmt_symbol_word_t>, std::vector<rmt_symbol_word_t>>
synchronize_patterns(const rmt_symbol_word_t *patternA, size_t lenA,
                     const rmt_symbol_word_t *patternB, size_t lenB) {
    uint32_t ticksA = total_ticks(patternA, lenA);
    uint32_t ticksB = total_ticks(patternB, lenB);
    
    if (ticksA > ticksB) {
        return { std::vector<rmt_symbol_word_t>(patternA, patternA + lenA),
                 pad_pattern(patternB, lenB, ticksA - ticksB) };
    } else if (ticksB > ticksA) {
        return { pad_pattern(patternA, lenA, ticksB - ticksA),
                 std::vector<rmt_symbol_word_t>(patternB, patternB + lenB) };
    } else {
        return { std::vector<rmt_symbol_word_t>(patternA, patternA + lenA),
                 std::vector<rmt_symbol_word_t>(patternB, patternB + lenB) };
    }
}

class RmtPulseGenerator {
public:
    RmtPulseGenerator(gpio_num_t gpioA, gpio_num_t gpioB, gpio_num_t gpioC, gpio_num_t gpioD);
    void begin(const std::vector<rmt_symbol_word_t>& patternA, const std::vector<rmt_symbol_word_t>& patternB,
                const std::vector<rmt_symbol_word_t>& patternC, const std::vector<rmt_symbol_word_t>& patternD);
private:
    rmt_channel_handle_t tx_channels[4];
    gpio_num_t tx_gpio_number[4];
    rmt_encoder_handle_t copyEncoder;
};