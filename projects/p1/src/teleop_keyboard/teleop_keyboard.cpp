#include <teleop_keyboard/teleop_keyboard.hpp>
#include <rix/msg/geometry/Twist2DStamped.hpp>
#include <iostream>
#include <vector>

using namespace std;

TeleopKeyboard::TeleopKeyboard(std::unique_ptr<rix::ipc::interfaces::IO> input,
                               std::unique_ptr<rix::ipc::interfaces::IO> output, double linear_speed,
                               double angular_speed)
    : input(std::move(input)), output(std::move(output)), linear_speed(linear_speed), angular_speed(angular_speed) {}

void TeleopKeyboard::spin(std::unique_ptr<rix::ipc::interfaces::Notification> notif) {
    Signal s(SIGINT);
    // Fifo fifo(input, Fifo::Mode::READ);
    uint32_t seq = 0;
    while(!s.is_ready() && !notif->is_ready()){
        uint8_t c;
        ssize_t n = input->read(&c,1);
        if( n <= 0) {
            break;
        }
        char ch = static_cast<char>(c);
        
        rix::msg::geometry::Twist2DStamped msg;
        msg.header.frame_id = "mbot";
        msg.header.seq = seq++;
        msg.header.stamp = rix::util::Time::now().to_msg();
        msg.twist.vx = 0.0;
        msg.twist.vy = 0.0;
        msg.twist.wz = 0.0;

        switch (ch) {
            case 'w' : msg.twist.vx = linear_speed; break;
            case 'a' : msg.twist.vy = linear_speed; break;
            case 's' : msg.twist.vx = -linear_speed; break;
            case 'd' : msg.twist.vy = -linear_speed; break;
            case 'q' : msg.twist.wz = angular_speed; break;
            case 'e' : msg.twist.wz = -angular_speed; break;
            case ' ' : break;
            default: continue;
        }

        size_t size = msg.size();
        size_t offset = 0;
        vector<uint8_t> buffer(size);
        msg.serialize(buffer.data(), offset);

        rix::msg::standard::UInt32 msg_size;
        msg_size.data = static_cast<uint32_t>(size);
        offset = 0;
        uint8_t size_buf[4];
        msg_size.serialize(size_buf, offset);

        output->write(size_buf, 4);
        output->write(buffer.data(), buffer.size());
    }

}