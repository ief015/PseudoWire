function Component:onSpawn()
	self:SetImage("str_toupper.png")
	self:SetToolTip("To Upper")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12} 
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, string.upper(self:GetInputValue(1)))
end
