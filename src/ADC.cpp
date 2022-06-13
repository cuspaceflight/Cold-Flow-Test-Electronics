#include "../include/ADC.hpp"


ADC::ADC(int bus, int addr, int freq) : freq(freq), values(), is_alive(true) {
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", bus);
    this->file = open(filename, O_RDWR);

    if (this->file < 0) {
        std::cerr << "could not open i2c bus " << bus << std::endl;
        exit(1);
    }

    if (ioctl(this->file, I2C_SLAVE, addr) < 0) {
        std::cerr << "could not communicate with i2c addr " << addr << " on bus " << bus << std::endl;
        exit(1);
    }

    this->thread_obj = new std::thread(&ADC::loop, this);
}

int ADC::recv() {
    char buf[3];
    for (int i = 0; i < 4; i++) {
        buf[0] = 0x01;
        buf[1] = 0x42 + (i << 4);
        buf[2] = 0x83;
        write(this->file, buf, 3);
        buf[0] = 0x00;
        write(this->file, buf ,1);
        read(this->file, buf, 2);
        int out = (buf[0]<<4)+buf[1];
        // TODO try using SMBus instead of i2c-dev (the file commands on /dev/i2c-%d)
        // TODO need a decoding function for each sensor
        this->values[i] = (float)out;
    }
    read(file, buf, 0);
    return 0;
}

void ADC::loop() {
    while (this->is_alive) {
        this->recv();
        std::this_thread::sleep_for(std::chrono::microseconds((const int) 1e6 / this->freq));
    }
    close(this->file);
}
