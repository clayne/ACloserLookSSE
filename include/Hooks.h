#pragma once

#include "RE/Skyrim.h"


namespace
{
	class FirstPersonStateEx : public RE::FirstPersonState
	{
	public:
		static void InstallHooks();

		void Hook_OnUpdate(RE::BSTSmartPointer<RE::TESCameraState>& a_newState);	// 03

	private:
		using OnUpdate_t = function_type_t<decltype(&FirstPersonState::OnUpdate)>;
		inline static OnUpdate_t* _OnUpdate = 0;
	};


	class ThirdPersonStateEx : public RE::ThirdPersonState
	{
	public:
		static void InstallHooks();

		void Hook_OnUpdate(RE::BSTSmartPointer<RE::TESCameraState>& a_newState);	// 03

	private:
		using OnUpdate_t = function_type_t<decltype(&ThirdPersonState::OnUpdate)>;
		inline static OnUpdate_t* _OnUpdate = 0;
	};
}


void InstallHooks();
