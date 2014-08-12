function Component:onSpawn()
	self:SetImage("math_abs.png")
	self:SetToolTip("Absolute\nOut = abs(A)")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}, --inport 1
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 2, 12} --outport 1
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.abs(self:GetInputValue(1)))
end
