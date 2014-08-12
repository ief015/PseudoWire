function Component:onSpawn()
	self:SetImage("math_add.png")
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 12 },
		{ "B", TYPE_NUMBER, 2, 20 },
		{ "C", TYPE_NUMBER, 2, 28 },
		{ "D", TYPE_NUMBER, 2, 36 },
	})
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 12 }
	})
	self:SetToolTip("Adding Gate")
end

function Component:onInputsTriggered()
	local a,b,c,d = self:GetAllInputValues()
	self:TriggerOutput(1, a+b+c+d)
end
