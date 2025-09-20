# Getting Started with ESP32-WROOM-32 in VS Code

This guide will help you connect and program your ESP32-WROOM-32 using Visual Studio Code and PlatformIO.

## Prerequisites

1. Visual Studio Code installed
2. PlatformIO extension installed in VS Code
3. ESP32-WROOM-32 board
4. Micro USB cable

## Step 1: Connect Your ESP32

1. Plug the Micro USB cable into your ESP32 board
2. Connect the other end to your computer's USB port
3. Windows should automatically install drivers (if not, you might need to install CP210x USB to UART bridge drivers)

## Step 2: Open Project in VS Code

1. Open the SimpleESP32Server project folder in VS Code
2. Make sure the PlatformIO extension is active

## Step 3: Configure WiFi Settings

1. Open `src/main.cpp`
2. Update the WiFi credentials:
   ```cpp
   const char* ssid = "YourWiFiName";       // Replace with your actual WiFi network name
   const char* password = "YourWiFiPassword"; // Replace with your actual WiFi password
   ```

## Step 4: Build and Upload

1. Click the PlatformIO icon in the VS Code sidebar
2. Under "Project Tasks", expand "esp32dev"
3. Click "Build" to compile the project
4. If successful, click "Upload" to upload the firmware to your ESP32

Note: Some ESP32 boards require pressing the BOOT button when you start uploading. If you see upload errors, try:
1. Press and hold the BOOT button on your ESP32
2. Click "Upload" in PlatformIO
3. Once you see "Connecting..." in the terminal, release the BOOT button

## Step 5: Monitor Serial Output

1. After successful upload, select "Monitor" from PlatformIO tasks
2. You should see serial output at 115200 baud rate
3. The ESP32 will attempt to connect to WiFi
4. Once connected, it will display its IP address (e.g., 192.168.1.100)

## Step 6: Access the Web Server

1. Open a web browser on a device connected to the same WiFi network
2. Enter the IP address shown in the serial monitor
3. You should see the ESP32 web interface
4. Try the LED control buttons and other features

## Common Issues and Solutions

### Cannot Upload Code
- Check USB connection
- Try a different USB port
- Press BOOT button during upload
- Make sure you have the correct driver installed

### WiFi Connection Fails
- Double-check your WiFi credentials
- Ensure your WiFi network is working
- Make sure the ESP32 is within range of your WiFi router

### Serial Monitor Shows Nothing
- Check that you selected the correct COM port
- Ensure baud rate is set to 115200
- Try pressing the RESET button on the ESP32

### Web Server Not Accessible
- Make sure your computer/phone is on the same WiFi network as the ESP32
- Check that you entered the correct IP address
- Try restarting the ESP32 (press RESET button)

## Project Structure

- `platformio.ini`: Project configuration file
- `src/main.cpp`: Main code file with server logic
- `data/index.html`: Web interface HTML file
