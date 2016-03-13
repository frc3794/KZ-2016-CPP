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

#include "powertrain.h"

///
/// This allows us to obtain the same tangential velocity in all the
/// wheels of the drive system, which consists of two Go-Kart wheels with
/// a diameter of 11 inches and an Omni wheel with a diameter of 7,9 inches.
///
/// Since the angular velocity of the gearboxes is the same, the only thing
/// that changes is the radius of the wheels. To obtain the same tangential
/// velocity, we need to reduce the angular speed of the Go-Kart wheels with
/// this calculated constant.
///
const float KART_TO_OMNI_RATIO = 7.9 / 10;

//===============================================================================
// Powertrain::Powertrain
//===============================================================================

Powertrain::Powertrain() {
    m_leftA  = new CANTalon (Motors::kLeftA);
    m_leftB  = new CANTalon (Motors::kLeftB);
    m_rightA = new CANTalon (Motors::kRightA);
    m_rightB = new CANTalon (Motors::kRightB);

    m_clutchA = new WinT_Motor (Motors::kClutchA);
    m_clutchB = new WinT_Motor (Motors::kClutchB);
    m_driveA  = new RobotDrive (m_leftA, m_rightA);
    m_driveB  = new RobotDrive (m_leftB, m_rightB);
}

//===============================================================================
// Powertrain::setSafetyEnabled
//===============================================================================

void Powertrain::setSafetyEnabled (bool enabled) {
    m_driveA->SetSafetyEnabled  (enabled);
    m_driveB->SetSafetyEnabled  (enabled);
    m_clutchA->SetSafetyEnabled (enabled);
    m_clutchB->SetSafetyEnabled (enabled);
}

//===============================================================================
// Powertrain::drive
//===============================================================================

void Powertrain::drive (float x, float y, float sensivity, bool inverted_drive) {
    sensivity = sensivity * 0.5;
    x = ADJUST_INPUT (x, 0) * -1;
    y = ADJUST_INPUT (y, 0) * (inverted_drive ? 1 : -1);

    m_driveA->ArcadeDrive (y * KART_TO_OMNI_RATIO * -1, x, true);
    m_driveB->ArcadeDrive (y * KART_TO_OMNI_RATIO * -1, x, true);

    m_clutchA->Set (y);
    m_clutchB->Set (y);
}

//===============================================================================
// Powertrain::drive
//===============================================================================

void Powertrain::drive (Joystick* joystick_a, Joystick* joystick_b) {
    float x_drive  = joystick_a->GetRawAxis (OI::kX_DriveAxis);
    float y_drive  = joystick_a->GetRawAxis (OI::kY_DriveAxis) * -1;
    float x_slow_b = joystick_b->GetRawAxis (OI::kX_SlowDriveAxis);
    float y_slow_b = joystick_b->GetRawAxis (OI::kY_SlowDriveAxis) * -1;

    bool move_with_b_joystick = (abs (x_slow_b) > abs (x_drive) ||
                                 (abs (y_slow_b) > abs (y_drive)));

    if (move_with_b_joystick) {
        drive (x_slow_b * 0.8,
               y_slow_b * 0.8, 1.0,
               joystick_b->GetRawButton (OI::kY_InvertButton));
    }

    else {
        drive (x_drive * 0.92,
               y_drive * 0.92,
               joystick_a->GetRawButton (X360_Mappings::kButtonLeftBumper) ? 1 : 0,
               joystick_a->GetRawButton (OI::kY_InvertButton));
    }
}
