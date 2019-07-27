#pragma once

#include "RE/Skyrim.h"


namespace
{
	class FirstPersonStateEx : public RE::FirstPersonState
	{
	public:
		static void InstallHooks();

		void Hook_OnUpdate(BSTSmartPointer<TESCameraState>& a_newState);	// 03

	private:
		using OnUpdate_t = function_type_t<decltype(&FirstPersonStateEx::Hook_OnUpdate)>;
		inline static OnUpdate_t* _OnUpdate = 0;
	};


	class ThirdPersonStateEx : public RE::ThirdPersonState
	{
	public:
		static void InstallHooks();

		void Hook_OnUpdate(BSTSmartPointer<TESCameraState>& a_newState);	// 03

	private:
		using OnUpdate_t = function_type_t<decltype(&ThirdPersonStateEx::Hook_OnUpdate)>;
		inline static OnUpdate_t* _OnUpdate = 0;
	};
}


void InstallHooks();
