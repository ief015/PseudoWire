function Component:onSpawn()
	self:SetImage("math_ceil.png")
	self:SetToolTip("Round Up (Ceiling)\nOut = ceil(A)")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.ceil(self:GetInputValue(1)))
end