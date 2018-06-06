# CrochetMatic Documentation
****
# **Overview**

The machine is made up of 3 separate but equally important components:

- XY Bed and RAMPS Board
- Needle Array and Drivers
- Yarn Extruder

You will need [Arduino](https://www.arduino.cc/en/Main/Software) (which is a wrapper for C++), [Fusion 360](https://www.autodesk.com/products/fusion-360/students-teachers-educators), a good text editor (I recommend [Sublime Text 3](https://www.sublimetext.com/) or [Brackets](http://brackets.io/)), [Repetier Host](https://www.repetier.com/) (use this to enter G-code into the machine) and a whole lot of patience. 

# **XY Bed and RAMPS Board**

This is the part of the machine will probably be the part that you will need to touch the least as it’s the one the part that’s been modified the least.

The bed can go from (0, 0) to (460, 460). Before you start any movements on the bed, remember to home the machine. To do that, enter G28 in Repetier Host. To move the bed anywhere on the XY plane, enter

    G1 X<some coordinate> Y<some coordinate>

For example, to move it to (150, 230) enter

    G1 X150 Y230.

The main board that will be controlling most of the movements on the machine is the Arduino Mega with the RAMPS 1.4 (or 1.5, I can’t remember but it shouldn’t make a difference) board. The RAMPS board doesn’t change anything software-wise on the Arduino Mega but it does rearrange the pins to allow for easier attachment of regular 3D printer components. The main thing you will have to look out for are two resistors and a potentiometer (a dial) attached to where the heat sensors normally are supposed to be. This is to trick the board into thinking that there is a heating element on the machine (there isn’t) so that it will use the extruder properly (how the latch servos and the yarn extruder are operated). If at any point the latch motor/yarn extruder refuse to move, check the Temperature Curve tab on Repetier Host. **If there’s no red line appearing on that tab or the red line is at a very high temperature, there’s something wrong with the resistor and potentiometer.**

# **Needle Array and Driver**

This is the part that will most likely require the most change and fuss. There are two major parts to each needle: the Hook (sometimes referred to as the Needle) and the Latch. The Hook is made up of the the outer aluminum tube (~0.375” diameter) and the hooked 3D printed piece on top. The Latch is made up of the inner aluminum tube (~0.25” diameter) and the straight 3D printed piece on top. The two pieces are driven by separate motors with the Hook driven by the top row of servos and the Latch driven by the bottom row of servos. Each needle is assigned a unique index that is important, as you will see later.

## Software and Code

To change the position of the Hook and Latch, enter G1 Z<position of the hook> E<position of the latch>. **The Z and E positions must be between 0 and 60**. Notice how the G1 command is used in both setting the XY position and the Needle positions. This means that you can move both the bed and the needle at the same time in one command. For example:

    G1 X230 Y150 Z50 E60

will move the bed to (230, 150), move the Hook to 50 mm, and the Latch to 60 mm. 

Both servos are controlled by the Arduino Mega through the Driver, a Teensy 3.5 board. The role of the Teensy is meant to convert the stepper instructions from the Mega into servo instructions for the Hook and Latch motors and how it does so can be seen in the [Encoder.ino](https://github.com/kevinguo344/CrochetMatic/blob/master/Servo_As_Stepper/Encoder.ino) file in the [Servo_As_Stepper](https://github.com/kevinguo344/CrochetMatic/tree/master/Servo_As_Stepper) folder of the repo. Most of that file is written using some unreadable C++ code that I don’t understand (thank you Francois for writing that part). Also in the same folder is the [Servo_As_Stepper.ino](https://github.com/kevinguo344/CrochetMatic/blob/master/Servo_As_Stepper/Servo_As_Stepper.ino) file, which is actually readable and serves many purposes. One of those is setting the position of the servos of the selected needle based on readings from [Encoder.ino](https://github.com/kevinguo344/CrochetMatic/blob/master/Servo_As_Stepper/Encoder.ino). Notice that when updating the positions of the Hook and Latch, safe_update_servos() will check if the Latch is in a position within an acceptable range of the Hook (set between 0 - 30 mm of the Hook). This is to ensure that the Hook and Latch don’t ram into each other excessively. It also means the position of the Latch is constrained by the Hook. For example, if you enter 

    G1 Z0 E60

this is a perfectly valid command, but safe_update_servos() will only allow the Latch to go up to 30 mm. 

The other purpose of [Servo_As_Stepper.ino](https://github.com/kevinguo344/CrochetMatic/blob/master/Servo_As_Stepper/Servo_As_Stepper.ino) is to read an I2C message sent by Mega when switching needles. To switch between needles, enter

    L<index of needle you want to change to + 1>.

For example, if you want to switch to needle #3, enter

    L4

**You must add 1 to the command as the Mega will not read L0 as a valid command.** The modified firmware for the Mega automatically takes the index you entered and subtracts 1 from it to get the real index of the needle you want to actuate. If the index you want to change to is different from the index that the Mega considers is the active index, it will send a message over I2C to the Teensy that will read something like this:

    “N3,30.00,60.00”

The number after the “N” is the index of the needle being switch to, the float after the first comma is the position of the Hook and the float after the second comma is the position of the position of the Latch. The purpose is to ensure that when switching between needles, they have the same Latch and Hook positions. This is important as the Mega has no concept of what needles are and switching between needles without first adjusting for their positions can cause all sorts of weirdness and problems (and for me a few long nights where everything broke unexpectedly.)

## Construction

To make a new needle, you have to: 
**3D print**

- [Latch piece](https://drive.google.com/open?id=1_G_ZzPGdB-DOTYKKikE7iZG9BdgS86AO)
- [Hook piece](https://drive.google.com/open?id=1Mstz9O5aiTzqiE2jS8zpKTY12x8Ez39k) (if you’re printing with the Prusa printers in Gates 229, print the piece rightside up with Quality: Detail for best results)
- [Inside Cuff](https://drive.google.com/open?id=13oYK71EhzFoGAy7Zjb5m-_4ahWUP36Zt)
- [Outside Cuff](https://drive.google.com/open?id=1tcIRFMjF_Ih0AJiE1JRWNW-GJruBUf-M)

**Cut (with a pipe cutter)**

- 0.25” diameter inside tube (DISCLAIMER: the Solidworks file has an inaccurate length for this piece, your best bet is to take out one of the current inside tubes and measure it)
- 0.375” diameter outside tube (length is in the Solidworks file)

**Laser Cut**

- rack (cut on a 0.118” thick piece of acrylic)
![rack (inside 0.118inch_acrylic_pieces.pdf)](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528085092733_Screen+Shot+2018-06-04+at+12.02.31+AM.png)

- motor assembly pieces (cut on a 0.118” thick piece of acrylic)
![motor assembly pieces (inside 0.118inch_acrylic_pieces.pdf)](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528085197540_Screen+Shot+2018-06-04+at+12.05.23+AM.png)

- motor assembly body (cut on a 0.22” thick piece of acrylic)
![motor assembly body (inside 0.22inch_acrylic_pices.pdf)](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528085254256_Screen+Shot+2018-06-04+at+12.07.22+AM.png)


The motor assembly should be assembled to look like this [Solidworks assembly.](https://drive.google.com/open?id=1PY2jOFtx3SUKURgYvvNYbJhoZnl5znZ5) 

The array of needles will require you to laser cut [this drawing](https://drive.google.com/open?id=1GG4V9Sef0AVVOfkVuH3kGr--Ud4mw8zD) on 1/4” thick acrylic (which is actually more like 0.236” thick). They use t-slots to keep them together and require 1/2” long #4 screws and nuts. Use the current assembly as a reference for how to put it together.

# **Yarn Extruder**

This is least defined part of the machine and so far has had the least amount of work on it. There are no mechanical components to it but the current Repetier firmware already has it defined. To change the extruder, enter T1. This will switch the current extruder to the yarn extruder and you can use G1 E<how far you want the extruder to move>. Ex: G1 E50 will rotate whatever stepper motor is attached 50 mm. **You will not be able to control the Latch until you enter T0.** The ultimate design of the extruder will need to be determined.

# **Generating, Postprocessing Gcode with Fusion360**

Gcode for producing knits from the right-most needle to the left-most needle is [here](https://drive.google.com/open?id=10pfYbwPgP1f0aLoWWFIcL8WUbAHbmvBw). Gcode for producing knits from the left-most needle to the right-most needle is [here](https://drive.google.com/open?id=13RGydKh0ll9xRudNLtzUSNaHm5B96foD). The file that will have the most up-to-date version of the yarn path is this [Fusion 360 file.](https://a360.co/2sFaVnc)

![Short Distance v20 is the latest version that works](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528140415349_Screen+Shot+2018-06-04+at+3.26.30+PM.png)


To generate a new gcode path, click on “Model” on the top-left corner and switch the workspace to “CAM”

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528140368144_cam-select.png)


On the left, you will see a couple selections labeled “Needle #<insert number here> Setup.” They are all identical except their origins have different values. **These setups are absolutely vital for the gcode to be generated properly as normally as this Fusion file will flip the Y and Z axis without them.**

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528140555146_Screen+Shot+2018-06-04+at+3.27.36+PM.png)


To generate gcode from an existing setup, open the chosen setup. Right-click on the Trace function inside…

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528140806578_Screen+Shot+2018-06-04+at+3.33.11+PM.png)


and select “Post Process”

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528141287886_Screen+Shot+2018-06-04+at+3.41.02+PM.png)


Ignore any error messages, keep the default settings, and when a popup shows up asking to save the files, remove everything inside the “Save As: ” box and enter <name of file>.gcode. .nc is a file format that’s completely useless for anything. 

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528141362136_Screen+Shot+2018-06-04+at+3.42.21+PM.png)


Open up the resulting gcode in your preferred text editor. It’ll look something like this:

![](https://d2mxuefqeaa7sj.cloudfront.net/s_33763546468B7BAFEF0C27EF442A853D9A173931323781A650A2CAD31BAD4261_1528141618611_Screen+Shot+2018-06-04+at+3.46.39+PM.png)


First task is to delete everything up to the G0 statement so 

    : (PGM, NAME="1001")
    ; T1  D=10 CR=5 - ZMIN=17 - BALL END MILL
    : G90 G40 G94
    G17
    G71
    M26
    ; TRACE5 5
    M9
    M26
    :T1 M6
    M26
    S5000 M3
    H0
    M8

will all have to be deleted. This is meant to act as parameters for a CNC machine, which doesn’t work with this machine. At the same time delete

    Z96
    Z85

after the G0 statement as the machine cannot have a Z coordinate of greater than 60. Keeping this statement will mess up the machine. In the same vein, at the bottom of the gcode file will be a bunch of statements that will mess up the machine if they are executed so delete the following at the end of the gcode:

    Z85
    G0 Z96
    G17
    M9
    M26
    G0 X0 Y0
    M30
    M2

What you should be left with will look something like this:

    G0 X<some coordinate> Y<some coordinate>
    G1 Z<some coordinate> F1000
    SOME XYZ COORDINATES
    .....
    SOME XYZ COORDINATES

The machine will not understand the G0 line nor will it understand just having the coordinates without a G1 in front of it. To make it machine-readable gcode, you will have to change the G0 to a G1 command (just change G0 to G1), and add G1 to every XYZ coordinate you find. In Sublime, holding down **Command** + **Option** on a Mac and dragging down the text will allow you to change multiple lines at once.

However, there are still a couple more things to do before you’re finished.

    G1 Z<some coordinate> F1000 

not only defines some Z coordinate but also the F statement defines the feedrate for the machine, basically how fast the machine will go through the sequence. For optimal use, change the feedrate from 1000 to 600 so that line will be

    G1 Z<some coordinate> F600

However, this is not finished as gcode generator has no concept of what an extruder is and therefore has to be placed manually. An extruder statement will look like this:

    G1 E<some coordinate>

Generally, you want to change E whenever Z changes. E should start at 40 (put an extruder statement near the beginning, right under the feedrate statement), and should go to 60 once a loop is complete and before the Hook starts to descend into the toothbrushes (put an extruder statement before Z starts descending). A good idea is to place an M400 statement after each extruder statement to ensure the actions are completed before the hook starts descending. 

