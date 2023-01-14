# ECE100 Final Project

Dates: Oct 2022 - Nov 2022

This project uses a bluetooth game controller to control an Arduino-based robot car and 3D-printed robot arm. This project builds on the OSOYOO robot car platform which we had been using throughout the semester. This is the final project for the Electrical and Computer Engineering Introduction to Profession class. A demo video is available at https://www.youtube.com/watch?v=dzqXelYSpZw.

A portion of the MeArm v0.4 is attached to the top of the robot car. The MeArm is a robot arm designed to be laser-cut, but can also be 3D-printed. It uses four SG90 or MG90 servos to move the different components of the arm.

A PlayStation 4 controller connects to a laptop via Bluetooth. A Python script is running on the laptop to read the data from the controller, and transmit the data to the Arduino via Wi-Fi. The Arduino is connected to the laptop's Wi-Fi hotspot using a Wi-Fi shield. The Arduino listens for UDP packets from the laptop, then takes the appropriate action based on the received data.

One of the main problems with this implementation is that the servos each require about 1 amp of current, which the Arduino cannot supply by itself. This problem could be fixed by adding an additional power supply for the servos. However, this problem was realized too late into the design process to change approaches. This is the reason for the servo stuttering seen in the demo video. It was discovered that connecting up one servo would work but with stuttering. Therefore, it was decided not to assemble the claw portion of the arm, because it would be impossible to connect without using an additional power supply.
