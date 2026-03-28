/////////////////////////////////////////////////////////////////////////////
// libsoundfont.c
// A SoundFont 2 cross platform file handling library
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#include "libsoundfont.h"
#include <stdio.h>
#include <string.h>

#pragma pack (push, 1)
typedef struct {
	uint8_t ckID[4];
	uint32_t ckSize;
} TRIFF_HEADER;

#pragma pack (pop)

typedef struct {
	sfPresetHeader PresetHeader;
	sfPresetHeader* NextPresetHeader;
} tphdr_chain_element;

// "Opaque" internal structure to manage the soundfond
typedef struct {
	// INFO chunk data
	sfVersionTag ifil_VersionTag;				/* ifil - Mandatory */
	uint8_t isng_TargetSoundEngine[256];		/* isng - Mandatory */
	uint8_t INAM_SFName[256];					/* INAM - Mandatory */
	uint8_t* irom_ROMName;						/* Optional */
	sfVersionTag iver_VersionTag;				/* Optional but no need to make a pointer just for it */
	uint8_t* ICRD_SFDate;						/* Optional */
	uint8_t* IENG_EngineerName;					/* Optional */
	uint8_t* IPRD_ProductName;					/* Optional */
	uint8_t* ICOP_Copyright;					/* Optional */
	uint8_t* ICMT_Comment;						/* Optional - Shall be allocated dynamically as it can be up to 65536 bytes */
	uint8_t* ISFT_ToolName;						/* Optional */

	// sdta data

	// ptda data
	tphdr_chain_element* phdr_chain_element;
} tSFHandler;

//-----------------------------------------------------------------------------

static unsigned int ReadChunkSize(FILE* SF2File)
{
	uint32_t ChunkSize;

	fread(&ChunkSize, sizeof(ChunkSize), 1, SF2File);

	return ChunkSize;
}  // ReadChunkSize
//-----------------------------------------------------------------------------

void* CreateSF2Handler(void)
{
	tSFHandler* LocalHandler;

	LocalHandler = new tSFHandler;
	if (LocalHandler)
	{
		memset(LocalHandler, 0, sizeof(tSFHandler));
	}

	return LocalHandler;
}  // CreateSF2Handler
//-----------------------------------------------------------------------------

//! Read a INFO-list chunk and fill related fields in the Handler structure
static void ReadINFOList(FILE* SF2File, tSFHandler* Handler, unsigned int ChunkSize)
{
	unsigned int SizeCounter = 4;		// Count INFO tag
	uint8_t SubChunkName[4];
	unsigned int SubChunkSize;

	while (SizeCounter < ChunkSize)
	{
		fread(&SubChunkName[0], 1, 4, SF2File);
		SubChunkSize = ReadChunkSize(SF2File);
		SizeCounter += 8;

		// Analyze the subchunk name and read data for the subchunk if identified
		if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'f') && (SubChunkName[2] == 'i') && (SubChunkName[3] == 'l'))
		{
			fread(&Handler->ifil_VersionTag, 4, 1, SF2File);
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 's') && (SubChunkName[2] == 'n') && (SubChunkName[3] == 'g'))
		{
			if (SubChunkSize <= 256)
			{
				fread(&Handler->isng_TargetSoundEngine[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'N') && (SubChunkName[2] == 'A') && (SubChunkName[3] == 'M'))
		{
			if (SubChunkSize <= 256)
			{
				fread(&Handler->INAM_SFName, 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'r') && (SubChunkName[2] == 'o') && (SubChunkName[3] == 'm'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->irom_ROMName == 0)
					Handler->irom_ROMName = new uint8_t[256];

				if (Handler->irom_ROMName)
					fread(&Handler->irom_ROMName[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'v') && (SubChunkName[2] == 'e') && (SubChunkName[3] == 'r'))
		{
			fread(&Handler->iver_VersionTag, 4, 1, SF2File);
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'C') && (SubChunkName[2] == 'R') && (SubChunkName[3] == 'D'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->ICRD_SFDate == 0)
					Handler->ICRD_SFDate = new uint8_t[256];

				if (Handler->ICRD_SFDate)
					fread(&Handler->ICRD_SFDate[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'E') && (SubChunkName[2] == 'N') && (SubChunkName[3] == 'G'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->IENG_EngineerName == 0)
					Handler->IENG_EngineerName = new uint8_t[256];

				if (Handler->IENG_EngineerName)
					fread(&Handler->IENG_EngineerName[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'P') && (SubChunkName[2] == 'R') && (SubChunkName[3] == 'D'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->IPRD_ProductName == 0)
					Handler->IPRD_ProductName = new uint8_t[256];

				if (Handler->IPRD_ProductName)
					fread(&Handler->IPRD_ProductName[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'C') && (SubChunkName[2] == 'O') && (SubChunkName[3] == 'P'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->ICOP_Copyright == 0)
					Handler->ICOP_Copyright = new uint8_t[256];

				if (Handler->ICOP_Copyright)
					fread(&Handler->ICOP_Copyright[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'C') && (SubChunkName[2] == 'M') && (SubChunkName[3] == 'T'))
		{
			if (SubChunkSize <= 65536)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->ICMT_Comment == 0)
					Handler->ICMT_Comment = new uint8_t[65536];

				if (Handler->ICMT_Comment)
					fread(&Handler->ICMT_Comment[0], 1, SubChunkSize, SF2File);
			}
		}
		else if ((SubChunkName[0] == 'I') && (SubChunkName[1] == 'S') && (SubChunkName[2] == 'F') && (SubChunkName[3] == 'T'))
		{
			if (SubChunkSize <= 256)
			{  // Allocate the optional string buffer if not yet existing, otherwise simply replace the content
				if (Handler->ISFT_ToolName == 0)
					Handler->ISFT_ToolName = new uint8_t[256];

				if (Handler->ISFT_ToolName)
					fread(&Handler->ISFT_ToolName[0], 1, SubChunkSize, SF2File);
			}
		}
		else
		{  // Unknwon tag : simply jump over
			fseek(SF2File, SubChunkSize, SEEK_CUR);
		}
		SizeCounter += SubChunkSize;
	}
}  // ReadINFOList
//-----------------------------------------------------------------------------

static void ReadptdaList(FILE* SF2File, tSFHandler* Handler, unsigned int ChunkSize)
{
	unsigned int SizeCounter = 4;		// Count INFO tag
	uint8_t SubChunkName[4];
	unsigned int SubChunkSize;

	sfPresetHeader PresetHeader;
	sfPresetBag PresetBag;
	sfModList ModList;
	sfGenList GenList;
	sfInst Inst;
	sfInstBag InstBag;
	sfInstGenList InstGenList;
	sfSample Sample;

	unsigned int ChunkSizeCounter;

	while (SizeCounter < ChunkSize)
	{
		fread(&SubChunkName[0], 1, 4, SF2File);
		SubChunkSize = ReadChunkSize(SF2File);
		SizeCounter += 8;

		// Analyze the subchunk name and read data for the subchunk if identified
		if ((SubChunkName[0] == 'p') && (SubChunkName[1] == 'h') && (SubChunkName[2] == 'd') && (SubChunkName[3] == 'r'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&PresetHeader, sizeof(sfPresetHeader), 1, SF2File);
				ChunkSizeCounter += sizeof(sfPresetHeader);
			}
		}
		else if ((SubChunkName[0] == 'p') && (SubChunkName[1] == 'b') && (SubChunkName[2] == 'a') && (SubChunkName[3] == 'g'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&PresetBag, sizeof(sfPresetBag), 1, SF2File);
				ChunkSizeCounter += sizeof(sfPresetBag);
			}
		}
		else if ((SubChunkName[0] == 'p') && (SubChunkName[1] == 'm') && (SubChunkName[2] == 'o') && (SubChunkName[3] == 'd'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&ModList, sizeof(sfModList), 1, SF2File);
				ChunkSizeCounter += sizeof(sfModList);
			}
		}
		else if ((SubChunkName[0] == 'p') && (SubChunkName[1] == 'g') && (SubChunkName[2] == 'e') && (SubChunkName[3] == 'n'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&GenList, sizeof(sfGenList), 1, SF2File);
				ChunkSizeCounter += sizeof(sfGenList);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'n') && (SubChunkName[2] == 's') && (SubChunkName[3] == 't'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&Inst, sizeof(sfInst), 1, SF2File);
				ChunkSizeCounter += sizeof(sfInst);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'b') && (SubChunkName[2] == 'a') && (SubChunkName[3] == 'g'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&InstBag, sizeof(sfInstBag), 1, SF2File);
				ChunkSizeCounter += sizeof(sfInstBag);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'm') && (SubChunkName[2] == 'o') && (SubChunkName[3] == 'd'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&ModList, sizeof(sfModList), 1, SF2File);
				ChunkSizeCounter += sizeof(sfModList);
			}
		}
		else if ((SubChunkName[0] == 'i') && (SubChunkName[1] == 'g') && (SubChunkName[2] == 'e') && (SubChunkName[3] == 'n'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&InstGenList, sizeof(sfInstGenList), 1, SF2File);
				ChunkSizeCounter += sizeof(sfInstGenList);
			}
		}
		else if ((SubChunkName[0] == 's') && (SubChunkName[1] == 'h') && (SubChunkName[2] == 'd') && (SubChunkName[3] == 'r'))
		{
			ChunkSizeCounter = 0;
			while (ChunkSizeCounter < SubChunkSize)
			{
				fread(&Sample, sizeof(sfSample), 1, SF2File);
				ChunkSizeCounter += sizeof(sfSample);
			}
		}
		else
		{
			fseek(SF2File, SubChunkSize, SEEK_CUR);
		}

		SizeCounter += SubChunkSize;
	}
}  // ReadptdaList
//-----------------------------------------------------------------------------

int LoadSF2(void* SFHandler, const char* FilePath)
{
	FILE* SF2File;
	uint8_t ChunkName[4]; 
	unsigned int TotalSize;
	unsigned int SizeCounter;
	unsigned int ListSize;
	bool ErrFound = false;
	tSFHandler* Handler = (tSFHandler*)SFHandler;

	SF2File = fopen(FilePath, "rb");
	if (SF2File == 0) return -1;		// Can not open file

	// Check RIFF marker
	fread(&ChunkName[0], 1, 4, SF2File);
	if (ChunkName[0] != 'R') ErrFound = true;
	if (ChunkName[1] != 'I') ErrFound = true;
	if (ChunkName[2] != 'F') ErrFound = true;
	if (ChunkName[3] != 'F') ErrFound = true;

	if (ErrFound)
	{
		fclose(SF2File);
		return -2;		// Not a valid SF2 file
	}

	TotalSize = ReadChunkSize(SF2File);

	// We must now find sfbk marker
	fread(&ChunkName[0], 1, 4, SF2File);
	if (ChunkName[0] != 's') ErrFound = true;
	if (ChunkName[1] != 'f') ErrFound = true;
	if (ChunkName[2] != 'b') ErrFound = true;
	if (ChunkName[3] != 'k') ErrFound = true;

	if (ErrFound)
	{
		fclose(SF2File);
		return -2;		// Not a valid SF2 file
	}
	
	SizeCounter = 12;

	while (SizeCounter < TotalSize)
	{
		// We must now find LIST
		fread(&ChunkName[0], 1, 4, SF2File);
		SizeCounter += 4;
		if (ChunkName[0] != 'L') ErrFound = true;
		if (ChunkName[1] != 'I') ErrFound = true;
		if (ChunkName[2] != 'S') ErrFound = true;
		if (ChunkName[3] != 'T') ErrFound = true;

		if (ErrFound)
		{
			fclose(SF2File);
			return -2;		// Not a valid SF2 file
		}

		ListSize = ReadChunkSize(SF2File);
		SizeCounter += 4;

		// Expected tags from this point are : INFO, sdta, pdta. Any other tag shall be ignored
		fread(&ChunkName[0], 1, 4, SF2File);
		SizeCounter += 4;
		if ((ChunkName[0] == 'I') && (ChunkName[1] == 'N') && (ChunkName[2] == 'F') && (ChunkName[3] == 'O'))
		{
			ReadINFOList(SF2File, Handler, ListSize);
		}
		/*
		else if ((ChunkName[0] == 's') && (ChunkName[1] == 'd') && (ChunkName[2] == 't') && (ChunkName[3] == 'a'))
		{

		}
		*/
		else if ((ChunkName[0] == 'p') && (ChunkName[1] == 'd') && (ChunkName[2] == 't') && (ChunkName[3] == 'a'))
		{
			ReadptdaList(SF2File, Handler, ListSize);
		}
		else
		{  // Unknown LIST marker : jump over the complete LIST
			fseek(SF2File, ListSize-4, SEEK_CUR);
		}
		
		SizeCounter += ListSize - 4;
	}

	fclose(SF2File);

	return 0;
}  // LoadSF2
//-----------------------------------------------------------------------------

void ReleaseSF2Handler(void* SFHandler)
{
	tSFHandler* Handler = (tSFHandler*)SFHandler;

	// Delete all optional INFO chunks (dynamically allocated)
	delete Handler->irom_ROMName;
	Handler->irom_ROMName = 0;
	delete Handler->ICRD_SFDate;
	Handler->ICRD_SFDate = 0;
	delete Handler->IENG_EngineerName;
	Handler->IENG_EngineerName = 0;
	delete Handler->IPRD_ProductName;
	Handler->IPRD_ProductName = 0;
	delete Handler->ICOP_Copyright;
	Handler->ICOP_Copyright=0;
	delete Handler->ICMT_Comment;
	Handler->ICMT_Comment = 0;
	delete Handler->ISFT_ToolName;
	Handler->ISFT_ToolName = 0;

	// Delete phdr chain list
	//if (Handler->)

	delete Handler;
}  // ReleaseSF2Handler
//-----------------------------------------------------------------------------
