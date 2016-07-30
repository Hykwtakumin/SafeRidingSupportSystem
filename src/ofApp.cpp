#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup(){
    
    //CV初期設定
//    camera.initGrabber(320,240);
    
    
    //画面の初期設定(混色の加算合成等)
    ofBackground(0, 0, 0);
    ofRectangle(0, 0, ridingMovie.getWidth(), ridingMovie.getHeight());
    
    ofEnableSmoothing();
    ofSetVerticalSync(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //動画データを読み込む
    ridingMovie.loadMovie("riding2.mp4");
    ridingMovie.play();
    curFlow = &flow;
}

void ofApp::update(){
//    camera.update();
    //動画再生を待機状態にする
//    if (camera.isFrameNew()) {
    ridingMovie.update();
    //Farneback法を用いたオプティカルフロー
    curFlow = &flow;
    //オプティカルフローを計算
    curFlow->calcOpticalFlow(ridingMovie);
//    }
}

void ofApp::draw(){
    ofBackground(0);
    ofSetColor(255);
//    ridingMovie.draw(0, 0, ofGetWidth(), ofGetHeight());
    ridingMovie.draw(0, 0, ofGetWidth(), ofGetHeight());
    curFlow->draw(0, 0, ofGetWidth(), ofGetHeight());
    
    ofVec2f force;
    force = flow.getTotalFlow();
//    force = flow.getTotalFlowInRegion(ofRectangle(0,0,flow.cols,flow.rows);
    //TODO X,Y成分のベクトル量を用いる
    //TODO　最適な閾値を実験によって決める
    string forces = ofToString(force,0);
    ofDrawBitmapString(forces, 10, 20);
    
}

void ofApp::keypressed(int key){
    switch (key) {
            //「0」キーで最初の画面に戻る
        case '0':
            ridingMovie.firstFrame();
            break;
    }
}

void ofApp::keyReleased(int key){
}

void ofApp::mouseMoved(int x, int y){
}

void ofApp::mouseDragged(int x, int y, int button){
    ridingMovie.setPosition((float)x/(float)ofGetWidth());
}

void ofApp::mousePressed(int x, int y, int button){
    ridingMovie.setPaused(true);
    ard.sendDigital(11, ARD_HIGH);
}

void ofApp::mouseReleased(int x, int y, int button){
    ridingMovie.setPaused(false);
    ard.sendDigital(11, ARD_LOW);
}

void ofApp::windowResized(int w, int h){
}

void ofApp::setupArduino(const int & version) {
    // 接続が確立したら、リスナーを削除
    ofRemoveListener(ard.EInitialized, this, &ofApp::setupArduino);
    // 1~13の全てのデジタルピンの初期設定(OUTPUTに)
    for (int i = 0; i < 13; i++){
        ard.sendDigitalPinMode(i, ARD_OUTPUT);
    }
    // Digital 13 をOUTPUTに
    ard.sendDigitalPinMode(11, ARD_OUTPUT);
    // Analog 0 をAnalog Inに
    ard.sendAnalogPinReporting(0, ARD_ANALOG);
    // Digital 11 をPWMに
    ard.sendDigitalPinMode(11, ARD_PWM);
    // 接続の確立のフラグをtrueに
    bSetupArduino = true;
    
}

void ofApp::updateArduino(){
    // Arduinoをアップデート、全てのデータとメッセージを取得
    ard.update();
}


