//Copyright 2023-Present riplin

#include <stdio.h>
#include <string.h>
#include <hag/testing/mock.h>
#include <hag/drivers/vga/regs.h>
#include <hag/drivers/vga/miscout.h>
#include <hag/drivers/vga/attribc/index.h>

#if 0
#define VERBOSE(s)s;
#else
#define VERBOSE(s)
#endif

uint8_t Internal_ReadPortByte(uint16_t reg);
#pragma aux Internal_ReadPortByte = \
    "in al, dx"                     \
    parm [dx]                       \
    value [al];

uint16_t Internal_ReadPortShort(uint16_t reg);
#pragma aux Internal_ReadPortShort =\
    "in ax, dx"                     \
    parm [dx]                       \
    value [ax];

void Internal_WritePortByte(uint16_t reg, uint8_t val);
#pragma aux Internal_WritePortByte =\
    "out dx, al"                    \
    parm [dx] [al];

namespace Hag { namespace Testing { namespace Mock { 


CustomPortHandler::~CustomPortHandler()
{

}

CustomPortHandler* CustomPortHandler::CheckTypeId(uint32_t id)
{
    return id == s_ID ? this : NULL;
}

struct Instance
{
    inline Instance()
        : Allocator(NULL)
        , PortHandlers(NULL)
        , PortMap(NULL)
        , Ports(NULL)
        , MemoryMap(NULL)
        , Memory(NULL)
    {

    }

    void Initialize(IAllocator& allocator);
    void Reset();

    void Shutdown();

    bool MarkPort(uint16_t port);

    void CachePort8(uint16_t port);
    void CachePort16(uint16_t port);

    bool MarkMemory(uint32_t offset);

    uint8_t& CacheMemory(uint32_t offset, uint16_t size, uint16_t count);

    IAllocator* Allocator;
    CustomPortHandler* PortHandlers;
    uint8_t* PortMap; //1 bit for every port in a 64k address range.
    uint8_t* Ports;

    uint8_t* MemoryMap;
    uint8_t* Memory;
};

void Instance::Initialize(IAllocator& allocator)
{
    if (Allocator != NULL)
        Shutdown();

    Allocator = &allocator;

    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    Ports = allocator.AllocateAs<uint8_t>(portsSize);
    PortMap = allocator.AllocateAs<uint8_t>(portMapSize);//64Kib / 8 - One bit per byte.
    Memory = allocator.AllocateAs<uint8_t>(memorySize);
    MemoryMap = allocator.AllocateAs<uint8_t>(memoryMapSize);//1MiB / 8 - One bit per byte.

    Reset();
}

void Instance::Reset()
{
    uint32_t portsSize = 0x10000;
    uint32_t portMapSize = portsSize >> 3;
    uint32_t memorySize = 1024 * 1024;
    uint32_t memoryMapSize = memorySize >> 3;

    memset(PortMap, 0, portMapSize);
    memset(Ports, 0, portsSize);
    memset(MemoryMap, 0, memoryMapSize);
    memset(Memory, 0, memorySize);

    CustomPortHandler* ptr = PortHandlers;
    while (ptr != NULL)
    {
        ptr->Reset();
        ptr = ptr->GetNext();
    }
}

void Instance::Shutdown()
{
    if (Allocator == NULL)
        return;

    while (PortHandlers != NULL)
    {
        CustomPortHandler* tmp = PortHandlers->GetNext();
        PortHandlers->~CustomPortHandler();
        Allocator->Free(PortHandlers);
        PortHandlers = tmp;
    }

    Allocator->Free(Ports);
    Ports = NULL;

    Allocator->Free(PortMap);
    PortMap = NULL;
    
    Allocator->Free(Memory);
    Memory = NULL;

    Allocator->Free(MemoryMap);
    MemoryMap = NULL;

    Allocator = NULL;
}

bool Instance::MarkPort(uint16_t port)
{
    if (Allocator == NULL)
        return false;

    uint16_t index = port >> 3;
    uint8_t bit = 1 << (port & 0x0007);
    bool ret = (PortMap[index] & bit) == 0x00;
    PortMap[index] |= bit;
    return ret;
}

void Instance::CachePort8(uint16_t port)
{
    if (Allocator == NULL)
        return;

    uint16_t index = port >> 3;
    uint8_t bit = 1 << (port & 0x0007);
    if (MarkPort(port))
    {
        Ports[port] = Internal_ReadPortByte(port);
    }
}

void Instance::CachePort16(uint16_t port)
{
    if (Allocator == NULL)
        return;

    if (MarkPort(port))
        Ports[port] = Internal_ReadPortByte(port);

    ++port;

    if (MarkPort(port))
        Ports[port] = Internal_ReadPortByte(port);
}

bool Instance::MarkMemory(uint32_t offset)
{
    if (Allocator == NULL)
        return false;

    uint32_t index = offset >> 3;
    uint8_t bit = 1 << (offset & 0x0007);
    bool ret = (MemoryMap[index] & bit) == 0x00;
    MemoryMap[index] |= bit;
    return ret;

}

uint8_t dummy[16] = { 0 };

uint8_t& Instance::CacheMemory(uint32_t offset, uint16_t size, uint16_t count)
{
    if (Allocator == NULL)
        return dummy[0];

    uint8_t* realMem = (uint8_t*)0x00000000;
    for (uint16_t i = 0; i < size * count; ++i)
    {
        if (MarkMemory(offset + i))
            Memory[offset + i] = realMem[offset + i];
    }
    return Memory[offset];
}

Instance s_Instance0;
Instance s_Instance1;

Instance* s_CurrentInstance = &s_Instance0;

const uint32_t CustomPortHandler::s_ID = 0x62947393;

class IndexedPort : public CustomPortHandler
{
public:
    inline IndexedPort(IAllocator& allocator, const char* name, uint16_t indexPort, uint16_t dataPort, uint16_t regCount, uint8_t indexMask)
        : CustomPortHandler(name)
        , m_Allocator(&allocator)
        , m_IndexPort(indexPort)
        , m_DataPort(dataPort)
        , m_RegisterCount(regCount)
        , m_IndexMask(indexMask)
    {
        m_Data = m_Allocator->AllocateAs<uint8_t>(regCount);
        m_DataMask = m_Allocator->AllocateAs<uint8_t>(regCount >> 3);
        Reset();
    }

    virtual ~IndexedPort()
    {
        m_Allocator->Free(m_DataMask);
        m_DataMask = NULL;
        m_Allocator->Free(m_Data);
        m_Data = NULL;
    }

    virtual bool CanHandle(uint16_t port)
    {
        return (port == m_IndexPort) || (port == m_DataPort);
    }

    virtual bool HasHandled(uint16_t port)
    {
        return (port == m_IndexPort) || (port == m_DataPort);
    }

    virtual uint8_t Read8(uint16_t port)
    {
        if (port == m_DataPort)
        {
            uint8_t index = GetIndex();
            if (index >= m_RegisterCount)
                return 0;

            uint8_t blockIndex = index >> 3;
            uint8_t bitMask = 1 << (index & 0x07);
            
            if ((m_DataMask[blockIndex] & bitMask) == 0x00)
            {
                m_DataMask[blockIndex] |= bitMask;
                uint8_t oldValue = Internal_ReadPortByte(port - 1);
                Internal_WritePortByte(port - 1, index);

                m_Data[index] = Internal_ReadPortByte(port);

                Internal_WritePortByte(port - 1, oldValue);
            }
            VERBOSE(printf("%s port read 0x%04X:0x%02X = 0x%02X\n", GetName(), port, index, m_Data[index]));
            return m_Data[index];
        }
        else if (port == m_IndexPort)
        {
            s_CurrentInstance->CachePort8(m_IndexPort);
            VERBOSE(printf("%s port read 0x%04X = 0x%02X\n", GetName(), port, s_CurrentInstance->Ports[m_IndexPort]));
            return s_CurrentInstance->Ports[m_IndexPort];
        }
        return 0;
    }

    virtual uint16_t Read16(uint16_t port)
    {
        return Read8(port) | (uint16_t(Read8(port + 1)) << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        if (port == m_DataPort)
        {
            uint8_t index = GetIndex();
            if (index >= m_RegisterCount)
                return;

            uint8_t blockIndex = index >> 3;
            uint8_t bitMask = 1 << (index & 0x07);
            m_DataMask[blockIndex] |= bitMask;
            m_Data[index] = value;           
            VERBOSE(printf("%s port write 0x%04X:0x%02X = 0x%02X\n", GetName(), port, index, value));
        }
        else if (port == m_IndexPort)
        {
            s_CurrentInstance->MarkPort(m_IndexPort);
            s_CurrentInstance->Ports[m_IndexPort] = value;
            VERBOSE(printf("%s port write 0x%04X = 0x%02X\n", GetName(), port, value));
        }
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        Write8(port, valueLo);
        Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        Write8(port, uint8_t(value));
        Write8(port + 1, uint8_t(value >> 8));
    }

    virtual void Reset()
    {
        memset(m_Data, 0, m_RegisterCount);
        memset(m_DataMask, 0, m_RegisterCount >> 3);
    }

    void FetchModifiedIndexedRegisters(IndexedRegisterCheckCallback_t callback, void* context)
    {
        uint8_t oldValue = Internal_ReadPortByte(m_IndexPort);
        
        for (uint32_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((m_DataMask[index] & bits) != 0)
            {
                Internal_WritePortByte(m_IndexPort, i);
                uint8_t originalValue = Internal_ReadPortByte(m_DataPort);
                callback(m_DataPort, uint8_t(i), m_Data[i], originalValue, context);
            }
        }

        Internal_WritePortByte(m_IndexPort, oldValue);
    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        IndexedPort* inst0 = this;
        IndexedPort* inst1 = (IndexedPort*)instance1;
        for (uint16_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((inst0->m_DataMask[index] & bits) !=
                (inst1->m_DataMask[index] & bits))
            {
                return true;
            }
            else if ((inst0->m_DataMask[index] & bits) != 0 &&
                     (inst1->m_DataMask[index] & bits) != 0)
            {
                if (inst0->m_Data[i] != inst1->m_Data[i])
                {
                    return true;
                }
            }
        }

        return false;
    }

    virtual void Report(CustomPortHandler* instance1)
    {
        printf("\n%s report:\n", GetName());
        IndexedPort* inst0 = this;
        IndexedPort* inst1 = (IndexedPort*)instance1;
        for (uint16_t i = 0; i < m_RegisterCount; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((inst0->m_DataMask[index] & bits) !=
                (inst1->m_DataMask[index] & bits))
            {
                if ((inst0->m_DataMask[index] & bits) != 0)
                {
                    printf("Only instance 0 has content in register 0x%02X = 0x%02X\n", i, inst0->m_Data[i]);
                }

                if ((inst1->m_DataMask[index] & bits) != 0)
                {
                    printf("Only instance 1 has content in register 0x%02X = 0x%02X\n", i, inst1->m_Data[i]);
                }
            }
            else if ((inst0->m_DataMask[index] & bits) != 0 &&
                     (inst1->m_DataMask[index] & bits) != 0)
            {
                if (inst0->m_Data[i] != inst1->m_Data[i])
                {
                    printf("Content of register 0x%02X differs: 0x%02X != 0x%02X\n", i, inst0->m_Data[i], inst1->m_Data[i]);
                }
            }
        }
    }


protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x23b2a89f;

private:
    IAllocator* m_Allocator;
    uint16_t m_IndexPort;
    uint16_t m_DataPort;
    uint16_t m_RegisterCount;
    uint8_t m_IndexMask;
    uint8_t* m_Data;
    uint8_t* m_DataMask;

    uint8_t GetIndex()
    {
        s_CurrentInstance->CachePort8(m_IndexPort);
        return s_CurrentInstance->Ports[m_IndexPort] & m_IndexMask;
    }
};

class AttributePortHandler : public CustomPortHandler
{
public:
    AttributePortHandler(IAllocator& allocator)
        : CustomPortHandler("Attribute Ports")
        , m_Allocator(allocator)
        , Mask(NULL)
        , Data(NULL)
        , m_InputStatus1(Hag::VGA::Register::InputStatus1D)
        , m_OriginalIndex(0)
        , m_CurrentIndex(0)
        , m_IsIndex(true)
    {
        if ((Internal_ReadPortByte(Hag::VGA::Register::MiscellaneousR) & 
             Hag::VGA::MiscellaneousOutput::IOAddressSelect) == 0x00)
             m_InputStatus1 = Hag::VGA::Register::InputStatus1B;

        m_OriginalIndex = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
        m_CurrentIndex = m_OriginalIndex;

        uint8_t size = 32;
        Mask = m_Allocator.AllocateAs<uint8_t>(size >> 3);
        Data = m_Allocator.AllocateAs<uint8_t>(size);
        
        Reset();
    }

    virtual ~AttributePortHandler()
    {
        if (Mask != NULL)
            m_Allocator.Free(Mask);
        Mask = NULL;

        if (Data != NULL)
            m_Allocator.Free(Data);
        Data = NULL;
    }

    void SyncToIndex()
    {
        Internal_ReadPortByte(m_InputStatus1);
    }

    bool MarkPort(uint8_t index)
    {
        uint8_t blockIndex = index >> 3;
        uint8_t bits = 1 << (index & 0x07);
        bool ret = (Mask[blockIndex] & bits) == 0x00;
        Mask[blockIndex] |= bits;
        return ret;
    }

    void CacheRegister(uint8_t index)
    {
        if (MarkPort(index))
        {
            SyncToIndex();
            uint8_t orgIndex = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
            Internal_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, (orgIndex & (~0x1f)) | (index & 0x1f));
            Data[index] = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerDataR);
            SyncToIndex();
            Internal_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, orgIndex);
        }
    }

    virtual bool CanHandle(uint16_t port)
    {
        return port == m_InputStatus1 ||
               port == Hag::VGA::Register::AttributeControllerIndex ||
               port == Hag::VGA::Register::AttributeControllerDataR;
    }

    virtual bool HasHandled(uint16_t port)
    {
        return port == Hag::VGA::Register::AttributeControllerIndex ||
               port == Hag::VGA::Register::AttributeControllerDataR;
    }

    virtual uint8_t Read8(uint16_t port)
    {
        if (port == m_InputStatus1)
        {
            m_IsIndex = true;
            return 0;
        }

        if (port == Hag::VGA::Register::AttributeControllerIndex)
        {
            return m_CurrentIndex;
        }

        if (port == Hag::VGA::Register::AttributeControllerDataR)
        {
            uint8_t index = m_CurrentIndex & 0x1f;
            CacheRegister(index);
            return Data[index];
        }

        return 0;
    }

    virtual uint16_t Read16(uint16_t port)
    {
        uint16_t lo = Hag::Testing::Mock::Port::Read8(port);
        uint16_t hi = Hag::Testing::Mock::Port::Read8(port + 1);
        return lo | (hi << 8);
    }

    virtual void Write8(uint16_t port, uint8_t value)
    {
        if (port == Hag::VGA::Register::AttributeControllerDataW)
        {
            if (m_IsIndex)
            {
                m_CurrentIndex = value;
            }
            else
            {
                uint8_t index = m_CurrentIndex & 0x1f;
                MarkPort(index);
                Data[index] = value;
            }
            m_IsIndex = !m_IsIndex;
        }
    }

    virtual void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
    {
        Hag::Testing::Mock::Port::Write8(port, valueLo);
        Hag::Testing::Mock::Port::Write8(port + 1, valueHi);
    }

    virtual void Write16(uint16_t port, uint16_t value)
    {
        Hag::Testing::Mock::Port::Write8(port, uint8_t(value));
        Hag::Testing::Mock::Port::Write8(port + 1, uint8_t(value >> 8));
    }

    void FetchModifiedIndexedRegisters(IndexedRegisterCheckCallback_t callback, void* context)
    {
        uint8_t oldValue = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerIndex);
        
        for (uint32_t i = 0; i < 32; ++i)
        {
            uint16_t index = i >> 3;
            uint8_t bits = 1 << (uint8_t(i) & 0x07);
            if ((Mask[index] & bits) != 0)
            {
                SyncToIndex();
                Internal_WritePortByte(Hag::VGA::Register::AttributeControllerDataW, i);
                uint8_t originalValue = Internal_ReadPortByte(Hag::VGA::Register::AttributeControllerDataR);
                callback(Hag::VGA::Register::AttributeControllerDataW, uint8_t(i), Data[i], originalValue, context);
            }
        }

        SyncToIndex();
        Internal_WritePortByte(Hag::VGA::Register::AttributeControllerIndex, oldValue);
    }

    virtual void Report(CustomPortHandler* instance1)
    {

    }

    virtual bool HasDifferences(CustomPortHandler* instance1)
    {
        return false;
    }

    virtual void Reset()
    {
        memset(Mask, 0, 32 >> 3);
        memset(Data, 0, 32);
    }

protected:
    template<typename T> friend T* customporthandler_cast(CustomPortHandler* ptr);
    virtual CustomPortHandler* CheckTypeId(uint32_t id)
    {
        return id == s_ID ? this : CustomPortHandler::CheckTypeId(id);
    }
    static const uint32_t s_ID = 0x5d038b73;

private:
    Hag::IAllocator& m_Allocator;
    uint8_t* Mask;
    uint8_t* Data;
    Hag::VGA::Register_t m_InputStatus1;
    uint8_t m_OriginalIndex;
    uint8_t m_CurrentIndex;
    bool m_IsIndex;
};

void Initialize(IAllocator& allocator)
{
    s_Instance0.Initialize(allocator);
    s_Instance1.Initialize(allocator);

    AttributePortHandler* attr0 = ::new(s_Instance0.Allocator->Allocate(sizeof(AttributePortHandler))) 
        AttributePortHandler(allocator);
    attr0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = attr0;

    AttributePortHandler* attr1 = ::new(s_Instance0.Allocator->Allocate(sizeof(AttributePortHandler))) 
        AttributePortHandler(allocator);
    attr1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = attr1;
}

void AddIndexedPort(const char* name, uint16_t indexPort, uint8_t indexMask, uint16_t dataPort, uint16_t regCount)
{
    if (s_Instance0.Allocator == NULL)
        return;

    IndexedPort* port0 = ::new(s_Instance0.Allocator->Allocate(sizeof(IndexedPort))) 
        IndexedPort(*s_Instance0.Allocator, name, indexPort, dataPort, regCount, indexMask);
    port0->SetNext(s_Instance0.PortHandlers);
    s_Instance0.PortHandlers = port0;

    IndexedPort* port1 = ::new(s_Instance1.Allocator->Allocate(sizeof(IndexedPort))) 
        IndexedPort(*s_Instance1.Allocator, name, indexPort, dataPort, regCount, indexMask);
    port1->SetNext(s_Instance1.PortHandlers);
    s_Instance1.PortHandlers = port1;
}

struct VerifyPaVContext
{
    PortAndValue* modifiedPortsAndValues;
    uint16_t* readPortsAndValues;
    PortAndIndexAndValue* modifiedIndexedPortsAndValues;
    PortAndIndex* readIndexedPortsAndValues;
    int modifiedCount;
    int readCount;
    int modifiedIndexedCount;
    int readIndexedCount;
    int verifiedCount;
};

void VerifyPortsAndValuesCallback(uint16_t port, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyPaVContext& context = *(VerifyPaVContext*)ctx;
    bool found = false;

    for (int i = 0; i < context.modifiedCount; ++i)
    {
        if (context.modifiedPortsAndValues[i].Port == port)
        {
            if (context.modifiedPortsAndValues[i].Value == modifiedValue)
            {
                found = true;
                context.verifiedCount++;
            }
            else
            {
                printf("Port 0x%04X modified value should be 0x%02X but is 0x%02X\n", port, context.modifiedPortsAndValues[i].Value, modifiedValue);
            }
            break;
        }
    }

    if (!found)
    {
        for (int i = 0; i < context.readCount; ++i)
        {
            if (context.readPortsAndValues[i] == port)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X marked read, value should be 0x%02X but is 0x%02X\n", port, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        printf("Port 0x%04X not found in test data. Original: 0x%02X", port, originalValue);
        if (originalValue != modifiedValue)
        {
            printf(", modified: 0x%02X\n", modifiedValue);
        }
        else
        {
            printf("\n");
        }
    }
}

void VerifyPortsAndValuesIndexedCallback(uint16_t port, uint8_t index, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyPaVContext& context = *(VerifyPaVContext*)ctx;
    bool found = false;

    for (int i = 0; i < context.modifiedIndexedCount; ++i)
    {
        if (context.modifiedIndexedPortsAndValues[i].Port == port &&
            context.modifiedIndexedPortsAndValues[i].Index == index)
        {
            if (context.modifiedIndexedPortsAndValues[i].Value == modifiedValue)
            {
                found = true;
                context.verifiedCount++;
            }
            else
            {
                printf("Port 0x%04X:0x%02X modified value should be 0x%02X but is 0x%02X\n", port, index, context.modifiedIndexedPortsAndValues[i].Value, modifiedValue);
            }
            break;
        }
    }

    if (!found)
    {
        for (int i = 0; i < context.readIndexedCount; ++i)
        {
            if (context.readIndexedPortsAndValues[i].Port == port &&
                context.readIndexedPortsAndValues[i].Index == index)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("Port 0x%04X:0x%02X marked read, value should be 0x%02X but is 0x%02X\n", port, index, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        printf("Port 0x%04X:0x%02X not found in test data. Original: 0x%02X", port, index, originalValue);
        if (originalValue != modifiedValue)
        {
            printf(", modified: 0x%02X\n", modifiedValue);
        }
        else
        {
            printf("\n");
        }
    }
}

int VerifyPortsAndValues(int instance, PortAndValue* modifiedPortsAndValues, int modifiedCount,
                         uint16_t* readPortsAndValues, int readCount,
                         PortAndIndexAndValue* modifiedIndexedPortsAndValues, int modifiedIndexedCount,
                         PortAndIndex* readIndexedPortsAndValues, int readIndexedCount)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    VerifyPaVContext context = 
    {
        modifiedPortsAndValues,
        readPortsAndValues,
        modifiedIndexedPortsAndValues,
        readIndexedPortsAndValues,
        modifiedCount,
        readCount,
        modifiedIndexedCount,
        readIndexedCount,
        0
    };

    FetchModifiedRegisters(instance, VerifyPortsAndValuesCallback, &context);
    FetchModifiedIndexedRegisters(instance, VerifyPortsAndValuesIndexedCallback, &context);

    return context.verifiedCount;
}

void FetchModifiedIndexedRegisters(int instance, IndexedRegisterCheckCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    Instance* inst = instance == 0 ? &s_Instance0 : &s_Instance1;
    
    CustomPortHandler* ptr = inst->PortHandlers;
    while (ptr != NULL)
    {
        IndexedPort* indexed = customporthandler_cast<IndexedPort>(ptr);
        if (indexed != NULL)
        {
            indexed->FetchModifiedIndexedRegisters(callback, context);
        }

        AttributePortHandler* attr = customporthandler_cast<AttributePortHandler>(ptr);
        if (attr != NULL)
        {
            attr->FetchModifiedIndexedRegisters(callback, context);
        }

        ptr = ptr->GetNext();
    }
}

void FetchModifiedRegisters(int instance, RegisterCheckCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    Instance* inst = instance == 0 ? &s_Instance0 : &s_Instance1;
    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((inst->PortMap[blockIndex] & bits) != 0x00)
        {
            uint8_t orgValue = Internal_ReadPortByte(uint16_t(i));
            callback(uint16_t(i), inst->Ports[i], orgValue, context);
        }
    }
}

struct VerifyBDAContext
{
    BDAFieldsAndValues* modifiedFieldsAndValues;
    int modifiedCount;
    uint8_t* readFields;
    int readCount;
    uint8_t* ignoreFields;
    int ignoreCount;
    int verifiedCount;
};

void VerifyBDAFieldsCallback(uint8_t field, uint8_t modifiedValue, uint8_t originalValue, void* ctx)
{
    VerifyBDAContext& context = *(VerifyBDAContext*)ctx;

    bool found = false;

    for (uint32_t i = 0; i < context.ignoreCount; ++i)
    {
        if (context.ignoreFields[i] == field)
        {
            found = true;
            context.verifiedCount++;
            break;
        }
    }

    if (!found)
    {
        for (uint32_t i = 0; i < context.modifiedCount; ++i)
        {
            if (context.modifiedFieldsAndValues[i].Field == field)
            {
                if (context.modifiedFieldsAndValues[i].Value == modifiedValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("BDA field 0x%02X modified value should be 0x%02X but is 0x%02X\n", field, 
                        context.modifiedFieldsAndValues[i].Value, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        for (uint32_t i = 0; i < context.readCount; ++i)
        {
            if (context.readFields[i] == field)
            {
                if (modifiedValue == originalValue)
                {
                    found = true;
                    context.verifiedCount++;
                }
                else
                {
                    printf("BDA field 0x%02X marked read, value should be 0x%02X but is 0x%02X\n", field, originalValue, modifiedValue);
                }
                break;
            }
        }
    }

    if (!found)
    {
        printf("BDA field 0x%02X not found in test data. Original: 0x%02X", field, originalValue);
        if (originalValue != modifiedValue)
        {
            printf(", modified: 0x%02X\n", modifiedValue);
        }
        else
        {
            printf("\n");
        }
    }

}

int VerifyBDAFieldsAndValues(int instance, BDAFieldsAndValues* modifiedFieldsAndValues, int modifiedCount,
                             uint8_t* readFields, int readCount, uint8_t* ignoreFields, int ignoreCount)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    VerifyBDAContext context =
    {
        modifiedFieldsAndValues,
        modifiedCount,
        readFields,
        readCount,
        ignoreFields,
        ignoreCount,
        0
    };

    FetchModifiedBDAFields(0, VerifyBDAFieldsCallback, &context);

    return context.verifiedCount;
}

void FetchModifiedBDAFields(int instance, BDAFieldCallback_t callback, void* context)
{
    if (s_Instance0.Allocator == NULL)
        return;

    uint8_t* realMem = (uint8_t*)0x00000000;
    for (uint32_t i = 0x400; i < 0x500; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_CurrentInstance->MemoryMap[blockIndex] & bits) != 0x00)
        {
            callback(i,  s_CurrentInstance->Memory[i], realMem[i], context);
        }
    }
}

void SelectInstance(int instance)
{
    s_CurrentInstance = (instance & 1) == 0 ? &s_Instance0 : &s_Instance1;
}

void Reset()
{
    s_Instance0.Reset();
    s_Instance1.Reset();
}

bool HasDifferences()
{
    if (s_Instance0.Allocator == NULL)
        return false;

    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_Instance0.PortMap[blockIndex] & bits) !=
            (s_Instance1.PortMap[blockIndex] & bits))
        {
            return true;
        }
        else if ((s_Instance0.PortMap[blockIndex] & bits) != 0 &&
                 (s_Instance1.PortMap[blockIndex] & bits) != 0)
        {
            if (s_Instance0.Ports[i] != s_Instance1.Ports[i])
            {
                return true;
            }
        }
    }

    CustomPortHandler* ptr0 = s_Instance0.PortHandlers;
    CustomPortHandler* ptr1 = s_Instance1.PortHandlers;
    while (ptr0 != NULL && ptr1 != NULL)
    {
        if (ptr0->HasDifferences(ptr1))
            return true;
        ptr0 = ptr0->GetNext();
        ptr1 = ptr1->GetNext();
    }

    for (uint32_t i = 0; i < (1024 * 1024) >> 3; ++i)
    {
        if (s_Instance0.MemoryMap[i] != 0 ||
            s_Instance1.MemoryMap[i] != 0)
        {
            for (uint32_t j = 0; j < 8; ++j)
            {
                uint8_t bits = uint8_t(1 << j);
                if ((s_Instance0.MemoryMap[i] & bits) !=
                    (s_Instance1.MemoryMap[i] & bits))
                {
                    return true;
                }
                else if ((s_Instance0.MemoryMap[i] & bits) != 0 &&
                    (s_Instance1.MemoryMap[i] & bits) != 0)
                {
                    if (s_Instance0.Memory[(i << 3) + j] !=
                        s_Instance1.Memory[(i << 3) + j])
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Report()
{
    if (s_Instance0.Allocator == NULL)
        return;

    printf("\nDifference report: \n");

    for (uint32_t i = 0; i < 0x10000; ++i)
    {
        uint16_t blockIndex = uint16_t(i >> 3);
        uint8_t bits = 1 << (uint8_t(i) & 0x07);
        if ((s_Instance0.PortMap[blockIndex] & bits) !=
            (s_Instance1.PortMap[blockIndex] & bits))
        {
            if (s_Instance0.PortMap[blockIndex] & bits)
            {
                printf("Only instance 0 has content in port 0x%04X = 0x%02X\n", i, s_Instance0.Ports[i]);
            }
            if (s_Instance1.PortMap[blockIndex] & bits)
            {
                printf("Only instance 1 has content in port 0x%04X = 0x%02X\n", i, s_Instance1.Ports[i]);
            }
        }
        else if ((s_Instance0.PortMap[blockIndex] & bits) != 0 &&
                 (s_Instance1.PortMap[blockIndex] & bits) != 0)
        {
            if (s_Instance0.Ports[i] != s_Instance1.Ports[i])
            {
                printf("Content of register 0x%02X differs: 0x%02X != 0x%02X\n", i, s_Instance0.Ports[i], s_Instance1.Ports[i]);
            }
        }
    }

    CustomPortHandler* ptr0 = s_Instance0.PortHandlers;
    CustomPortHandler* ptr1 = s_Instance1.PortHandlers;
    while (ptr0 != NULL && ptr1 != NULL)
    {
        ptr0->Report(ptr1);
        ptr0 = ptr0->GetNext();
        ptr1 = ptr1->GetNext();
    }

    uint8_t* realmem = 0x00000000;

    printf("\nMemory report:\n");
    for (uint32_t i = 0; i < (1024 * 1024) >> 3; ++i)
    {
        if (s_Instance0.MemoryMap[i] != 0 ||
            s_Instance1.MemoryMap[i] != 0)
        {
            for (uint32_t j = 0; j < 8; ++j)
            {
                uint8_t bits = uint8_t(1 << j);
                if ((s_Instance0.MemoryMap[i] & bits) !=
                    (s_Instance1.MemoryMap[i] & bits))
                {
                    if ((s_Instance0.MemoryMap[i] & bits) != 0)
                    {
                        printf("Only Instance 0 has content in memory offset 0x%05X = 0x%02X",
                            (i << 3) + j, s_Instance0.Memory[(i << 3) + j]);
                        if (realmem[(i << 3) + j] != s_Instance0.Memory[(i << 3) + j])
                        {
                            printf(" original: 0x%02X\n", realmem[(i << 3) + j]);
                        }
                        else
                        {
                            printf("\n");
                        }
                    }
                    if ((s_Instance1.MemoryMap[i] & bits) != 0)
                    {
                        printf("Only Instance 1 has content in memory offset 0x%05X = 0x%02X",
                            (i << 3) + j, s_Instance1.Memory[(i << 3) + j]);
                        if (realmem[(i << 3) + j] != s_Instance1.Memory[(i << 3) + j])
                        {
                            printf(" original: 0x%02X\n", realmem[(i << 3) + j]);
                        }
                        else
                        {
                            printf("\n");
                        }
                    }
                }
                else if ((s_Instance0.MemoryMap[i] & bits) != 0 &&
                    (s_Instance1.MemoryMap[i] & bits) != 0)
                {
                    if (s_Instance0.Memory[(i << 3) + j] !=
                        s_Instance1.Memory[(i << 3) + j])
                    {
                        printf("Content of memory offset 0x%05X differs: 0x%02X != 0x%02X original: 0x%02X\n", (i << 3) + j,
                            s_Instance0.Memory[(i << 3) + j], s_Instance1.Memory[(i << 3) + j], realmem[(i << 3) + j]);
                    }
                }
            }
        }
    }
}

int Shutdown()
{
    s_Instance1.Shutdown();
    s_Instance0.Shutdown();
    return 0;
}

namespace Port
{

uint8_t Read8(uint16_t port)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    uint8_t ret = 0;
    bool handled = false;
    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            ret = handler->Read8(port);
            if (handler->HasHandled(port))
            {
                handled = true;
                break;
            }
        }
        handler = handler->GetNext();
    }

    if (handled)
    {
        s_CurrentInstance->MarkPort(port);
        s_CurrentInstance->Ports[port] = ret;
    }
    else
    {
        s_CurrentInstance->CachePort8(port);
        ret = s_CurrentInstance->Ports[port];
        VERBOSE(printf("Port read 0x%04X = 0x%02X\n", port, ret));
    }
    return ret;
}

uint16_t Read16(uint16_t port)
{
    if (s_Instance0.Allocator == NULL)
        return 0;

    uint16_t ret = 0;
    bool handled = false;
    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            ret = handler->Read16(port);
            if (handler->HasHandled(port))
            {
                handled = true;
                break;
            }
        }
        handler = handler->GetNext();
    }

    if (handled)
    {
        s_CurrentInstance->MarkPort(port);
        s_CurrentInstance->MarkPort(port + 1);
        s_CurrentInstance->Ports[port] = ret;
    }
    else
    {
        s_CurrentInstance->CachePort16(port);
        ret = *((uint16_t*)(&s_CurrentInstance->Ports[port]));
        VERBOSE(printf("Port read 0x%04X = 0x%04X\n", port, ret));
    }
    return ret;
}

void Write8(uint16_t port, uint8_t value)
{
    if (s_Instance0.Allocator == NULL)
        return;

    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    bool handled = false;
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            handler->Write8(port, value);
            if (handled = handler->HasHandled(port))
            {
                break;
            }
        }
        handler = handler->GetNext();
    }
    if (!handled)
    {
        VERBOSE(printf("Port write 0x%04X = 0x%02X\n", port, value));
    }
    s_CurrentInstance->MarkPort(port);
    s_CurrentInstance->Ports[port] = value;
}

void Write16(uint16_t port, uint16_t value)
{
    if (s_Instance0.Allocator == NULL)
        return;

    CustomPortHandler* handler = s_CurrentInstance->PortHandlers;
    bool handled = false;
    while (handler != NULL)
    {
        if (handler->CanHandle(port))
        {
            handler->Write16(port, value);
            if (handled = handler->HasHandled(port))
            {
                break;
            }
        }
        handler = handler->GetNext();
    }
    if (!handled)
    {
        VERBOSE(printf("Port write 0x%04X = 0x%04X\n", port, value));
    }
    s_CurrentInstance->MarkPort(port);
    s_CurrentInstance->MarkPort(port + 1);
    *((uint16_t*)(&s_CurrentInstance->Ports[port])) = value;
}

void Write8(uint16_t port, uint8_t valueLo, uint8_t valueHi)
{
    uint16_t val16 = (uint16_t(valueHi) << 16) || valueLo;
    Write16(port, val16);
}

}

namespace Memory
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count)
    {
        if (s_Instance0.Allocator == NULL)
            return dummy[0];

        VERBOSE(printf("Memory access 0x%04X, size %i\n", offset, size * count));
        return s_CurrentInstance->CacheMemory(offset, size, count);
    }
}

namespace BDA
{
    uint8_t& Ref(uint16_t offset, uint16_t size, uint16_t count)
    {
        if (s_Instance0.Allocator == NULL)
            return dummy[0];

        VERBOSE(printf("BDA access 0x%04X, size %i\n", 0x400 + offset, size * count));
        return s_CurrentInstance->CacheMemory(0x400 + offset, size, count);
    }
}

}}}
