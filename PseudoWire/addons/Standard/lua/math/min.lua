function Component:onSpawn()
	self:SetImage("math_min.png")
	self:SetToolTip("Minimum\nOut = min(a,b,c,d)")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12},
		{"B", TYPE_NUMBER, 2, 28}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local a,b
function Component:onInputsTriggered()
	a,b = self:GetAllInputValues()
	self:TriggerOutput(1, math.min(a,b))
end
