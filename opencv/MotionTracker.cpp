#include "stdafx.h"
#include "MotionTracker.h"

MotionTracker::MotionTracker()
{
	xROI = 150; // define the top-left corner of our
	yROI = 150; // region-of-interest
	widthROI = 100;
	heightROI = 100;
	firstTime = true;
}
int MotionTracker::get_xCenter()	 { return xROI + widthROI / 2; }
int MotionTracker::get_yCenter()	 { return yROI + heightROI / 2; }
int MotionTracker::get_widthROI()	{ return widthROI; }
int MotionTracker::get_heightROI() { return heightROI; }

int MotionTracker::start()
{

	Ptr<Tracker> tracker = Tracker::create("MIL");

	// Read video
	VideoCapture video("videos/vehicle.mp4");

	// Check video is open
	if (!video.isOpened())
	{
		cout << "Could not open video file" << endl;
		return 1;
	}

	// Read first frame. 
	Mat frame;
	video.read(frame);

	// Define an initial bounding box
	Rect2d bbox(287, 23, 86, 320);

	

	// Initialize tracker with first frame and bounding box
	tracker->init(frame, bbox);

	while (video.read(frame))
	{
		// Update tracking results
		tracker->update(frame, bbox);

		// Draw bounding box
		rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
		feedNewframe(frame);
		// Display result
		int x = get_xCenter();
		int y = get_yCenter();
		int roi = get_widthROI();
		int roh = get_heightROI();

		imshow("detected ", frame);
		cout << x + " " + y +  roi   + roh;
		int k = waitKey(1);
		if (k == 27) break;

	}


}
MotionTracker::~MotionTracker()
{
}

void MotionTracker::feedNewframe(Mat frame) {
	Mat diffPrevious; // difference between the previous and current frame
	Mat grayDiffPrevious;

	int x, y;
	int weight, sumWeight, sumWeightX, sumWeightY;
	int xCenter, yCenter;

	if (firstTime) {
		frame.copyTo(currentFrame);
		frame.copyTo(previousFrame);
		frame.copyTo(previousPreviousFrame);
		firstTime = false;
	}
	previousFrame.copyTo(previousPreviousFrame);
	currentFrame.copyTo(previousFrame);
	frame.copyTo(currentFrame);
	// get the diff between the current frame and the second frame before it
	absdiff(previousPreviousFrame, frame, diffPrevious);
	 
	cvtColor(diffPrevious, grayDiffPrevious, CV_BGR2GRAY);

	sumWeightX = sumWeightY = sumWeight = 0; // set them all to zero
	for (y = yROI; y < yROI + heightROI; y++) { // visit pixels row-by-row
		// inside each row, visit pixels from left to right
		for (x = xROI; x < xROI + widthROI; x++) {
			// weight of the pixel x,y
			weight = grayDiffPrevious.at<unsigned char>(y, x);
			sumWeight = sumWeight + weight;
			sumWeightX = sumWeightX + x * weight;
			sumWeightY += y * weight;
		}
	}
	if (sumWeight != 0) {
		xCenter = sumWeightX / sumWeight;
		yCenter = sumWeightY / sumWeight;
	}
	xROI = xCenter - widthROI / 2; // make the ROI "follow" the center-of-mass
	yROI = yCenter - heightROI / 2;
	// let's keep the ROI inside the limits at all times
	if (xROI < leftLimit) { // the ROI is starting to go out on the left
		xROI = leftLimit; // keep it inside
	}
	if (yROI < topLimit) { // the ROI is starting to go up out of the frame
		yROI = topLimit; // keep it inside
	}
	if (xROI + widthROI >= rightLimit) { // ROI starts to go out on the right
		xROI = rightLimit - widthROI; // keep it inside
	}
	if (yROI + heightROI >= bottomLimit) { // ROI starts to go out at bottom
		yROI = bottomLimit - heightROI; // keep it inside
	}

}
 
void MotionTracker::setROI(int new_xROI, int new_yROI, int new_widthROI, int new_heightROI) {
	xROI = new_xROI;
	yROI = new_yROI;
	widthROI = new_widthROI;
	heightROI = new_heightROI;
}
void MotionTracker::setLimits(int left, int top, int right, int bottom) {
	leftLimit = left;
	rightLimit = right;
	topLimit = top;
	bottomLimit = bottom;
}
