function Component:onSpawn()
	self:SetImage("math_interpolate.png")
	self:SetToolTip("Interpolate\nOut = A% between Min-Max")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12},
		{"Min", TYPE_NUMBER, 2, 20},
		{"Max", TYPE_NUMBER, 2, 28}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

local a,min,max
function Component:onInputsTriggered()
	a,min,max = self:GetAllInputValues()
	self:TriggerOutput(1, (a-min)/(max-min))
end