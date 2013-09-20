/*
 * ArucoTestApp.cpp
 *
 * Created by Daniel Scheibel on 7/13/13.
 *
 * Copyright (c) 2012, Red Paper Heart, All rights reserved.
 * This code is intended for use with the Cinder C++ library: http://libcinder.org
 *
 * To contact Red Paper Heart, email hello@redpaperheart.com or tweet @redpaperhearts
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/Capture.h"

#include "CinderOpenCv.h"

#include "markerdetector.h"
#include "cvdrawingutils.h"

using namespace ci;
using namespace ci::app;


class ArucoTestApp : public AppNative {
  public:
    void prepareSettings( Settings *settings );
	void setup();
	void update();
	void draw();
    
    gl::TextureRef          mInput;
    Surface             	mSurf;
    CaptureRef              mCapture;
    
    aruco::CameraParameters mCamParam;
    aruco::MarkerDetector mMarkerDetector;
    std::vector<aruco::Marker> mMarkers;
};

void ArucoTestApp::prepareSettings( cinder::app::AppBasic::Settings *settings ) {
    settings->setFrameRate( 60.0 );
    settings->setWindowSize( 640, 480 );
}

void ArucoTestApp::setup(){
    // CAMERA
    try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
	}
}


void ArucoTestApp::update(){
    if( mCapture->checkNewFrame() ){
        mSurf = mCapture->getSurface();
        mInput = gl::Texture::create( mSurf );
    }
    if( mSurf ){
        cv::Mat input = toOcv( mSurf );
        mMarkerDetector.detect( input, mMarkers );
    }
}

void ArucoTestApp::draw(){
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    gl::enableAlphaBlending();
    gl::color( ColorA(1,1,1,1.0f) );
    if( mInput ){
        gl::draw( mInput );
    }
    gl::color( ColorA(1,1,1,0.7f) );
    for( int i=0; i<mMarkers.size(); i++){
        for( int ii = 0; ii < mMarkers[i].size(); ii++){
            gl::drawSolidCircle(Vec2f(mMarkers[i][ii].x, mMarkers[i][ii].y), 10.0f);
        }
    }
}

CINDER_APP_NATIVE( ArucoTestApp, RendererGl )
