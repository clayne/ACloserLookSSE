#pragma once

#include "LookHandler.h"

namespace Hooks
{
	class FirstPersonStateEx
	{
	public:
		static void InstallHooks()
		{
			REL::Relocation<std::uintptr_t> vTable{ RE::VTABLE_FirstPersonState[0] };
			_Update = vTable.write_vfunc(0x3, Hook_Update);

			SKSE::log::info("Installed hook for FirstPersonStateEx");
		}

		static void Hook_Update(RE::TESCameraState* a_this, RE::BSTSmartPointer<RE::TESCameraState>& a_newState)  // 03
		{
			auto lookHandler = LookHandler::GetSingleton();
			if (a_this->camera && lookHandler->NeedsUpdate()) {
				auto playerCamera = static_cast<RE::PlayerCamera*>(a_this->camera);
				lookHandler->Update(playerCamera);
			}

			_Update(a_this, a_newState);
		}

	private:
		static inline REL::Relocation<decltype(&RE::TESCameraState::Update)> _Update;
	};
	class HorseCameraStateEx
	{
	public:
		static void InstallHooks()
		{
			REL::Relocation<std::uintptr_t> vTable{ RE::VTABLE_HorseCameraState[0] };
			_Update = vTable.write_vfunc(0x3, Hook_Update);

			SKSE::log::info("Installed hook for HorseCameraStateEx");
		}

		static void Hook_Update(RE::TESCameraState* a_this, RE::BSTSmartPointer<RE::TESCameraState>& a_newState)  // 03
		{
			auto lookHandler = LookHandler::GetSingleton();
			if (a_this->camera && lookHandler->NeedsUpdate()) {
				auto playerCamera = static_cast<RE::PlayerCamera*>(a_this->camera);
				lookHandler->Update(playerCamera);
			}

			_Update(a_this, a_newState);
		}

	private:
		static inline REL::Relocation<decltype(&RE::TESCameraState::Update)> _Update;
	};

		class ThirdPersonStateEx
	{
	public:
		static void InstallHooks()
		{
			REL::Relocation<std::uintptr_t> vTable{ RE::VTABLE_ThirdPersonState[0] };
			_Update = vTable.write_vfunc(0x3, Hook_Update);

			SKSE::log::info("Installed hook for ThirdPersonStateEx");
		}

		static void Hook_Update(RE::TESCameraState* a_this, RE::BSTSmartPointer<RE::TESCameraState>& a_newState)  // 03
		{
			auto lookHandler = LookHandler::GetSingleton();
			if (a_this->camera && lookHandler->NeedsUpdate()) {
				auto playerCamera = static_cast<RE::PlayerCamera*>(a_this->camera);
				lookHandler->Update(playerCamera);
			}

			_Update(a_this, a_newState);
		}

	private:
		static inline REL::Relocation<decltype(&RE::TESCameraState::Update)> _Update;
	};

	inline void Install()
	{
		FirstPersonStateEx::InstallHooks();
		HorseCameraStateEx::InstallHooks();
		ThirdPersonStateEx::InstallHooks();

		SKSE::log::info("Installed hooks");
	}
}
