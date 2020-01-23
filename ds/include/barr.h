#ifndef BARRH
#define BARRH

/* BIT ARRAY */

/*******************************************************************************
                            FUNCTION: BARR SET BIT
DESCRIPTION:
    - Returns new bit array with a the given bit set to a given value.
PARAMETERS: 
    - Bit array to modify.
    - Number of bit to modify (bit 1 is the rightmost bit).
    - The value of the modified bit.
RETURN VALUE: 
    - Returns a bit array with the given bit changed. 
*******************************************************************************/
size_t BARRSetBit(size_t arr, int index, int value);

/*******************************************************************************
                            FUNCTION: BARR SET ON
DESCRIPTION:
    - Returns new bit array with a the given bit set to 1.
PARAMETERS: 
    - Bit array to modify.
    - Number of bit to set (bit 1 is the rightmost bit).
RETURN VALUE: 
    - Returns a bit array with the given bit set to one. 
*******************************************************************************/
size_t BARRSetOn(size_t arr, int index);

/*******************************************************************************
                            FUNCTION: BARR SET OFF
DESCRIPTION:
    - Returns new bit array with a the given bit set to zero.
PARAMETERS: 
    - Bit array to modify.
    - Number of bit to set (bit 1 is the rightmost bit).
RETURN VALUE: 
    - Returns a bit array with the given bit set to zero. 
*******************************************************************************/
size_t BARRSetOff(size_t arr, int index);

/*******************************************************************************
                            FUNCTION: BARR IS ON
DESCRIPTION:
    - Checks wheter a given bit in a bit array is set.
PARAMETERS: 
    - Bit array to check.
    - Number of bit to check (bit 1 is the rightmost bit).
RETURN VALUE: 
    - Returns zero if the bit is set to 0, a non zero value otherwise.
*******************************************************************************/
int BARRIsOn(size_t arr, int index);

/*******************************************************************************
                            FUNCTION: BARR IS OFF
DESCRIPTION:
    - Checks wheter a given bit in a bit array is not set.
PARAMETERS: 
    - Bit array to check.
    - Number of bit to check (bit 1 is the rightmost bit).
RETURN VALUE: 
    - Returns zero if the bit is set to 1, a non zero value otherwise.
*******************************************************************************/
int BARRIsOff(size_t arr, int index);

/*******************************************************************************
                            FUNCTION: BARR FLIP
DESCRIPTION:
    - Flips a bit in a bit array.
PARAMETERS: 
    - Bit array to modify.
    - Number of bit to modify (bit 1 is the rightmost bit).
RETURN VALUE: 
    - Returns bit array with the given bit flipped.
*******************************************************************************/
size_t BARRFlip(size_t arr, int index);

/*******************************************************************************
                            FUNCTION: BARR ROTATE LEFT
DESCRIPTION:
    - Rotates all bits in a bit array a given amount of bits left. 
PARAMETERS: 
    - Bit array to modify.
    - Number of bits to rotate by.
RETURN VALUE: 
    - Returns a rotated bit array.
*******************************************************************************/
size_t BARRRotateLeft(size_t arr, size_t amount);

/*******************************************************************************
                            FUNCTION: BARR ROTATE RIGHT
DESCRIPTION:
    - Rotates all bits in a bit array a given amount of bits right. 
PARAMETERS: 
    - Bit array to modify.
    - Number of bits to rotate by.
RETURN VALUE: 
    - Returns a rotated bit array.
*******************************************************************************/
size_t BARRRotateRight(size_t arr, size_t amount);

/*******************************************************************************
                            FUNCTION: BARR MIRROR
DESCRIPTION:
    - Mirrors all bits in a given bit array.
PARAMETERS: 
    - Bit array to modify.
RETURN VALUE: 
    - Returns a mirrored bit array.
*******************************************************************************/
size_t BARRMirror(size_t arr);

/*******************************************************************************
                            FUNCTION: BARR COUNT ON
DESCRIPTION:
    - Count the number of set bits in a bit array.
PARAMETERS: 
    - Bit array.
RETURN VALUE: 
    - Returns the number of set bits.
*******************************************************************************/
size_t BARRCountOn(size_t arr);

/*******************************************************************************
                            FUNCTION: BARR COUNT OFF
DESCRIPTION:
    - Count the number of unset bits in a bit array.
PARAMETERS: 
    - Bit array.
RETURN VALUE: 
    - Returns the number of unset bits.
*******************************************************************************/
size_t BARRCountOff(size_t arr);

/*******************************************************************************
                            FUNCTION: BARR COUNT (2)
DESCRIPTION:
    - Count the number of set bits in a bit array (alternative implementation).
PARAMETERS: 
    - Bit array.
RETURN VALUE: 
    - Returns the number of set bits.
*******************************************************************************/
size_t BARRCountOnLUT(size_t arr);

/*******************************************************************************
                            FUNCTION: BARR MIRROR 
DESCRIPTION:
    - Mirrors all bits in a given bit array (alternative implementation)..
PARAMETERS: 
    - Bit array to modify.
RETURN VALUE: 
    - Returns a mirrored bit array.
*******************************************************************************/
size_t BARRMirrorLUT(size_t arr);

#endif
