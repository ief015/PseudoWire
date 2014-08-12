function Component:onSpawn()
	self:SetImage("math_neg.png")
	self:SetToolTip("Negate\nOut = -A")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}, --inport 1
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 2, 12} --outport 1
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, -self:GetInputValue(1))
end
