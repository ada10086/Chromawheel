#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxMLTK.h"
#include "ofMath.h"

using namespace std;

class ofApp : public ofBaseApp{

	public:
        int numberOfOutputChannels = 0;
        int numberOfInputChannels = 2;
        int sampleRate = 44100;
        int frameSize = 1024;
        int numberOfBuffers = 1;
    
        MLTK mltk;
        ofSoundStream soundStream;

        vector<Real> correctedHPCP;
        vector<Real> hpcpSmooth;
    
        bool showGui = true;
        ofxPanel gui;
        ofParameter<bool> showFps;
        ofParameter<float> smoothingValue;

        void audioIn(ofSoundBuffer &inBuffer);
    
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};
