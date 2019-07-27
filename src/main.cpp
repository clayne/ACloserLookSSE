#include "skse64_common/skse_version.h"

#include "Events.h"
#include "Hooks.h"
#include "LookHandler.h"
#include "Papyrus.h"
#include "version.h"

#include "SKSE/API.h"


namespace
{
	enum : UInt32
	{
		kSerializationVersion = 1,

		kACloserLook = 'ACLK',
		kLookHandler = 'LOOK',
		kInputEventHandler = 'INPT',
	};


	std::string DecodeTypeCode(UInt32 a_typeCode)
	{
		constexpr std::size_t SIZE = sizeof(UInt32);

		std::string sig;
		sig.resize(SIZE);
		char* iter = reinterpret_cast<char*>(&a_typeCode);
		for (std::size_t i = 0, j = SIZE - 2; i < SIZE - 1; ++i, --j) {
			sig[j] = iter[i];
		}
		return sig;
	}


	void SaveCallback(SKSE::SerializationInterface* a_intfc)
	{
		auto inputEventHandler = Events::InputEventHandler::GetSingleton();
		if (!inputEventHandler->Save(a_intfc, kInputEventHandler, kSerializationVersion)) {
			_ERROR("Failed to save InputEventHandler!\n");
		}

		auto lookHandler = LookHandler::GetSingleton();
		if (!lookHandler->Save(a_intfc, kLookHandler, kSerializationVersion)) {
			_ERROR("Failed to save LookHandler regs!\n");
		}

		_MESSAGE("Finished saving data");
	}


	void LoadCallback(SKSE::SerializationInterface* a_intfc)
	{
		UInt32 type;
		UInt32 version;
		UInt32 length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version != kSerializationVersion) {
				_ERROR("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, DecodeTypeCode(type).c_str());
				continue;
			}

			switch (type) {
			case kInputEventHandler:
				{
					auto inputEventHandler = Events::InputEventHandler::GetSingleton();
					inputEventHandler->Clear();
					if (!inputEventHandler->Load(a_intfc)) {
						_ERROR("Failed to load InputEventHandler!\n");
					}
				}
				break;
			case kLookHandler:
				{
					auto lookHandler = LookHandler::GetSingleton();
					lookHandler->Clear();
					if (!lookHandler->Load(a_intfc)) {
						_ERROR("Failed to load LookHandler regs!\n");
					}
				}
				break;
			default:
				_ERROR("Unrecognized record type (%s)!", DecodeTypeCode(type).c_str());
				break;
			}
		}

		_MESSAGE("Finished loading data");
	}


	void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type) {
		case SKSE::MessagingInterface::kDataLoaded:
			Events::SinkEventHandlers();
			break;
		}
	}
}


extern "C" {
	bool SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
	{
		SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\ACloserLookSSE.log");
		SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
		SKSE::Logger::UseLogStamp(true);
		SKSE::Logger::HookPapyrusLog(true);
		SKSE::Logger::SetPapyrusLogFilter("ACloserLook");

		_MESSAGE("ACloserLookSSE v%s", ACLK_VERSION_VERSTRING);

		a_info->infoVersion = SKSE::PluginInfo::kVersion;
		a_info->name = "ACloserLookSSE";
		a_info->version = ACLK_VERSION_MAJOR;

		if (a_skse->IsEditor()) {
			_FATALERROR("Loaded in editor, marking as incompatible!\n");
			return false;
		}

		switch (a_skse->RuntimeVersion()) {
		case RUNTIME_VERSION_1_5_73:
		case RUNTIME_VERSION_1_5_80:
			break;
		default:
			_FATALERROR("Unsupported runtime version %08X!\n", a_skse->RuntimeVersion());
			return false;
		}

		return true;
	}


	bool SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
	{
		_MESSAGE("ACloserLookSSE loaded");

		if (!SKSE::Init(a_skse)) {
			return false;
		}

		auto messaging = SKSE::GetMessagingInterface();
		if (messaging->RegisterListener("SKSE", MessageHandler)) {
			_MESSAGE("Messaging interface registration successful");
		} else {
			_FATALERROR("Messaging interface registration failed!\n");
			return false;
		}

		InstallHooks();
		RegisterPapyrusFunctions();


		auto serialization = SKSE::GetSerializationInterface();
		serialization->SetUniqueID(kACloserLook);
		serialization->SetSaveCallback(SaveCallback);
		serialization->SetLoadCallback(LoadCallback);

		return true;
	}
};
