def crc_ccitt_update (crc, data) :
    crc_new = ((crc >> 8) | (crc << 8))
    crc_new ^= data
    crc_new ^= (crc_new & 0xff) >> 4
    crc_new ^= (crc_new << 12)
    crc_new ^= (crc_new & 0xff) << 5
    return crc_new & 0xFFFF
