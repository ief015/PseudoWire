function Component:onSpawn()
	self:SetImage("str_rev.png")
	self:SetToolTip("Reverse")
	
	self:SetInputs({
		{"Str", TYPE_STRING, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, string.reverse(self:GetInputValue(1)))
end
