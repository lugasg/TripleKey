#ifndef APP_OPENCLAW_H
#define APP_OPENCLAW_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

#define OPENCLAW_API_URL "http://192.168.1.100:18790/status.json"

struct CodexAccount {
    String label;       // "AH", "AW", "LH", "LW"
    uint8_t pct;        // percentage value
};

struct AgentInfo {
    String label;       // "XH", "CD", "TD", "HW"
    uint8_t usagePct;   // usage percentage
};

struct OpenClawStatus {
    CodexAccount codex[4];  // AH, AW, LH, LW
    AgentInfo agents[4];    // XH, CD, TD, HW
    bool dataValid;
    unsigned long lastUpdate;
};

void fetchOpenClawStatus(OpenClawStatus* status);
uint16_t getCodexColor(uint8_t pct);
uint16_t getAgentColor(uint8_t pct);

#endif
