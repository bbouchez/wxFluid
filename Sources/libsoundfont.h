/////////////////////////////////////////////////////////////////////////////
// libsoundfont.h
// A SoundFont 2 cross platform file handling library
// Copyright:   (c) Benoit BOUCHEZ
/////////////////////////////////////////////////////////////////////////////

#ifndef __LIBSOUNDFONT_H__
#define __LIBSOUNDFONT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack (push, 1)
typedef struct
{
	uint16_t wMajor;
	uint16_t wMinor;
} sfVersionTag;

typedef struct
{
	uint8_t achPresetName[20];
	uint16_t wPreset;
	uint16_t wBank;
	uint16_t wPresetBagNdx;
	uint32_t dwLibrary;
	uint32_t dwGenre;
	uint32_t dwMorphology;
} sfPresetHeader;

typedef struct
{
	uint16_t wGenNdx;
	uint16_t wModNdx;
} sfPresetBag;

typedef struct
{
	uint16_t sfModSrcOper;
	uint16_t sfModDestOper;
	int16_t modAmount;
	uint16_t sfModAmtSrcOper;
	uint16_t sfModTransOper;
} sfModList;

typedef struct {
	uint8_t byLo;
	uint8_t byHi;
} rangesType;

typedef union {
	rangesType ranges;
	int16_t shAmount;
	uint16_t wAmount;
} genAmountType;

typedef struct
{
	uint16_t sfGenOper;
	genAmountType genAmount;
} sfGenList;

typedef struct
{
	uint8_t achInstName[20];
	uint16_t wInstBagNdx;
} sfInst;

typedef struct
{
	uint16_t wInstGenNdx;
	uint16_t wInstModNdx;
} sfInstBag;

typedef struct
{
	uint16_t sfGenOper;
	uint16_t genAmount;
} sfInstGenList;

typedef enum {
	monoSample = 1,
	rightSample = 2,
	leftSample = 4,
	linkedSample = 8,
	RomMonoSample = 0x8001,
	RomRightSample = 0x8002,
	RomLeftSample = 0x8004,
	RomLinkedSample = 0x8008
} SFSampleLink;

typedef struct 
{
	uint8_t achSampleName[20];
	uint32_t dwStart;
	uint32_t dwEnd;
	uint32_t dwStartLoop;
	uint32_t dwEndLoop;
	uint32_t SampleRate;
	uint8_t byOriginalPitch;
	int8_t chPitchCorrection;
	uint16_t wSampleLink;
	uint16_t sfSampleType;			// See SFSampleLink
} sfSample;

#pragma pack (pop)

//! Create and initialize a SoundFond Handler structure
//! \return pointer to new handler if it has been created successfully, 0 otherwise
void* CreateSF2Handler(void);

int LoadSF2(void* Handler, const char* FilePath);
void ReleaseSF2Handler(void* Handler);

#ifdef __cplusplus
}
#endif

#endif