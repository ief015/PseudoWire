function Component:onSpawn()
	self:SetImage("str_len.png")
	self:SetToolTip("Length")
	
	self:SetInputs({
		{"A", TYPE_STRING, 2, 12}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, #self:GetInputValue(1))
end
