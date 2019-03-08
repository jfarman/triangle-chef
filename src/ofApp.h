#pragma once

#include "ofMain.h"
#include <math.h>.

class ofApp : public ofBaseApp{

	public:
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
		
	protected:
		ofPoint startPoint;
		ofPoint endPoint;
		ofPolyline bisectingLine;
		
		bool isDraggingEndPoint;
		bool isDraggingStartPoint;

		float lineSlope;
		float lineIntercept;

		ofPoint bisectingLineLerpPoint;
		bool isBisectingLineLerpPointActive;
		bool isDraggingBisectingLineLerpPoint;

		float currentLerpIndex;

		const float POINT_RADIUS = 7.0f;
		const float LINE_WIDTH = 3.0f;
		const float CLICK_BUFFER = 7.0f;

		float windowWidth;
		float windowHeight;

		bool shouldUpdateTriangle;

		void reset();
		void drawBisection(ofPoint vertexA, ofPoint vertexB);
};
