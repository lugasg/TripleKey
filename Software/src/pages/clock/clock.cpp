#include "clock.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_audio.h"
const char *number_name[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

std::vector<String> clock_name;
static uint8_t clock_type_num = 0;
int8_t clock_type_index;
bool f_updateall = false;
static uint8_t num_old[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

extern page_t page_weather;
extern page_t page_mijia;

bool autotheme = true;  //是否时钟主题自动切换
bool clockaudio;  //是否整点报时

// Convert 24H to 12H format and display time
void dispTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	char path[100];
	if (autotheme && sec == 0)
	{
		clock_type_index++;
		if (clock_type_index >= clock_type_num)
		{
			clock_type_index = 0;
		}
		f_updateall = true;
	}

	// Convert to 12-hour format
	uint8_t display_hour = hour % 12;
	if (display_hour == 0) display_hour = 12;

	if (num_old[0] != display_hour / 10 || f_updateall)
	{
		num_old[0] = display_hour / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[display_hour / 10]);
		myDrawPNG(0, 0, path, 0);
	}
	if (num_old[1] != display_hour % 10 || f_updateall)
	{
		num_old[1] = display_hour % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[display_hour % 10]);
		myDrawPNG(64, 0, path, 0);
	}

	if (num_old[2] != min / 10 || f_updateall)
	{
		num_old[2] = min / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[min / 10]);
		myDrawPNG(0, 0, path, 1);
	}
	if (num_old[3] != min % 10 || f_updateall)
	{
		num_old[3] = min % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[min % 10]);
		myDrawPNG(64, 0, path, 1);
	}

	if (num_old[4] != sec / 10 || f_updateall)
	{
		num_old[4] = sec / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[sec / 10]);
		myDrawPNG(0, 0, path, 2);
	}
	if (num_old[5] != sec % 10 || f_updateall)
	{
		num_old[5] = sec % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[sec % 10]);
		myDrawPNG(64, 0, path, 2);
	}

	f_updateall = false;
}

// New function to display date and weekday
void dispDate()
{
	static const char* weekdays[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
	char path[100];
	static uint8_t old_month = 0xFF;
	static uint8_t old_day = 0xFF;
	static uint8_t old_wday = 0xFF;
	
	if (old_month != timeInfo.tm_mon + 1 || old_day != timeInfo.tm_mday || old_wday != timeInfo.tm_wday || f_updateall)
	{
		// Clear the third row display area first
		// Add code to clear the display area if needed
		
		// Display month and day (MM/DD)
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[(timeInfo.tm_mon + 1) / 10]);
		myDrawPNG(0, 0, path, 2);
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[(timeInfo.tm_mon + 1) % 10]);
		myDrawPNG(32, 0, path, 2);
		
		// Draw a slash between month and day
		// Add code to draw a slash if needed
		
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[timeInfo.tm_mday / 10]);
		myDrawPNG(64, 0, path, 2);
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[timeInfo.tm_mday % 10]);
		myDrawPNG(96, 0, path, 2);
		
		// Display weekday
		// You'll need to add code to display the weekday string
		// This might require adding new image assets for text display
		
		old_month = timeInfo.tm_mon + 1;
		old_day = timeInfo.tm_mday;
		old_wday = timeInfo.tm_wday;
	}
}

// 获取时钟类型数目
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	// Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		// Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		// Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			// Serial.print("  DIR : ");
			// Serial.println(file.name());
			clock_name.push_back(file.name());
			// Serial.println(clock_name[dir_num]);

			if (levels)
			{
				listDir(fs, file.path(), levels - 1);
			}
		}

		file = root.openNextFile();
	}
}

static void init(void *data)
{
	clock_type_num = clock_name.size();
	if (clock_type_num == 0)
	{
		// 获取时钟类型数目
		listDir(LittleFS, "/clock_theme/", 1);

		clock_type_num = clock_name.size();
		Serial.println(clock_type_num);
	}
	clock_type_index = random(clock_name.size());

	for (uint8_t i = 0; i < 6; i++)
	{
		num_old[i] = 0xff;
	}
	// clock_type_index=12;
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	//
	manager_setBusy(false);
}
uint32_t t_old, t_now = 0;
uint8_t hour, minute, second = 0;
static void loop(void *data)
{
	KEY_TYPE key;

	t_now = millis();
	if (t_now - t_old >= 1000)
	{
		t_old = t_now;
		if (getLocalTime(&timeInfo))
		{
			dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
			dispDate(); // Add date display
			if (timeInfo.tm_min == 0 && timeInfo.tm_sec == 0 && timeInfo.tm_hour > 7 && timeInfo.tm_hour < 22)
			{
				if (clockaudio == true)
				{
					app_audio_sayTimeCN(timeInfo.tm_hour, timeInfo.tm_min);
				}
			}

			// Serial.printf("%02d:%02d:%02d\r\n", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
	}

	key = app_key_get();
	switch (key)
	{
	case KEY1_DOUBLE:
		manager_switchToPage(&page_weather);
		break;
	case KEY1_LONG:
		manager_switchToPage(&page_mijia);
		break;
	// case KEY1_SHORT:
	case KEY2_SHORT:

		getLocalTime(&timeInfo);
		app_audio_sayTimeCN(timeInfo.tm_hour, timeInfo.tm_min);
		break;

	case KEY4_LONG:				  // 长按
		manager_switchToParent(); // 进入父项目 //退出
		break;

	case ENC_NEXT:
		clock_type_index++;
		if (clock_type_index >= clock_type_num)
		{
			clock_type_index = 0;
		}
		f_updateall = true;
		dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		break;
	case ENC_PREV:
		clock_type_index--;
		if (clock_type_index < 0)
		{
			clock_type_index = clock_type_num - 1;
		}
		f_updateall = true;
		dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		break;
	default:
		break;
	}
}

static void exit(void *data)
{

	manager_setBusy(true);
}
#include "img.h"
page_t page_clock = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Clock",
	.title_cn = "时间",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};