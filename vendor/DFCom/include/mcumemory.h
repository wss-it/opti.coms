/**
 * @file  mcumemory.h
 * @brief Schnittstelle fuer die Klasse MCUMemory.
 */
//////////////////////////////////////////////////////////////////////
#ifndef _MCUMEMORY_H_
#define _MCUMEMORY_H_

//#define ATMEGA_128                     0
//#define ATMEGA_256                     0
#define ARMCORTEX_M4                   1

// Groesse Program - Speicher
#define ATMEGA128_SIZE_PROGRAM         0x20000
#define ATMEGA256_SIZE_PROGRAM         0x40000
#define ARMCORTEX_M4_SIZE_PROGRAM     0x200000

#ifdef ARMCORTEX_M4
#define MCU_SIZE_PROGRAM ARMCORTEX_M4_SIZE_PROGRAM
#elif ATMEGA_256
#define MCU_SIZE_PROGRAM ATMEGA256_SIZE_PROGRAM
#else
#define MCU_SIZE_PROGRAM ATMEGA128_SIZE_PROGRAM
#endif

// Groesse Bootloader - Speicher
#define ATMEGA128_SIZE_BOOTLOADER      0x1000
#define ATMEGA256_SIZE_BOOTLOADER      0x2000
#define ARMCORTEX_M4_SIZE_BOOTLOADER  0x20000

#ifdef ATMEGA_256
#define ATMEGA_SIZE_BOOTLOADER ATMEGA256_SIZE_BOOTLOADER
#else
#define ATMEGA_SIZE_BOOTLOADER ATMEGA128_SIZE_BOOTLOADER
#endif

// Restspeicher fuer Anwendungsfirmware
#define ATMEGA128_SIZE_APPLICATION     (ATMEGA128_SIZE_PROGRAM - ATMEGA128_SIZE_BOOTLOADER)
#define ATMEGA256_SIZE_APPLICATION     (ATMEGA256_SIZE_PROGRAM - ATMEGA256_SIZE_BOOTLOADER)
#define ARMCORTEX_M4_SIZE_APPLICATION  (ARMCORTEX_M4_SIZE_PROGRAM - ARMCORTEX_M4_SIZE_BOOTLOADER)

#ifdef ARMCORTEX_M4
#define MCU_SIZE_APPLICATION ARMCORTEX_M4_SIZE_APPLICATION
#elif ATMEGA_256
#define MCU_SIZE_APPLICATION ATMEGA256_SIZE_APPLICATION
#else
#define MCU_SIZE_APPLICATION ATMEGA128_SIZE_APPLICATION
#endif

// Groesse EEP - Speicher
#define ATMEGA128_SIZE_EEPROM          0x1000
#define ATMEGA256_SIZE_EEPROM          0x2000
#define ARMCORTEX_M4_SIZE_EEPROM       0

// 4kByte EEPROM - Speicher
#ifdef ARMCORTEX_M4
#define MCU_SIZE_EEPROM                ARMCORTEX_M4_SIZE_EEPROM
#elif ATMEGA_256
#define MCU_SIZE_EEPROM                ATMEGA256_SIZE_EEPROM
#else
#define MCU_SIZE_EEPROM                ATMEGA128_SIZE_EEPROM
#endif

class MCUMemory
{
public:
    MCUMemory();
    virtual ~MCUMemory();

    // Ruecksetzen
    void Reset();

    // EEPROM
    inline unsigned char *GetMemEeprom(unsigned int address) { 
        if (MCU_SIZE_EEPROM == 0 || address >= MCU_SIZE_EEPROM)
        { return 0; }
        return &m_eeprom[address]; 
    }
    inline unsigned char GetEeprom(unsigned int address) { 
        if (MCU_SIZE_EEPROM == 0 || address >= MCU_SIZE_EEPROM)
        { return 0; }
        return m_eeprom[address]; 
    }
    inline unsigned int GetEepromRange(unsigned int address, unsigned char *value, unsigned int size) { 
        if (MCU_SIZE_EEPROM == 0 || (address + size) >= MCU_SIZE_EEPROM)
        { return 0; }
        unsigned int i;
        for (i=address; i < (address + size); i++) {
        
            value[i - address] = m_eeprom[i];
        }
        return (i - address);
    }
    inline void SetEeprom(unsigned int address, unsigned char value) { 
        if (MCU_SIZE_EEPROM == 0 || address >= MCU_SIZE_EEPROM)
        { return; }
        m_eeprom[address] = value; 
    }
    inline void SetEepromRange(unsigned int address, unsigned char *value, unsigned int size) { 
        if (MCU_SIZE_EEPROM == 0 || (address + size) >= MCU_SIZE_EEPROM)
        { return; }
        for (unsigned int i=address; i < (address + size); i++) {
        
            m_eeprom[i] = value[i - address];
            SetEepromUsebit(i);
        }
    }
    // PROGRAMFLASH
    inline unsigned char *GetMemProgram(unsigned int address) { 
        if (address >= MCU_SIZE_PROGRAM) 
        { return 0; }
        return &m_program[address]; 
    }
    inline unsigned char GetProgram(unsigned int address) { 
        if (address >= MCU_SIZE_PROGRAM) 
        { return 0; }
        return m_program[address]; 
    }
    inline unsigned int GetProgramRange(unsigned int address, unsigned char *value, unsigned int size) { 
        if ((address + size) >= MCU_SIZE_PROGRAM)
        { return 0; }
        unsigned int i;
        for (i=address; i < (address + size); i++) {
        
            value[i - address] = m_program[i];
        }
        return (i - address);
    }
    inline void SetProgram(unsigned int address, unsigned char value) { 
        if (address >= MCU_SIZE_PROGRAM)
        { return; }
        m_program[address] = value;
        SetProgramUsebit(address);
    }
    inline void SetProgramRange(unsigned int address, unsigned char *value, unsigned int size) { 
        if ((address + size) >= MCU_SIZE_PROGRAM)
        { return; }
        for (unsigned int i=address; i < (address + size); i++) {
        
            m_program[i] = value[i - address];
            SetProgramUsebit(i);
        }
    }

    // Helper
    unsigned char GetEepromUsebit(unsigned int address);
    unsigned char GetProgramUsebit(unsigned int address);
    unsigned int GetEepromFirstAddress();
    unsigned int GetProgramFirstAddress();
    unsigned int GetEepromLastAddress();
    unsigned int GetProgramLastAddress();
    unsigned int GetProgramSize();

protected:
    unsigned char *m_eeprom;
    unsigned char *m_program;
    unsigned int *m_eepromUse;
    unsigned int *m_programUse;
    
    // Helper
    void SetEepromUsebit(unsigned int address);
    void SetProgramUsebit(unsigned int address);
};

#endif

