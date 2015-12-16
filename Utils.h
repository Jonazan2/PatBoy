#ifndef _UTILS_H_
#define _UTILS_H_

#include "Types.h"

/**************************************************************************//**
* \brief Singleton class that has methods related to bit operations.
******************************************************************************/

class Utils{

private:
    Utils(){};
    static Utils* instance;

public:
    static Utils* getInstance();
    
    bool isBitSet(const byte, const short) const;
    void setBit(byte*, const short);
    void toggleBit(byte*, const short);
    void clearBit(byte *value, const short position);
    bool testBit(byte, short);
    byte getBitValue(byte, short);
};

#endif