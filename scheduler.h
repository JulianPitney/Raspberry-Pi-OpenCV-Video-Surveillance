#pragma once
#include <list>
#include </home/pi/surveillance_proj/events.h>
#include </home/pi/surveillance_proj/image_processor.h>
#include </home/pi/surveillance_proj/response_module.h>
#include <unistd.h>

using namespace cv;

class Scheduler {

public:

	Scheduler(int input_camera_index, string save_directory, string cascade_path, string cascade_name, string nested_cascade_name, VideoCapture camera);
	list<Event*>* get_event_schedule();
	int add_event(Event *input_event);
	int remove_event(long int eventID);
	int create_new_event();
	int delete_existing_event();
	void print_event_schedule();
	void set_scheduler_resolution(int scheduler_resolution);
	int get_scheduler_resolution();
	void set_camera_index(int input_index);
	int get_camera_index();
	void set_save_directory(string input_save_directory);
	string get_save_directory();
	Image_Processor* get_image_processor();
	Response_Module* get_response_module();
	int display_camera_feed();
	VideoCapture* get_camera();

	int scheduler_execution_cycle(); // Main execution loop of Scheduler.


private:

	VideoCapture camera;
	int camera_index;
	string save_directory;
	list<Event*> event_schedule; // Using list to store events because we'll need arbitrary insertion to place new events in appropriate location
	int num_of_events;			// but will never need arbitrary access as we only access the event at the end of the list.			
	long int next_eventID = 0;
	Image_Processor* frame_processor;
	Response_Module* responder;

	Event* check_overdueEvents();
	int process_overdueEvents(Event *input_event);
	int scheduler_resolution; // Determines delay between each execution cycle of Scheduler (in ms).
							  // Higher value = higher delay = less CPU time = less resolution.	
							  // Lower value = lower delay = more CPU time = more resolution.
				
};
