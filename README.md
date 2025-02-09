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
+	To learn and apply ITO and Non-ITO film to create a dirt sensor
+	To learn basic ideas of how to create IoT communication system
+	To learn basic ideas of how to create a mobile app

