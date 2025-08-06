# Project Overview

This project is a web-based controller for an "Infinity Mirror" device. It consists of a single `index.html` file that uses HTML, CSS, and JavaScript to create a user interface for controlling the device's LED patterns. The controller communicates with the Infinity Mirror via the Web Bluetooth API.

## Building and Running

This is a static web page and does not require a build process. To run the controller, simply open the `index.html` file in a web browser that supports the Web Bluetooth API (e.g., Chrome on a desktop or Android device).

1.  Open `index.html` in a compatible web browser.
2.  Click the "BLE接続" (BLE Connect) button to search for and connect to the "InfinityMirror" device.
3.  Once connected, use the buttons to change the LED patterns on the device.

## Development Conventions

*   **Structure:** All code (HTML, CSS, and JavaScript) is contained within the `index.html` file.
*   **Styling:** CSS is used for styling the user interface, with different color gradients for each button.
*   **Functionality:** JavaScript is used to handle the Web Bluetooth API connection and to send commands to the device.
*   **Commands:** The controller sends string-based commands to the device, such as `MODE:0` for the rainbow pattern.
