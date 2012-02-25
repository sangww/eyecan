/*
    eyeCan: a program that enables people to use PC with their eyes
    Copyright (C) 2012  Sang-won Leigh

    eyeCan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    eyeCan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "TestController.h"
#include <iostream>
#include <time.h>

using namespace std;

void TestController::eyeMove(int _x, int _y, onScreenPosition pos)
{
}
void TestController::eyeClick(int _x, int _y, onScreenPosition pos)
{
}
void TestController::eyeShortClick(int _x, int _y, onScreenPosition pos)
{
}
void TestController::eyeLongClick(int _x, int _y, onScreenPosition pos)
{
}
void TestController::eyeBlink(int _x, int _y)
{
}
void TestController::eyeGesture(int _x, int _y)
{
}
void TestController::eyeOutofScreen(int _x, int _y, onScreenPosition pos)
{
}
void TestController::eyeDwell(int _x, int _y, onScreenPosition pos,int dwellTime)
{
}
void TestController::eyeClose(int _x, int _y, onScreenPosition pos,int state)
{
}




TestController::TestController(){

}

TestController::~TestController(){
	
}

int TestController::setup(ActionDecoder *_am){

	//set callback
	am = _am;
	int id = am->setCallback(this);

	fontWS.loadFont("fonts/NanumGothic.otf",12);	

	//UI setup
	wstate=L"준비 완료";

	//return handler id
	return id;
}

void TestController::update()
{	
}
void TestController::draw()
{	
}
void TestController::setupWindow()
{
	ofSetWindowPosition(500, 500);
	ofSetWindowShape(200, 100);
}


//--------------------------------------------------------------
void TestController::mouseDragged(int x, int y, int button){	
}
//--------------------------------------------------------------
void TestController::mousePressed(int x, int y, int button){
}
//--------------------------------------------------------------
void TestController::mouseReleased(int x, int y, int button){	
}
void TestController::keyPressed(int key){
}
