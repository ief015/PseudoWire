function Component:onSpawn()
	self:SetImage("math_log.png")
	self:SetToolTip("Natural Logarithm\nOut = log(A)")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.log(self:GetInputValue(1)))
end