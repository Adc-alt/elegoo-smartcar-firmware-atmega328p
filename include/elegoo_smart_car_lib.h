// CRITERIO USADO SCREAMING_SNAKE_CASE
// CriterioCamelCase
// CriterioPascalCase
// CriterioSnakeCase
// CriterioKebabCase

// Clases      PascalCase
// Funciones   CamelCase
// Carpetas    Snake_Case
// Archivos    Snake_Case

#define USB_SPEED 9600

// Definiciones para librería led
#define NUM_LEDS 1    // Número de LEDs en la tira
#define BRIGHTNESS 10 // Brillo del LED (0-255)

// UART
#define UART_RX 1
#define UART_TX 0

// Switch
#define SWITCH_PIN 2

// RGB LED
#define RGB_PIN 4 // Pin RGB

// MOTOR
#define STBY 3       // Pin Motor TB6612FNG Standby //Raro porque es PWM
#define RIGHT_PWM 5  // Pin Motor PWM Right Wheel Section
#define LEFT_PWM 6   // Pin Motor PWM Left Wheel Section
#define M_14_RIGHT 7 // Pin Motor Drive Right Wheel Section
#define M_23_LEFT 8  // Pin Motor Drive Left Wheel Section

// IR SENSOR
#define IR_PIN 9 // Pin Infrarojo para recibir del sensor

// SENSOR SERVO
#define SERVO_PIN 10
// #define SERVO_2       11 Es por si queremos conectar un servo adicional
#define ECHO_PIN 12
#define TRIG_PIN 13

// IR SENSOR
#define LINE_RIGHT_PIN A0
#define LINE_MIDDLE_PIN A1
#define LINE_LEFT_PIN A2

// PARA HALLAR EL VOLTAGE
#define BATTERY_VOLTAGE_PIN A3

// I2C PARA MPU
#define SCL_PIN A4
#define SDA_PIN A5
