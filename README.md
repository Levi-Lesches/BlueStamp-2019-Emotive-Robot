# Emotive-Robot

A robot that interacts with you and is cute like WALL-E. I made this project in [BlueStamp Engineering](https://bluestampengineering.com), summer of 2019.  

Base project: 
- Has a robot body (from the neck up)
  - Has a head that can pan and tilt 
  - Has two LED matrices that act as eyes
  - Has eyebrows that can articulate emotion
- Can react to touch via an ultrasonic sensor
- Can play audio through a [Wave Shield](https://learn.adafruit.com/adafruit-wave-shield-audio-shield-for-arduino)
- Can display messages through an LCD screen
- Can be used one of two ways: 
  1. Autonmously. The robot will interact with you normally.
  2. Interactively. The robot can be controlled through a Bluetooth-connected app for Android.
  
Stretch goals: 
1. Add in a Raspberry Pi for more complex control with Python
2. Add a camera for Computer Vision functionality
3. Port the app to [Flutter](https://flutter.dev), so it can be used for Android and iOS
4. Have the robot mimic emotions using [emotional analysis](http://www.paulvangent.com/2016/04/01/emotion-recognition-with-python-opencv-and-a-face-dataset)
5. Have the robot take a picture of you and send it to your phone via Bluetooth
6. Use text-to-speech to let the robot speak messages instead of using the LCD
7. Use Artificial Intelligence to make custom messages (may need to be connected to the app)
8. Enable a security mode from the app: 
   1. Angry eyes
   2. Motion sensor set to alarm
   3. Have the speakers play an alarm sound
   4. Maybe send a notification from the app that the alarm was tripped.
9. Add idle animations to the LED matrices (eg, pong)

Documentation for this project can be found on my [Bluestamp project page](https://bluestampengineering.com/student-projects/levi-l).
