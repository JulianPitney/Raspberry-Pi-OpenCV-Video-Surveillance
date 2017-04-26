#pragma once
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>

using namespace std;


// Event base class

class Event {

public:
	
	Event(time_t execution_deadline, time_t event_creation_time, string event_name, int priority);
	virtual int execute_event(cv::VideoCapture input_cap, int camera_index, string save_directory) = 0;
	time_t get_execution_deadline();
	time_t get_event_creation_time();
	void set_eventID(long int input_eventID);
	long int get_eventID();
	string get_eventName();
	int get_event_priority();

private:

	time_t execution_deadline;
	time_t event_creation_time;
	long int eventID;
	string event_name;
	int event_priority;
};



// SurveillancePhoto class

class SurveillancePhoto : public Event {

public:

	SurveillancePhoto(time_t execution_deadline, time_t event_creation_time, string event_name, int priority);
	int execute_event(cv::VideoCapture input_cap, int camera_index, string save_directory);

private:

};



// SurveillanceVideo class

class SurveillanceVideo : public Event {

public:

	SurveillanceVideo(time_t videoLen_s, time_t execution_deadline, time_t event_creation_time, string event_name, int priority); // Note: There is a +2 offset in recording time determined by videoLen_s.
	int execute_event(cv::VideoCapture input_cap, int camera_index, string save_directory);														// i.e it will always record an extra 2 seconds for some reason.

private:

	time_t videoLen_s;
};
