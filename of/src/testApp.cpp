#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	obstacles = new Obstacles();
	eye = new Eye(320,240);
	drawEye = 1;
	serial[0].enumerateDevices();
	//serial[0].setVerbose(true);
	//serial[0].setup(SERIAL_PORT_A,115200);
	serial[1].setVerbose(true);
	serial[1].setup(SERIAL_PORT_A,115200);
	//serial.setup("/dev/tty.usbserial-A600aimy",115200);
	obstacles->makeObstacles("/Users/pp/obstacles.txt");
	player[0] = new Player(obstacles);
	player[1] = new Player(obstacles);
	player[1]->setPos(ofGetWidth() - 50, ofGetHeight()/2);
	commandMap.insert(make_pair("left,walk",'f'));
	commandMap.insert(make_pair("right,walk",'g'));
	commandMap.insert(make_pair("up,walk",'h'));
	commandMap.insert(make_pair("down",'i'));
	commandMap.insert(make_pair("shock,walk",'j'));
	//ofSetFrameRate(10);
}

//--------------------------------------------------------------
void testApp::update(){
	eye->update();
	//player[0]->setHeading(&serial[0]);
	for(int i = 0; i < 2; i++){
		player[i]->setPosFromCamera(eye->getColorPos(i),320,240);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	if(drawEye){
		ofSetColor(255,255,255);
		eye->draw();
	} else {
		ofSetColor(0,0,255);
		for(int i = 0; i < ofGetWidth(); i += 20){
		//ofLine(i,0,i,ofGetHeight());
			for(int j = 0; j < ofGetHeight(); j += 20){
				ofFill();
				ofEllipse(i + 10, j + 10, 5,5);
			}
		}
		obstacles->draw();
		player[0]->draw();
		player[1]->draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='e'){
		drawEye = !drawEye;
	}
	if(key=='w'){
		printf("saying forward\n");
		serial[0].writeByte('h');
	}
	if(key=='a'){
		serial[0].writeByte('f');
	}
	if(key=='s'){
		serial[0].writeByte('g');
	}
	if(key=='z'){
		serial[0].writeByte('i');
	}
	if(key=='y'){
		serial[1].writeByte('h');
	}
	if(key=='g'){
		serial[1].writeByte('f');
	}
	if(key=='h'){
		serial[1].writeByte('g');
	}
	if(key=='b'){
		serial[1].writeByte('i');
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

