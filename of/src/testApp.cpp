#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	obstacles = new Obstacles();
	eye = new Eye(320,240);
	drawEye = 1;
	serial[0].enumerateDevices();
	serial[0].setVerbose(true);
	//serial[0].setup(SERIAL_PORT_A,115200);
	//ofSleepMillis(5000);
	serial[1].setVerbose(true);
	//serial[1].setup(SERIAL_PORT_B,115200);
	//serial[1].setup(SERIAL_PORT_B,115200);
	//serial.setup("/dev/tty.usbserial-A600aimy",115200);
	obstacles->makeObstacles("/Users/pp/obstacles.txt");
	player[0] = new Player(obstacles, &serial[0]);
	player[1] = new Player(obstacles, &serial[1]);
	//player[1]->setPos(ofGetWidth() - 50, ofGetHeight()/2);
	player[0]->setGoal(ofGetWidth() - 50, ofGetHeight()/2);
	player[1]->setGoal(50, ofGetHeight()/2);
	player[0]->setColor(0,0,255);
	player[1]->setColor(255, 255, 0);
	guns[0] = new Gun(100,ofGetHeight()/2,obstacles);
	guns[1] = new Gun(ofGetWidth() - 100, ofGetHeight()/2,obstacles);
	guns[0]->setColor(255,0,0);
	guns[1]->setColor(0,0,255);
	for(int i = 0; i < 2; i++){
		guns[i]->setMove(0.04);
		//guns[i]->setMinMax(-1.2,1.2);
	}
	guns[0]->setMinMax(-0.7,0.7);
	guns[1]->setMinMax(-4.0,-2.6);
	commandMap.insert(make_pair("left,walk",'f'));
	commandMap.insert(make_pair("right,walk",'g'));
	commandMap.insert(make_pair("up,walk",'h'));
	commandMap.insert(make_pair("down",'i'));
	commandMap.insert(make_pair("shock,walk",'j'));
	franklinBook.loadFont("frabk.ttf", 32);
	ofSetFrameRate(30);
	int isRunning = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	eye->update();
	//player[0]->setHeading(&serial[0]);
	if(isRunning){
		for(int i = 0; i < 2; i++){
			player[i]->setPosFromCamera(eye->getColorPos(i),320,240);
			player[i]->update();
			guns[i]->aim(player[i]);
			guns[i]->update();
		}
	}
}

void testApp::reset(){
	isRunning = 0;
	for(int i = 0; i < 2; i++){
		player[i]->reset();
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
		ofSetColor(255,255,255);
		for(int i = 0; i < 2; i++){
			player[i]->draw();
			guns[i]->draw();
		}
	}
	if(player[0]->isDead || player[1]->isDead){
		if(player[0]->isDead){
			output = "Player 1 has died.  Player 2 has won";
			if(!FAKING){
				serial[0].writeByte('l');
				serial[1].writeByte('k');
			}
		} else {
			output = "Player 1 has won.  Player 2 has died";
			if(!FAKING){
				serial[0].writeByte('k');
				serial[1].writeByte('l');
			}
		}
	} else {
		ostringstream si; 
		si << "Player 1 strength = " << player[0]->strength << " Player 2 strength = " << player[1]->strength << endl;
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
	if(key=='y'){
		serial[0].setup(SERIAL_PORT_A,115200);
	}
	if(key=='u'){
		serial[0].setup(SERIAL_PORT_B,115200);
	}
	if(key=='r'){
		isRunning = !isRunning;
	}
	if(key=='p'){
		reset();
	}
	if(key=='0'){
		for(int i = 0; i < 2; i++){
			player[i]->setOffset();
		}
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

