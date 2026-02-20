#include "openclaw.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_openclaw.h"

static OpenClawStatus clawStatus;
static unsigned long lastDataUpdate;
static unsigned long lastTimeUpdate;

// Time display - for screen 3 (staggered layout)
static uint8_t oldHour = 0xFF;
static uint8_t oldMin = 0xFF;

// Draw a progress bar
static void drawProgressBar(Arduino_GFX* gfx, int x, int y, int w, int h, uint8_t pct, uint16_t color) {
    // Draw border
    gfx->drawRect(x, y, w, h, DARKGREY);

    // Calculate fill width (leave 1px padding on each side)
    int fillW = (w - 2) * pct / 100;

    // Draw fill
    gfx->fillRect(x + 1, y + 1, fillW, h - 2, color);
}

// Draw Screen 1 - Codex Balance
static void drawCodexScreen() {
    gfx[0]->fillScreen(BLACK);
    gfx[0]->setUTF8Print(true);
    gfx[0]->setFont(&MiSans_Demibold_12);
    gfx[0]->setTextColor(WHITE);

    // Title
    gfx[0]->setCursor(5, 14);
    gfx[0]->print("Codex");

    // Layout: label(18px) + bar(rest), no percentage text
    int startY = 22;
    int barHeight = 22;
    int labelWidth = 20;  // narrower label area
    int barX = labelWidth + 2;
    int barWidth = 128 - barX - 2;  // fill remaining width
    int gap = 2;

    for (int i = 0; i < 4; i++) {
        int y = startY + i * (barHeight + gap);

        // Label - smaller font
        gfx[0]->setFont(&MiSans_MediumEN_12);
        gfx[0]->setCursor(2, y + 14);
        gfx[0]->setTextColor(clawStatus.dataValid ? WHITE : DARKGREY);
        gfx[0]->print(clawStatus.codex[i].label);

        // Progress bar
        uint16_t color = clawStatus.dataValid ?
            getCodexColor(clawStatus.codex[i].pct) : DARKGREY;
        drawProgressBar(gfx[0], barX, y + 3, barWidth, barHeight - 6,
            clawStatus.dataValid ? clawStatus.codex[i].pct : 0, color);
    }
}

// Draw Screen 2 - Agent Context Usage
static void drawAgentScreen() {
    gfx[1]->fillScreen(BLACK);
    gfx[1]->setUTF8Print(true);
    gfx[1]->setFont(&MiSans_Demibold_12);
    gfx[1]->setTextColor(WHITE);

    // Title
    gfx[1]->setCursor(5, 14);
    gfx[1]->print("Context");

    // Same layout as Codex screen
    int startY = 22;
    int barHeight = 22;
    int labelWidth = 20;
    int barX = labelWidth + 2;
    int barWidth = 128 - barX - 2;
    int gap = 2;

    for (int i = 0; i < 4; i++) {
        int y = startY + i * (barHeight + gap);

        // Label - smaller font
        gfx[1]->setFont(&MiSans_MediumEN_12);
        gfx[1]->setCursor(2, y + 14);
        gfx[1]->setTextColor(clawStatus.dataValid ? WHITE : DARKGREY);
        gfx[1]->print(clawStatus.agents[i].label);

        // Progress bar
        uint16_t color = clawStatus.dataValid ?
            getAgentColor(clawStatus.agents[i].usagePct) : DARKGREY;
        drawProgressBar(gfx[1], barX, y + 3, barWidth, barHeight - 6,
            clawStatus.dataValid ? clawStatus.agents[i].usagePct : 0, color);
    }
}

// Draw Screen 3 - Time (hour top, minute bottom, centered)
static void drawTimeScreen() {
    if (!getLocalTime(&timeInfo)) {
        return;
    }

    // Convert to 12-hour format
    uint8_t hour = timeInfo.tm_hour % 12;
    if (hour == 0) hour = 12;
    uint8_t min = timeInfo.tm_min;

    // Only redraw if changed
    if (hour == oldHour && min == oldMin) {
        return;
    }

    oldHour = hour;
    oldMin = min;

    gfx[2]->fillScreen(BLACK);
    gfx[2]->setFont(&MiSans_Demibold_12);
    gfx[2]->setTextSize(3);  // 3x scale → ~36px tall per line

    char hourStr[4];
    char minStr[4];
    sprintf(hourStr, "%2d", hour);
    sprintf(minStr, "%02d", min);

    // Hour - upper half, centered
    gfx[2]->setTextColor(QINGSHUILAN);
    gfx[2]->setCursor(14, 50);
    gfx[2]->print(hourStr);

    // Minute - lower half, centered
    gfx[2]->setTextColor(WHITE);
    gfx[2]->setCursor(14, 100);
    gfx[2]->print(minStr);

    gfx[2]->setTextSize(1);  // reset
}

// Refresh all display content
static void refreshDisplay() {
    drawCodexScreen();
    drawAgentScreen();
    drawTimeScreen();
}

static void init(void *data) {
    clawStatus.dataValid = false;
    clawStatus.lastUpdate = 0;
    lastDataUpdate = 0;
    lastTimeUpdate = 0;
    oldHour = 0xFF;
    oldMin = 0xFF;

    // Initialize labels
    clawStatus.codex[0] = {"AH", 0};
    clawStatus.codex[1] = {"AW", 0};
    clawStatus.codex[2] = {"LH", 0};
    clawStatus.codex[3] = {"LW", 0};
    clawStatus.agents[0] = {"XH", 0};
    clawStatus.agents[1] = {"CD", 0};
    clawStatus.agents[2] = {"TD", 0};
    clawStatus.agents[3] = {"HW", 0};
}

static void enter(void *data) {
    // Clear all screens
    gfx1->fillScreen(BLACK);
    gfx2->fillScreen(BLACK);
    gfx3->fillScreen(BLACK);

    // Show loading indicator
    dispProcessing(1);
    gfx[1]->setUTF8Print(true);
    gfx[1]->setFont(&MiSans_Demibold_12);
    gfx[1]->setTextColor(WHITE);
    gfx[1]->setCursor(30, 70);
    gfx[1]->print("Loading...");

    // Fetch initial data
    fetchOpenClawStatus(&clawStatus);
    lastDataUpdate = millis();
    lastTimeUpdate = millis();

    // Draw all screens
    refreshDisplay();

    manager_setBusy(false);
}

static void loop(void *data) {
    // Update time every second
    if (millis() - lastTimeUpdate >= 1000) {
        lastTimeUpdate = millis();
        drawTimeScreen();
    }

    // Refresh data every 2 minutes
    if (millis() - lastDataUpdate >= 2 * 60 * 1000) {
        Serial.println("[OpenClaw] Refreshing data...");
        fetchOpenClawStatus(&clawStatus);
        lastDataUpdate = millis();
        drawCodexScreen();
        drawAgentScreen();
    }

    // Handle key events
    KEY_TYPE key = app_key_get();
    switch (key) {
    case ENC_NEXT:
    case ENC_PREV:
        // Manual refresh on encoder
        fetchOpenClawStatus(&clawStatus);
        lastDataUpdate = millis();
        refreshDisplay();
        break;

    case KEY4_LONG:
        // Exit page
        manager_switchToParent();
        break;

    default:
        break;
    }
}

static void exit(void *data) {
    manager_setBusy(true);
}

#include "img.h"
page_t page_openclaw = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "OpenClaw",
    .title_cn = "OpenClaw",
    .icon = img_bits,
    .icon_width = img_width,
    .icon_height = img_height,
    .sleep_enable = false,
    .wakeup_btn_effect_enable = false,
    .acc_enable = false,
};
