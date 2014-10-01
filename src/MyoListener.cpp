//
//  MyoListener.cpp
//  MyoCinder
//
//  Created by Anthony Tripaldi on 9/29/14.
//
//

#include "MyoListener.h"

void MyoListener::onUnpair(myo::Myo* myo, uint64_t timestamp)
{
    // We've lost a Myo.
    // Let's clean up some leftover state.
    roll_w = 0;
    pitch_w = 0;
    yaw_w = 0;
    onArm = false;
}

void MyoListener::onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
{
    using std::atan2;
    using std::asin;
    using std::sqrt;
    // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
    float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                       1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
    float pitch = asin(2.0f * (quat.w() * quat.y() - quat.z() * quat.x()));
    float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                      1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
    // Convert the floating point angles in radians to a scale from 0 to 20.
    roll_w = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
    pitch_w = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
    yaw_w = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
}

void MyoListener::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
{
    currentPose = pose;
    // Vibrate the Myo whenever we've detected that the user has made a fist.
    if (pose == myo::Pose::fist) {
        myo->vibrate(myo::Myo::vibrationMedium);
    }
}

void MyoListener::onArmRecognized(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection)
{
    onArm = true;
    whichArm = arm;
}

void MyoListener::onArmLost(myo::Myo* myo, uint64_t timestamp)
{
    onArm = false;
}