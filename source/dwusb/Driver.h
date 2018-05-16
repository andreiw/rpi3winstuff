/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

#pragma warning(disable: 4214)
#pragma warning(disable: 4201)

#include <usb.h>
#include <usbspec.h>
#include <Wdfusb.h>
#include <ucx/1.4/ucxclass.h>

typedef enum _USB_HUB_FEATURE_SELECTOR {
	C_HUB_LOCAL_POWER = 0,
	C_HUB_OVER_CURRENT = 1
} USB_HUB_FEATURE_SELECTOR, *PUSB_HUB_FEATURE_SELECTOR;

typedef enum _USB_PORT_FEATURE_SELECTOR {
	PORT_CONNECTION = 0,
	PORT_ENABLE = 1,
	PORT_SUSPEND = 2,
	PORT_OVER_CURRENT = 3,
	PORT_RESET = 4,
	PORT_LINK_STATE = 5,
	PORT_POWER = 8,
	PORT_LOW_SPEED = 9,
	C_PORT_CONNECTION = 16,
	C_PORT_ENABLE = 17,
	C_PORT_SUSPEND = 18,
	C_PORT_OVER_CURRENT = 19,
	C_PORT_RESET = 20,
	PORT_TEST = 21,
	PORT_INDICATOR = 22,
	PORT_U1_TIMEOUT = 23,
	PORT_U2_TIMEOUT = 24,
	C_PORT_LINK_STATE = 25,
	C_PORT_CONFIG_ERROR = 26,
	PORT_REMOTE_WAKE_MASK = 27,
	BH_PORT_RESET = 28,
	C_BH_PORT_RESET = 29,
	FORCE_LINKPM_ACCEPT = 30
} USB_PORT_FEATURE_SELECTOR, *PUSB_PORT_FEATURE_SELECTOR;

struct _UCX_URB_DATA {
	ULONG ProcessorNumber;
#ifdef WIN64
	ULONG Padding;
#endif
	PVOID Reserved7[7];
};

typedef VOID(*PFN_CHANNEL_CALLBACK)(PVOID);

typedef struct _TRANSFER_URB {

	struct _URB_HEADER Hdr;

	PVOID UsbdPipeHandle;
	ULONG TransferFlags;
	ULONG TransferBufferLength;
	PVOID TransferBuffer;
	PMDL TransferBufferMDL;
	union {
		ULONG Timeout;              // no Linked Urbs
		PVOID ReservedMBNull;       // fields
	};
	struct _UCX_URB_DATA UrbData;   // fields for HCD use

	union {
		struct {
			ULONG StartFrame;
			ULONG NumberOfPackets;
			ULONG ErrorCount;
			USBD_ISO_PACKET_DESCRIPTOR IsoPacket[1];
		} Isoch;
		UCHAR SetupPacket[8];
	} u;

} TRANSFER_URB, *PTRANSFER_URB;


#include "device.h"
#include "queue.h"
#include "trace.h"

#include "dwc_otg_regs.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD dwusbEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP dwusbEvtDriverContextCleanup;

WDFDEVICE
Controller_GetWdfDevice(
	_In_ UCXCONTROLLER UcxController
);

VOID
Controller_SetChannelCallback(
	_In_ UCXCONTROLLER UcxController,
	_In_ int Channel,
	_In_ PFN_CHANNEL_CALLBACK Callback,
	_In_opt_ PVOID Context
);

#define USB_MAX_ADDRESS_COUNT 127

typedef struct _USB_ADDRESS_LIST {
	RTL_BITMAP Bitmap;
	ULONG Bits[4];
} USB_ADDRESS_LIST, *PUSB_ADDRESS_LIST;

typedef struct _CONTROLLER_DATA {
	dwc_otg_core_global_regs_t* CoreGlobalRegs;
	dwc_otg_host_global_regs_t* HostGlobalRegs;

	volatile uint32_t* PcgcCtl;

	WDFDEVICE WdfDevice;
	WDFINTERRUPT WdfInterrupt;

	PFN_CHANNEL_CALLBACK ChannelCallbacks[16];
	PVOID ChannelCallbackContext[16];

	PVOID CommonBufferBase[8];
	PHYSICAL_ADDRESS CommonBufferBaseLA[8];

	BOOLEAN UsbAddressInit;
	USB_ADDRESS_LIST UsbAddressList;

	BOOLEAN SmDpcInited;
	KDPC SmDpc;

	BOOLEAN ChSmDpcInited[8];
	KDPC ChSmDpc[8];

	PEX_TIMER ChResumeTimers[8];
	PVOID ChResumeContexts[8];

	INT ChTtHubs[8];
	INT ChTtPorts[8];

	PVOID ChTrDatas[8];

	UCXROOTHUB RootHub;

	volatile char ChannelMask;
} CONTROLLER_DATA, *PCONTROLLER_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(CONTROLLER_DATA, ControllerGetData)

EXTERN_C_END
