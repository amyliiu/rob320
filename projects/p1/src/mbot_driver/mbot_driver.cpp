#include "mbot_driver/mbot_driver.hpp"
#include <iostream>

using namespace rix::ipc;
using namespace rix::msg;

using namespace std;

MBotDriver::MBotDriver(std::unique_ptr<interfaces::IO> input, std::unique_ptr<MBotBase> mbot)
    : input(std::move(input)), mbot(std::move(mbot)) {}

void MBotDriver::spin(std::unique_ptr<interfaces::Notification> notif) {
    Signal s(SIGINT);


    while(!s.is_ready() && !notif->is_ready()){
        uint8_t read_size[4];
        ssize_t n = input->read(read_size, 4);
        if( n<= 0) {
            break;
        }

        rix::msg::standard::UInt32 UInt_size;
        size_t offset = 0;
        UInt_size.deserialize(read_size, sizeof(read_size), offset);

        uint32_t size = UInt_size.data;
        vector<uint8_t> buffer(size);

        n = input->read(buffer.data(), size);
        
        Twist2DStamped msg;
        offset = 0;
        msg.deserialize(buffer.data(), size, offset);

        mbot->drive(msg);
    }
    mbot->drive(Twist2DStamped());
}