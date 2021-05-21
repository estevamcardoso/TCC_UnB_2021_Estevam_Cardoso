#ifndef _SAVE_SAMPLES_H
#define _SAVE_SAMPLES_H

#include <LITTLEFS.h>
#include "interrupt_handler.h"
#include "var_handler.h"

// format and initialize FS
bool FS_init()
{
    if (!LITTLEFS.format())
    {
#if DEBUG == true
        Serial.println("LittleFS format failed");
#endif
        return false;
    }
#if DEBUG == true
    Serial.println("File System formated!");
#endif
    if (!LITTLEFS.begin(true))
    { // format if fail true
#if DEBUG == true
        Serial.println("LITTLEFS Mount Failed");
#endif
        return false;
    }
#if DEBUG == true
    Serial.print("Total bytes in memory: ");
    Serial.println(LITTLEFS.totalBytes());
    Serial.print("Total bytes used: ");
    Serial.println(LITTLEFS.usedBytes());
#endif

    //LITTLEFS.end();
    return true;
}

// Save samples in file with base 64 to save space in flash and email
void FS_save_samples(volatile uint16_t *samples)
{

    snprintf(file_name, 10, "/%d.txt", file_count);
#if DEBUG == true
    Serial.printf("Creating file: %s\n", file_name);
#endif

    File file = LITTLEFS.open(file_name, FILE_APPEND);
    for (int i = 0; i < SAMPLE_AMOUNT; i++)
    {
        if (samples[i] < 0)
            samples[i] = 0;
        file.print(samples[i], 10);
        //file.print(samples[i],64);
        file.print(" ");
        //appendNumberFile(LITTLEFS, file_name, samples[i], 64);
    }
    file.close();
#if DEBUG == true
    Serial.printf("Done file: %s\n", file_name);
#endif
    file_count++;
}

#endif