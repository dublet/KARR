#ifndef HAVE_KARR_STATUS_H
#define HAVE_KARR_STATUS_H

#include <atomic>

namespace KARR {
    class Status {
	private:
	    std::atomic_ushort _speed;
	    std::atomic_uint _rpm;
	    std::atomic_uint _fuelLevel;
	    std::atomic_ushort _engineTemp;

	    static Status _instance;

	    Status();
	    Status(const Status &);
	public:
	    virtual ~Status();

	    static Status &instance();

	    const unsigned int getRpm();
	    void setRpm(unsigned int);

	    const unsigned short getSpeed();
	    void setSpeed(unsigned short);

	    const unsigned int getFuelLevel();
	    void setFuelLevel(unsigned int);

	    const unsigned short getEngineTemp();
	    void setEngineTemp(unsigned short);

    };
}
#endif
