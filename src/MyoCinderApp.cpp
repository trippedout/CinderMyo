#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "CinderMyo.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class MyoCinderApp : public AppNative {
  public:
	void setup();
	void update();
	void draw();
protected:
    SimpleMyoListenerRef      mMyoListener;
    CinderMyoRef              mMyoRef;
};

void MyoCinderApp::setup()
{
    //synchronous - make sure MyoConnect is running and is connected.
    mMyoRef = CinderMyo::create("net.trippedout.myo_test");
    
    //use the simple listener built into the lib
    mMyoListener = SimpleMyoListener::create();
    mMyoRef->addListener(mMyoListener.get());
    
    //make sure we're good to go
    mMyoRef->vibrate();
}

void MyoCinderApp::update()
{
    if(mMyoRef)
    {
        mMyoRef->update();
    }
}

void MyoCinderApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    
    gl::pushMatrices();
    {
        gl::translate(Vec2i(getWindowWidth()/2, getWindowHeight()/2));
        
        if(mMyoListener->isOnArm())
            gl::rotate(*mMyoListener->getOrientation());
        
        gl::drawCylinder(10, 30, 100);
    }
    gl::popMatrices();
}

CINDER_APP_NATIVE( MyoCinderApp, RendererGl )
