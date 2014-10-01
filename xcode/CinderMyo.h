//
//  CinderMyo.h
//  MyoCinder
//
//  Created by Anthony Tripaldi on 9/29/14.
//
//

#pragma once

#include "cinder/app/AppNative.h"
#include <myo/myo.hpp>

namespace cinder {
    
    /*
     * main cinder class responsible for all things myo
     */
    class CinderMyo;
    typedef std::shared_ptr<CinderMyo> CinderMyoRef;

    class CinderMyo
    {
    protected:
        myo::Hub*   mHub;
        myo::Myo*   mDevice;
        
    private:
        CinderMyo(std::string app_id, int connectWaitTime = 10000); //default to 10 seconds
        
    public:
        ~CinderMyo();
        
        // app_id is past along to myo sdk for hub creation, as pointed out in myo docs:
        // myo::Hub hub("com.example.hello-myo");
        static CinderMyoRef create(std::string app_id) { return std::shared_ptr<CinderMyo>(new CinderMyo(app_id)); };
        
        static CinderMyoRef create(std::string app_id, int connectWaitTime) { return std::shared_ptr<CinderMyo>(new CinderMyo(app_id, connectWaitTime)); };
        
        //updates hub - pass an update rate if you want, if not, defaults to the apps framerate
        void update(float updateRate = -1.0);
        
        //sets listener to hub
        void addListener(myo::DeviceListener* listener) { mHub->addListener(listener); }
        
        //remove listener and reference
        void removeListener(myo::DeviceListener* listener) { mHub->removeListener(listener); }
        
        void vibrate(myo::Myo::VibrationType vibrationType = myo::Myo::vibrationShort) { mDevice->vibrate(vibrationType); }
    };
      
        
    /*
     * Simple listener class to grab basic data from device
     */
    class SimpleMyoListener;
    typedef std::shared_ptr<SimpleMyoListener> SimpleMyoListenerRef;

    class SimpleMyoListener : public myo::DeviceListener
    {
    private:
        SimpleMyoListener()
        : mRoll(0), mPitch(0), mYaw(0), mCurrentPose(),
        mOrientation(), mAccelData(), mGyroData(),
        mShowDebugMsgs(false), mIsOnArm(false)
        { }
        
    protected:
        bool mShowDebugMsgs;
        
        ci::Quatf mOrientation;
        
        ci::Vec3f mAccelData, mGyroData;
        
        int mRoll, mPitch, mYaw;
        
        bool mIsOnArm;
        
        myo::Pose mCurrentPose;
        myo::Arm mCurrentArm;
        myo::XDirection mCurrentDirection;
        
    public:
        // static creator
        static SimpleMyoListenerRef create() { return std::shared_ptr<SimpleMyoListener>(new SimpleMyoListener()); }
        
        // overrides for setting simple data and info
        
        void onOrientationData(myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat)
        {
            // Calculate Euler angles (roll, pitch, and yaw) from the unit quaternion.
            float roll = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                               1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
            float pitch = asin(2.0f * (quat.w() * quat.y() - quat.z() * quat.x()));
            float yaw = atan2(2.0f * (quat.w() * quat.z() + quat.x() * quat.y()),
                              1.0f - 2.0f * (quat.y() * quat.y() + quat.z() * quat.z()));
            
            // Convert the floating point angles in radians to a scale from 0 to 20.
            mRoll = static_cast<int>((roll + (float)M_PI)/(M_PI * 2.0f) * 18);
            mPitch = static_cast<int>((pitch + (float)M_PI/2.0f)/M_PI * 18);
            mYaw = static_cast<int>((yaw + (float)M_PI)/(M_PI * 2.0f) * 18);
            
            mOrientation = ci::Quatf(quat.w(), quat.x(), quat.y(), quat.z());
        }
        
        void onAccelerometerData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel)
        {
            mAccelData = Vec3f(accel.x(), accel.y(), accel.z());
        }
        
        void onGyroscopeData(myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro)
        {
            mGyroData = Vec3f(gyro.x(), gyro.y(), gyro.z());
        }
        
        void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
        {
            if(mShowDebugMsgs) app::console() << "onPose() " << timestamp << " " << pose.toString() << std::endl;
            mCurrentPose = pose;
        }
        
        void onArmRecognized(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection)
        {
            if(mShowDebugMsgs) app::console() << "onArmRecognized() arm: " << arm << std::endl;
            
            mIsOnArm = true;
            mCurrentArm = arm;
            mCurrentDirection = xDirection;
        }
        
        void onArmLost(myo::Myo* myo, uint64_t timestamp)
        {
            if(mShowDebugMsgs) app::console() << "onArmLost()" << std::endl;
            
            mIsOnArm = false;
        }
        
        
        // getters
        
        ci::Quatf* getOrientation() { return &mOrientation; }
        
        ci::Vec3f* getAccelerometerData() { return &mAccelData; }
        
        ci::Vec3f* getGyroscopeData() { return &mGyroData; }
        
        myo::Pose* getCurrentPose() { return &mCurrentPose; }
        
        myo::Arm* getCurrentArm() { return &mCurrentArm; }
        
        myo::XDirection* getCurrentDirection() { return &mCurrentDirection; }
        
        bool isOnArm() { return mIsOnArm; }
    };


}; // namespace cinder;