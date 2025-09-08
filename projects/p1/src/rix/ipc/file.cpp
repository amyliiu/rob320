#include "rix/ipc/file.hpp"

namespace rix
{
    namespace ipc
    {
        /**< TODO */
        bool File::remove(const std::string &pathname)
        {
            return ::unlink(pathname.c_str()) == 0;
        }

        File::File() : fd_(-1) {}

        File::File(int fd) : fd_(fd) {}

        /**< TODO */
        File::File(std::string pathname, int creation_flags, mode_t mode)
        {
            fd_ = ::open(pathname.c_str(), creation_flags, mode);
        }

        /**< TODO */
        File::File(const File &src)
        {
            fd_ = ::dup(src.fd_);
        }

        /**< TODO */
        File &File::operator=(const File &src)
        {
            if (this != &src)
            {
                if (fd_ != -1)
                {
                    ::close(fd_);
                }
                fd_ = ::dup(src.fd_);
            }
            return *this;
            return *this;
        }

        File::File(File &&src) : fd_(-1) { std::swap(fd_, src.fd_); }

        File &File::operator=(File &&src)
        {
            if (fd_ > 0)
            {
                ::close(fd_);
                fd_ = -1;
            }
            std::swap(fd_, src.fd_);
            return *this;
        }

        /**< TODO */
        File::~File()
        {
            if (fd_ != -1)
            {
                ::close(fd_);
            }
        }

        /**< TODO */
        ssize_t File::read(uint8_t *buffer, size_t size) const
        {
            return ::read(fd_, buffer, size);
        }

        /**< TODO */
        ssize_t File::write(const uint8_t *buffer, size_t size) const
        {
            return ::write(fd_, buffer, size);
        }

        int File::fd() const { return fd_; }

        /**< TODO */
        bool File::ok() const
        {
            return fd_ != -1;
        }

        /**< TODO */
        void File::set_nonblocking(bool status)
        {
            int flags = ::fcntl(fd_, F_GETFL, 0);
            if (flags == -1)
            {
                return;
            }
            if (status)
            {
                flags |= O_NONBLOCK;
            }
            else
            {
                flags &= ~O_NONBLOCK;
            }
            ::fcntl(fd_, F_SETFL, flags);
        }
        /**< TODO */
        bool File::is_nonblocking() const
        {
            int flags = ::fcntl(fd_, F_GETFL, 0);
            return (flags & O_NONBLOCK) != 0;
        }

        /**< TODO */
        bool File::wait_for_writable(const util::Duration &duration) const
        {
            if (fd_ == -1)
            {
                return false;
            }

            struct pollfd pfd;
            pfd.fd = fd_;
            pfd.events = POLLOUT;
            int timeout_ms = static_cast<int>(duration.to_milliseconds());

            int ret = ::poll(&pfd, 1, timeout_ms);
            if (ret > 0 && (pfd.revents & POLLOUT))
            {
                return true;
            }
            return false;
        }

        /**< TODO */
        bool File::wait_for_readable(const util::Duration &duration) const
        {
            if (fd_ == -1)
            {
                return false;
            }

            struct pollfd pfd;
            pfd.fd = fd_;
            pfd.events = POLLIN;
            int timeout_ms = static_cast<int>(duration.to_milliseconds());

            int ret = ::poll(&pfd, 1, timeout_ms);
            if (ret > 0 && (pfd.revents & POLLIN))
            {
                return true;
            }
            return false;
        }

    } // namespace ipc
} // namespace rix