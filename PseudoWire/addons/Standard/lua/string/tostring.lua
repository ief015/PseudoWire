function Component:onSpawn()
	self:SetImage("str_tostr.png")
	self:SetToolTip("To String")
	
	self:SetInputs({
		{"Input", TYPE_ANY, 2, 12} 
	})
	
	self:SetOutputs({
		{"Out", TYPE_STRING, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, tostring(self:GetInputValue(1)))
end
