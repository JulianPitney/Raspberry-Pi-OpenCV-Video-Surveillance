#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>
#include <string.h>

using namespace std;
using namespace cv;

struct secondary_cascade_return_pkg {

	int ret_val;
	string save_dir;
};


class Image_Processor {


public:

	Image_Processor(string cascade_path, string cascade_name, string nested_cascade_name);
	void set_cascade_path(string input_path); // Sets path of cascade XML files.
	void set_cascade_name(string input_name); // Sets primary cascade XML file.
	void set_nested_cascade_name(string input_name); // Sets secondary cascade XML file.


	string cascade_path;
	string cascade_name;
	string nested_cascade_name;

	int frame_colour_analysis(Mat input_frame, int camera_index); // Scans frame for objects based on specified colours.
	int frame_contour_analysis(Mat input_frame, int camera_index); // Scans frame for objects based on specified shapes.
	secondary_cascade_return_pkg frame_humanface_analysis(Mat input_frame, int camera_index, string save_dir, int scheduler_number); // This is actually really powerful because we can detect all kinds of objects with the
																	// same function just by providing different classifiers (xml files in openCV data folder).																	// Just need to set a way for user to specify which classifiers to use in scans
	secondary_cascade_return_pkg detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale, bool tryflip, string save_dir, int scheduler_number); // Support function for frame_humanface_analysis().

	int primary_cascade_detections = 0;
	int secondary_cascade_detections = 0;
};
