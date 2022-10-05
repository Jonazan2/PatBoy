#ifndef _UTILS_H_
#define _UTILS_H_

#include "Types.h"

/*!
 * @brief Check if a bit is 1 inside a byte for a specific position
 * @param value byte that will be check
 * @param position Position of the bit that is going to be check
 */
inline bool isBitSet(const byte value, const unsigned short position) {
	return (value & (1 << position)) != 0;
}

/*!
 * @brief Gets a bit inside a byte for a specific position
 * @param value byte value
 * @param position Position of the bit that is going to be get
 */
inline byte getBitValue(const byte inData, const unsigned short position) {
	return (inData >> position) & 1;
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
