#include "board_def.h"
#include "manager/manager.h"
#include "app/app_key.h"

void setup()
{
	Serial.begin(115200);
	board_init();
	app_key_init();
	manager_init();
}

void loop()
{
	manager_loop();
}
