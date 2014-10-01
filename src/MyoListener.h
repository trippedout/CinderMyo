//
//  MyoListener.h
//  MyoCinder
//
//  Created by Anthony Tripaldi on 9/29/14.
//
//

#pragma once

#include <myo/myo.hpp>

class MyoListener;
typedef std::shared_ptr<MyoListener> MyoListenerRef;

class MyoListener : public myo::DeviceListener
{
private:
    MyoListener()
    : onArm(false), roll_w(0), pitch_w(0), yaw_w(0), currentPose()
    { }
    
public:
    static MyoListenerRef create()
    {
        return std::shared_ptr<MyoListener>(new MyoListener());
    }
    
    // onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
    virtual void onUnpair(myo::Myo* myo, uint64_t timestamp);
    
    // onOrientationData() is called whenever the Myo device provides its current orientation, which is represented
    // as a unit quaternion.
    void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat);
    
    // onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
    // making a fist, or not making a fist anymore.
    void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
    
    // onArmRecognized() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
    // arm. This lets Myo know which arm it's on and which way it's facing.
    virtual void onArmRecognized(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection);
    
    // onArmLost() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
    // it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
    // when Myo is moved around on the arm.
    virtual void onArmLost(myo::Myo* myo, uint64_t timestamp);
    
    
    // There are other virtual functions in DeviceListener that we could override here, like onAccelerometerData().
    // For this example, the functions overridden above are sufficient.
    // We define this function to print the current values that were updated by the on...() functions above.
    void print()
    {
        // Clear the current line
        std::cout << '\r';
        // Print out the orientation. Orientation data is always available, even if no arm is currently recognized.
        std::cout << '[' << std::string(roll_w, '*') << std::string(18 - roll_w, ' ') << ']'
        << '[' << std::string(pitch_w, '*') << std::string(18 - pitch_w, ' ') << ']'
        << '[' << std::string(yaw_w, '*') << std::string(18 - yaw_w, ' ') << ']';
        if (onArm) {
            // Print out the currently recognized pose and which arm Myo is being worn on.
            // Pose::toString() provides the human-readable name of a pose. We can also output a Pose directly to an
            // output stream (e.g. std::cout << currentPose;). In this case we want to get the pose name's length so
            // that we can fill the rest of the field with spaces below, so we obtain it as a string using toString().
            std::string poseString = currentPose.toString();
            std::cout << '[' << (whichArm == myo::armLeft ? "L" : "R") << ']'
            << '[' << poseString << std::string(14 - poseString.size(), ' ') << ']';
        } else {
            // Print out a placeholder for the arm and pose when Myo doesn't currently know which arm it's on.
            std::cout << "[?]" << '[' << std::string(14, ' ') << ']';
        }
        std::cout << std::flush;
    }
    
protected:
    bool        onArm;
    myo::Arm    whichArm;
    
    int roll_w, pitch_w, yaw_w;
    myo::Pose currentPose;
};
