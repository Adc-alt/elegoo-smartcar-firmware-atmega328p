#include "command_receiver.h"

CommandReceiver::CommandReceiver(Stream& in) : in(in), bufferIndex(0)
{
  // Buffer estático - no fragmenta la memoria como String
  buffer[0] = '\0';
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
  if (strcmp(color, "cyan") == 0)
    return LedColor::CYAN;
  if (strcmp(color, "gray") == 0)
    return LedColor::GRAY;
  if (strcmp(color, "salmon") == 0)
    return LedColor::SALMON;

  return LedColor::NONE;
}

// Helper para encontrar una clave en JSON (parseo manual - sin objetos anidados)
const char* CommandReceiver::findJsonKey(const char* json, const char* key)
{
  char searchKey[32];
  snprintf(searchKey, sizeof(searchKey), "\"%s\"", key);
  const char* pos = strstr(json, searchKey);
  if (pos)
  {
    // Buscar el ':' después de la clave
    pos = strchr(pos, ':');
    if (pos)
    {
      pos++; // Saltar el ':'
      // Saltar espacios
      while (*pos == ' ' || *pos == '\t')
        pos++;
      return pos;
    }
  }
  return nullptr;
}

// Extraer valor entero desde una posición en JSON
int CommandReceiver::extractIntValue(const char* start)
{
  if (!start)
    return 0;
  return atoi(start);
}

// Extraer valor string desde una posición en JSON
const char* CommandReceiver::extractStringValue(const char* start, char* output, size_t maxLen)
{
  if (!start || *start != '"')
    return nullptr;

  start++; // Saltar la comilla inicial
  size_t i = 0;
  while (*start != '"' && *start != '\0' && i < (maxLen - 1))
  {
    output[i++] = *start++;
  }
  output[i] = '\0';
  return output;
}

bool CommandReceiver::tryReceive(CommandFrame& commandFrame)
{
  // Verificar timeout ANTES de leer - si llevamos más de 500ms recibiendo sin '\n', descartar
  // Aumentado a 500ms para dar más tiempo (el JSON puede tardar en llegar completo)
  if (processingMessage && (millis() - lastMessageTime) > 500)
  {
    Serial.println(F(">>> ERROR: Timeout recibiendo mensaje, descartando"));
    bufferIndex       = 0;
    buffer[0]         = '\0';
    processingMessage = false;
  }

  while (in.available())
  {
    char c = in.read();

    if (!processingMessage)
    {
      processingMessage = true;
      bufferIndex       = 0;
      buffer[0]         = '\0';
      lastMessageTime   = millis(); // Iniciar timeout cuando empezamos a recibir
    }

    if (c == '\n')
    {
      if (bufferIndex > 0)
      {
        buffer[bufferIndex] = '\0'; // Terminar string

        // DEBUG: Mostrar el JSON recibido
        Serial.print(F(">>> JSON recibido: "));
        Serial.println((const char*)buffer);

        // Parseo manual - buscar "type":"command" directamente en el string
        const char* typePos = findJsonKey(buffer, "type");
        if (typePos && strncmp(typePos, "\"command\"", 9) == 0)
        {
          commandFrame.lastUpdate_ms = millis();
          commandFrame.hasNewCommand = true;
          lastMessageTime            = millis();

          // Buscar "actuators" y luego parsear manualmente
          const char* actuatorsPos = findJsonKey(buffer, "actuators");
          if (actuatorsPos && *actuatorsPos == '{')
          {
            // Buscar "servo" dentro de actuators
            const char* servoPos = strstr(actuatorsPos, "\"servo\"");
            if (servoPos)
            {
              const char* anglePos = findJsonKey(servoPos, "angle");
              if (anglePos)
              {
                commandFrame.servoHasCommand = true;
                commandFrame.servoAngle      = extractIntValue(anglePos);
              }
            }

            // Buscar "motors" dentro de actuators
            const char* motorsPos = strstr(actuatorsPos, "\"motors\"");
            if (motorsPos)
            {
              // Buscar "left" dentro de motors
              const char* leftPos = strstr(motorsPos, "\"left\"");
              if (leftPos)
              {
                char actionStr[16];
                const char* actionPos = findJsonKey(leftPos, "action");
                if (actionPos && extractStringValue(actionPos, actionStr, sizeof(actionStr)))
                {
                  commandFrame.motors.leftAction = parseMotorAction(actionStr);
                }
                const char* speedPos = findJsonKey(leftPos, "speed");
                if (speedPos)
                {
                  commandFrame.motors.leftSpeed = extractIntValue(speedPos);
                }
              }

              // Buscar "right" dentro de motors
              const char* rightPos = strstr(motorsPos, "\"right\"");
              if (rightPos)
              {
                char actionStr[16];
                const char* actionPos = findJsonKey(rightPos, "action");
                if (actionPos && extractStringValue(actionPos, actionStr, sizeof(actionStr)))
                {
                  commandFrame.motors.rightAction = parseMotorAction(actionStr);
                }
                const char* speedPos = findJsonKey(rightPos, "speed");
                if (speedPos)
                {
                  commandFrame.motors.rightSpeed = extractIntValue(speedPos);
                }
              }
            }

            // Buscar "led" dentro de actuators
            const char* ledPos = strstr(actuatorsPos, "\"led\"");
            if (ledPos)
            {
              char colorStr[16];
              const char* colorPos = findJsonKey(ledPos, "color");
              if (colorPos && extractStringValue(colorPos, colorStr, sizeof(colorStr)))
              {
                commandFrame.ledHasCommand = true;
                commandFrame.ledColor      = parseLedColor(colorStr);
              }
            }
          }

          bufferIndex       = 0;
          buffer[0]         = '\0';
          processingMessage = false;
          return true;
        }
      }
      bufferIndex       = 0;
      buffer[0]         = '\0';
      processingMessage = false;
    }
    else if (c != '\r')
    {
      if (bufferIndex < (BUFFER_SIZE - 1)) // Dejar espacio para '\0'
      {
        buffer[bufferIndex++] = c;
        buffer[bufferIndex]   = '\0';
      }
      else
      {
        // Buffer lleno - el mensaje es demasiado grande
        // NO descartar, esperar a que llegue el '\n' para procesar lo que tenemos
        // O mejor: aumentar BUFFER_SIZE si esto ocurre frecuentemente
        Serial.println(F(">>> WARNING: Buffer lleno, esperando fin de mensaje..."));
        // Continuar leyendo hasta encontrar '\n' pero NO procesar
        while (in.available() && in.peek() != '\n')
        {
          in.read(); // Descartar caracteres hasta el '\n'
        }
        if (in.available() && in.peek() == '\n')
        {
          in.read(); // Descartar el '\n'
          // Resetear y esperar siguiente mensaje
          bufferIndex       = 0;
          buffer[0]         = '\0';
          processingMessage = false;
        }
        return false;
      }
    }
  }
  return false;
}

void CommandReceiver::checkTimeout(unsigned long timeoutMs)
{
  if (lastMessageTime > 0 && (millis() - lastMessageTime) > timeoutMs)
  {
    processingMessage = false;
    bufferIndex       = 0;
    buffer[0]         = '\0';
  }
}