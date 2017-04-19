#include "StreamerMod.h"

#include <codecvt>
#include <fstream>
#include <map>
#include <sstream>
#include <windows.h>
#include "core\Patcher.h"
#include "gh3\QbStruct.h"

#define BUF_SIZE 512
static const LPVOID loadPakDetourLoc = (LPVOID)0x4A178E;
static GH3P::Patcher g_patcher = GH3P::Patcher(__FILE__);

std::map<std::string, std::pair<std::wstring, std::wstring>> songNames;

void loadSongList();
void loadPakDetourNaked();
void loadPakDetour();

void ApplyHack()
{
	loadSongList();
	g_patcher.WriteJmp(loadPakDetourLoc, loadPakDetourNaked);
}

void loadSongList()
{
	songNames = std::map<std::string, std::pair<std::wstring, std::wstring>>();

	std::wifstream songlist("songs.csv");
	songlist.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

	wchar_t buffer[BUF_SIZE];
	std::stringstream ss;

	// Discard the csv header
	songlist.getline(buffer, BUF_SIZE);

	while (!songlist.eof())
	{
		std::string songName;
		std::wstring title;
		std::wstring artist;

		// checksum
		songlist.getline(buffer, BUF_SIZE, L',');

		// internal name
		songlist.getline(buffer, BUF_SIZE, L',');
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		songName = converter.to_bytes(buffer);

		// title
		songlist.getline(buffer, BUF_SIZE, L',');
		title = buffer;

		// artist
		songlist.getline(buffer, BUF_SIZE, L'\n');
		artist = buffer;

		songNames.insert({ songName, std::pair<std::wstring, std::wstring>(title, artist) });
	}
}

__declspec(naked) void loadPakDetourNaked()
{
	static LPVOID retAddr = (LPVOID)0x4A1794;

	__asm {
		pushad;
		push ebx;
		call loadPakDetour;
		pop ebx;
		popad;
	EXIT:
		push ebp;
		xor ebp, ebp;
		push esi;
		push edi;
		push ebp;
		jmp retAddr;
	}
}

void __cdecl loadPakDetour(GH3::QbStruct args)
{
}