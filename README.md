# Smart Home Control System  

## Description  
The **Smart Home Control System** is a prototype mobile-based application integrated with a TM4C123GH6PM microcontroller to provide users with efficient control and monitoring of home appliances. This system ensures convenience and safety by offering real-time monitoring, appliance control, and alert mechanisms.  

## Features  
1. **Lamp Control**  
   - Turn a 220V lamp ON/OFF via the mobile application while retaining manual switch functionality.  

2. **Plug Control**  
   - Remotely enable/disable a plug to restrict usage when turned off.  

3. **Door Monitoring**  
   - Display door status (open/closed) using a magnetic switch sensor.  

4. **Temperature Monitoring**  
   - Display the current room temperature on the mobile application.  

5. **Temperature Alarm**  
   - Trigger a software-based and physical alarm when the temperature exceeds a predefined threshold.  

6. **Door Activity Logging**  
   - Save and display timestamps for door openings and closings.  

## Hardware Requirements  
- TM4C123GH6PM microcontroller board  
- Magnetic door switch  
- Temperature sensor (e.g., LM35 or DHT11)  
- 220V lamp with manual switch  
- Plug and relay module  
- Alarm module (buzzer or equivalent)  
- Power supply and connecting wires  
- Mounted prototype surface  

## Software Requirements  
- Android Studio (for mobile app development)  
- Keil µVision (for TM4C123 programming)  
- Embedded C programming  

## Repository Contents  
- **/Code**: Source code for the TM4C123 microcontroller and the Android application (APK included).  
- **/Documentation**:  
  - Technical report detailing project components, wiring diagrams, flowcharts, and team contributions.  
  - Prototype photos and demo video.  

## How to Use  
1. **Setup Hardware**  
   - Connect all components as per the provided wiring diagram in the documentation.  
   - Mount the hardware components on a suitable surface with hidden wiring.  

2. **Load Microcontroller Code**  
   - Use Keil µVision to compile and flash the provided code onto the TM4C123GH6PM board.  

3. **Install the Mobile Application**  
   - Download and install the APK from the `/Code` folder on an Android device.  

4. **Control and Monitor**  
   - Use the mobile application to control appliances, monitor room conditions, and view door activity logs.  

5. **Set Alarms**  
   - Configure temperature thresholds via the application to trigger alarms if exceeded.  
