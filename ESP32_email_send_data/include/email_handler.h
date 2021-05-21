#ifndef _EMAIL_HANDLER_H
#define _EMAIL_HANDLER_H

#include "Arduino.h"
#include <EMailSender.h>
#include <WiFi.h>
#include <LITTLEFS.h>
#include "var_handler.h"

const char *ssid = "Your_Wifi";
const char *password = "Your_Wifi_password";

uint8_t connection_state = 0;
uint16_t reconnect_interval = 10000;

EMailSender emailSend("Your_Email", "Your_Email_password");

bool connect_wifi()
{
// WIFI -----------------------
#if DEBUG == true
    Serial.print("Connecting to ");
#endif
    if (ssid)
    {
        WiFi.begin(ssid, password);
#if DEBUG == true
        Serial.println(ssid);
#endif
    }
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 50)
    {
        delay(200);
#if DEBUG == true
        Serial.print(".");
#endif
    }
#if DEBUG == true
    Serial.println("");
#endif
    if (i == 51)
    {
#if DEBUG == true
        Serial.print("Connection: TIMEOUT on attempt: ");
#endif
        return false;
    }
#if DEBUG == true
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
#endif
    return true;
}

bool connect_and_send()
{
    // WIFI -------------------------
    if (!connect_wifi())
        return false;
#if DEBUG == true
    // FILES ------------------------
    // Looking for the files in the FS
    Serial.println("ReadDir");
    File dir = LITTLEFS.open("/");
    File file = dir.openNextFile();
    while (file)
    {
        Serial.print(file.name());
        Serial.println(file.size());
        file = dir.openNextFile();
    }
#endif

    // EMAIL ------------------------
    EMailSender::EMailMessage message;
    message.subject = "Email_subject";
    message.message = "Email_message";

    EMailSender::FileDescriptior fileDescriptor[FILES_PER_EMAIL];
    for (uint8_t i = 0; i < FILES_PER_EMAIL; i++)
    {
        snprintf(file_name, 10, "/%d.txt", file_to_send);
        file_to_send++;
        fileDescriptor[i].filename = F(file_name);
        fileDescriptor[i].url = F(file_name);
        fileDescriptor[i].mime = MIME_TEXT_PLAIN;
        fileDescriptor[i].storageType = EMailSender::EMAIL_STORAGE_TYPE_LITTLE_FS;
    }

    EMailSender::Attachments attachs = {FILES_PER_EMAIL, fileDescriptor};

    EMailSender::Response resp = emailSend.send("Receiver_Email", message, attachs);
#if DEBUG == true
    Serial.println("Sending status: ");
    Serial.println(resp.status);
    Serial.println(resp.code);
    Serial.println(resp.desc);
#endif
    // Disconnecting and turning WiFi off
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    return true;
}

#endif