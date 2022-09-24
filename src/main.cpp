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
	auto inputEventHandler = Events::InputEventHandler::GetSingleton();
	inputEventHandler->Clear();
	auto lookHandler = LookHandler::GetSingleton();
	lookHandler->Clear();

	std::uint32_t type;
	std::uint32_t version;
	std::uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (version != kSerializationVersion) {
			_ERROR("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, DecodeTypeCode(type).c_str());
			continue;
		}

		switch (type) {
		case kInputEventHandler:
			if (!inputEventHandler->Load(a_intfc)) {
				inputEventHandler->Clear();
				_ERROR("Failed to load InputEventHandler!\n");
			}
			break;
		case kLookHandler:
			if (!lookHandler->Load(a_intfc)) {
				lookHandler->Clear();
				_ERROR("Failed to load LookHandler regs!\n");
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
