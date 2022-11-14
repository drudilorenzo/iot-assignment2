#include <Arduino.h>

#include <components.h>
#include <scheduling.h>
#include <utils.h>
#include <consts.h>

Components::LCD* lcd;
Components::Led* led_a;
Components::Led* led_b;
Components::Led* led_c;
Components::Button* btn;
Components::Pir* pir;
Components::LightSensor* light_sensor;
Components::Sonar* sonar;
Components::Motor* valve;
Components::Potentiometer* pot;

Tasks::BlinkLed* blink_led;
Tasks::SmartLight* smart_light;
Tasks::SmartBridge* smart_bridge;
Tasks::PrintDebug<bool>* print_bool = new Tasks::PrintDebug<bool>(1000);
Tasks::PrintDebug<float>* print_float = new Tasks::PrintDebug<float>(1000);
Tasks::PrintDebug<uint16_t>* print_uint = new Tasks::PrintDebug<uint16_t>(1000);
Tasks::PrintDebug<ButtonState>* print_btn = new Tasks::PrintDebug<ButtonState>(1000);

Scheduling::Scheduler* scheduler = new Scheduling::Scheduler();

void setup() {
    Serial.begin(9600);
    
    lcd = new Components::LCD(2, 16, 0x27);
    led_a = new Components::Led(LED_A_PIN);
    led_b = new Components::Led(LED_B_PIN);
    led_c = new Components::Led(LED_C_PIN);
    btn = new Components::Button(BUTTON_PIN);
    pir = new Components::Pir(PIR_PIN);
    valve = new Components::Motor(SERVO_PIN);
    light_sensor = new Components::LightSensor(LIGHT_SENSOR_PIN);
    sonar = new Components::Sonar(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN);
    pot = new Components::Potentiometer(POT_PIN, 0, 180);

    smart_light = new Tasks::SmartLight(led_a, light_sensor, pir, 10);
    blink_led = new Tasks::BlinkLed(led_b, BLINKING_PERIOD);
    smart_bridge = new Tasks::SmartBridge(
        lcd,
        led_b,
        valve,
        sonar,
        btn,
        pot,
        blink_led,
        smart_light
    );

    scheduler->schedule(print_float);
    // scheduler->schedule(smart_light);
    // scheduler->schedule(smart_bridge);

    print_float->addComponent("Sonar", sonar);

    Logger::Logger::getInstance().setLevel(Logger::LogLevel::Info);
}

void loop() {
    scheduler->tick();
}