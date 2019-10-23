#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Setup the Window
    //
    //  ofClear(backgroundColor.get());;
    ofSetVerticalSync(true);
    ofSetBackgroundAuto(true);
    ofClear(0);
    ofSetWindowTitle("Chroma Disk");
//    ofSetFrameRate(20);
    soundStream.setup(numberOfOutputChannels, numberOfInputChannels, sampleRate, frameSize, numberOfBuffers);
    mltk.setup(frameSize, sampleRate, frameSize/2, true);
    mltk.run();
    
    correctedHPCP.assign(mltk.getData("HPCP").size(), 0);
    hpcpSmooth.assign(mltk.getData("HPCP").size(), 0);
    
    gui.setup("Press 'd' to toggle.");
    gui.add(showFps.set("Show Framerate In Title", true));
    gui.add(smoothingValue.set("Smoothing Value", 0.5, 0.0, 1.0));
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if(showFps.get()){
        ofSetWindowTitle("Listening Machine - Week 2 - " + to_string(ofGetFrameRate()));
    }
    mltk.run();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    if(showGui){
        gui.draw();
    }
    //draw hpcp
    vector<Real> hpcp = mltk.getData("HPCP");
//    float hpcpWidth = ofGetWidth()/hpcp.size();
//    ofSetColor(255, 0, 0);
//    for(int i = 0 ; i < hpcp.size(); i++ ){
//        ofDrawRectangle(i*hpcpWidth, ofGetHeight()/2 , hpcpWidth, -hpcp[i]*200);
//    }
//
//
//    //draw chroma
//    vector<Real> chromavector = mltk.getData("Chromagram");
//    float chromaWidth = ofGetWidth()/chromavector.size();
//    ofSetColor(0, 255, 0);
//    for(int i = 0 ; i < chromavector.size(); i++ ){
//        ofDrawRectangle(i*chromaWidth, ofGetHeight() , chromaWidth, -chromavector[i]*200);
//    }
    
    //color wheel
//    float R = ofGetHeight()/2;
//    for(int i = 0 ; i < 360; i ++ ){
//        ofPath circle;
//        circle.arc(ofGetWidth()/2, ofGetHeight()/2,R,R,i,i+1);
//        ofColor c;
//        c.setHsb(ofMap(i,0,359,0,255), 255, 255);
//        circle.setColor(c);
//        circle.draw();
//    }
    
    //12 pies
//    float R = ofGetHeight()/2;
//    for(int i = 0 ; i < 360; i += 30 ){
//        ofPath circle;
//        circle.arc(ofGetWidth()/2, ofGetHeight()/2,R,R,i,i+30);
//        ofColor c;
//        c.setHsb(ofMap(i,0,359,0,255), 255, 255);
//        circle.setColor(c);
//        circle.draw();
//    }
    
//    vector<Real> spectrumCQ = mltk.getData("SpectrumCQ");
//    float spectrumCQSum = accumulate(spectrumCQ.begin(),spectrumCQ.end(),0.f);
    
    //correct/raise ~0 hpcp values to even out color
    for(int i = 0 ; i < hpcp.size(); i ++){
        if(hpcp[i] < 0.2){
            correctedHPCP[i] = 0.2;
        } else {
            correctedHPCP[i] = hpcp[i];
        }
    }
    
    //smooth corrected hpcp values
    for(int i = 0; i < correctedHPCP.size(); i++){
        hpcpSmooth[i] = ofInterpolateCosine(correctedHPCP[i], hpcpSmooth[i], smoothingValue.get());
    }
    
    //get sum to calculate weight of each hpcp value
    //float hpcpSum = accumulate(hpcp.begin(),hpcp.end(),0.f);
    float hpcpSmoothSum = accumulate(hpcpSmooth.begin(),hpcpSmooth.end(),0.f);
    
    
    float R = ofGetHeight()/2;
    int nBins = hpcp.size();
    float sum = hpcpSmoothSum;
//    int nBins = spectrumCQ.size();
//    float sum = spectrumCQSum;
    float angleBegin = 0 ;
    float angleEnd = 0 ;
    for(int i = 0 ; i < nBins; i ++ ){
        ofPath circle;
        float weight= hpcpSmooth[i]/sum;
//        float weight= spectrumCQ[i]/sum;
        circle.setCircleResolution(100);
        angleEnd = angleBegin + 360*weight;
        circle.arc(ofGetWidth()/2, ofGetHeight()/2,R,R,angleBegin,angleEnd);
        ofColor c;
        circle.setCurveResolution(100);
        c.setHsb(ofMap(i,0,nBins,0,255), 255, 255);
        circle.setColor(c);
        circle.draw();
        angleBegin = angleEnd;
    }
    
    ofSetColor(0);
    ofDrawCircle(ofGetWidth()/2, ofGetHeight()/2, 100);

    ofColor r = ofColor::red;
    ofSetColor(r);
    ofDrawRectangle(0, 0, 100, 100);
    ofColor b = ofColor::blue;
    b.lerp(r,0.5);
    ofSetColor(b);
    ofDrawRectangle(100, 0, 100, 100);

    
}


void ofApp::audioIn(ofSoundBuffer &inBuffer){
    inBuffer.getChannel(mltk.leftAudioBuffer, 0);
    inBuffer.getChannel(mltk.rightAudioBuffer, 1);
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd'){
        showGui = !showGui;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
