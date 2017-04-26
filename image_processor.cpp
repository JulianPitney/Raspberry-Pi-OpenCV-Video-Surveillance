#include </home/pi/surveillance_proj/image_processor.h>

Image_Processor::Image_Processor (string cascade_path, string cascade_name, string nested_cascade_name) {

	this->cascade_path = cascade_path;
	this->cascade_name = cascade_name;
	this->nested_cascade_name = nested_cascade_name;

	cout << "Successfully created Image_Processor...\n";
}

void Image_Processor::set_cascade_path(string input_path) {

	this->cascade_path = input_path;
	cout << "cascade_path set to: " << input_path << endl;
}

void Image_Processor::set_cascade_name(string input_name) {

	this->cascade_name = input_name;
	cout << "primary_cascade_name set to: " << input_name << endl;
}

void Image_Processor::set_nested_cascade_name(string input_name) {

	this->nested_cascade_name = input_name;
	cout << "secondary_cascade_name set to: " << input_name << endl;
}


int Image_Processor::frame_colour_analysis (Mat input_frame, int camera_index) {

	int ret_val = 0;
	int colour_arr_count = 0;
	string colour_arr[7] = { "orange", "yellow", "green", "blue", "violet", "red" };
	int hue_arr[14] = { 0,20,22,38,38,75,75,130,130,160,160,179 }; // 	iLowH_orange, iHighH_orange, iLowH_yellow, iHighH_yellow,
																   //   iLowH_green, iHighH_green, iLowH_blue, iHighH_blue,															   //   iLowH_violet, iHighH_violet, iLowH_red, iHighH_red.
	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;

	int iLastX = -1;
	int iLastY = -1;

	Mat imgHSV;
	cvtColor(input_frame, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	for (int i = 0; i < 12; i+=2)
	{
		Mat imgThresholded;

		inRange(imgHSV, Scalar(hue_arr[i], iLowS, iLowV), Scalar(hue_arr[i+1], iHighS, iHighV), imgThresholded); //Threshold the image

																										
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));	  //morphological opening (removes small objects from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area is <= 10000, I consider that the there are no objects in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;
			ret_val = 1;
		}

		colour_arr_count++;
	}
	return ret_val;
}

int Image_Processor::frame_contour_analysis (Mat input_frame, int camera_index) {

	return 0;
}


secondary_cascade_return_pkg Image_Processor::detectAndDraw(Mat& img, CascadeClassifier& cascade, CascadeClassifier& nestedCascade, double scale, bool tryflip, string save_dir, int scheduler_number) {

	secondary_cascade_return_pkg ret_pkg;
	ret_pkg.ret_val = 0;
	ret_pkg.save_dir = "empty";
	int nested_obj_size = 0;
	double t = 0;
	vector<Rect> faces, faces2;
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	t = (double)cvGetTickCount();
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE,
		Size(30, 30));
	if (tryflip)
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); r++)
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)cvGetTickCount() - t;
	printf("detection time = %g ms\n", t / ((double)cvGetTickFrequency()*1000.));
	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);

			ret_pkg.ret_val = 1;
		}
		else
			rectangle(img, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
				cvPoint(cvRound((r.x + r.width - 1)*scale), cvRound((r.y + r.height - 1)*scale)),
				color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			//|CASCADE_DO_CANNY_PRUNING
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
			radius = cvRound((nr.width + nr.height)*0.25*scale);
			circle(img, center, radius, color, 3, 8, 0);
		}

		nested_obj_size = nestedObjects.size();
	}


	if (nested_obj_size > 0)
        {
        	ret_pkg.ret_val = 2;
                vector<int> compression_params;
                compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
                compression_params.push_back(100);
                save_dir += "SECCASDEC#";
                save_dir += to_string(this->secondary_cascade_detections);
                save_dir += "_CAMERA#";
                save_dir += to_string(scheduler_number);
                save_dir += ".jpg";

                imwrite(save_dir, img, compression_params);
                this->secondary_cascade_detections++;
                ret_pkg.save_dir = save_dir;
         }

	

	return ret_pkg;
}


secondary_cascade_return_pkg Image_Processor::frame_humanface_analysis (Mat input_frame, int camera_index, string save_dir, int scheduler_number) {
	
	secondary_cascade_return_pkg ret_pkg;

	ret_pkg.ret_val = -1;
	ret_pkg.save_dir = "empty";
	double scale = 1;
	bool tryflip = false;

	CascadeClassifier cascade, nestedCascade;
	string cascade_full_path = this->cascade_path + this->cascade_name; 
	string nestedCascade_full_path = this->cascade_path + this->cascade_name;

	nestedCascade.load(nestedCascade_full_path); 
	cascade.load(cascade_full_path);
			

	if (input_frame.empty())
	{
		cout << "Error: frame_humanface_analysis() received empty frame. \n";
		return ret_pkg;
	}
	else
	{
		ret_pkg = detectAndDraw(input_frame, cascade, nestedCascade, scale, tryflip, save_dir, scheduler_number);
		return ret_pkg;
	}

}

