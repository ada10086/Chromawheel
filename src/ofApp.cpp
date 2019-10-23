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

    
    vector<ofColor> colors;
    colors.assign(hpcp.size(), 0);
    
    for(int i = 0 ; i < hpcp.size(); i++ ){
        ofColor cCurrent;
        cCurrent.setHsb(ofMap(i,0,hpcp.size(),0,255), 255, 255);
        colors[i] = cCurrent;
    }
    
    //mesh
    ofMesh mesh;
    int num_slices = 12;
    float innerRadius = 0;
    float outerRadius = R;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    ofVec3f center(ofGetWidth()/2, ofGetHeight()/2);
    float angleBegin = 0 ;
    for(int i = 0; i < num_slices; i++){
        float pct = hpcpSmooth[i]/sum;
        float angle = TWO_PI * pct;
        float s = sin(angleBegin + angle);
        float c = cos(angleBegin + angle);
        //translate to center of screen
        mesh.addVertex( ofVec3f(s*innerRadius, c*innerRadius) + center);
        mesh.addColor(ofColor(255));
//        mesh.addColor(ofColor(ofMap(i,0,num_slices,0,255)));

        mesh.addVertex( ofVec3f(s*outerRadius, c*outerRadius) + center);
        mesh.addColor(colors[i]);

//        if (num_slices%10 == 0 ){
//            mesh.addColor(colors[i/10]);
//        }
//        mesh.addColor(ofColor(ofMap(i+1,0,num_slices,0,255)));
        angleBegin += angle;
    }
    ofEnableNormalizedTexCoords();
    mesh.draw();
    ofDisableNormalizedTexCoords();

    
//    //------------inefficient way----------------
//    float angleBegin = 0 ;
//    float angleEnd = 0 ;
//    ofColor cBegin;
//    ofColor cEnd;
//    for(int i = 0 ; i < nBins; i ++ ){
//        ofPath circle;
//        float weight= hpcpSmooth[i]/sum;
////        float weight= spectrumCQ[i]/sum;
//        circle.setCircleResolution(100);
//        angleEnd = angleBegin + 360*weight;
//        circle.arc(ofGetWidth()/2, ofGetHeight()/2,R,R,angleBegin,angleEnd);
//
//        ofColor cCurrent;
//        circle.setCurveResolution(100);
//
//        cCurrent.setHsb(ofMap(i,0,nBins,0,255), 255, 255);
//        circle.setColor(255);
////        circle.setColor(cCurrent);
//        circle.draw();
//
//        ofColor cNext;
//        cNext.setHsb(ofMap(i+1,0,nBins,0,255), 255, 255);
//
//
//
//        //---------
//        //drawing gradient from angleBegin to angleEnd
//        ofxColorGradient<ofColor> gradient;
//        gradient.addColor(cCurrent);
//        gradient.addColor(cNext);
//
//        float angle0 = angleBegin;
//        float angle1= 0.0f ;
//
//        for(float i = 0.0f; i < 1.0f; i+= 0.01f){
//            ofPath circle;
//
//            float step = (angleEnd - angleBegin)/100;
//
//            angle1 = angle0 + step;
//            circle.arc(ofGetWidth()/2, ofGetHeight()/2,R,R,angle0, angle1);   //increment by 1 ranging from anglemiddle to angle end
//            circle.setCurveResolution(100);
//            circle.setColor(gradient.getColorAtPercent(i));
//            circle.draw();
//            angle0 = angle1;
//        }
//        angleBegin = angleEnd;
//    }
    
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
