//
//  CinderMyo.cpp
//  MyoCinder
//
//  Created by Anthony Tripaldi on 9/29/14.
//
//

#include "CinderMyo.h"

using namespace std;
using namespace ci;
using namespace cinder;

namespace cinder
{
    CinderMyo::CinderMyo(string app_id, int connectWaitTime)
    {
        app::console() << "Get Myo! wait for " << connectWaitTime << "ms to connect." << endl;
        
        mHub = new myo::Hub(app_id);
        mDevice = mHub->waitForMyo(connectWaitTime);
        
        if(!mDevice)
            throw std::runtime_error("Unable to find a Myo!");
        
        app::console() << "Myo Attached::" << mDevice << endl;
    }
    
    CinderMyo::~CinderMyo()
    {
        delete mHub;
        mHub = NULL;
    }
    
    void CinderMyo::update(float updateRate)
    {
        if(updateRate == -1)
            updateRate = app::getFrameRate();
        
        mHub->run(1000 / updateRate);
    }
    
    
} //cinder