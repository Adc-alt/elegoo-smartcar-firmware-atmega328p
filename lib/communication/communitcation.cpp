#include "communication.h"

Communication::Communication() : processingMessage(false), lastMessageTime(0), lastSentTime(0)
{
}

void Communication::begin(unsigned long baudRate)
{
  Serial.begin(baudRate);
}

bool Communication::readMessage(JsonDocument& docReceive)
{
  bool messageReceived = false;

  while (Serial.available())
  {
    char c = Serial.read();

    if (!processingMessage)
    {
      processingMessage = true;
    }

    if (c == '\n')
    {
      if (buffer.length() > 0)
      {
        DeserializationError err = deserializeJson(docReceive, buffer);

        if (err)
        {
          Serial.println(err.f_str());
        }
        else
        {
          lastMessageTime = millis();
          messageReceived = true;
        }
      }

      buffer            = "";
      processingMessage = false;
    }
    else if (c != '\r')
    {
      buffer += c;
    }
  }

  return messageReceived;
}

void Communication::sendMessage(const JsonDocument& docSend)
{
  unsigned long currentTime = millis();

  if (currentTime - lastSentTime >= INTERVAL)
  {
    lastSentTime = currentTime;

    serializeJson(docSend, Serial);
    Serial.write('\n');
  }
}

void Communication::checkTimeout()
{
  if ((millis() - lastMessageTime) > TIMEOUT_INTERVAL)
  {
    processingMessage = false;
    Serial.println("TIMEOUT");
  }
}
