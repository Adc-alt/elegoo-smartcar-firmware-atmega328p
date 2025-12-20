#include "command_receiver.h"

CommandReceiver::CommandReceiver(Stream& in) : in(in)
{
  buffer.reserve(256);
}

MotorAction CommandReceiver::parseMotorAction(const char* action)
{
  if (!action)
    return MotorAction::NONE;

  if (strcmp(action, "forward") == 0)
    return MotorAction::FORWARD;
  if (strcmp(action, "reverse") == 0)
    return MotorAction::REVERSE;
  if (strcmp(action, "force_stop") == 0)
    return MotorAction::FORCE_STOP;
  if (strcmp(action, "free_stop") == 0)
    return MotorAction::FREE_STOP;

  return MotorAction::NONE;
}

LedColor CommandReceiver::parseLedColor(const char* color)
{
  if (!color)
    return LedColor::NONE;

  if (strcmp(color, "black") == 0)
    return LedColor::BLACK;
  if (strcmp(color, "blue") == 0)
    return LedColor::BLUE;
  if (strcmp(color, "red") == 0)
    return LedColor::RED;
  if (strcmp(color, "yellow") == 0)
    return LedColor::YELLOW;
  if (strcmp(color, "purple") == 0)
    return LedColor::PURPLE;
  if (strcmp(color, "green") == 0)
    return LedColor::GREEN;

  return LedColor::NONE;
}

bool CommandReceiver::tryReceive(CommandFrame& commandFrame)
{
  while (in.available())
  {
    char c = in.read();

    if (!processingMessage)
    {
      processingMessage = true;
      buffer            = "";
    }

    if (c == '\n')
    {
      if (buffer.length() > 0)
      {
        StaticJsonDocument<256> doc;
        DeserializationError error = deserializeJson(doc, buffer);

        // DEBUG: Mostrar el JSON recibido (siempre visible para confirmar recepción)
        Serial.print(F(">>> JSON recibido: "));
        Serial.println(buffer);

        if (error)
        {
          Serial.print(F(">>> ERROR parseando JSON: "));
          Serial.println(error.c_str());
        }
        else if (doc["type"] == "command")
        {
#ifdef DEBUG_COMMAND_RECEIVER
          Serial.println(F("[CommandReceiver] Comando válido recibido"));
#endif

          commandFrame.lastUpdate_ms = millis();
          commandFrame.hasNewCommand = true;
          lastMessageTime            = millis();

          // Parsear actuadores
          if (doc.containsKey("actuators"))
          {
            JsonObject actuators = doc["actuators"];

            // Motores
            if (actuators.containsKey("motors"))
            {
              JsonObject motors = actuators["motors"];

              if (motors.containsKey("left"))
              {
                JsonObject left                = motors["left"];
                const char* action             = left["action"];
                commandFrame.motors.leftAction = parseMotorAction(action);
                commandFrame.motors.leftSpeed  = left["speed"] | 0;
#ifdef DEBUG_COMMAND_RECEIVER
                Serial.print(F("[CommandReceiver] Motor izquierdo: "));
                Serial.print(action);
                Serial.print(F(" speed: "));
                Serial.println(commandFrame.motors.leftSpeed);
#endif
              }

              if (motors.containsKey("right"))
              {
                JsonObject right                = motors["right"];
                const char* action              = right["action"];
                commandFrame.motors.rightAction = parseMotorAction(action);
                commandFrame.motors.rightSpeed  = right["speed"] | 0;
#ifdef DEBUG_COMMAND_RECEIVER
                Serial.print(F("[CommandReceiver] Motor derecho: "));
                Serial.print(action);
                Serial.print(F(" speed: "));
                Serial.println(commandFrame.motors.rightSpeed);
#endif
              }
            }

            // Servo
            if (actuators.containsKey("servo"))
            {
              JsonObject servo             = actuators["servo"];
              commandFrame.servoHasCommand = true;
              commandFrame.servoAngle      = servo["angle"] | 90;
#ifdef DEBUG_COMMAND_RECEIVER
              Serial.print(F("[CommandReceiver] Servo: "));
              Serial.println(commandFrame.servoAngle);
#endif
            }

            // LED
            if (actuators.containsKey("led"))
            {
              JsonObject led             = actuators["led"];
              const char* color          = led["color"];
              commandFrame.ledHasCommand = true;
              commandFrame.ledColor      = parseLedColor(color);
#ifdef DEBUG_COMMAND_RECEIVER
              Serial.print(F("[CommandReceiver] LED: "));
              Serial.println(color);
#endif
            }
          }

          buffer            = "";
          processingMessage = false;
          return true;
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
  return false;
}

void CommandReceiver::checkTimeout(unsigned long timeoutMs)
{
  if (lastMessageTime > 0 && (millis() - lastMessageTime) > timeoutMs)
  {
    processingMessage = false;
    buffer            = "";
  }
}