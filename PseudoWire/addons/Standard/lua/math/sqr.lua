function Component:onSpawn()
	self:SetImage("math_sqr.png")
	self:SetToolTip("Square\nOut = A*A")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local a
function Component:onInputsTriggered()
	a = self:GetInputValue(1)
	self:TriggerOutput(1, a*a )
end