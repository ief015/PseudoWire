function Component:onSpawn()
	self:SetImage("comp_gt.png")
	self:SetToolTip("Greater Than\nA > B")
	
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
	self:TriggerOutput(1, IFELSE(a > b, 1, 0))
end