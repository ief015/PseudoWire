function Component:onSpawn()
	self:SetImage("comp_e.png")
	self:SetToolTip("Equal To\nA == B")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12},
		{"B", TYPE_NUMBER, 2, 28}
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
	
	self:TriggerOutput(1, 1) --default is true (0==0)
end

local a,b
function Component:onInputsTriggered()
	a,b = self:GetAllInputValues()
	self:TriggerOutput(1, IFELSE(a == b, 1, 0))
end