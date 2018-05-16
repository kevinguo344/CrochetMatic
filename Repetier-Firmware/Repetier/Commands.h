/*
    This file is part of Repetier-Firmware.

    Repetier-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Repetier-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Repetier-Firmware.  If not, see <http://www.gnu.org/licenses/>.

    This firmware is a nearly complete rewrite of the sprinter firmware
    by kliment (https://github.com/kliment/Sprinter)
    which based on Tonokip RepRap firmware rewrite based off of Hydra-mmm firmware.

  Functions in this file are used to communicate using ascii or repetier protocol.
*/

#define SERVOMIN 150
#define SERVOMAX 600

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED
const int topServo = 0;
const int botServo = 1;

extern float cZ;
extern float cE;
extern int cN;
class Commands
{
public:
    static void commandLoop();
    static void checkForPeriodicalActions(bool allowNewMoves);
    static void processArc(GCode *com);
    static void processGCode(GCode *com);
    static void processMCode(GCode *com);
    static void processLCode(GCode *com);
    static void executeGCode(GCode *com);
    static void waitUntilEndOfAllMoves();
    static void waitUntilEndOfAllBuffers();
    static void printCurrentPosition(FSTRINGPARAM(s));
    static void printTemperatures(bool showRaw = false);
    static void setFanSpeed(int speed, bool immediately = false); /// Set fan speed 0..255
    static void setFan2Speed(int speed); /// Set fan speed 0..255
    static void changeFeedrateMultiply(int factorInPercent);
    static void changeFlowrateMultiply(int factorInPercent);
    static void reportPrinterUsage();
    static void emergencyStop();
    static void checkFreeMemory();
    static void writeLowestFreeRAM();
    static void needleSequence();
    static void initializeDriver();
    static void pullDown();
    static void closeUp();
    static void rest();
    static void descendDown();
    static void ascendUp();
    static long convertAngle(int a);
    static float contsrain(float x, float a, float b);
    static void extendUpOpened();
    static void extendUpClosed();
    static void setAngles(float t, float b);
    static void setAnglesRelative(float t, float b);
private:
    static int lowestRAMValue;
    static int lowestRAMValueSend;
};

#endif // COMMANDS_H_INCLUDED
