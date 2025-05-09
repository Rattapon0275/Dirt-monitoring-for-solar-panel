# Dirt monitoring for solar panel
Designed by Rattapon Wayamanon, Porramee Yensiri

## Introduction
Light intensity is one of the factors that affects the performance of photovoltaic cells to generate electrical energy. Higher light intensity can increase the current generated by the panels. Dirtiness on the solar panels, such as dust, leaves, can reduce light intensity that the panel should fully receive. People mostly ignore it, so it may cause their solar panels to not work in full performance of them. From this problem, we have the idea that is to design a dirt monitoring system for solar panels, to detect the dirtiness on the panels and clean them if it is too dirty!

![diagram](https://github.com/user-attachments/assets/a68454ee-c0b2-4fff-b0f7-60457a46309a)

Dirt monitoring system is designed to support solar panels in households. The system can detect dirt on the panels by dirt sensor attached on the panels. Each solar panel has one outdoor controller that includes dirt detection and other parameters (temperature under panel, light intensity, rain, voltage and current flow from the panel, and energy) for only itself.  The dirtiness will be converted into score and is sent with all other data to keep in a cloud memory via WiFi for monitoring in far distance. The users can monitor the dirtiness of their panels with the other parameters in a mobile app. When dirtiness score is high, the mobile app will notify user to clean the solar panels. The user can command the outdoor controller to clean their panels. Especially for some dirty panels whose dirtiness score is in critical, the outdoor controller automatically clean the panels by itself and the mobile app will notify user to inform automatic cleaning action.

![IoT proposal presentation](https://github.com/user-attachments/assets/2a089381-2dc5-4453-a166-fb9d56e7ea08)

## Use cases
+ **Case 1**
  + Title : Less dirt or raindrops are detected, no warning to clean panel.
  + Actors : User, mobile app, outdoor controller
  + Scenario :
    1.	Outdoor controller detects the dirtiness of solar panel with light intensity, rain, temperature under the panel, voltage, current and its energy.
    2.	Outdoor controller gives a dirtiness score of 1.
    3.	Outdoor controller sends all data detected to cloud.
    4.	User opens mobile app.
    5.	Mobile app shows the value of the dirtiness of solar panel with other parameters on its UI.
+ **Case 2**
  + Title : Dirt is detected, mobile app warns user to clean panel.
  + Actors : User, mobile app, outdoor controller
  + Scenario :
    1.	Outdoor controller detects the dirtiness of solar panel with light intensity, rain, temperature under the panel, voltage, current and its energy.
    2.	Outdoor controller gives a dirtiness score of 2.
    3.	Outdoor controller sends all data detected to cloud.
    4.	Notification alerts user on the mobile phone to clean panel.
    5.	User opens mobile app.
    6.	Mobile app shows the value of the dirtiness of solar panel with other parameters on its UI.
+ **Case 3**
  + Title : User commands to clean panel.
  + Actors : User, mobile app, outdoor controller, springer
  + Scenario :
    1.	User opens mobile app.
    2.	User clicks a cleaning button.
    3.	Cleaning command is sent to cloud.
    4.	Outdoor controller reads the cleaning command from cloud.
    5.	Outdoor controller turns on springer to clean panel for 1 minute.
    6.	Outdoor controller automatically turns off springer.
+ **Case 4**
  + Title : Too much dirt is detected, mobile app notifies user for automatic cleaning.
  + Actors : User, mobile app, outdoor controller, springer
  + Scenario :
    1.	Outdoor controller detects the dirtiness of solar panel with light intensity, rain, temperature under the panel, voltage, current and its energy.
    2.	Outdoor controller gives a dirtiness score of 3.
    3.	Outdoor controller turn on springer to clean the panel without command from user.
    4.	Outdoor controller sends all data detected to cloud.
    5.	Notification alerts user on the mobile phone to inform automatic cleaning.
    6.	User opens mobile app.
    7.	Mobile app shows the value of the dirtiness of solar panel with other parameters on its UI.

  
## Objectives
+	To create dirt monitoring for solar panels
+	To create a IoT monitoring system and a mobile app for users to monitor any parameters detected on solar panels
+	To clean dirt on solar panels by users or itself in case of exceeding a specific limit

## Scope
+	1 outdoor controller with user app for 1 user
+	Real-time monitoring (Back up historical data in future)
+	Dirt sensor is detected dust or particles that have the same size as dust only.
+	No electrical load, only solar panels with battery and outdoor controller
+	Monitor dirtiness on each panel, temperature under panel, light intensity, rain, voltage and current flow from the panel, and energy generated from the panel

## List of features
+	Monitor the dirtiness (dust) of solar panels in a mobile app.
+	Monitor other parameters on the solar panels (temperature under panel, light intensity, rain, voltage and current flow from the panel, and energy) in a mobile app.
+	Command cleaning solar panels via a mobile app or automatically clean by themselves if the dirtiness score reaches a specific limit.

## Learning Goals
+ To learn and apply copper tape on glass to create a dirt sensor
+ To learn basic ideas of how to create IoT communication system
+ To learn basic ideas of how to create a dashboard and notification on a mobile app

## Challenges: Dirt Sensor Sensitivity
+ The first approach of the dirt sensor shows a little change of capacitance approximately 0.212 pF, which was insufficient for the ESP32 to detect. It was required to increase the offset and change on the sensor’s capacitance.
![image](https://github.com/user-attachments/assets/d4060bbb-5fa4-4b90-8e65-ec5a01cfd1fa)

+ The latest approach of the dirt sensor uses this layout of copper tape to increase both the offset and the change on the capacitance. It gives 37.32 pF and 3.49 pF for the offset and change, respectively. This value is sufficient for ESP32 to detect it.
![image](https://github.com/user-attachments/assets/c72e5e04-576c-48fa-93e5-f7ab7a26e9fe)

## Future Improvement
+ Create the function for backup data 
+ Set up sleep modes for longer standalone operation
+ Add more nodes of the outdoor controller for multiple solar panels

## Weekly Update
![image](https://github.com/user-attachments/assets/41c7870b-2805-4b9a-b31a-86497f003847)

### **Week 1**
  + ***Dirt Detection***: Try creating two parallel copper strips attached on a glass plate, then the result is shown via the difference of capacitance between having the plate with and without powder (to simulate dust) as shown below. While the powder is being on the plate, the capacitance measured between two strips by LCR meter is more than the capacitance as no powder on the plate, in a unit of 0.1 pF. This experiment can be concluded that powder(or dust) affect a small capacitance change between copper strips. The next step is to try increasing the capacitance of the powder more obviously, in a unit of nF or uF, or find the solution to convert this small change into dirt score.<br><br>
![image](https://github.com/user-attachments/assets/c073fd8a-03e0-429d-999c-8ed51b7b8920) <br>
  + ***Other Detection***: Choose sensors for each detection. INA226 is a voltage/current/power sensor, to measure those magnitudes from solar panel terminal. This sensor transmits data via I2C communication. BH1750FVI is used for light intensity detection, using I2C communication for data transmission. To detect temperature under solar panel, thermocouple Type K (0-600 °C) is used to measure temperature, convert the data by MAX6675 and send it via SPI communication. Rain Sensor Module is used for detecting rain, but not to know the amount of rain as detection. This sensor is read data via ADC interfacing. All sensors that are mentioned before have already bought and they are being tried coding and testing.
  + ***Mobile App and Cloud***: We surveyed various cloud and mobile applications available on the internet, such as Google Sheets, Thunkable, etc. In conclusion, we found many attractive software options, but some require a cost for certain processes. Therefore, we selected Netpie as our main software due to its user-friendly dashboard and its accompanying mobile app, which shares the same name and can be downloaded for free on mobile devices.
### **Week 2**
  + ***Dirt Detection***: Implemented an advanced copper tape design to improve capacitance variation in the experiment, aiming to enhance sensitivity in dirt detection. The new design successfully increased the capacitance change from 0.1 pF to 0.5-0.6 pF, indicating an improvement in detection capability. However, despite this increase, the change remains relatively small and still poses challenges for accurate detection. Factors such as the arrangement, width, and spacing of the copper tape could further influence capacitance behavior. The next step is to refine the design by optimizing these parameters to achieve a greater capacitance increase, making the changes more distinguishable and improving overall detection performance.
![image](https://github.com/user-attachments/assets/3f7d2926-59e6-4314-bece-debc32285dea)<br>
  + ***Other Detection***: All sensors have already been coded and examined. Those sensors worked properly according to the detection we designed. This task is waiting for integration with other parts of the outdoor controller.
  + ***Mobile App and Cloud***: Created a project in Netpie and established an initial connection between the sensor and the system. Simulated sensor values were successfully displayed on the dashboard, confirming that the setup is functioning as expected. The next step is to refine the integration and conduct testing with real sensor data to improve accuracy and reliability.
### **Week 3**
  + ***Dirt Detection***: Advance the design of the dirt sensor. We decrease the width of the gap between two copper strips to 1 millimeter and increase more area of the sensor plain. After experimenting with this approach, the capacitance of this sensor increases obviously. The capacitance change after pouring talcum on this sensor rises to 3.5 pF with the offset of 37.3 pF. We can conclude that decreasing the width of the copper strips gap and expanding more sensor plain area can raise the capacitance of the sensor. Now we finish designing the dirt sensor. The next step of this task is to find the best solution for converting the change in capacitance of this sensor to a suitable form of data read by the microcontroller ESP32.
  + ***Other Detection***: This task is waiting for integration with other parts of the outdoor controller.
  + ***Cleaning Action***: Complete cleaning action design. We use a 5-Volt DC pump for supplying water through a rubber tube to clean a solar panel. Water is kept and supplied from a small tank that the pump is held inside. We have already got the pump and examined this design. One problem occurs: The pump works with a low flow rate of water. It causes the panel may not be completely clean for a whole panel. Separating the water way more may help to spread water through more area on the panel, even if the water flow rate may be reduced.
  + ***Mobile App and Cloud***: Write C code to implement the interconnectivity among ESP32 and a NETPIE device. C code is written by using Visual Studio Code, set up as Espressif ESP32 DEV module and framework "ESP-IDF". The libraries, extensions, and standard functions of programming are different from programming using the framework "Arduino" which is similar to Arduino-IDE. Therefore, the C code is needed to rewrite to a new version from some examples of this kind of code in workshops. The code is being written.
### **Week 4**
  + ***Dirt Detection***: Detect the change in capacitance of the dirt sensor via the capacitive sensing inside ESP32. Since ESP32 has a feature to detect the capacitive sense and collect as a value of counts, the capacitance change of the sensor should also be detected by this feature. We tried this idea and then found that this feature in ESP32 can detect this change in the sensor. The sensitivity and resolution of the feature are enough to read the change. The value varies from 5-6 counts in maximum, with an offset of approximately 130 counts, so these values are enough and suitable to be detected via ESP32. The next step is to duplicate this sensor to be a prototype and use it in this project, design a support and shield for this sensor, and then write C code for the capacitive sensing function of this sensor.
![image](https://github.com/user-attachments/assets/9926827e-fd9e-4d53-80c1-eb2ad6ee8f6a)
  + ***Other Detection***: This task is waiting for integration with other parts of the outdoor controller.
  + ***Cleaning Action***: Finish coding for cleaning action in the outdoor controller. It needs only a digital output signal to control whether a DC pump is turned on or off. This task is waiting for integration with other parts of the outdoor controller.
  + ***Mobile App and Cloud***: Finish simple C code for implementation of interconnectivity. ESP32 can now send and receive data with a NETPIE device. The communication is perfectly smooth and has no problems right now. But this program does not include any functions used in the outdoor controller at the present; only simple data is used for this experiment to examine whether they can communicate with each other or not. The next step is to integrate this program with other functions in the outdoor controller, starting with the sensors on the outdoor controller.
### **Week 5**
  + ***Dirt Detection***: Finish making the prototype of the dirt sensor with programming. The sensor prototype used for the outdoor controller has already been done. The sensor is then connected to ESP32 to design programming for conversion of the amount of dirtiness (capacitive count) into a dirt score. The score is classified whether there is so much dirt on the solar panel surface or not. If there is, the score is given to be 1; otherwise 0. This score will be sent to be shown on the NETPIE dashboard. This task is waiting for integration with other parts of the outdoor controller.<br><br>
![image](https://github.com/user-attachments/assets/c72e5e04-576c-48fa-93e5-f7ab7a26e9fe) <br>
  + ***Outdoor Controller***: Combine the codes for other detection, cleaning action, and wireless communication (WiFi) using MQTT with NETPIE. We have already finished coding the combination of those functions, and it has been tested. This system then works correctly EXCEPT cleaning action. The DC pump does not get enough electric current to drive itself because it is directly connected to the GPIO pin of ESP32. It cannot supply enough current for it certainly. Thus, we need to pull energy from the battery directly to supply the pump. To complete that, we also need a 3.3-volt relay to control the working of the DC pump. Now, we have all those components. This system is being examined again.
  + ***Energy Management***: Design the energy management system of the outdoor controller. The outdoor controller is supplied by the source voltage, at least 3.3 volts, and we suppose that it needs 0.5 amperes to work all day. We stated that the outdoor controller consumes energy as a load of photovoltaic cells. Thus, the source voltage and current should be the rating of the battery. We have already bought the battery pack, which has 3.7 volts and 9000 mAhr. It means that the outdoor controller must have enough energy to work for not more than 9/0.5 = 18 hours on this battery pack without external energy use. For photovoltaic cells, we would say that they can supply rated energy only for 6-8 hours because this is the sunny time for a day. Thanks to the battery pack can support the whole time the cells cannot supply rated energy. Now we have already got a solar panel, solar charger, and the battery for our project. The next step is to examine this energy management system and combine it with other parts of the outdoor controller.
  + ***Mobile App and Cloud***: Design the real-time data display for detected parameters from the outdoor controller. According to the outdoor controller task, we need to ensure that the data is sent and shown on the dashboard in the platform  NETPIE correctly. Therefore, we created the dashboard for the real-time data display, as well as the toggle button to click for the cleaning process. Only the dashboard for the dirt score display is not created now, it is waiting for the fabrication.
### **Week 6**
  + ***Outdoor Controller***: Finish cleaning action test and combine the dirt detection code with the remaining. After we put a relay between the battery and the DC pump and tested it, we found that the cleaning process has now worked correctly as we designed before. Furthermore, we have already finished combining the source code for the dirt detection with the remainder of the outdoor controller we did last week. It was tested, and it then works properly as we designed.
  + ***Energy Management***: Finish the energy management system of the outdoor controller. We have already done the experiment to test whether the battery system works or not. We must have another concern that the electrical measurement (voltage, current, power) takes on a solar panel terminal. Therefore, this is another test we need to do. The result is satisfactory. The system can work as same as we designed, and the INA226 sensor can sense voltage, current, and power correctly. This task is waiting for integration with other parts of the outdoor controller.
  + ***Mobile App and Cloud***: Select the method for notification. We choose "PUSH" for the notification to the user. PUSH is available in NETPIE, and it notifies on the NETPIE app itself. This is the benefit of using PUSH. The PUSH notification is now being designed and created.
### **Week 7**
  + ***Mobile App and Cloud***: Finish creating the notification for the NETPIE app. PUSH notification has already been created and tested. Now, the notification works correctly as designed. It notifies on the NETPIE app when the dirt score is given 1.
  + ***Outdoor Controller***: According to the progress on ***Mobile App and Cloud*** this week, we found that the battery pack has inadequate energy to supply to the outdoor controller and DC pump. We decided to supply the outdoor controller with a power bank instead of the battery pack. We need to avoid buying more components. Moreover, the energy in the battery pack is still enough for only the DC pump. In conclusion, ESP32 and other components using a 3.3V DC bus are supplied by the power bank, and the DC pump is supplied by the battery pack. Now, all components for the outdoor controller are contained in a control box. The dirt sensor is glued on a stand made from a 3D printer.<br><br>
![image](https://github.com/user-attachments/assets/c0baf366-28d6-432d-af80-b48dbf01bdfb)
![image](https://github.com/user-attachments/assets/75c3e635-e6fc-4b67-baff-c148cd5e7c14)
![image](https://github.com/user-attachments/assets/f5c968d3-118d-4ba6-a583-a90d0c7e9c03)<br>

### **Week 8**
  + -
### **Week 9**
  + -
