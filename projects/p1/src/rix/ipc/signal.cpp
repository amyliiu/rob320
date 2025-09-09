#include "rix/ipc/signal.hpp"

#include <iostream>
#include <stdexcept>

using namespace std;

namespace rix {
namespace ipc {

std::array<Signal::SignalNotifier, 32> Signal::notifier = {};

/**< TODO */
Signal::Signal(int signum) : signum_(signum - 1) {
    if (signum < 1 || signum > 32) {
        throw invalid_argument("signum must be between 1 and 32");
    }
    if (notifier[signum_].is_init == true) {
        throw invalid_argument("signal already has a notifier");
    }
   notifier[signum_].is_init = true;
   notifier[signum_].pipe = Pipe::create();
   ::signal(signum_+1, handler);
}

/**< TODO */
Signal::~Signal() {
    if(signum_ >= 0 and notifier[signum_].is_init) {
        ::signal(signum_ + 1, SIG_DFL);
        notifier[signum_] = SignalNotifier{};
    }
}

Signal::Signal(Signal &&other) : signum_(-1) { std::swap(signum_, other.signum_); }

Signal &Signal::operator=(Signal &&other) {
    if (signum_ >= 0 && signum_ < 32) {
        if (notifier[signum_].is_init) {
            notifier[signum_].is_init = false;
            notifier[signum_].pipe = {};
            ::signal(signum_+1, SIG_DFL);
        }
    }
    signum_ = -1;
    std::swap(signum_, other.signum_);
    return *this;
}

/**< TODO */
bool Signal::raise() const { 
    if(notifier[signum_].is_init == false){
        return false;
    }
    return (::raise(signum_+1) == 0);
}

/**< TODO */
bool Signal::kill(pid_t pid) const {
    if(notifier[signum_].is_init == false){
        return false;
    }
    return (::kill(pid, signum_+1) == 0);
}

int Signal::signum() const { 
    if(signum_ < 0 || signum_ > 31) {
        return -1;
    }
    return signum_ + 1;
}

/**< TODO */
bool Signal::wait(const rix::util::Duration &d) const {
    if(notifier[signum_].is_init == false){
        return false;
    }
    if( notifier[signum_].pipe[0].wait_for_readable(d) == false){
        return false;
    }
    char buf;
    ::read(notifier[signum_].pipe[0].fd(), &buf, 1);
    return true;
    
}

/**< TODO */
void Signal::handler(int signum) {
    if(notifier[signum-1].is_init == false) {
        return;
    }
    char c = 1;
    ::write(notifier[signum-1].pipe[1].fd(), &c, 1 );
}

}  // namespace ipc
}  // namespace rixsi