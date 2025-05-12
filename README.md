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
Behind the scenes the transmit configuration contains .loop_count=-1 which sets an infinite loop.  Even with synchronisation manager installed, and with perfectly matching the number of ticks this approach still results in imperfect synchronisation.  There’s a small phase shift ranging from 50 to 300 ns which is constant with respect to time, but varies from example to example.
 
