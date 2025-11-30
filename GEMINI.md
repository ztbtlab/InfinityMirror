# Project Overview

This project is a web-based controller for an "Infinity Mirror" device.

## Project Structure

*   `web/`: Contains the web-based controller (`index.html`).
*   `firmware/`: Contains the microcontroller source code.

## Building and Running (Web Controller)

The web controller is a static web page.

1.  Open `web/index.html` in a compatible web browser (Chrome, Edge, etc. supporting Web Bluetooth API).
2.  Click the "BLE接続" (BLE Connect) button to search for and connect to the "InfinityMirror" device.
3.  Once connected, use the buttons to change the LED patterns on the device.

## Development Conventions

*   **Web:** HTML, CSS, and JavaScript are contained within `web/index.html`.
*   **Firmware:** Microcontroller code resides in the `firmware/` directory.
