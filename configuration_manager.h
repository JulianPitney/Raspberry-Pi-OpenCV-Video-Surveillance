#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include </home/pi/surveillance_proj/scheduler.h>

using namespace std;

class Configuration_Manager {

public:
	
	int cascade_files_size = 17;
	string cascade_files[17] = { "haarcascade_eye.xml", "haarcascade_eye_tree_eyeglasses.xml", "haarcascade_frontalcatface.xml"
								, "haarcascade_frontalcatface_extended.xml", "haarcascade_frontalface_alt.xml", "haarcascade_frontalface_alt_tree.xml"
								, "haarcascade_frontalface_alt2.xml", "haarcascade_frontalface_default.xml", "haarcascade_fullbody.xml"
								, "haarcascade_lefteye_2splits.xml", "haarcascade_licence_plate_rus_16stages.xml", "haarcascade_lowerbody.xml"
								, "haarcascade_profileface.xml", "haarcascade_righteye_2splits.xml", "haarcascade_russian_plate_number.xml"
								, "haarcascade_smile.xml", "haarcascade_upperbody.xml"};

	int init_scheduler();
	int scheduler_manager();
	int configure_scheduler(Scheduler *input_sched);
	int configure_events(Scheduler *input_sched);
	int configure_image_processor(Scheduler *input_sched);
	int configure_response_module(Scheduler *input_sched);
	int config_manager_mainloop();
	Scheduler* get_scheduler(int scheduler_index);
	int get_scheduler_list_size();
	int select_camera_feed();
	int display_all_feeds();


private:

	vector<int> camera_indexes_in_use;
	vector<Scheduler*> scheduler_list;
};
