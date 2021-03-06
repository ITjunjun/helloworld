#include "utility.h"


int calcShortHmacDigest(uint8 *pKey, uint32 hmacPn, uint8 *pMacMsg, uint16 macMsgLen, uint8 *pHmacDigest, uint8 hmacDigestSize)
{
    int status;
    uint8  hmac_digest[20];
    uint8  *shortHmac_buf = NULL;
    uint16 hmac_len;


    shortHmac_buf = (uint8 *)malloc( 2048 );

    if (shortHmac_buf == NULL)
    {
        printf("[ERROR] shortHmac_buf kmalloc failure\n");
        return -1;
    }

    hmac_len = 0;

    //[1]MAC Management Message
    memcpy(shortHmac_buf, pMacMsg, macMsgLen);
    hmac_len += macMsgLen;
    //[2]HMAC_PN
    shortHmac_buf[hmac_len++] = (hmacPn >> 24) & 0xFF;
    shortHmac_buf[hmac_len++] = (hmacPn >> 16) & 0xFF;
    shortHmac_buf[hmac_len++] = (hmacPn >>  8) & 0xFF;
    shortHmac_buf[hmac_len++] = (hmacPn      ) & 0xFF;

    status = hmacGenerate(pKey, shortHmac_buf, hmac_len, hmac_digest);

    memcpy(pHmacDigest, hmac_digest, hmacDigestSize);

    free( shortHmac_buf );

    return status;
}


uint8   key[20] = {
  0xB6, 0x1F, 0x8F, 0x08, 0x6B, 0x92, 0x8F, 0x88,
  0x35, 0x1C, 0x13, 0x36, 0xC9, 0x16, 0x5A, 0x1C,
  0xBF, 0xB9, 0x18, 0x88
};
uint8   msg[] = {
  0x06, 0x06, 0x02, 0x00, 0x08, 0x0F, 0x02, 0x00,
  0x08, 0x0A, 0x01, 0x01, 0x1B, 0x01, 0x00, 0x1F,
  0x01, 0x00, 0x28, 0x01, 0x0F, 0x30, 0x02, 0x00,
  0x00, 0x95, 0x15, 0x00, 0x4E, 0x61, 0x36, 0x7E,
  0x04, 0x02, 0x01, 0x34, 0x6F, 0x9A, 0x21, 0xE6,
  0x5C, 0x5C, 0x80, 0xD4, 0x8F, 0x22, 0x25, 0x29
};
uint16  len = 25;

int main(void)
{
    uint8   digest[32];
    uint32  pn = 0;
    int     status;

    status = calcShortHmacDigest(key, pn, msg, len, digest, 10);
    printf("calcShortHmacDigest ... %d\n", status);
    mem_dump("HMAC Digest", digest, 20);

    return 0;
}

