ScriptName ACloserLook_MCM Extends SKI_ConfigBase


; Called when the config menu is initialized.
Event OnConfigInit()
	ModName = "$ACloserLook"
EndEvent


; Called when the config menu is closed.
Event OnConfigClose()
EndEvent


; Called when a version update of this script has been detected.
; a_version - The new version.
Event OnVersionUpdate(Int a_version)
EndEvent


; Called when a new page is selected, including the initial empty page.
; a_page - The name of the the current page, or "" if no page is selected.
Event OnPageReset(String a_page)
	If (a_page == "")
		SetCursorFillMode(TOP_TO_BOTTOM)
		AddSliderOptionST("FOVDiff_S", "$ACloserLook_FOVDiff_OptionText", ACloserLook.GetFOVDiff() * -1.0)
		AddSliderOptionST("Step_S", "$ACloserLook_Step_OptionText", ACloserLook.GetStep())
		AddKeyMapOptionST("Key_K", "$ACloserLook_Key_OptionText", ACloserLook.GetKey())
	EndIf
EndEvent


State FOVDiff_S
	Event OnSliderOpenST()
		SetSliderDialogStartValue(ACloserLook.GetFOVDiff() * -1.0)
		SetSliderDialogDefaultValue(30.0)
		SetSliderDialogRange(1.0, 100.0)
		SetSliderDialogInterval(1.0)
	EndEvent

	Event OnSliderAcceptST(Float a_value)
		ACloserLook.SetFOVDiff(a_value * -1.0)
		SetSliderOptionValueST(a_value)
	EndEvent

	Event OnDefaultST()
		ACloserLook.SetFOVDiff(-30.0)
		SetSliderOptionValueST(30.0)
	EndEvent

	Event OnHighlightST()
		SetInfoText("$ACloserLook_FOVDiff_InfoText")
	EndEvent
EndState


State Step_S
	Event OnSliderOpenST()
		SetSliderDialogStartValue(ACloserLook.GetStep())
		SetSliderDialogDefaultValue(1.0)
		SetSliderDialogRange(1.0, 20.0)
		SetSliderDialogInterval(1.0)
	EndEvent

	Event OnSliderAcceptST(Float a_value)
		ACloserLook.SetStep(a_value)
		SetSliderOptionValueST(a_value)
	EndEvent

	Event OnDefaultST()
		ACloserLook.SetStep(1.0)
		SetSliderOptionValueST(1.0)
	EndEvent

	Event OnHighlightST()
		SetInfoText("$ACloserLook_Step_InfoText")
	EndEvent
EndState


State Key_K
	Event OnKeyMapChangeST(Int a_newKeyCode, String a_conflictControl, String a_conflictName)
		If (HandleKeyConflict(a_conflictControl, a_conflictName))
			ACloserLook.SetKey(a_newKeyCode)
			SetKeymapOptionValueST(a_newKeyCode)
		EndIf
	EndEvent

	Event OnDefaultST()
		ACloserLook.SetKey(-1)
		SetKeymapOptionValueST(-1)
	EndEvent

	Event OnHighlightST()
		SetInfoText("$ACloserLook_Key_InfoText")
	EndEvent
EndState


Bool Function HandleKeyConflict(String a_conflictControl, String a_conflictName)
	If (a_conflictControl != "")
		String msg
		If (a_conflictName != "")
			msg = "$ACloserLook_KeyConflict_Control{" + a_conflictControl + "}_Name{" + a_conflictName + "}"
		Else
			msg = "$ACloserLook_KeyConflict_Control{" + a_conflictControl + "}"
		EndIf
		Return ShowMessage(msg, True, "$Yes", "$No")
	EndIf
	Return True
EndFunction
