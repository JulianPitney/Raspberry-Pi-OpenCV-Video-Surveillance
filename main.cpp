#include </home/pi/surveillance_proj/events.h>
#include </home/pi/surveillance_proj/configuration_manager.h>
#include </home/pi/surveillance_proj/image_processor.h>


int scheduler_unitTest(Scheduler *input_scheduler, int test_size) {

	Event *temp = NULL;
	time_t current_time;

	// Populate scheduler->event_schedule with (test_size) random Events
	for (int i = 0; i < test_size; i++)
	{

		if (i % 2 == 0)
		{
			time(&current_time);
			temp = new SurveillancePhoto(current_time + 30, 5, "testPhoto", 7);
		}
		else
		{
			time(&current_time);
			temp = new SurveillanceVideo(10, current_time + 60, 15, "testVideo", 5);
		}

		input_scheduler->add_event(temp);

	}

	return 0;
}


int main(int argc, char* argv[])
{


	Configuration_Manager *config = new Configuration_Manager;

	while (1)
	{
		int num_of_schedulers = 0;
		char menu_choice;
		cout << "\n\n";
		cout << "[1] Enter Configuration Manager\n";
		cout << "[2] Begin Surveillance Cycle\n";
		cout << "[3] View Camera Feed\n";
		cout << "[4] Exit\n";
		cout << "\nInput: ";
		cin >> menu_choice;

		switch (menu_choice)
		{
			case('1'):

				cout << "Entering configuration manager...\n";
				config->config_manager_mainloop();
				break;

			case('2'):

				cout << "Beginning surveillance cycle (Hold Q/q to interrupt cycle)...\n";
				num_of_schedulers = config->get_scheduler_list_size();

				if (num_of_schedulers > 0)
				{
					while (1)
					{
						for (int i = 0; i < num_of_schedulers; i++)
						{
							cout << "Running execution cycle for Scheduler_" << i << ".\n";
							config->get_scheduler(i)->scheduler_execution_cycle();
						}
						
						// Add ability to exit surveillance cycle here (linux version)

					}
				}
				else
				{
					cout << "Error: No Schedulers found, returning to menu. \n";
				}

				break;

			case('3'):
				config->select_camera_feed();
				break;

			case('4'):

				cout << "Exiting program...\n";
				cout << "Cleaning things up...\n";
				for (unsigned int i = 0; i < config->get_scheduler_list_size(); i++)
				{
					config->get_scheduler(i)->get_camera()->~VideoCapture();
				}
				cout << "Goodbye!\n";
				return 0;

			default:

				cout << "Input not recognized, please try again.\n";
				continue;
		}
	}


	return 0;
}





