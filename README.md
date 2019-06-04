
# RangerGlove
Built your own Virtual Reality glove

This is a tutorial of how to create a virtual reality glove using off-the-shelf components. 

Please review the Components_Needed file to obtain the necessary components. 

Feather M0 = Microcontroller

BNO055 = Captures the rotation of the hand

Rubber Sensor = Captures the finger one way movement

Vibrating Motors = Haptic feedback to the user from the virtural world


Below is a component visualization of components assembled.

![alt text](https://github.com/mireyarod23/RangerGlove/blob/master/Image/glove.PNG)

NOTE: This visualization is scalable since only one component per finger is shown.

Schematic of all the components

![alt text](https://github.com/mireyarod23/RangerGlove/blob/master/Image/schematic.PNG)

NOTE: If you would like to see a break down schematic of each component look inside the Schematic folder. 


<h2>Arduino</h2> 

The Arduino folder contains the script that allows the Feather MO create stand alone WIFI port to allow the Unity project to communicate.


<h2>Unity</h2>

The Unity folder contains the 3D enviorment. The Feather M0 and Unity game engine was able to establish connection through TCP socket.  


<h2>Results</h2>

![alt text](https://github.com/mireyarod23/RangerGlove/blob/master/Image/hand_unity.PNG)

