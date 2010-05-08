#include "testApp.h"
#include <ARToolKitPlus/TrackerMultiMarkerImpl.h>

class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};

static    MyLogger      logger;

ARToolKitPlus::TrackerMultiMarker *tracker;

//--------------------------------------------------------------
void testApp::setup(){
	obstacles = new Obstacles();
	//eye = new Eye(320,240);
	width = 640;
	height = 480;
	
	grabber.initGrabber(width, height);
	convert.allocate(width, height);
	gray.allocate(width, height);
	numPixels = width*height*bpp;
	cameraBuffer = new unsigned char[numPixels];
	bDraw = false;
	useBCH = false;
	ofBackground(127,127,127);
	idNumb.loadFont("frabk.ttf", 24);	
	drawEye = 0;
	serial.setVerbose(true);
	obstacles->makeObstacles("/Users/pp/obstacles.txt");
	player = new Player(obstacles, &serial);
	player->setGoal(ofGetWidth() - 50, ofGetHeight()/2);
	player->setColor(255, 255, 0);
	franklinBook.loadFont("frabk.ttf", 32);
	ofSetFrameRate(30);
	int isRunning = 0;
	tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 10, 10>(width,height);
	
	const char* description = tracker->getDescription();
	printf("ARToolKitPlus compile-time information:\n%s\n\n", description);
	
    // set a logger so we can output error messages
    tracker->setLogger(&logger);
	tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);	
	
    if( !tracker->init( "LogitechPro4000.dat", "markerboard_480-499.cfg", 1.0f, 1000.0f) )            // load std. ARToolKit camera file
	{
		printf("ERROR: init() failed\n");
		delete cameraBuffer;
		delete tracker;
		return;
	}
	
    // define size of the marker
    //tracker->setPatternWidth(80); // I'm not sure how to define the size with multimarkers since it doesnt seem to have this option.
	
	// the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(useBCH ? 0.125f : 0.250f);
	
    // set a threshold. alternatively we could also activate automatic thresholding
    //tracker->setThreshold(20);
	tracker->activateAutoThreshold(true);
    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);
	
    // RPP is more robust than ARToolKit's standard pose estimator
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);
	
    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);
	
}

//--------------------------------------------------------------
void testApp::update(){
	//eye->update();
	
	grabber.grabFrame();
	if(grabber.isFrameNew())
	{
		
		//convert our camera frame to grayscale
		convert.setFromPixels(grabber.getPixels(), width, height);
		gray = convert;
		
		//find the marker and get back the confidence
		int markerId = tracker->calc(gray.getPixels());
	}
	
	int numDetected = tracker->getNumDetectedMarkers();
	
	ofxVec2f arPos = ofxVec2f(0.0,0.0);
	float a;
	
	for(int i=0; i<numDetected; i++)
	{
		
		ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(i);
		
		arPos.set(marker.pos[0],marker.pos[1]);
		
		float m34[ 3 ][ 4 ];
		float c[ 2 ] = { 0.0f, 0.0f };
		float w = 40.0f;
		
		tracker->rppGetTransMat( &marker, c, w, m34 );
		
		float m[ 16 ]; //this is some crazy matrix transformative stuff. I think initially it came out of one of the arToolkit functions.... but i got it from here: http://chihara.naist.jp/people/STAFF/imura/computer/OpenGL/artp/disp_content
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m[ j * 4 + i ] = m34[ i ][ j ];
			}
		}
		for ( int j = 0; j < 3; j++ ) {
			m[ j * 4 + 3 ] = 0.0f;
		}
		m[ 3 * 4 + 3 ] = 1.0f;
		
		//tracker->arGetAngle(rot,&wa,&wb,&wc);
		a = 0.0;
		int aset = 0;
		if(m34[1][0] > 0.998 && !aset){
			a = atan2(m34[0][2],m34[2][2]);
			aset = 1;
		}
		if(m34[1][0] < -0.998 && !aset){
			a = atan2(m34[0][2],m34[2][2]);
			aset = 1;
		}
		if(!aset){
			a = atan2(-m34[2][0],m34[0][0]);
			aset = 1;
		}
		cout << "Angle = "<<a<<endl;
	}	
	
	cout<<"Marker at "<<arPos.x<<","<<arPos.y<<".  Angle = "<<a<<endl;
	
	if(isRunning){
		//player->setPosFromCamera(eye->getColorPos(0),320,240);
		player->setPosFromCamera(&arPos, width, height);
		player->setHeading(a);
		player->update();
	}
}

void testApp::reset(){
	isRunning = 0;
	player->reset();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(0xffffff);
	//grabber.draw(0, 0);

	if(drawEye){
		ofSetColor(255,255,255);
		eye->draw();
	} else {
		ofSetColor(0,0,255);
		for(int i = 0; i < ofGetWidth(); i += 20){
			for(int j = 0; j < ofGetHeight(); j += 20){
				ofFill();
				ofEllipse(i + 10, j + 10, 5,5);
			}
		}
		obstacles->draw();
		ofSetColor(255,255,255);
		player->draw();
	}
	if(player->isDead){
		output = "Player has died.";
		if(!FAKING){
			serial.writeByte('l');
		}
	} else {
		ostringstream si; 
		si << "Player strength = " << player->strength << endl;
		output = si.str();
	}
	
	ofSetColor(255, 255, 0);
	franklinBook.drawString(output, 50, ofGetHeight() - 50);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='e'){
		drawEye = !drawEye;
	}
	if(key=='w'){
		printf("saying forward\n");
		serial.writeByte('h');
	}
	if(key=='a'){
		serial.writeByte('f');
	}
	if(key=='s'){
		serial.writeByte('g');
	}
	if(key=='z'){
		serial.writeByte('i');
	}
	if(key=='y'){
		serial.setup(SERIAL_PORT_A,115200);
	}
	if(key=='u'){
		//serial[0].setup(SERIAL_PORT_B,115200);
	}
	if(key=='r'){
		isRunning = !isRunning;
	}
	if(key=='p'){
		reset();
	}
	if(key=='0'){
		for(int i = 0; i < 2; i++){
			player->setOffset();
		}
	}
	if(key=='f'){
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	eye->setColor(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

