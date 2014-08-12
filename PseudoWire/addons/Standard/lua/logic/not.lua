function Component:onSpawn()
	self:SetImage("logic_not.png")
	self:SetToolTip("Not\nOut = ~a")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12} --inport 1
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 2, 12} --outport 1
	})
	
	self:TriggerOutput(1, 1) -- default is true (~0)
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, IFELSE(self:GetInputValue(1) == 0, 1, 0))
end
