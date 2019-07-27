#include "Hooks.h"

#include "skse64_common/SafeWrite.h"

#include "LookHandler.h"


namespace
{
	void FirstPersonStateEx::InstallHooks()
	{
		// ??_7FirstPersonState@@6B@
		constexpr std::uintptr_t VTBL = 0x016A9E70;	// 1_5_80
		REL::Offset<OnUpdate_t**> vFunc(VTBL + (0x8 * 0x3));
		_OnUpdate = *vFunc;
		SafeWrite64(vFunc.GetAddress(), unrestricted_cast<std::uintptr_t>(&FirstPersonStateEx::Hook_OnUpdate));

		_MESSAGE("Installed hook for %s", typeid(FirstPersonStateEx).name());
	}


	void FirstPersonStateEx::Hook_OnUpdate(BSTSmartPointer<TESCameraState>& a_newState)
	{
		auto lookHandler = LookHandler::GetSingleton();
		if (camera && lookHandler->NeedsUpdate()) {
			auto playerCamera = static_cast<RE::PlayerCamera*>(camera);
			lookHandler->Update(playerCamera);
		}

		_OnUpdate(this, a_newState);
	}


	void ThirdPersonStateEx::InstallHooks()
	{
		// ??_7ThirdPersonState@@6B@
		constexpr std::uintptr_t VTBL = 0x0162F2B0;	// 1_5_80
		REL::Offset<OnUpdate_t**> vFunc(VTBL + (0x8 * 0x3));
		_OnUpdate = *vFunc;
		SafeWrite64(vFunc.GetAddress(), unrestricted_cast<std::uintptr_t>(&ThirdPersonStateEx::Hook_OnUpdate));

		_MESSAGE("Installed hook for %s", typeid(ThirdPersonStateEx).name());
	}


	void ThirdPersonStateEx::Hook_OnUpdate(BSTSmartPointer<TESCameraState>& a_newState)
	{
		auto lookHandler = LookHandler::GetSingleton();
		if (camera && lookHandler->NeedsUpdate()) {
			auto playerCamera = static_cast<RE::PlayerCamera*>(camera);
			lookHandler->Update(playerCamera);
		}

		_OnUpdate(this, a_newState);
	}
}


void InstallHooks()
{
	FirstPersonStateEx::InstallHooks();
	ThirdPersonStateEx::InstallHooks();
	_MESSAGE("Installed hooks");
}
