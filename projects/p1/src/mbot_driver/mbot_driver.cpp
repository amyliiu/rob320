#include "mbot_driver/mbot_driver.hpp"
#include <iostream>

using namespace rix::ipc;
using namespace rix::msg;

using namespace std;

MBotDriver::MBotDriver(std::unique_ptr<interfaces::IO> input, std::unique_ptr<MBotBase> mbot)
    : input(std::move(input)), mbot(std::move(mbot)) {}

void MBotDriver::spin(std::unique_ptr<interfaces::Notification> notif) {
    Signal s(SIGINT);
    File f(STDIN_FILENO);

    while(!s.is_ready() && !notif->is_ready()){
        uint8_t read_size[4];
        ssize_t n = f.read(read_size, 4);
        if(n != 4){
            break;
        }
        cout << "here";

        rix::msg::standard::UInt32 UInt_size;
        size_t offset = 0;
        UInt_size.deserialize(read_size, sizeof(read_size), offset);

        uint32_t size = UInt_size.data;
        
        vector<uint8_t> buffer(size);
        n = f.read(buffer.data(), size);
        if (n != static_cast<ssize_t>(size)) {
            break;
        }
        Twist2DStamped msg;
        offset = 0;
        msg.deserialize(buffer.data(), size, offset);
        // offset += sizeof(size) + size;

        mbot->drive(msg);
    }
    mbot->drive(Twist2DStamped());

}