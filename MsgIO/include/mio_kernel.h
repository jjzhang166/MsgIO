#pragma once

#include "mio_time.h"

namespace mio {
static const short EVKERNEL_READ;
static const short EVKERNEL_WRITE;

class kernel {
private: //Disable copy and assignment
    kernel(const kernel&);         
    void operator=(const kernel&);

public:
	kernel();
	~kernel();

	size_t max() const;


	class event {
    public:
		int ident() const;
	};


	int add_fd(int fd, short event);
	int remove_fd(int fd, short event);


// 	class timer {
//     private: //Disable copy and assignment
//         timer(const timer&);         
//         void operator=(const timer&);
// 
// 	public:
// 		timer();
// 		~timer();
// 		int ident() const;
// 
//     private:
//         int _fd;
//         friend class kernel;
// 	};
// 
// 	int add_timer(timer* tm, const timespec* value, const timespec* interval);
// 	int remove_timer(int ident);
// 	static int read_timer(event e);

	int add_kernel(kernel* pt);
	int ident() const;

	class backlog {
	public:
		backlog();
		~backlog();
		event operator[] (int n) const;
	private:
		backlog(const backlog&);
	};

	int wait(backlog* result);
	int wait(backlog* result, int timeout_msec);

	int reactivate(event e);
	int remove(event e);

private:
    int _fd;
};
} //namespace mio