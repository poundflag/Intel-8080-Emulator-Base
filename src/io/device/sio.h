#include "../model/iodevice.h"
#include <ncurses.h>

#ifndef __SIO_H__
#define __SIO_H__

class SIO : public IODevice {
private:
  int index = 0;
  std::string test = "\r\r\rY\rPRINT \"HELLO WORLD\"\r\rPRINT 1+1\r";

public:
  SIO(uint8_t ports[], int size);
  void setValue(uint8_t value, uint8_t portAddress) override;
  uint8_t getValue(uint8_t portAddress) override;
};

#endif // __SIO_H__