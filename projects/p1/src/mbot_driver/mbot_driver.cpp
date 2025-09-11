#include "mbot_driver/mbot_driver.hpp"

using namespace rix::ipc;
using namespace rix::msg;

using namespace std;

MBotDriver::MBotDriver(std::unique_ptr<interfaces::IO> input, std::unique_ptr<MBotBase> mbot)
    : input(std::move(input)), mbot(std::move(mbot)) {}

void MBotDriver::spin(std::unique_ptr<interfaces::Notification> notif) {
    Signal s(SIGINT);

    File f(STDIN_FILENO);

    size_t offset = 0;
    while(s.is_ready()){
        uint8_t size;
        f.read(&size, 4);
        
        uint8_t buffer;
        f.read(&buffer, size);

        Twist2DStamped msg;
        msg.deserialize(&buffer, size, offset);

        mbot->drive(msg);
    }
    mbot->drive(0)

}