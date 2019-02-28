#include "misc.h"

/**
  * @brief  constructor function
  * @param  no
  * @retval no
  */
Miscellaneous::Miscellaneous()
{
	
}

/**
  * @brief  destructor function
  * @param  no
  * @retval no
  */
Miscellaneous::~Miscellaneous()
{
	
}

/**
  * @brief  or check
  * @param  pbuf: 		pointer to a unsigned char buffer
  * @param  BufLen:     the length of buffer
  * @retval or check result
  */
unsigned char Miscellaneous::ORCheck(unsigned char *pBuf, unsigned int BufLen)
{
    unsigned char Result = 0;
    unsigned int i = 0;

    for(i = 0; i < BufLen; i++)
        Result ^= *pBuf++;
    return Result;
}

