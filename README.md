# embedded_web_server
A embedded web_server running on a ESP-32 microcontroller.

This software is compiled using arduino IDE in the sequence:

1. Send the contents of `upload_file/` to the controllers flash memory, erasing all previous data;
2. Send `embedded_web_server.ino` as the firmware, maintaining the flash memory;
