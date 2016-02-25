/*
 * Copyright (c) 2016 WinT 3794 <http://wint3794.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

///
/// Library includes
///
#include <math.h>
#include <memory.h>
#include <WPILib.h>

using namespace std;

///
/// Global variables
///
#define WinT_Motor       CANTalon
#define MIN_OUTPUT       0.100
#define LOOP_TIME        0.050
#define USES_OFFICIAL_DS false

///
/// Motor/actuator identifiers
///
namespace Motors {
enum {
    kLeftA               = 2,
    kLeftB               = 3,
    kRightA              = 4,
    kRightB              = 5,
    kClutchA             = 1,
    kClutchB             = 9,
    kLeftShooter         = 7,
    kRightShooter        = 8,
    kIntakeMotor         = 6,
    kShooterActuator     = 10,
};
}

///
/// Digital sensor identifiers
///
namespace Sensors {
enum {
    kShooterRadarPing    = 1,
    kShooterRadarEcho    = 1
};
}

///
/// Components of the pneumatic system
///
namespace Pneumatics {
enum {
    kCompressor          = 0,
    kLifterSolenoid_A    = 1,
    kLifterSolenoid_B    = 2
};
}

///
/// Controller mappings for Xbox 360
///
namespace X360_Mappings {
enum {
    kAxisLeftX           = 0,
    kAxisLeftY           = 1,
    kAxisRightX          = USES_OFFICIAL_DS ? 4 : 3,
    kAxisRightY          = USES_OFFICIAL_DS ? 5 : 4,
    kAxisLeftTrigger     = 2,
    kAxisRightTrigger    = USES_OFFICIAL_DS ? 3 : 2,
    kButtonA             = 1,
    kButtonB             = 2,
    kButtonX             = 3,
    kButtonY             = 4,
    kButtonBack          = 7,
    kButtonStart         = 8,
    kButtonLeftBumper    = 5,
    kButtonRightBumper   = 6,
};
}

///
/// Controller mappings for Xbox One controllers
///
namespace XBOX_ONE_Mappings {
enum {
    kAxisLeftX           = 0,
    kAxisLeftY           = 1,
    kAxisRightX          = USES_OFFICIAL_DS ? 4 : 3,
    kAxisRightY          = USES_OFFICIAL_DS ? 5 : 4,
    KAxisLeftTrigger     = 2,
    kAxisRightTrigger    = USES_OFFICIAL_DS ? 3 : 5,
    kButtonA             = 1,
    kButtonB             = 2,
    kButtonX             = 3,
    kButtonY             = 4,
    kButtonBack          = 7,
    kButtonStart         = 8,
    kButtonLeftBumper    = 5,
    kButtonRightBumper   = 6,
};
}

///
/// Defines the joysticks, buttons and axes used by each system
///
namespace OI {
enum ShooterInterface {
    kShooterJoystick     = 1,
    kBruteShootButton    = X360_Mappings::kButtonB,
    kSmartShootButton    = X360_Mappings::kButtonY,
    kEnableActuator      = X360_Mappings::kButtonRightBumper,
    kShootLeftAxis       = X360_Mappings::kAxisLeftTrigger,
    kShootRightAxis      = X360_Mappings::kAxisRightTrigger,
};

enum IntakeInterface {
    kIntakeJoystick      = 1,
    kIntakeTake          = X360_Mappings::kButtonLeftBumper,
    kIntakeGive          = X360_Mappings::kButtonRightBumper,
};

enum PowertrainInterface {
    kPowertrainJoystickA = 0,
    kPowertrainJoystickB = 1,
    kY_DriveAxis         = X360_Mappings::kAxisLeftY,
    kX_DriveAxis         = X360_Mappings::kAxisLeftX,
    kY_SlowDriveAxis     = X360_Mappings::kAxisRightY,
    kX_SlowDriveAxis     = X360_Mappings::kAxisRightX,
    kY_InvertButton      = X360_Mappings::kButtonA,
    kBlockStickB         = X360_Mappings::kButtonX,
    kSensivityAxis       = X360_Mappings::kAxisLeftTrigger,
};
}

///
/// Avoids involuntary movement and allows the program to control
/// how sensible the robot actuators are.
///
/// First, we check that the input meets the minimum value required
/// to move a motor. If not, the function will return 0.
///
/// Then, we apply the following function to adjust the input value
/// to the defined sensivity:
///
/// y = (a)(x^3) + (1 - a)(x)
///
/// Where:
///     - x is the input (from -1 to 1)
///     - a is the sensivity (from 0 to 1)
///     - y is the resultant (from -1 to 1)
///
/// We raise x to the third power (x^3) in order to:
///     - Obtain a 'curved' output
///     - Obtain both positive and negative values
///
/// When the sensivity is set to 0, y = x^3
///
inline float ADJUST_INPUT (float input, float sensivity) {
    if (abs (input) < abs (MIN_OUTPUT))
        return 0;

    float s = abs (sensivity);
    float final = (s * pow (input, 3)) + (1 - s) * input;
    return final;
}

///
/// Overloaded function with a sensivity of 0.2
///
inline float ADJUST_INPUT (float input) {
    return ADJUST_INPUT (input, 0.2);
}

