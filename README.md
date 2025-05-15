# Arduino Esp32 RMT Pulse Sequence Synchronisation
Sends synchronised pulses to GPIO pins with zero processor overhead using the RMT peripheral hardware.  Note that this doesn't work on a standard esp32 board.   
```
Board     | Number of RMT Channels | Sync Manager Supported
ESP32     |     8                  |     NO
ESP32-S3  |     4                  |     YES
ESP32-C3  |     2                  |     YES
ESP32-C6  |     2                  |     YES
```
### Limitations

#### Synchronisation

Behind the scenes the transmit configuration variable .loop_count=-1 sets an infinite loop (no cpu overhead).  However, this approach still results in imperfect synchronisation.  Specifically, a small phase shift ranging from 50 to 300 ns which is constant with respect to time, but varies from example to example.  If you need nanosecond perfect synchronisation then you need to lean on the cpu with a single shot approach (.loop_count=0) where transmits and synch manager are contained in the loop part of the code.   

#### Tick Number

Default tick-width is 1 us set by the .resolution_hz @ 1MHz.  Bare in mind that the rmt_symbol_word_t only has 15 bits to encode ticks.   

#### Callbacks
 
RMT callbacks can't be used with this approach because the pulse sequence never finishes to initiate the callback. 