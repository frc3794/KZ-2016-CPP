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
    m_leftA   = make_unique<WinT_Motor> (Motors::kLeftA);
    m_leftB   = make_unique<WinT_Motor> (Motors::kLeftB);
    m_rightA  = make_unique<WinT_Motor> (Motors::kRightA);
    m_rightB  = make_unique<WinT_Motor> (Motors::kRightB);
    m_clutchA = make_unique<WinT_Motor> (Motors::kClutchA);
    m_clutchB = make_unique<WinT_Motor> (Motors::kClutchB);

    m_leftA->SetInverted   (true);
    m_leftB->SetInverted   (true);
}

//===============================================================================
// Powertrain::setSafetyEnabled
//===============================================================================

void Powertrain::setSafetyEnabled (bool enabled) {
    m_leftA->SetSafetyEnabled   (enabled);
    m_leftB->SetSafetyEnabled   (enabled);
    m_rightA->SetSafetyEnabled  (enabled);
    m_rightB->SetSafetyEnabled  (enabled);
    m_clutchA->SetSafetyEnabled (enabled);
    m_clutchB->SetSafetyEnabled (enabled);
}

//===============================================================================
// Powertrain::drive
//===============================================================================

void Powertrain::drive (float x, float y, float sensivity, bool inverted_drive) {
    sensivity = sensivity * 0.75;
    x = ADJUST_INPUT (x, sensivity);
    y = ADJUST_INPUT (y, sensivity) * (inverted_drive ? 1 : -1);

    computeDriveOutputs (x, y, true);

    m_clutchA->Set (y);
    m_clutchB->Set (y);
    m_leftA->Set   (m_output.left);
    m_leftB->Set   (m_output.left);
    m_rightA->Set  (m_output.right);
    m_rightB->Set  (m_output.right);
}

//===============================================================================
// Powertrain::drive
//===============================================================================

void Powertrain::drive (const Joystick& joystick_a, const Joystick& joystick_b) {
    float x_drive  = joystick_a.GetRawAxis (OI::kX_DriveAxis);
    float y_drive  = joystick_a.GetRawAxis (OI::kY_DriveAxis) * -1;
    float x_slow_b = joystick_b.GetRawAxis (OI::kX_SlowDriveAxis);
    float y_slow_b = joystick_b.GetRawAxis (OI::kY_SlowDriveAxis) * -1;

    bool move_with_b_joystick = (abs (x_slow_b) > abs (x_drive) ||
                                 abs (y_slow_b) > abs (y_drive));

    if (move_with_b_joystick) {
        drive (x_slow_b * 0.8,
               y_slow_b * 0.8, 1.0,
               joystick_b.GetRawButton (OI::kY_InvertButton));
    }

    else {
        drive (x_drive, y_drive,
               joystick_a.GetRawAxis (OI::kSensivityAxis),
               joystick_a.GetRawButton (OI::kY_InvertButton));
    }
}

//===============================================================================
// Powertrain::computeDriveOutputs
//===============================================================================

void Powertrain::computeDriveOutputs (float x, float y, bool squared) {
    m_output.left  = 0;
    m_output.right = 0;
    y *= KART_TO_OMNI_RATIO;

    if (squared) {
        if (y >= 0)
            y = (y * y);
        else
            y = - (y * y);
        if (x >= 0)
            x = (x * x);
        else
            x = - (x * x);
    }

    if (y > 0) {
        if (x > 0) {
            m_output.left  = y - x;
            m_output.right = max (y, x);
        } else {
            m_output.left  = max (y, -x);
            m_output.right = y + x;
        }
    }

    else {
        if (x > 0) {
            m_output.left  = - max (-y, x);
            m_output.right = y + x;
        } else {
            m_output.left  = y - x;
            m_output.right = - max (-y, -x);
        }
    }
}
