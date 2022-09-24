ScriptName ACloserLook


; @brief Returns the current difference to adjust the fov by.
; @return a_diff - Returns the current fov difference.
Float Function GetFOVDiff() Global Native


; @brief Sets the difference to adjust the fov by.
; @param a_diff - The difference to adjust the fov by.
Function SetFOVDiff(Float a_diff) Global Native


; @brief Returns the current step to adjust the fov by each tick.
; @param a_step - Returns the current step.
Float Function GetStep() Global Native


; @brief Sets the step to adjust the fov by each tick.
; @param a_step - The step to adjust the fov by each tick.
Function SetStep(Float a_step) Global Native


; @brief Returns the currently bound key.
; @return - Returns -1 if unbound, else returns the current key binding.
Int Function GetKey() Global Native


; @brief Sets the current key binding.
; @param a_key - The key to bind.
Function SetKey(Int a_key) Global Native
