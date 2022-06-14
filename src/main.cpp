#include <string>

#include "State.hpp"
#include "Display.hpp"
#include "Relay.hpp"
#include "PT.hpp"
#include "Logger.hpp"
#include "defines.hpp"
#include "LoadCell.hpp"
#include "ADC.hpp"
#include "Servo.hpp"

int main() {
    auto PTs = std::make_shared<std::vector<PT*>>();

    PTs->push_back(new PT(1, M32JM_ADDR, SAMPLING_FREQ));
    PTs->push_back(new PT(3, M32JM_ADDR, SAMPLING_FREQ));
    PTs->push_back(new PT(4, M32JM_ADDR, SAMPLING_FREQ));

    auto LCs = std::make_shared<std::vector<LoadCell*>>();

    LCs->push_back(new LoadCell(7,25));

    auto ADCs = std::make_shared<std::vector<ADC*>>();

    ADCs->push_back(new ADC(1,ADC0_ADDR,300));
    ADCs->push_back(new ADC(3,ADC1_ADDR,300));

    auto relays = std::make_shared<Relay>();
    auto servos = std::make_shared<Servo>(4, 40);
    auto machine = std::make_shared<StateMachine>(relays, servos);
    auto logger = std::make_shared<Logger>(machine, relays, PTs);
    auto display = std::make_shared<Display>(machine, relays, PTs, LCs, ADCs, logger);

    if ((*LCs)[0]->init()) {
        machine->update('o');
    } else {
        machine->update('e');
        display->write_error("Check load cell");
    }

    while (display->open) {
        display->update();
        machine->process();
    }
}
