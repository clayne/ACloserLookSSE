#include "LookHandler.h"


LookHandler* LookHandler::GetSingleton()
{
	static LookHandler singleton;
	return &singleton;
}


bool LookHandler::Save(const SKSE::SerializationInterface* a_intfc, UInt32 a_typeCode, UInt32 a_version)
{
	Locker locker(_lock);

	if (!a_intfc->OpenRecord(a_typeCode, a_version)) {
		return false;
	}

	if (!a_intfc->WriteRecordData(&_step, sizeof(_step))) {
		return false;
	}

	if (!a_intfc->WriteRecordData(&_targetFOVDiff, sizeof(_targetFOVDiff))) {
		return false;
	}

	return true;
}


bool LookHandler::Load(const SKSE::SerializationInterface* a_intfc)
{
	Locker locker(_lock);

	if (!a_intfc->ReadRecordData(&_step, sizeof(_step))) {
		return false;
	}

	if (!a_intfc->ReadRecordData(&_targetFOVDiff, sizeof(_targetFOVDiff))) {
		return false;
	}

	return true;
}


void LookHandler::Clear()
{
	Locker locker(_lock);
	_step = DF_STEP;
	_targetFOVDiff = DF_TARGET_FOV_DIFF;
	_setTargetFOV = false;
	_adjusted = false;
	_needsUpdate = false;
}


bool LookHandler::LookHandler::NeedsUpdate() const
{
	Locker locker(_lock);
	return _needsUpdate;
}


void LookHandler::Update(RE::PlayerCamera* a_camera)
{
	Locker locker(_lock);
	auto fov = a_camera->worldFOV;

	if (_setTargetFOV) {
		_oldFOV = fov;
		_targetFOV = fov + _targetFOVDiff;
		_setTargetFOV = false;
	}

	auto diff = std::fabs(fov - _targetFOV);
	auto step = fov <= _targetFOV ? _step : -1.0 * _step;
	if (diff > _step) {
		fov += step;
	} else {
		fov = _targetFOV;
	}
	a_camera->worldFOV = fov;
	_needsUpdate = fov != _targetFOV;
}


bool LookHandler::IsAdjusted() const
{
	Locker locker(_lock);
	return _adjusted;
}


void LookHandler::AdjustFOV()
{
	Locker locker(_lock);
	if (!_needsUpdate && !_adjusted) {
		_setTargetFOV = true;
		_needsUpdate = true;
		_adjusted = true;
	}
}


void LookHandler::RestoreFOV()
{
	Locker locker(_lock);
	_targetFOV = _oldFOV;
	_needsUpdate = true;
	_adjusted = false;
}


float LookHandler::GetFOVDiff() const
{
	Locker locker(_lock);
	return _targetFOVDiff;
}


void LookHandler::SetFOVDiff(float a_diff)
{
	Locker locker(_lock);
	_targetFOVDiff = a_diff;
}


float LookHandler::GetStep() const
{
	Locker locker(_lock);
	return _step;
}


void LookHandler::SetStep(float a_step)
{
	Locker locker(_lock);
	_step = a_step;
}


LookHandler::LookHandler() :
	_lock(),
	_targetFOV(0.0),
	_targetFOVDiff(DF_TARGET_FOV_DIFF),
	_oldFOV(0.0),
	_step(DF_STEP),
	_setTargetFOV(false),
	_adjusted(false),
	_needsUpdate(false)
{}
