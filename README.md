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

####Synchronisation

Behind the scenes the transmit configuration variable .loop_count=-1 sets an infinite loop.  Even with synch manager set up, and with a perfectly matching the number of ticks between channels this approach still results in imperfect synchronisation.  There’s a small phase shift ranging from 50 to 300 ns which is constant with respect to time, but varies from example to example.

####Tick Number

The rmt_symbol_word_t has only 15 bits to encode ticks.  By default the .resolution_hz is set to 1MHz meaning each tick corresponds to 1 µs.  That tick-width can be changed if you wish.  

 
