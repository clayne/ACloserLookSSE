#pragma once

namespace Events
{
	class InputEventHandler : public RE::BSTEventSink<RE::InputEvent*>
	{
	public:
		using EventResult = RE::BSEventNotifyControl;

		static InputEventHandler* GetSingleton();

		virtual EventResult ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource) override;

		bool Save(const SKSE::SerializationInterface* a_intfc, std::uint32_t a_typeCode, std::uint32_t a_version);
		bool Load(const SKSE::SerializationInterface* a_intfc);
		void Clear();
		std::uint32_t GetKey() const;
		void SetKey(std::uint32_t a_key);

	private:
		using Lock = std::recursive_mutex;
		using Locker = std::lock_guard<Lock>;

		enum : std::uint32_t
		{
			kInvalid = static_cast<std::uint32_t>(-1),
			kKeyboardOffset = 0,
			kMouseOffset = 256
		};

		InputEventHandler();
		InputEventHandler(const InputEventHandler&) = delete;
		InputEventHandler(InputEventHandler&&) = delete;
		virtual ~InputEventHandler() = default;

		InputEventHandler& operator=(const InputEventHandler&) = delete;
		InputEventHandler& operator=(InputEventHandler&&) = delete;

		mutable Lock _lock;
		std::uint32_t _key;
	};

	void SinkEventHandlers();
}
