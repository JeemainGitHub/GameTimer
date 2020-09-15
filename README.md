# GameTimer
This is a PlatformIO project for a timer, which can be used in parlor games.

Components:
- ATtiny85
- Neopixel 16 LED ring
- buzzer
- one touch button for setup and handling
- batterie case for 3x AA
- DC/DC step-up converter (0.8-4,5 VDC to 5 VDC)
- ISP programming interface

Functionality:
The timer contains a menue with 4 options:
1. heart massage and breath display for ACLS
2. game timer with animated led's
3. different animated light scenes
4. setup menu for tone and brightness

Library:
- ButtonFkt (returns click and three button hold times)
