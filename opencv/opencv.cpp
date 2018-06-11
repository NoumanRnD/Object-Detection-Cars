// opencv.cpp : Defines the entry point for the console application.
//

#pragma warning (disable : 4996)
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include "stdafx.h"

#include "MotionTracker.h"




int main(int argc, char **argv)
{
	// Set up tracker. 
	// Instead of MIL, you can also use 
	// BOOSTING, KCF, TLD, MEDIANFLOW or GOTURN  
	MotionTracker obj ;
	obj.start();
	return 0;

}
