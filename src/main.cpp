#include "Events.h"
#include "Hooks.h"
#include "LookHandler.h"
#include "Papyrus.h"
#include "version.h"

enum : std::uint32_t
{
	kSerializationVersion = 1,

	kACloserLook = 'ACLK',
	kLookHandler = 'LOOK',
	kInputEventHandler = 'INPT',
};

  void InitializeLogging()
{
	auto path = SKSE::log::log_directory();
	if (!path) {
		SKSE::stl::report_and_fail("Unable to lookup SKSE logs directory.");
	}
	*path /= SKSE::PluginDeclaration::GetSingleton()->GetName();
	*path += L".log";

	std::shared_ptr<spdlog::logger> log;
	if (IsDebuggerPresent()) {
		log = std::make_shared<spdlog::logger>(
			"Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
	} else {
		log = std::make_shared<spdlog::logger>(
			"Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
	}
#ifdef NDEBUG
	log->set_level(spdlog::level::level_enum::info);
#else
	log->set_level(spdlog::level::level_enum::debug);
#endif

	log->flush_on(spdlog::level::level_enum::trace);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] [%t] [%s:%#] %v");
}


std::string DecodeTypeCode(std::uint32_t a_typeCode)
{
	constexpr std::size_t SIZE = sizeof(std::uint32_t);

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
		SKSE::log::error("Failed to save InputEventHandler!\n");
	}

	auto lookHandler = LookHandler::GetSingleton();
	if (!lookHandler->Save(a_intfc, kLookHandler, kSerializationVersion)) {
		SKSE::log::error("Failed to save LookHandler regs!\n");
	}

	SKSE::log::info("Finished saving data");
}

void LoadCallback(SKSE::SerializationInterface* a_intfc)
{
	auto inputEventHandler = Events::InputEventHandler::GetSingleton();
	inputEventHandler->Clear();
	auto lookHandler = LookHandler::GetSingleton();
	lookHandler->Clear();

	std::uint32_t type;
	std::uint32_t version;
	std::uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (version != kSerializationVersion) {
			SKSE::log::error("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, DecodeTypeCode(type).c_str());
			continue;
		}

		switch (type) {
		case kInputEventHandler:
			if (!inputEventHandler->Load(a_intfc)) {
				inputEventHandler->Clear();
				SKSE::log::error("Failed to load InputEventHandler!\n");
			}
			break;
		case kLookHandler:
			if (!lookHandler->Load(a_intfc)) {
				lookHandler->Clear();
				SKSE::log::error("Failed to load LookHandler regs!\n");
			}
			break;
		default:
			SKSE::log::error("Unrecognized record type (%s)!", DecodeTypeCode(type).c_str());
			break;
		}
	}

	SKSE::log::info("Finished loading data");
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		Events::SinkEventHandlers();
		break;
	}
}

extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	SKSE::Logger::OpenRelative(FOLDERID_Documents, L"\\My Games\\Skyrim Special Edition\\SKSE\\ACloserLookSSE.log");
	SKSE::Logger::SetPrintLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::SetFlushLevel(SKSE::Logger::Level::kDebugMessage);
	SKSE::Logger::UseLogStamp(true);

	_MESSAGE("ACloserLookSSE v%s", ACLK_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "ACloserLookSSE";
	a_info->version = ACLK_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		_FATALERROR("Loaded in editor, marking as incompatible!");
		return false;
	}

	auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		_FATALERROR("Unsupported runtime version %s!", ver.GetString().c_str());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool APIENTRY SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	_MESSAGE("ACloserLookSSE loaded");

	if (!SKSE::Init(a_skse)) {
		return false;
	}

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}

	Hooks::Install();
	Papyrus::Register();

	auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(kACloserLook);
	serialization->SetSaveCallback(SaveCallback);
	serialization->SetLoadCallback(LoadCallback);

	return true;
}
