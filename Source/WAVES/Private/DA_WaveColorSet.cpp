#include "DA_WaveColorSet.h"

FWaveColorDef UDA_WaveColorSet::FindByFrequency(EWaveFrequency Freq) const
{
	for (const FWaveColorDef& Def : Colors)
	{
		if (Def.Frequency == Freq)
			return Def;
	}

	// Return a default-initialized struct if not found
	return FWaveColorDef();
}


int32 UDA_WaveColorSet::GetIndexByFrequency(EWaveFrequency Freq) const
{
	for (int32 i = 0; i < Colors.Num(); ++i)
	{
		if (Colors[i].Frequency == Freq) return i;
	}
	return INDEX_NONE;
}

EWaveFrequency UDA_WaveColorSet::GetNext(EWaveFrequency Current) const
{
	if (Colors.Num() == 0) return Current;
	const int32 Idx = GetIndexByFrequency(Current);
	const int32 NextIdx = (Idx == INDEX_NONE) ? 0 : (Idx + 1) % Colors.Num();
	return Colors[NextIdx].Frequency;
}

EWaveFrequency UDA_WaveColorSet::GetPrev(EWaveFrequency Current) const
{
	if (Colors.Num() == 0) return Current;
	const int32 Idx = GetIndexByFrequency(Current);
	const int32 PrevIdx = (Idx == INDEX_NONE) ? 0 : (Idx - 1 + Colors.Num()) % Colors.Num();
	return Colors[PrevIdx].Frequency;
}
