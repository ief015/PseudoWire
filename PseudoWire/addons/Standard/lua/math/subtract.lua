function Component:onSpawn()
	self:SetImage("math_sub.png")
	self:SetToolTip("Subtract\nOut = A - B - C - D")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 1, 12}, --inport 1
		{"B", TYPE_NUMBER, 1, 20}, --inport 2
		{"C", TYPE_NUMBER, 1, 28}, --inport 3
		{"D", TYPE_NUMBER, 1, 36}  --inport 4
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 1, 12} --outport 1
	})
end

local a,b,c,d
function Component:onInputsTriggered()
	a,b,c,d = self:GetAllInputValues()
	self:TriggerOutput(1, a - b - c - d)
end
