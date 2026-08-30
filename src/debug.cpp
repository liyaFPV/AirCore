#include <Arduino.h>

#ifdef DEV_BUILD
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_heap_caps.h"

void printDebugInfo()
{
    esp_reset_reason_t resetReason = esp_reset_reason();

    Serial.println();
    Serial.println("========================================");
    Serial.println("           ESP32 DEBUG BUILD");
    Serial.println("========================================");

    // Reset reason
    Serial.printf("[RESET] reason: %d", resetReason);

    switch (resetReason) {
        case ESP_RST_UNKNOWN:
            Serial.println(" (UNKNOWN)");
            break;

        case ESP_RST_POWERON:
            Serial.println(" (POWER ON)");
            break;

        case ESP_RST_EXT:
            Serial.println(" (EXTERNAL PIN)");
            break;

        case ESP_RST_SW:
            Serial.println(" (SOFTWARE RESET / REBOOT)");
            break;

        case ESP_RST_PANIC:
            Serial.println(" (PANIC / CRASH)");
            break;

        case ESP_RST_INT_WDT:
            Serial.println(" (INTERRUPT WATCHDOG)");
            break;

        case ESP_RST_TASK_WDT:
            Serial.println(" (TASK WATCHDOG)");
            break;

        case ESP_RST_WDT:
            Serial.println(" (WATCHDOG)");
            break;

        case ESP_RST_BROWNOUT:
            Serial.println(" (BROWNOUT / LOW VOLTAGE)");
            break;

        case ESP_RST_DEEPSLEEP:
            Serial.println(" (DEEP SLEEP)");
            break;

        case ESP_RST_SDIO:
            Serial.println(" (SDIO)");
            break;

        default:
            Serial.println(" (UNKNOWN)");
            break;
    }

    // Uptime
    Serial.printf("[TIME] uptime: %lu ms\n", millis());

    // Heap
    Serial.printf("[HEAP] free:      %u bytes\n", ESP.getFreeHeap());
    Serial.printf("[HEAP] min free:  %u bytes\n", ESP.getMinFreeHeap());
    Serial.printf("[HEAP] size:      %u bytes\n", ESP.getHeapSize());

    // Largest free block
    Serial.printf(
        "[HEAP] largest:   %u bytes\n",
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)
    );

    // CPU
    Serial.printf("[CPU] frequency:  %u MHz\n", ESP.getCpuFreqMHz());

    // Chip
    esp_chip_info_t chipInfo;
    esp_chip_info(&chipInfo);

    Serial.printf(
        "[CHIP] cores:     %d\n",
        chipInfo.cores
    );

    Serial.printf(
        "[CHIP] revision:  %d\n",
        chipInfo.revision
    );

    // Flash
    Serial.printf(
        "[FLASH] size:     %u bytes\n",
        ESP.getFlashChipSize()
    );

    Serial.printf(
        "[FLASH] speed:    %u Hz\n",
        ESP.getFlashChipSpeed()
    );

    Serial.println("========================================");
    Serial.println();
}

void debugMemory()
{
    static uint32_t lastPrint = 0;

    if (millis() - lastPrint < 5000)
        return;

    lastPrint = millis();

    Serial.printf(
        "[MEM] free=%u | min=%u | largest=%u\n",
        ESP.getFreeHeap(),
        ESP.getMinFreeHeap(),
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)
    );
}
#endif
