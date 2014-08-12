function Component:onSpawn()
	self:SetImage("math_sqrt.png")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 36}
	})
	self:SetToolTip("Square Root\nOut = sqrt(A)")
end

local a, out
function Component:onInputsTriggered()
	a = self:GetInputValue(1)
	out = math.sqrt(a)
	
	self:TriggerOutput(1, out)
	self:SetToolTip("Square Root\nsqrt( " .. a .. " ) = " .. out)
end
