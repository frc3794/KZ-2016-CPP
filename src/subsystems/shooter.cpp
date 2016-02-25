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

#include "shooter.h"

///
/// The constants used for calculating the initial velocity and
/// motor output values based on the projectile motion concepts
///
/// Please note that these values are on the metric system.
///
const float kANGLE     = 62.00;
const float kHEIGHT    = 2.050;
const float kGRAVITY   = 9.807;
const float kMAX_RANGE = 1.958;
const float kFRICTION  = 0.470;

using namespace std;

//===============================================================================
// Shooter::Shooter
//===============================================================================

Shooter::Shooter() {
    m_actuator   = make_unique<WinT_Motor> (Motors::kShooterActuator);
    m_motorLeft  = make_unique<WinT_Motor> (Motors::kLeftShooter);
    m_motorRight = make_unique<WinT_Motor> (Motors::kRightShooter);
    m_ultrasonic = make_unique<Ultrasonic> (Sensors::kShooterRadarPing,
                                            Sensors::kShooterRadarEcho);

    m_motorLeft->SetInverted           (true);
    m_motorLeft->SetSafetyEnabled      (false);
    m_motorRight->SetSafetyEnabled     (false);

    m_maxInitialVelocity = getInitialVelocity (kMAX_RANGE);
}

//===============================================================================
// Shooter::shoot
//===============================================================================

void Shooter::shoot (float inches) {
    float output = getInitialVelocity (inches * 0.0254) / m_maxInitialVelocity;

    if (output > 1)
        output = 1;

    shoot (output, output);
}

//===============================================================================
// Shooter::shoot
//===============================================================================

void Shooter::shoot (float left, float right) {
    m_motorLeft->Set  (ADJUST_INPUT (left,  0));
    m_motorRight->Set (ADJUST_INPUT (right, 0));
}

//===============================================================================
// Shooter::shoot
//===============================================================================

void Shooter::shoot (const Joystick& joystick) {
    if (joystick.GetRawButton (OI::kSmartShootButton))
        shoot (m_ultrasonic->GetRangeInches());

    else if (joystick.GetRawButton (OI::kBruteShootButton))
        shoot (1, 1);

    else {
        shoot (joystick.GetRawAxis (OI::kShootLeftAxis),
               joystick.GetRawAxis (OI::kShootRightAxis));
    }

    if (joystick.GetRawButton (OI::kEnableActuator))
        enableActuator (0.85);
}

//===============================================================================
// Shooter::enableActuator
//===============================================================================

void Shooter::enableActuator (float act_output) {
    m_actuator->Set (ADJUST_INPUT (act_output));
}

//===============================================================================
// Shooter::getInitialVelocity
//===============================================================================

float Shooter::getInitialVelocity (float range) {
    float n = pow (range, 2) * kGRAVITY;
    float d = (range * sin (2 * kANGLE)) + (2 * kHEIGHT * pow (cos (kANGLE), 2));

    float velocity = sqrt (n / d);
    float friction = velocity * kFRICTION;

    return velocity + friction;
}
