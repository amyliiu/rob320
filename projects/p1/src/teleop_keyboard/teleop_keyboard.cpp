#include <teleop_keyboard/teleop_keyboard.hpp>
#include <rix/msg/geometry/Twist2DStamped.hpp>

TeleopKeyboard::TeleopKeyboard(std::unique_ptr<rix::ipc::interfaces::IO> input,
                               std::unique_ptr<rix::ipc::interfaces::IO> output, double linear_speed,
                               double angular_speed)
    : input(std::move(input)), output(std::move(output)), linear_speed(linear_speed), angular_speed(angular_speed) {}

void TeleopKeyboard::spin(std::unique_ptr<rix::ipc::interfaces::Notification> notif) {
    Signal s(SIGINT);
    Fifo fifo("teleop", Fifo::Mode::READ);
    File f(STDOUT_FILENO);

    uint32_t seq = 0;
    while(!s.is_ready() && !notif->is_ready()){
        uint8_t c;
        fifo.read(&c,1);
        char ch = static_cast<char>(c);
        
        rix::msg::geometry::Twist2DStamped msg;
        msg.header.frame_id = "mbot";
        msg.header.seq = seq++;
        // msg.header.stamp = rix::util::Time::now();
        // msg.header.stamp.set(rix::util::Time::now());

        msg.twist.vx = 0.0;
        msg.twist.vy = 0.0;
        msg.twist.wz = 0.0;

        switch (c) {
            case 'W' : msg.twist.vx = linear_speed; break;
            case 'A' : msg.twist.vy = linear_speed; break;
            case 'S' : msg.twist.vx = -linear_speed; break;
            case 'D' : msg.twist.vy = -linear_speed; break;
            case 'Q' : msg.twist.wz = angular_speed; break;
            case 'E' : msg.twist.wz = -angular_speed; break;
            case ' ' : break;
            default: continue;
        }

        size_t size = msg.size();
        size_t offset = 0;
        vector<uint8_t> buffer(size);
        msg.serialize(buffer.data(), offset);

        uint32_t size32 = static_cast<uint32_t>(size);
        f.write(reinterpret_cast<const uint8_t*>(&size32), sizeof(size32));


        f.write(buffer.data(), buffer.size());

    }

}