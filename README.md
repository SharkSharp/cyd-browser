# CYD-Browser

![](./media/showcase.gif)

## Overview

This project is a proof of concept for a server-side rendered browser for the CYD (CheapYellowDisplay).

## Motivation

I was frustrated trying to build pretty layouts with LVGL without having experience. As a web developer, I have experience with websites, so the idea of a browser on the CYD was very appealing. I built this proof of concept to see if it was viable, if others would be interested in this concept, and if it would make a great platform to build a desk companion with media control and other functionalities.

## How It Works

I have a server running Node.js and Puppeteer that sends a screenshot of the browser every time the ESP32 requests it. Every touch event is captured by the CYD and sent to the server to be executed in the browser. The idea is inspired by the old days when everything was server-side, but it goes a little beyond that.

## Improvements and Ideas

- [ ] Change the stream dynamic so the server updates the screen only with changes.
- [ ] Implement gestures for back page and other actions.
- [ ] Create example pages.
- [ ] Simplify the process of sending credentials to the server.
- [ ] Allow for an on-screen keyboard.
- [ ] Other improvements and features.

## Setup

To get started with the CYD-Browser project, follow these steps:

1. **Configure the CYD-Browser:**

   - Open the `cyd-browser.ino` file.
   - Set your Wi-Fi SSID and password.
   - Set the server address to point to your Node.js server.

2. **Install Dependencies and Run the Server:**
   - Navigate to the `cyd-browser-server` folder.
   - Install the necessary dependencies by running:
     ```sh
     npm install
     ```
   - Start the development server with:
     ```sh
     npm run dev
     ```
   - Alternatively, you can run the server inside a Docker container:
     ```sh
     docker-compose up
     ```

These steps will set up the server and allow the CYD to communicate with it, enabling the browser functionality on your device.

## Conclusion

This project demonstrates the potential of using a server-side rendered browser on the CYD. It opens up possibilities for creating a versatile desk companion with various functionalities. Your feedback and contributions are welcome!
