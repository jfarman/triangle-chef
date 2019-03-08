#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::update(){
	if (isDraggingEndPoint) {
		endPoint = ofPoint(mouseX, mouseY);
		shouldUpdateTriangle = true;
	}
	else if (isDraggingStartPoint) {
		startPoint = ofPoint(mouseX, mouseY);
		shouldUpdateTriangle = true;
	}
	if (shouldUpdateTriangle) {
		bisectingLine.clear();
		bisectingLine.addVertex(startPoint.x, startPoint.y);
		bisectingLine.addVertex(endPoint.x, endPoint.y);
		
		if (isBisectingLineLerpPointActive) {
			bisectingLineLerpPoint = (ofPoint)bisectingLine.getPointAtIndexInterpolated(currentLerpIndex);
			float lerpPointDistance = bisectingLineLerpPoint.distance(startPoint);
			float triangleHeight = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
			currentLerpIndex = lerpPointDistance / triangleHeight;
		}

		lineSlope = (endPoint.x - startPoint.x) / (endPoint.y - startPoint.y);
		lineIntercept = endPoint.x - (lineSlope*endPoint.y);

		if (shouldUpdateTriangle) shouldUpdateTriangle = false;
	}
	else if (isDraggingBisectingLineLerpPoint) {
		float max = MAX(startPoint.y, endPoint.y);
		float min = MIN(startPoint.y, endPoint.y);
		float y = ofClamp(mouseY, min, max);
		ofPoint linePoint = ofPoint(lineSlope * y + lineIntercept, y);
		bisectingLineLerpPoint = linePoint;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofPushMatrix();

	ofSetColor(0, 90, 60);
	ofSetLineWidth(LINE_WIDTH);

	float halfVertex = 45.0 / 2.0f;
	float triangleHeight = ofDist(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	float triangleLength = triangleHeight / cos(ofDegToRad(halfVertex));

	ofVec2f directionVector = ofVec2f(endPoint.x - startPoint.x, endPoint.y - startPoint.y);
	ofVec2f normalized = directionVector.getNormalized();
	int angle = normalized.angle(ofVec2f(0, -1));

	double addRadians = ofDegToRad(angle + halfVertex);
	double subRadians = ofDegToRad(angle - halfVertex);

	// Adapted from https://math.stackexchange.com/q/1101779
	ofPoint vertexA = ofPoint(startPoint.x - (triangleLength * sin(addRadians)), startPoint.y - (triangleLength * cos(addRadians)));
	ofPoint vertexB = ofPoint(startPoint.x - (triangleLength * sin(subRadians)), startPoint.y - (triangleLength * cos(subRadians)));

	ofNoFill();
	ofSetColor(ofColor::mediumPurple);
	ofDrawTriangle(startPoint.x, startPoint.y, vertexA.x, vertexA.y, vertexB.x, vertexB.y);
	
	ofFill();
	
	ofSetColor(ofColor::magenta);
	ofDrawCircle(startPoint, POINT_RADIUS);
	ofDrawCircle(endPoint, POINT_RADIUS);

	bisectingLine.draw();

	string currentLerpIndexString = "n/a";

	if (isBisectingLineLerpPointActive) {
		float lerpPointDistance = bisectingLineLerpPoint.distance(startPoint);
		currentLerpIndex = lerpPointDistance / triangleHeight;
		currentLerpIndexString = ofToString(currentLerpIndex, 4);

		ofSetColor(ofColor::orange);
		ofDrawCircle(bisectingLineLerpPoint, POINT_RADIUS);

		ofPolyline lineA; // todo: draw triangle using these lines instead of ofDrawTriangle()
		lineA.addVertex(startPoint.x, startPoint.y);
		lineA.addVertex(vertexA);
		ofPoint interpolatedLineAPoint = (ofPoint)lineA.getPointAtIndexInterpolated(currentLerpIndex);

		ofPolyline lineB;
		lineB.addVertex(startPoint.x, startPoint.y);
		lineB.addVertex(vertexB);
		ofPoint interpolatedLineBPoint = (ofPoint)lineB.getPointAtIndexInterpolated(currentLerpIndex);

		ofPolyline lineAB;
		lineAB.addVertex(interpolatedLineAPoint);
		lineAB.addVertex(interpolatedLineBPoint);
		lineAB.draw();

		ofSetColor(ofColor::lightGreen);
		ofDrawCircle(interpolatedLineAPoint, POINT_RADIUS);
		ofDrawCircle(interpolatedLineBPoint, POINT_RADIUS);
	}

	ofSetColor(ofColor::white);
	ofDrawBitmapString("interpolation index along bisection of triangle = " + currentLerpIndexString, 10, 25);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'r') reset();
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
	ofPoint clickedPoint = ofPoint(x, y);
	ofPoint linePoint = ofPoint(lineSlope * y + lineIntercept, y);
	// allow line clicks within a reasonable buffer distance
	float maxDistance = LINE_WIDTH + CLICK_BUFFER;
	if (endPoint.distance(clickedPoint) < POINT_RADIUS + CLICK_BUFFER) {
		isDraggingEndPoint = true;
	}
	else if (startPoint.distance(clickedPoint) < POINT_RADIUS + CLICK_BUFFER) {
		isDraggingStartPoint = true;
	}
	else if (linePoint.distance(clickedPoint) < maxDistance) {
		bisectingLineLerpPoint = linePoint;
		isBisectingLineLerpPointActive = true;
		isDraggingBisectingLineLerpPoint = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	isDraggingEndPoint = false;
	isDraggingStartPoint = false;
	isDraggingBisectingLineLerpPoint = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

	float widthRatio = (float)w / windowWidth;
	float heightRatio = (float)h / windowHeight;
	
	startPoint = ofPoint(startPoint.x * widthRatio, startPoint.y * heightRatio);
	endPoint = ofPoint(endPoint.x * widthRatio, endPoint.y * heightRatio);

	windowWidth = w;
	windowHeight = h;

	shouldUpdateTriangle = true;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------  
void ofApp::setup() {
	ofBackground(ofColor::black);
	ofSetFullscreen(false);
	
	reset();
}

//--------------------------------------------------------------  
void ofApp::reset() {
	windowWidth = ofGetWindowWidth();
	windowHeight = ofGetWindowHeight();

	// Arbitrary initial position for triangle
	startPoint = ofPoint(windowWidth / 8.0f, windowHeight / 4.0f);
	endPoint = ofPoint(windowWidth / 2.0, windowHeight / 1.5f);

	shouldUpdateTriangle = true;
	isBisectingLineLerpPointActive = false;
}