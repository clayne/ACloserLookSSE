#include "Events.h"

#include "LookHandler.h"

namespace Events
{
	InputEventHandler* InputEventHandler::GetSingleton()
	{
		static InputEventHandler singleton;
		return std::addressof(singleton);
	}

	auto InputEventHandler::ProcessEvent(RE::InputEvent* const* a_event, [[maybe_unused]] RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
		-> EventResult
	{
		using EventType = RE::INPUT_EVENT_TYPE;
		using DeviceType = RE::INPUT_DEVICE;

		if (_key == kInvalid) {
			return EventResult::kContinue;
		}

		if (!a_event) {
			return EventResult::kContinue;
		}

		for (auto event = *a_event; event; event = event->next) {
			if (event->eventType != EventType::kButton) {
				continue;
			}

			auto button = static_cast<RE::ButtonEvent*>(event);
			if (!button->IsDown()) {
				continue;
			}

			auto key = button->idCode;
			switch (button->device) {
			case DeviceType::kMouse:
				key += kMouseOffset;
				break;
			case DeviceType::kKeyboard:
				key += kKeyboardOffset;
				break;
			case DeviceType::kGamepad:
			default:
				continue;
			}

			auto ui = RE::UI::GetSingleton();
			auto controlMap = RE::ControlMap::GetSingleton();
			if (ui->GameIsPaused() || !controlMap->IsMovementControlsEnabled()) {
				continue;
			}

			if (key == _key) {
				auto lookHandler = LookHandler::GetSingleton();
				if (lookHandler->IsAdjusted()) {
					lookHandler->RestoreFOV();
				} else {
					lookHandler->AdjustFOV();
				}
				break;
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

		if (!a_intfc->WriteRecordData(_key)) {
			return false;
		}

		return true;
	}

	bool InputEventHandler::Load(const SKSE::SerializationInterface* a_intfc)
	{
		Locker locker(_lock);

		if (!a_intfc->ReadRecordData(_key)) {
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
		auto deviceManager = RE::BSInputDeviceManager::GetSingleton();
		deviceManager->AddEventSink(InputEventHandler::GetSingleton());
		_MESSAGE("Added input event sink");
	}
}
