function Component:onSpawn()
	self:SetImage("math_floor.png")
	self:SetToolTip("Round Down (Floor)\nOut = floor(A)")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.floor(self:GetInputValue(1)))
end