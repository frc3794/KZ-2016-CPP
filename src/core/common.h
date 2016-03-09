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
#define WinT_Motor       Talon
#define MIN_OUTPUT       0.100
#define SD               SmartDashboard
#define SOL_VALUE        DoubleSolenoid::Value
const bool USES_OFFICIAL_DS = true;
const bool IS_CLONE = true;

///
/// Motor/actuator identifiers
///
namespace Motors {
const int kLeftA               = IS_CLONE ? 3 : 2;
const int kLeftB               = IS_CLONE ? 4 : 3;
const int kRightA              = IS_CLONE ? 1 : 4;
const int kRightB              = IS_CLONE ? 2 : 5;
const int kClutchA             = IS_CLONE ? 0 : 1;
const int kClutchB             = IS_CLONE ? 7 : 9;
const int kLeftShooter         = IS_CLONE ? 6 : 7;
const int kRightShooter        = IS_CLONE ? 6 : 8;
const int kIntakeMotor         = IS_CLONE ? 5 : 6;
const int kHandsActuator       = IS_CLONE ? 9 : 10;
const int kShooterActuator     = IS_CLONE ? 8 : 9;
}

///
/// Digital sensor identifiers
///
namespace Sensors {
const int kShooterRadarPing    = 1;
const int kShooterRadarEcho    = 2;
}

///
/// Components of the pneumatic system
///
namespace Pneumatics {
const int kCompressor          = 0;
const int kLifterPiston_Up     = 0;
const int kLifterPiston_Down   = 1;
}

///
/// Controller mappings for Xbox 360
///
namespace X360_Mappings {
const int kAxisLeftX           = 0;
const int kAxisLeftY           = 1;
const int kAxisRightX          = USES_OFFICIAL_DS ? 4 : 3;
const int kAxisRightY          = USES_OFFICIAL_DS ? 5 : 4;
const int kAxisLeftTrigger     = 2;
const int kAxisRightTrigger    = USES_OFFICIAL_DS ? 3 : 2;
const int kButtonA             = 1;
const int kButtonB             = 2;
const int kButtonX             = 3;
const int kButtonY             = 4;
const int kButtonBack          = 7;
const int kButtonStart         = 8;
const int kButtonLeftBumper    = 5;
const int kButtonRightBumper   = 6;
}

///
/// Controller mappings for Xbox One controllers
///
namespace XBOX_ONE_Mappings {
const int kAxisLeftX           = 0;
const int kAxisLeftY           = 1;
const int kAxisRightX          = USES_OFFICIAL_DS ? 4 : 3;
const int kAxisRightY          = USES_OFFICIAL_DS ? 5 : 4;
const int kAxisLeftTrigger     = 2;
const int kAxisRightTrigger    = USES_OFFICIAL_DS ? 3 : 5;
const int kButtonA             = 1;
const int kButtonB             = 2;
const int kButtonX             = 3;
const int kButtonY             = 4;
const int kButtonBack          = 7;
const int kButtonStart         = 8;
const int kButtonLeftBumper    = 5;
const int kButtonRightBumper   = 6;
}

///
/// Defines the joysticks, buttons and axes used by each system
///
namespace OI {
/* Shooter interface */
const int kBruteShootButton    = X360_Mappings::kButtonB;
const int kSmartShootButton    = X360_Mappings::kButtonY;
const int kEnableActuator      = X360_Mappings::kAxisLeftY;
const int kShootLeftAxis       = X360_Mappings::kAxisLeftTrigger;
const int kShootRightAxis      = X360_Mappings::kAxisRightTrigger;

/* Intake & hands interface */
const int kLiftHand            = X360_Mappings::kButtonBack;
const int kDropHand            = X360_Mappings::kButtonStart;
const int kIntakeTake          = X360_Mappings::kAxisLeftTrigger;
const int kIntakeGive          = X360_Mappings::kAxisRightTrigger;

/* Piston lifter interface */
const int kEnableCompressor    = X360_Mappings::kButtonX;
const int kLifterUp            = X360_Mappings::kButtonLeftBumper;
const int kLifterDown          = X360_Mappings::kButtonRightBumper;

/* Powertrain */
const int kY_DriveAxis         = X360_Mappings::kAxisLeftY;
const int kX_DriveAxis         = X360_Mappings::kAxisLeftX;
const int kY_SlowDriveAxis     = X360_Mappings::kAxisRightY;
const int kX_SlowDriveAxis     = X360_Mappings::kAxisRightX;
const int kY_InvertButton      = X360_Mappings::kButtonA;
const int kBlockStickB         = X360_Mappings::kButtonX;
const int kSensivityAxis       = X360_Mappings::kAxisLeftTrigger;
}

///
/// Avoids involuntary movement and allows the program to control
/// how sensible the robot actuators are.
///
/// First, we check that the input meets the minimum value required
/// to move a motor. If not, the function will return 0.
///
/// Then, we apply the following function to adjust the input value
/// to the defined sensitivity:
///
/// y = (a)(x^3) + (1 - a)(x)
///
/// Where:
///     - x is the input (from -1 to 1)
///     - a is the sensitivity (from 0 to 1)
///     - y is the resultant (from -1 to 1)
///
/// We raise x to the third power (x^3) in order to:
///     - Obtain a 'curved' output
///     - Obtain both positive and negative values
///
/// When the sensitivity is set to 0, y = x^3
///
inline float ADJUST_INPUT (float input, float sensitivity) {
    if (abs (input) < abs (MIN_OUTPUT))
        return 0;

    float s = abs (sensitivity);
    float final = (s * pow (input, 3)) + (1 - s) * input;
    return final;
}

///
/// Overloaded function with a sensitivity of 0.2
///
inline float ADJUST_INPUT (float input) {
    return ADJUST_INPUT (input, 0.2);
}

