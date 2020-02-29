#pragma once

#include "RE/Skyrim.h"


namespace Papyrus
{
	class ACloserLook
	{
	public:
		static float GetFOVDiff(RE::StaticFunctionTag*);
		static void SetFOVDiff(RE::StaticFunctionTag*, float a_diff);
		static float GetStep(RE::StaticFunctionTag*);
		static void SetStep(RE::StaticFunctionTag*, float a_step);
		static UInt32 GetKey(RE::StaticFunctionTag*);
		static void SetKey(RE::StaticFunctionTag*, UInt32 a_key);

		static bool Register(RE::BSScript::Internal::VirtualMachine* a_vm);
	};


	void Register();
}
