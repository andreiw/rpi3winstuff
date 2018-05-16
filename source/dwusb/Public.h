/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_dwusb,
    0x621f4a60,0x81fa,0x452e,0x88,0x20,0x37,0x49,0xe8,0x58,0xfc,0xaf);
// {621f4a60-81fa-452e-8820-3749e858fcaf}
