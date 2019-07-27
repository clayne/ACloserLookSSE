#include "Events.h"

#include "LookHandler.h"


namespace Events
{
	InputEventHandler* InputEventHandler::GetSingleton()
	{
		static InputEventHandler singleton;
		return &singleton;
	}


	auto InputEventHandler::ReceiveEvent(RE::InputEvent** a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
		-> EventResult
	{
		using EventType = RE::InputEvent::EventType;
		using DeviceType = RE::DeviceType;

		if (_key == kInvalid) {
			return EventResult::kContinue;
		}

		if (!a_event || !*a_event) {
			return EventResult::kContinue;
		}
		auto event = *a_event;

		if (event->eventType != EventType::kButton) {
			return EventResult::kContinue;
		}

		auto button = static_cast<RE::ButtonEvent*>(event);
		if (!button->IsDown() || button->deviceType != DeviceType::kKeyboard) {
			return EventResult::kContinue;
		}

		auto mm = RE::MenuManager::GetSingleton();
		auto inputMM = RE::InputMappingManager::GetSingleton();
		if (mm->GameIsPaused() || !inputMM->IsMovementControlsEnabled()) {
			return EventResult::kContinue;
		}

		if (button->keyMask == _key) {
			auto lookHandler = LookHandler::GetSingleton();
			if (lookHandler->IsAdjusted()) {
				lookHandler->RestoreFOV();
			} else {
				lookHandler->AdjustFOV();
			}
		}

		return EventResult::kContinue;
	}


	bool InputEventHandler::Save(const SKSE::SerializationInterface* a_intfc, UInt32 a_typeCode, UInt32 a_version)
	{
		Locker locker(_lock);

		if (!a_intfc->OpenRecord(a_typeCode, a_version)) {
			return false;
		}

		if (!a_intfc->WriteRecordData(&_key, sizeof(_key))) {
			return false;
		}

		return true;
	}


	bool InputEventHandler::Load(const SKSE::SerializationInterface* a_intfc)
	{
		Locker locker(_lock);

		if (!a_intfc->ReadRecordData(&_key, sizeof(_key))) {
			return false;
		}

		return true;
	}


	void InputEventHandler::Clear()
	{
		Locker locker(_lock);
		_key = kInvalid;
	}


	UInt32 InputEventHandler::GetKey() const
	{
		Locker locker(_lock);
		return _key;
	}


	void InputEventHandler::SetKey(UInt32 a_key)
	{
		Locker locker(_lock);
		_key = a_key;
	}


	InputEventHandler::InputEventHandler() :
		_lock(),
		_key(kInvalid)
	{}


	void SinkEventHandlers()
	{
		auto inputManager = RE::InputManager::GetSingleton();
		inputManager->AddEventSink(InputEventHandler::GetSingleton());
		_MESSAGE("Added input event sink");
	}
}
