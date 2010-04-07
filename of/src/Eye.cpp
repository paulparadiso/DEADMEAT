/*
 *  Eye.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 4/7/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Eye.h"

Eye::Eye(int _cW, int _cH){
	camWidth = _cW;
	camHeight = _cH;
	grabber.listDevices();
	grabber.setDeviceID(CAM_ID);
	grabber.initGrabber(camWidth,camHeight);
	cImg.allocate(camWidth,camHeight);
	camImg.allocate(camWidth,camHeight);
	cImgHSV.allocate(camWidth,camHeight);
	hueImg.allocate(camWidth,camHeight);
	satImg.allocate(camWidth,camHeight);
	briImg.allocate(camWidth,camHeight);
	for(int i = 0; i < NUM_COLORS; i++){
		colors[i].allocate(camWidth,camHeight);
		colorTrackedPixels[i] = new unsigned char [camWidth * camHeight];
		trackedTexture[i].allocate(camWidth,camHeight, GL_LUMINANCE);
	}	
}

void Eye::update(){
	grabber.grabFrame();
	if(grabber.isFrameNew()){
		camImg.setFromPixels(grabber.getPixels(), camWidth, camHeight);
		trackColors();
	}
}

ofxVec2f * Eye::getColorPos(int _i){
	return &colorsToTrack[_i].pos;
}

void Eye::draw(){
	camImg.draw(0,0);
}

void Eye::setColor(int _x, int _y, int _button){
	unsigned char * huePixels = hueImg.getPixels();
	unsigned char * satPixels = satImg.getPixels();
	
	int x = MIN(_x,hueImg.width-1);
	int y = MIN(_y,hueImg.height-1);
	
	if(_button == 0) {
		colorsToTrack[colorIndex].hue = huePixels[_x+(y*hueImg.width)];
		colorsToTrack[colorIndex].sat = satPixels[x+(y*satImg.width)];
		unsigned char *cPix = cImg.getPixels();
		colorIndex = (colorIndex + 1) % NUM_COLORS;
	}
	
}

void Eye::trackColors(){
	cImg.setFromPixels(grabber.getPixels(),camWidth,camHeight);
	//camImg.setFromPixels(grabber.getPixels(),camWidth,camHeight);
	cImgHSV.setFromPixels(cImg.getPixels(), camWidth, camHeight);
	cImgHSV.convertRgbToHsv();
	cImgHSV.convertToGrayscalePlanarImages(hueImg, satImg, briImg);
	unsigned char * huePixels = hueImg.getPixels();
	unsigned char * satPixels = satImg.getPixels();
	int nPixels = camWidth * camHeight;
	for(int i = 0; i < nPixels; i++){
		for(int j = 0; j < NUM_COLORS; j++){
			if ((huePixels[i] >= colorsToTrack[j].hue - 12 && huePixels[i] <= colorsToTrack[j].hue + 12) &&
				(satPixels[i] >= colorsToTrack[j].sat - 24 && satPixels[i] <= colorsToTrack[j].sat + 200)){
				colorTrackedPixels[j][i] = 255;
			} else {
				colorTrackedPixels[j][i] = 0;
			}
		}
	}
	for(int i = 0; i < NUM_COLORS; i++){
		colors[i].setFromPixels(colorTrackedPixels[i], camWidth, camHeight);
		finder[i].findContours(colors[i], 10,nPixels/3, 1, false, true);
		trackedTexture[i].loadData(colorTrackedPixels[i], camWidth, camHeight, GL_LUMINANCE);
		
		if(finder[i].blobs.size() > 0) {
			colorsToTrack[i].pos = ofxVec2f(finder[i].blobs[0].centroid.x,finder[i].blobs[0].centroid.y);
		}
	}
	//cout<<"Color pos 0 = "<<colorsToTrack[0].pos.x<<" "<<colorsToTrack[0].pos.y<<endl;
}