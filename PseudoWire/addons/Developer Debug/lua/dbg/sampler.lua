local label = ""

function Component:onSpawn()
	self:SetInputs({
		{"Value", TYPE_ANY, 2, self:GetHeight()/2}
	})
end

function Component:onBuildEdit()
	self:AddUIString("label", "Label", false, label)
	return true
end

function Component:onEditSubmit(settings)
	label = settings["label"]
end

function Component:onGameSave()
	local data = {}
	data.label = label
	return data
end

function Component:onGameLoad(data)
	label = data.label
end

function Component:onInputsTriggered()
	if(label ~= "") then
		Msg(label .. ": " .. self:GetInputValue(1))
	else
		Msg(self:GetInputValue(1))
	end
end
