#pragma once
#include <iostream>
#include <ctime>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>

class Response_Module {


public:

	Response_Module();
	int response_execution_cycle(std::string save_dir);

	void set_colour_detected(bool colour_detected);
	bool get_colour_detected();
	void set_primary_cascade_detected(bool primary_cascade_detected);
	bool get_primary_cascade_detected();
	void set_secondary_cascade_detected(bool secondary_cascade_detected);
	bool get_secondary_cascade_detected();

	time_t get_colour_responseCD();
	void set_colour_responseCD(time_t colour_responseCD);
	time_t get_primary_cascade_responseCD();
	void set_primary_cascade_responseCD(time_t primary_cascade_responseCD);
	time_t get_secondary_cascade_responseCD();
	void set_secondary_cascade_responseCD(time_t secondary_cascade_responseCD);
	
	std::string get_mail_destination_address();
	void set_mail_destination_address(std::string input_address);

private:

	bool colour_detected;
	bool primary_cascade_detected;
	bool secondary_cascade_detected;

	time_t last_colour_response;
	time_t last_primary_cascade_response;
	time_t last_secondary_cascade_response;

	time_t colour_response_cooldown; // Set these in configuration to set cooldown time between each response type.
	time_t primary_cascade_response_cooldown;
	time_t secondary_cascade_response_cooldown;

	int colour_detected_response();
	int primary_cascade_detected_response();
	int secondary_cascade_detected_response(std::string save_dir);
	
	std::string mail_destination_address;
	
};
