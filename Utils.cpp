#include "Utils.h"

Utils* Utils::instance = 0;

Utils* Utils::getInstance() {
    if ( !instance ) {
        instance = new Utils();
    }
    return instance;
}

/*!
 * @brief Sets a bit in a byte. If the bit is 1, it doesn't change.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be set
 */
bool Utils::isBitSet(const byte value, const short position) const {
    bool resul = false;
    if ( (value & ( 1 << position )) != 0 ) {
        resul = true;
    }
    return resul;
}

bool Utils::testBit( byte inData, short inBitPosition ){
	byte lMsk = 1 << inBitPosition ;
	return ( inData & lMsk ) ? true : false ;
}

byte Utils::getBitValue(byte inData, short inBitPosition) {
    byte lMsk = 1 << inBitPosition ;
	return ( inData & lMsk ) ? 1 : 0 ;
}

/*!
* @brief Sets a bit in a byte. If the bit is 1, it doesn't change. 
* @param *value pointer to the byte that is going to be changed
* @param position Position of the bit that is going to be set
*/
void Utils::setBit(byte *value, const short position) {
    byte mask = 1 << position;
    *value = *value | mask;
}

/*!
 * @brief Toggle a bit in a byte. If the bit is 1, it changes to 0, if it is 0, then it changes to 1.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be toggle
 */
void Utils::toggleBit(byte *value, const short position) {
    *value = *value ^ (1 << position);
}

/*!
 * @brief Clear a bit in a byte. The bit's value will be always 0 after this operation.
 * @param *value pointer to the byte that is going to be changed
 * @param position Position of the bit that is going to be set
 */
void Utils::clearBit(byte *value, const short position) {
    *value = *value & ~(1 << position);
}
