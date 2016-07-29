
#include "AdsLib.h"

#include <iostream>

struct remote_target {
    const char* ip_v4;
    AmsAddr server;
};

namespace examples
{
static void NotifyCallback(const AmsAddr* pAddr, const AdsNotificationHeader* pNotification, uint32_t hUser)
{
    const uint8_t* data = reinterpret_cast<const uint8_t*>(pNotification + 1);
    std::cout << "hUser 0x" << std::hex << hUser <<
        " sample time: " << std::dec << pNotification->nTimeStamp <<
        " sample size: " << std::dec << pNotification->cbSampleSize <<
        " value:";
    for (size_t i = 0; i < pNotification->cbSampleSize; ++i) {
        std::cout << " 0x" << std::hex << (int)data[i];
    }
    std::cout << '\n';
}

namespace by_address
{
const uint32_t READ_WRITE_M_BASE = 0x4020;
void read(long port, const AmsAddr& server, std::ostream& out)
{
    uint32_t bytesRead;
    uint32_t buffer;

    out << __FUNCTION__ << "():\n";
    for (size_t i = 0; i < 8; ++i) {
        const long status = AdsSyncReadReqEx2(port,
                                              &server,
                                              READ_WRITE_M_BASE,
                                              0, // address of MAIN.byByte
                                              sizeof(buffer),
                                              &buffer,
                                              &bytesRead);
        if (status) {
            out << "ADS read failed with: " << std::dec << status << '\n';
            return;
        }
        out << "ADS read " << std::dec << bytesRead << " bytes, value: 0x" << std::hex << buffer << '\n';
    }
}

void notification(long port, const AmsAddr& server, std::ostream& out)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERCYCLE,
        0,
        { 4000000 }
    };
    uint32_t hNotify;
    uint32_t hUser = 0;

    const long addStatus = AdsSyncAddDeviceNotificationReqEx(port,
                                                             &server,
                                                             READ_WRITE_M_BASE,
                                                             4,
                                                             &attrib,
                                                             &NotifyCallback,
                                                             hUser,
                                                             &hNotify);
    if (addStatus) {
        out << "Add device notification failed with: " << std::dec << addStatus << '\n';
        return;
    }

    std::cout << "Hit ENTER to stop notifications\n";
    std::cin.ignore();

    const long delStatus = AdsSyncDelDeviceNotificationReqEx(port, &server, hNotify);
    if (delStatus) {
        out << "Delete device notification failed with: " << std::dec << delStatus;
        return;
    }
}
}

namespace by_name
{
struct auto_handle {
    auto_handle(long port, const AmsAddr& server, const std::string name, std::ostream& out)
        : m_out(out), m_port(port), m_server(server), m_name(name), m_handle(0)
    {
        const long handleStatus = AdsSyncReadWriteReqEx2(m_port,
                                                         &m_server,
                                                         ADSIGRP_SYM_HNDBYNAME,
                                                         0,
                                                         sizeof(m_handle),
                                                         &m_handle,
                                                         m_name.size(),
                                                         m_name.c_str(),
                                                         nullptr);
        if (handleStatus) {
            m_out << "Create handle for '" << m_name << "' failed with: 0x" << std::hex << handleStatus << '\n';
        }
    }

    const uint32_t get() const
    {
        return m_handle;
    }

    ~auto_handle()
    {
        const long releaseHandle = AdsSyncWriteReqEx(m_port,
                                                     &m_server,
                                                     ADSIGRP_SYM_RELEASEHND,
                                                     0,
                                                     sizeof(m_handle),
                                                     &m_handle);
        if (releaseHandle) {
            m_out << "Release handle 0x" << std::hex << m_handle << "' failed with: 0x" << releaseHandle << '\n';
        }
    }
private:
    uint32_t m_handle;
    long m_port;
    AmsAddr m_server;
    std::ostream& m_out;
    const std::string m_name;
};

void notification(long port, const AmsAddr& server, std::ostream& out)
{
    const AdsNotificationAttrib attrib = {
        1,
        ADSTRANS_SERVERCYCLE,
        0,
        { 4000000 }
    };
    uint32_t hNotify;
    uint32_t hUser = 0;

    out << __FUNCTION__ << "():\n";
    auto_handle handle(port, server, "MAIN.byByte", out);

    const long addStatus = AdsSyncAddDeviceNotificationReqEx(port,
                                                             &server,
                                                             ADSIGRP_SYM_VALBYHND,
                                                             handle.get(),
                                                             &attrib,
                                                             &NotifyCallback,
                                                             hUser,
                                                             &hNotify);
    if (addStatus) {
        out << "Add device notification failed with: " << std::dec << addStatus << '\n';
        return;
    }

    std::cout << "Hit ENTER to stop by name notifications\n";
    std::cin.ignore();

    const long delStatus = AdsSyncDelDeviceNotificationReqEx(port, &server, hNotify);
    if (delStatus) {
        out << "Delete device notification failed with: " << std::dec << delStatus;
        return;
    }
}

void read(long port, const AmsAddr& server, std::ostream& out)
{
    uint32_t bytesRead;
    uint32_t buffer;

    out << __FUNCTION__ << "():\n";
    auto_handle handle(port, server, "MAIN.byByte", out);

    for (size_t i = 0; i < 8; ++i) {
        const long status = AdsSyncReadReqEx2(port,
                                              &server,
                                              ADSIGRP_SYM_VALBYHND,
                                              handle.get(),
                                              sizeof(buffer),
                                              &buffer,
                                              &bytesRead);
        if (status) {
            out << "ADS read failed with: " << std::dec << status << '\n';
            return;
        }
        out << "ADS read " << std::dec << bytesRead << " bytes, value: 0x" << std::hex << buffer << '\n';
    }
}
}

void readState(long port, const AmsAddr& server, std::ostream& out)
{
    uint16_t adsState;
    uint16_t devState;

    const long status = AdsSyncReadStateReqEx(port, &server, &adsState, &devState);
    if (status) {
        out << "ADS read failed with: " << std::dec << status << '\n';
        return;
    }
    out << "ADS state: " << std::dec << adsState << " devState: " << std::dec << devState << '\n';
}

void run(const remote_target& target, std::ostream& out = std::cout)
{
    // add local route to your EtherCAT Master
    if (AdsAddRoute(target.server.netId, target.ip_v4)) {
        out << "Adding ADS route failed,\n" <<
            "- did you specified valid addresses?\n" <<
            "- is TWinCAT System Manager started?";
        return;
    }

    // open a new ADS port
    const long port = AdsPortOpenEx();
    if (!port) {
        out << "Open ADS port failed\n";
        return;
    }

    const AmsAddr& server = target.server;

    by_address::notification(port, server, out);
    by_name::notification(port, server, out);
    by_address::read(port, server, out);
    by_name::read(port, server, out);
    readState(port, server, out);

    const long closeStatus = AdsPortCloseEx(port);
    if (closeStatus) {
        out << "Close ADS port failed with: " << std::dec << closeStatus << '\n';
    }

#ifdef _WIN32
    // WORKAROUND: On Win7 std::thread::join() called in destructors
    //             of static objects might wait forever...
    AdsDelRoute(target.server.netId);
#endif
}
}

int main()
{
    // example for TWinCAT2
    const remote_target target_tc2 = {
        "10.8.9.36",                    // ip v4
        {
            { 10, 0, 96, 139, 1, 1 },   // AMS Net Id
            AMSPORT_R0_PLC              // AMS Port number
        }
    };

    /*
       // example for TWinCAT3
       const remote_target target_tc3 = {
        "192.168.0.232",                 // ip v4
        {
            { 192, 168, 0, 231, 1, 1 },  // AMS Net Id
            AMSPORT_R0_PLC_TC3           // AMS Port number
        }
       };
     */

    examples::run(target_tc2, std::cout);
}
