#include "app_openclaw.h"
#include "board_def.h"  // For RGB565 macro

// Color definitions for status display
#define COLOR_GREEN  RGB565(0, 180, 100)
#define COLOR_YELLOW RGB565(255, 225, 0)
#define COLOR_RED    RGB565(255, 50, 50)

// Parse percentage from string like "112k/272k (41%)"
static uint8_t parseContextPct(const char* line) {
    if (line == nullptr || line[0] == '\0') {
        return 0;
    }
    String s = line;
    int start = s.indexOf('(');
    int end = s.indexOf('%');
    if (start >= 0 && end > start) {
        return (uint8_t)s.substring(start + 1, end).toInt();
    }
    return 0;
}

void fetchOpenClawStatus(OpenClawStatus* status) {
    if (WiFi.status() != WL_CONNECTED) {
        status->dataValid = false;
        return;
    }

    WiFiClient client;
    HTTPClient http;

    http.begin(client, OPENCLAW_API_URL);
    http.setTimeout(10000);
    http.setUserAgent("TripleKey/1.0");

    int httpCode = http.GET();
    Serial.printf("[OpenClaw] HTTP GET code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        JsonDocument doc;  // Use JsonDocument instead of deprecated DynamicJsonDocument
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
            Serial.printf("[OpenClaw] JSON parse error: %s\n", error.c_str());
            status->dataValid = false;
        } else {
            // Parse Codex accounts
            // quota.codex.accounts[0] = aiqi (hourlyPct, weeklyPct)
            // quota.codex.accounts[1] = luga (hourlyPct, weeklyPct)
            JsonArray accounts = doc["quota"]["codex"]["accounts"];

            if (accounts.size() >= 2) {
                // aiqi hourly and weekly
                status->codex[0].label = "AH";
                status->codex[0].pct = accounts[0]["hourlyPct"] | 0;

                status->codex[1].label = "AW";
                status->codex[1].pct = accounts[0]["weeklyPct"] | 0;

                // luga hourly and weekly
                status->codex[2].label = "LH";
                status->codex[2].pct = accounts[1]["hourlyPct"] | 0;

                status->codex[3].label = "LW";
                status->codex[3].pct = accounts[1]["weeklyPct"] | 0;
            }

            // Parse Agent context usage
            // openclaw.agents.main/code/todo/hardware -> contextUsageLine
            JsonObject agents = doc["openclaw"]["agents"];

            if (!agents["main"].isNull()) {
                status->agents[0].label = "XH";
                status->agents[0].usagePct = parseContextPct(agents["main"]["contextUsageLine"]);
            }

            if (!agents["code"].isNull()) {
                status->agents[1].label = "CD";
                status->agents[1].usagePct = parseContextPct(agents["code"]["contextUsageLine"]);
            }

            if (!agents["todo"].isNull()) {
                status->agents[2].label = "TD";
                status->agents[2].usagePct = parseContextPct(agents["todo"]["contextUsageLine"]);
            }

            if (!agents["hardware"].isNull()) {
                status->agents[3].label = "HW";
                status->agents[3].usagePct = parseContextPct(agents["hardware"]["contextUsageLine"]);
            }

            status->dataValid = true;
            status->lastUpdate = millis();

            Serial.println("[OpenClaw] Data parsed successfully");
            Serial.printf("  Codex: AH=%d%% AW=%d%% LH=%d%% LW=%d%%\n",
                status->codex[0].pct, status->codex[1].pct,
                status->codex[2].pct, status->codex[3].pct);
            Serial.printf("  Agents: XH=%d%% CD=%d%% TD=%d%% HW=%d%%\n",
                status->agents[0].usagePct, status->agents[1].usagePct,
                status->agents[2].usagePct, status->agents[3].usagePct);
        }
    } else {
        status->dataValid = false;
        Serial.printf("[OpenClaw] HTTP request failed: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

// Color for Codex balance (green = good, yellow = warning, red = low)
// <=10% red, <=30% yellow, >30% green
uint16_t getCodexColor(uint8_t pct) {
    if (pct <= 10) return COLOR_RED;
    if (pct <= 30) return COLOR_YELLOW;
    return COLOR_GREEN;
}

// Color for Agent context usage (green = low, yellow = medium, red = high)
// <=50% green, <=70% yellow, >70% red
uint16_t getAgentColor(uint8_t pct) {
    if (pct <= 50) return COLOR_GREEN;
    if (pct <= 70) return COLOR_YELLOW;
    return COLOR_RED;
}
