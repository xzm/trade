#ifndef	thnht_stock_cppstkfilter_h_
#define	thnht_stock_cppstkfilter_h_

#include "stkfilter.h"

namespace thnht {
namespace stock {

struct StkFilter {

	static bool Loaded()
	{
		return instance().mod_ != 0;
	}

	static bool Load( char const* lib = "stkfilter.dll" )
	{
		return instance().mLoad(lib);
	}

	static void Unload()
	{
		instance().mUnload();
	}

	static void RegisterErrorHandler(StkErrorHandler h)
	{
		if(!Loaded())	return;
		instance().RegisterErrorHandler_(h);
	}

	static void* Allocate(DWORD size)
	{
		if(!Loaded())	return NULL;
		return instance().Allocate_(size);
	}

	static void Free(void* p)
	{
		if(!Loaded())	return;
		instance().Free_(p);
	}

	static bool PacketRegisterHandler( StkPacketHandler h )
	{
		if(!Loaded())	return FALSE;
		return 0 != instance().PacketRegisterHandler_( h );
	}

	static bool PacketGet( COMM_PACKET* buf, DWORD* pSize )
	{
		if(!Loaded())	return FALSE;
		return 0 != instance().PacketGet_(buf,pSize);
	}

	static bool FileRegisterHandler(
						StkFileBeforeReceiveHandler before,
						StkFileAfterReceiveHandler after
					)
	{
		if(!Loaded())	return FALSE;
		return 0 != instance().FileRegisterHandler_(before,after);
	}

	static bool GetCardNumber( BYTE* cardno )
	{
		if(!Loaded())	return FALSE;
		if( instance().GetCardNumber_ == 0 )
			return false;
		return 0 != instance().GetCardNumber_(cardno);
	}

	~StkFilter()
	{
		mUnload();
	}

private:

	static StkFilter& instance()
	{
		static StkFilter the_instance("stkfilter.dll");
		return the_instance;
	}

	StkFilter( char const* dll )
	{
		init();
		if( dll )
			mLoad( dll );
	}

	void mUnload()
	{
		if( ! Loaded() )
			return;

		if( FileRegisterHandler_ )
			FileRegisterHandler_( 0, 0 );
		if( PacketRegisterHandler_ )
			PacketRegisterHandler_( 0 );

		::FreeLibrary( mod_ );

		init();
	}

	void init()
	{
		mod_ = 0;
		RegisterErrorHandler_ = 0;
		Allocate_ = 0;
		Free_ = 0;
		PacketRegisterHandler_ = 0;
		PacketGet_ = 0;
		FileRegisterHandler_ = 0;
		GetCardNumber_ = 0;
	}

	bool mLoad( char const* lib )
	{
		mUnload();

		mod_ = ::LoadLibrary(lib);

		if( mod_ == 0 )
			return false;

		RegisterErrorHandler_
			= (FuncRegisterErrorHandler)::GetProcAddress(mod_,"StkRegisterErrorHandler");
		Allocate_
			= (FuncAllocate)::GetProcAddress(mod_,"StkAllocate");
		Free_
			= (FuncFree)::GetProcAddress(mod_,"StkFree");
		PacketRegisterHandler_
			= (FuncPacketRegisterHandler)::GetProcAddress(mod_,"StkPacketRegisterHandler");
		PacketGet_
			= (FuncPacketGet)::GetProcAddress(mod_,"StkPacketGet");
		FileRegisterHandler_
			= (FuncFileRegisterHandler)::GetProcAddress(mod_,"StkFileRegisterHandler");
		GetCardNumber_ = (FuncGetCardNumber)::GetProcAddress(mod_,"StkGetCardNumber");

		return true;
	}

private:

	HMODULE		mod_;

//	typedef void (__stdcall*StkErrorHandler)( DWORD e_code, char const* msg );
//	typedef BOOL (__stdcall*StkPacketHandler)( COMM_PACKET const* packet, DWORD size );

	typedef void (__stdcall* FuncRegisterErrorHandler)(StkErrorHandler);

	typedef void* (__stdcall* FuncAllocate)(DWORD);
	typedef void (__stdcall* FuncFree)(void*);

	typedef BOOL (__stdcall* FuncPacketRegisterHandler)( StkPacketHandler handler );
	typedef BOOL (__stdcall* FuncPacketGet)( COMM_PACKET* buf, DWORD* pSize );

//	typedef BOOL (__stdcall* StkFileBeforeReceiveHandler)( StkFileInfo* );
//	typedef void (__stdcall* StkFileAfterReceiveHandler)( StkFileInfo const* );
	typedef BOOL (__stdcall* FuncFileRegisterHandler)(
									StkFileBeforeReceiveHandler,
									StkFileAfterReceiveHandler
								);

	typedef BOOL (__stdcall* FuncGetCardNumber)( BYTE* cardno );

	FuncRegisterErrorHandler	RegisterErrorHandler_;
	FuncAllocate				Allocate_;
	FuncFree					Free_;
	FuncPacketRegisterHandler	PacketRegisterHandler_;
	FuncPacketGet				PacketGet_;
	FuncFileRegisterHandler		FileRegisterHandler_;
	FuncGetCardNumber			GetCardNumber_;
};

}	// namespace stock
}	// namespace thnht

#endif
