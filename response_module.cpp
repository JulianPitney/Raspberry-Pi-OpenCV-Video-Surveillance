#include </home/pi/surveillance_proj/response_module.h>

using namespace std;


Response_Module::Response_Module() {

	this->last_colour_response = 0;
	this->last_primary_cascade_response = 0;
	this->last_secondary_cascade_response = 0;

	this->colour_response_cooldown = 10;
	this->primary_cascade_response_cooldown = 10;
	this->secondary_cascade_response_cooldown = 10;

	this->colour_detected = false;
	this->primary_cascade_detected = false;
	this->secondary_cascade_detected = false;

	cout << "Reponse_Module successfully created...\n";
}

void Response_Module::set_colour_detected(bool colour_detected) {

	this->colour_detected = colour_detected;
}

bool Response_Module::get_colour_detected() {

	return this->colour_detected;
}

void Response_Module::set_primary_cascade_detected(bool primary_cascade_detected) {

	this->primary_cascade_detected = primary_cascade_detected;
}

bool Response_Module::get_primary_cascade_detected() {

	return this->primary_cascade_detected;
}

void Response_Module::set_secondary_cascade_detected(bool secondary_cascade_detected) {

	this->secondary_cascade_detected = secondary_cascade_detected;
}

bool Response_Module::get_secondary_cascade_detected() {

	return this->secondary_cascade_detected;
}




time_t Response_Module::get_colour_responseCD() {

	return this->colour_response_cooldown;
}

void Response_Module::set_colour_responseCD(time_t colour_responseCD) {

	this->colour_response_cooldown = colour_responseCD;
}

time_t Response_Module::get_primary_cascade_responseCD() {

	return this->primary_cascade_response_cooldown;
}

void Response_Module::set_primary_cascade_responseCD(time_t primary_cascade_responseCD) {

	this->primary_cascade_response_cooldown = primary_cascade_responseCD;
}

time_t Response_Module::get_secondary_cascade_responseCD() {

	return this->secondary_cascade_response_cooldown;
}

void Response_Module::set_secondary_cascade_responseCD(time_t secondary_cascade_responseCD) {

	this->secondary_cascade_response_cooldown = secondary_cascade_responseCD;
}

string Response_Module::get_mail_destination_address() {

	return this->mail_destination_address;
}

void Response_Module::set_mail_destination_address(string input_address) {

	this->mail_destination_address = input_address;
	cout << "Response_Module::mail_destination_address changed to: " << input_address << endl;
}

int Response_Module::colour_detected_response() {

	
	cout << "Colour detected...\n";
	return 0;
}

int Response_Module::primary_cascade_detected_response() {

	cout << "Primary cascade detected...\n";
	return 0;
}

int Response_Module::secondary_cascade_detected_response(std::string save_dir) {
	
	cout << "Secondary cascade detected...\n";
	cout << "save_dir=" << save_dir << endl;
	std::string mail_command = "echo 'test' | mutt -s 'Secondary Cascade Detection' -a ";
	mail_command += save_dir;
	mail_command += " -- ";
	mail_command += this->get_mail_destination_address();
	size_t command_size = mail_command.size();
	char command[command_size];
	mail_command.copy(command, command_size, 0);
	system(command);
	return 0;
}


int Response_Module::response_execution_cycle(std::string save_dir) {

	time_t current_time = time(&current_time);

	if (this->colour_detected && difftime(current_time, this->last_colour_response) >= this->colour_response_cooldown)
	{
		this->colour_detected_response();
		this->last_colour_response = time(&last_colour_response);
	}

	time(&current_time);

	if (this->primary_cascade_detected && difftime(current_time, this->last_primary_cascade_response) >= this->primary_cascade_response_cooldown)
	{
		this->primary_cascade_detected_response();
		this->last_primary_cascade_response = time(&last_primary_cascade_response);
	}

	time(&current_time);

	if (this->secondary_cascade_detected && difftime(current_time, this->last_secondary_cascade_response) >= this->secondary_cascade_response_cooldown)
	{
		this->secondary_cascade_detected_response(save_dir);
		this->last_secondary_cascade_response = time(&last_secondary_cascade_response);		
	}
	

	this->colour_detected = false;
	this->primary_cascade_detected = false;
	this->secondary_cascade_detected = false;


	return 0;
}
