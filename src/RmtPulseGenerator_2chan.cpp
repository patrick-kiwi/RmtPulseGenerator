#include "RmtPulseGenerator_2chan.h"

RmtPulseGenerator::RmtPulseGenerator(gpio_num_t gpioA, gpio_num_t gpioB) {
    tx_gpio_number[0] = gpioA;
    tx_gpio_number[1] = gpioB;
    tx_channels[0] = nullptr;
    tx_channels[1] = nullptr;
    copyEncoder = nullptr;
}

void RmtPulseGenerator::begin(const std::vector<rmt_symbol_word_t>& patternA, const std::vector<rmt_symbol_word_t>& patternB) {
    auto [syncedA, syncedB] = synchronize_patterns(patternA.data(), patternA.size(),
                                                    patternB.data(), patternB.size());
    for (int i = 0; i < 2; ++i) {
        rmt_tx_channel_config_t config = {
            .gpio_num = tx_gpio_number[i],
            .clk_src = RMT_CLK_SRC_DEFAULT,
            .resolution_hz = 1000000,
            .mem_block_symbols = 48,
            .trans_queue_depth = 8
        };
        ESP_ERROR_CHECK(rmt_new_tx_channel(&config, &tx_channels[i]));
        ESP_ERROR_CHECK(rmt_enable(tx_channels[i]));
    }
    rmt_sync_manager_handle_t sync_mgr = nullptr;
    rmt_sync_manager_config_t sync_config = {
        .tx_channel_array = tx_channels,
        .array_size = 2
    };
    ESP_ERROR_CHECK(rmt_new_sync_manager(&sync_config, &sync_mgr));
    rmt_copy_encoder_config_t enc_config = {};
    ESP_ERROR_CHECK(rmt_new_copy_encoder(&enc_config, &copyEncoder));
    rmt_transmit_config_t tx_config = { .loop_count = -1 };
    
    // Note: We use the `data()` and `size()` methods to get the raw array and its size
    ESP_ERROR_CHECK(rmt_transmit(tx_channels[0], copyEncoder, syncedA.data(), syncedA.size() * sizeof(rmt_symbol_word_t), &tx_config));
    ESP_ERROR_CHECK(rmt_transmit(tx_channels[1], copyEncoder, syncedB.data(), syncedB.size() * sizeof(rmt_symbol_word_t), &tx_config));
}