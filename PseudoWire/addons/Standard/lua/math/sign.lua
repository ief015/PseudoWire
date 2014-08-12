function sign(a)
	if(a > 0) then
		return 1
	elseif(a < 0) then
		return -1
	else
		return 0
	end
end

function Component:onSpawn()
	self:SetImage("math_sign.png")
	self:SetToolTip("Sign\n-1 0 +1")
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}
	})
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, sign(self:GetInputValue(1)))
end