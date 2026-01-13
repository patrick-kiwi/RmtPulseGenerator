# Arduino ESP32 RMT Pulse Sequence Synchronisation

This library allows you to send **synchronised pulse sequences to GPIO pins** on supported ESP32 chips with **zero processor overhead** by leveraging the **RMT (Remote Control) peripheral hardware**.

> ⚠️ **Not supported on standard ESP32 boards.**

| Board     | RMT Channels | Sync Manager Supported |
|-----------|--------------|------------------------|
| ESP32     | 8            | ❌ No                  |
| ESP32-S3  | 4            | ✅ Yes                 |
| ESP32-C3  | 2            | ✅ Yes                 |
| ESP32-C6  | 2            | ✅ Yes                 |

---

## Limitations

### Synchronisation

This library uses `.loop_count = -1` for continuous transmission without CPU involvement. However, using this approach, a **fixed phase offset** of **50–300 ns** occurs between channels. This offset is **constant over time**.

If you require **nanosecond-accurate synchronisation**, you must use `.loop_count = 0` (single-shot mode) and coordinate transmission start times **manually in your main loop**, using the sync manager.

### Tick Resolution

The default tick resolution is **1 µs**, set by `.resolution_hz = 1 MHz`.  
Note: `rmt_symbol_word_t` only allocates **15 bits for tick encoding**, so the **maximum pulse duration is 32,767 ticks (≈32.8 ms at 1 MHz)**.

### Callbacks

RMT callbacks **are not compatible** with this approach.  
Because the sequence is looped infinitely (`.loop_count = -1`), the RMT never triggers a completion event, so no callbacks are invoked.


## Dynamic Pattern Updates

The library supports changing pulse patterns on the fly while maintaining synchronization across all four channels.

### How It Works

The `update()` method allows you to change any or all channel patterns at runtime:

```cpp
rmt.update(newPatternA, patternB, patternC, patternD);
```

Internally, this briefly stops all channels (~microseconds), reconfigures them with the new patterns, recreates the sync manager, and restarts transmission. All channels remain perfectly synchronized after the update.

### Example Usage

```cpp
RmtPulseGenerator rmt(GPIO_NUM_10, GPIO_NUM_11, GPIO_NUM_12, GPIO_NUM_13);

// Define patterns
const std::vector<rmt_symbol_word_t> narrowPulse = {{
    { .duration0 = 1, .level0 = 1, .duration1 = 9, .level1 = 0 },
}};

const std::vector<rmt_symbol_word_t> widePulse = {{
    { .duration0 = 5, .level0 = 1, .duration1 = 9, .level1 = 0 },
}};

// Start with narrow pulse on channel 0
rmt.begin(narrowPulse, narrowPulse, narrowPulse, narrowPulse);

// Later, switch channel 0 to wide pulse
rmt.update(widePulse, narrowPulse, narrowPulse, narrowPulse);
```

### Notes

- The update causes a brief output gap (microseconds) during reconfiguration
- Pattern synchronization is automatically recalculated on each update
- The `begin()` method must be called before `update()`

