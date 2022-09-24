#pragma once

namespace Papyrus
{
	class ACloserLook
	{
	public:
		static float GetFOVDiff(RE::StaticFunctionTag*);
		static void SetFOVDiff(RE::StaticFunctionTag*, float a_diff);
		static float GetStep(RE::StaticFunctionTag*);
		static void SetStep(RE::StaticFunctionTag*, float a_step);
		static std::uint32_t GetKey(RE::StaticFunctionTag*);
		static void SetKey(RE::StaticFunctionTag*, std::uint32_t a_key);

		static bool Register(RE::BSScript::IVirtualMachine* a_vm);
	};

	void Register();
}
