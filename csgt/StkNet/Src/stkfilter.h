#ifndef	thnht_hibrocast_stkfilter_h_
#define	thnht_hibrocast_stkfilter_h_

#ifdef	__cplusplus
extern "C" {
#endif

//////////////////////////////////////////////
// general APIs

typedef enum StkErrorCode {
	STK_ERROR_NONE = 0,
	STK_ERROR_LOW_MEMORY = 1,
	STK_ERROR_LOW_DISKSPACE = 2,
	STK_ERROR_CONNECT_TO_SOURCE = 3,
	STK_ERROR_RECEIVE_FROM_SOURCE = 4,
	STK_ERROR_CREATE_PATH = 5,
	STK_ERROR_CREATE_FILE = 6,

	STK_ERROR_USER_ERROR_HANDLER = 100,
	STK_ERROR_USER_PACKET_HANDLER = 101,
	STK_ERROR_USER_FILE_BEFORE_HANDLER = 102,
	STK_ERROR_USER_FILE_AFTER_HANDLER = 103,

	STK_ERROR_FILE_PACKET_STDEXCEPT = 1000,
	STK_ERROR_FILE_PACKET_MFCEXCEPT = 1001,
	STK_ERROR_PACKET_GET = 1002,
	STK_ERROR_PACKET_PUT = 1004,

	STK_ERROR_UNKNOWN = 0xffffffff
} StkErrorCode;

// e_code should be of type StkErrorCode, but for the sake of Platform ABI,
// we use DWORD instead:
typedef void (__stdcall*StkErrorHandler)( DWORD e_code, char const* msg );

void __stdcall StkRegisterErrorHandler( StkErrorHandler );

void* __stdcall StkAllocate( DWORD );
void __stdcall StkFree( void* );

// general APIs
//////////////////////////////////////////////

//////////////////////////////////////////////
// PACKET APIs

// a return value of TRUE means the packet could be discarded,
// FALSE means the packet will be read later.
typedef BOOL (__stdcall*StkPacketHandler)(struct COMM_PACKET const*, DWORD);

BOOL __stdcall StkPacketRegisterHandler( StkPacketHandler );
BOOL __stdcall StkPacketGet(struct COMM_PACKET *buf, DWORD* size);
BOOL __stdcall StkPacketPut(struct COMM_PACKET const* packet, DWORD size);

// PACKET APIs
//////////////////////////////////////////////

//////////////////////////////////////////////
// FILE APIs

// information about a to-be-received or already-received file:

#pragma pack(push,8)
typedef struct StkFileInfo {
	WORD		m_wVersion;			// version of this structure, currently 1.
	WORD		m_wFileType;		// see enum FILE_TYPE.
	DWORD		m_dwReserved;		// reserved.
	DWORD		m_dwSize;			// size of the file.
	time_t		m_tLastModified;	// last modified date/time.
	char*		m_szServerPath;		// the relative path and name on server.
	char*		m_szLocalPath;		// the path and name on local machine.
} StkFileInfo;
#pragma pack(pop)

// When the DLL sees a NEW file that is being broadcasted, it calls
// a client-registered BeforeReceiveHandler, with all the information
// about the file in an StkFileInfo structure.
//
// The BeforeReceiveHandler determines whether to receive the file, and
// where it should be stored.
//
// A return value of TRUE from the BeforeReceiveHandler indicates that the
// file should be received, FALSE otherwise.
//
// If a client software wants to store the file in a different directory
// or under a different name, the BeforeReceiveHandler should put the
// calculated path/filename into m_szLocalPath and return TRUE.
//
// m_szLocalPath is initially a null pointer, which means that m_szServerPath
// will be used to name the file.
//
// All members of the StkFileInfo structure, except m_szLocalPath, shouldn't
// be alterred in any way.
typedef BOOL (__stdcall* StkFileBeforeReceiveHandler)( StkFileInfo* );

// After the DLL successfully received a file, it calls a client-registered
// AfterReceiveHandler with the full information of the file, including the
// customized file path and name, if any.
//
// The callback shouldn't alter the passed-in structure in anyway, including
// freeing any buffer used by the structure.
//
// A client software could use this hook to read in some data, update its
// display, etc.
//
// Lengthy operations of AfterReceiveHandler is not desirable, as it would
// block the file-receive thread entirely.  If such lengthy operations are
// really neccessary, we suggest queuing the events up first, then
// processing them in another thread.
typedef void (__stdcall* StkFileAfterReceiveHandler)( StkFileInfo const* );

// A FALSE return value from StkFileRegisterHandler() means the
// file-receive thread cannot be started up or shuted down, depending on
// its parameters.
//
// A non-NULL value of the StkFileBeforeReceiveHandler will start the
// receive thread.  A NULL value will stop the receiver.  Specifically,
// StkFileRegisterHandler(0,0); stops the file-receive thread and cleans
// up anything it uses, including deleting partially received files etc.
//
// A NULL value of StkFileAfterReceiveHandler simply means that the client
// software doesn't care the file-received events.
//
// Calls to these registered functions are NOT synchronized with any client
// software threads.  If these functions access any shared resources, some
// sort of mutexes might be neccessary.
BOOL __stdcall StkFileRegisterHandler(
				StkFileBeforeReceiveHandler,
				StkFileAfterReceiveHandler
			);

// FILE APIs
//////////////////////////////////////////////

//////////////////////////////////////////////
// Misc.

BOOL __stdcall StkGetCardNumber( BYTE* );

// Misc.
//////////////////////////////////////////////


#ifdef	__cplusplus
}
#endif

#endif
