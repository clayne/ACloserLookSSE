#pragma once

class LookHandler
{
public:
	static LookHandler* GetSingleton();

	bool Save(const SKSE::SerializationInterface* a_intfc, UInt32 a_typeCode, UInt32 a_version);
	bool Load(const SKSE::SerializationInterface* a_intfc);
	void Clear();
	bool NeedsUpdate() const;
	void Update(RE::PlayerCamera* a_camera);
	bool IsAdjusted() const;
	void AdjustFOV();
	void RestoreFOV();
	float GetFOVDiff() const;
	void SetFOVDiff(float a_diff);
	float GetStep() const;
	void SetStep(float a_step);

private:
	using Lock = std::recursive_mutex;
	using Locker = std::lock_guard<Lock>;

	LookHandler();
	LookHandler(const LookHandler&) = delete;
	LookHandler(LookHandler&&) = delete;
	~LookHandler() = default;

	LookHandler& operator=(const LookHandler&) = delete;
	LookHandler& operator=(LookHandler&&) = delete;

	static constexpr float DF_TARGET_FOV_DIFF = -30.0;
	static constexpr float DF_STEP = 1.0;

	mutable Lock _lock;
	float _targetFOV;
	float _targetFOVDiff;
	float _oldFOV;
	float _step;
	bool _setTargetFOV;
	bool _adjusted;
	bool _needsUpdate;
};
