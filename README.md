CinderMyo
=========

A very simple wrapper for interacting with the Myo C++ library. Mainly used with the SimpleMyoListener
to get data in a format cinder can deal with ie Quatf and Vec3f for orientation and accelerometer and 
gyroscopic data.

Installation
------------

Add the CinderMyo header and impl to your project, and make sure your build settings point to the 
Myo SDK include files. Also add a Copy Build Phase passing the myo framework to your project on compile.

Full description written by Myo here: 

https://developer.thalmic.com/docs/api_reference/platform/the-sdk.html#building-cxx-apps-macosx

The header should have sufficient comments to get you started. Have fun!

Feedback and comments, find me [@trippedout](http://www.twitter.com/trippedout)