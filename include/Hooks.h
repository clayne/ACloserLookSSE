#pragma once

#include "RE/Skyrim.h"
#include "REL/Relocation.h"

#include "LookHandler.h"


namespace Hooks
{
	template <std::uint64_t CAMID>
	class TESCameraStateEx
	{
	public:
		using Self = TESCameraStateEx<CAMID>;


		static void InstallHooks()
		{
			REL::Offset<std::uintptr_t> vTable = REL::ID(CAMID);
			_Update = vTable.WriteVFunc(0x3, &Self::Hook_Update);

			_MESSAGE("Installed hook for %s", typeid(Self).name());
		}

		static void Hook_Update(RE::TESCameraState* a_this, RE::BSTSmartPointer<RE::TESCameraState>& a_newState)	// 03
		{
			auto lookHandler = LookHandler::GetSingleton();
			if (a_this->camera && lookHandler->NeedsUpdate()) {
				auto playerCamera = static_cast<RE::PlayerCamera*>(a_this->camera);
				lookHandler->Update(playerCamera);
			}

			_Update(a_this, a_newState);
		}

	private:
		using OnUpdate_t = decltype(&RE::TESCameraState::Update);
		static inline REL::Function<OnUpdate_t> _Update;
	};


	using FirstPersonStateEx = TESCameraStateEx<267810>;
	using HorseCameraStateEx = TESCameraStateEx<267749>;
	using ThirdPersonStateEx = TESCameraStateEx<256647>;


	inline void Install()
	{
		FirstPersonStateEx::InstallHooks();
		HorseCameraStateEx::InstallHooks();
		ThirdPersonStateEx::InstallHooks();

		_MESSAGE("Installed hooks");
	}
}
