#include "rix/ipc/fifo.hpp"

namespace rix {
namespace ipc {

/**< TODO */
Fifo::Fifo(const std::string &pathname, Mode mode, bool nonblocking) {
    mode_ = mode;
    pathname_ = pathname;
    ::mkfifo(pathname.c_str(), S_IRUSR | S_IWUSR);
    int flags;
    if (mode == Mode::READ){
        flags = O_RDONLY;
    }
    else {
        flags = O_WRONLY;
    }
    if (nonblocking) {
        flags |= O_NONBLOCK;
    }
    fd_ = ::open(pathname.c_str(), flags);
}

Fifo::Fifo() {}

/**< TODO */
Fifo::Fifo(const Fifo &other) {
    mode_ = other.mode_;
    pathname_ = other.pathname_;
    fd_ = ::dup(other.fd_);
}

/**< TODO */
Fifo &Fifo::operator=(const Fifo &other) {
    if(this != &other){
        ::close(fd_);
        mode_ = other.mode_;
        pathname_ = other.pathname_;

        if(other.fd_ != -1) {
            fd_ = ::dup(other.fd_);
        }
    }
    return *this;
}

Fifo::Fifo(Fifo &&other)
    : File(std::move(other)),
      pathname_(std::move(other.pathname_)),
      mode_(std::move(other.mode_)) {
}

Fifo &Fifo::operator=(Fifo &&other) {
    if (fd_ > 0) {
        ::close(fd_);
        fd_ = -1;
        pathname_ = "";
    }
    std::swap(fd_, other.fd_);
    pathname_ = std::move(other.pathname_);
    mode_ = std::move(other.mode_);
    return *this;
}

Fifo::~Fifo() {}

std::string Fifo::pathname() const { return pathname_; }

Fifo::Mode Fifo::mode() const { return mode_; }

}  // namespace ipc
}  // namespace rix