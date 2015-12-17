#ifndef _UTILS_H_
#define _UTILS_H_

#include "Types.h"

/*!
 * @brief Sets a bit in a byte. If the bit is 1, it doesn't change.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be set
 */
inline bool isBitSet(const byte value, const unsigned short position) {
    bool resul = false;
    if ( (value & ( 1 << position )) != 0 ) {
        resul = true;
    }
    return resul;
}


inline byte getBitValue(const byte inData, const unsigned short position) {
    byte lMsk = 1 << position ;
    return ( inData & lMsk ) ? 1 : 0 ;
}

/*!
 * @brief Sets a bit in a byte. If the bit is 1, it doesn't change.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be set
 */
inline void setBit(byte *value, const unsigned short position) {
    byte mask = 1 << position;
    *value = *value | mask;
}

/*!
 * @brief Toggle a bit in a byte. If the bit is 1, it changes to 0, if it is 0, then it changes to 1.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be toggle
 */
inline void toggleBit(byte *value, const unsigned short position) {
    *value = *value ^ (1 << position);
}

/*!
 * @brief Clear a bit in a byte. The bit's value will be always 0 after this operation.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be set
 */
inline void clearBit(byte *value, const unsigned short position) {
    *value = *value & ~(1 << position);
}

#endif /*_UTILS_H_*/
