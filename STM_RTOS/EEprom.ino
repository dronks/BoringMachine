void EEPROMWriteInt(int p_address, int p_value) {
     byte lowByte = ((p_value >> 0) & 0xFF);
     byte highByte = ((p_value >> 8) & 0xFF);

     EEPROM.write(p_address, lowByte);
     EEPROM.write(p_address + 1, highByte);
}

unsigned int EEPROMReadInt(int p_address) {
     uint16 Data;
     byte lowByte;
     EEPROM.read(p_address, &Data);
     lowByte = Data;
     byte highByte;
     EEPROM.read(p_address + 1, &Data);
     highByte = Data;
     return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          EEPROM.write(ee++, *p++);

    return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
    byte* p = (byte*)(void*)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
          *p++ = EEPROM.read(ee++);
    return i;
}

