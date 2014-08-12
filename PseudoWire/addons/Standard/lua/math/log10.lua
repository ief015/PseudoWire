function Component:onSpawn()
	self:SetImage("math_log10.png")
	self:SetToolTip("Logarithm\nOut = log10(A)")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.log10(self:GetInputValue(1)))
end