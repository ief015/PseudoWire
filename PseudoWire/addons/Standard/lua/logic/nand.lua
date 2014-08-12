function Component:onSpawn()
	self:SetImage("logic_nand.png")
	self:SetToolTip("Not And\nOut = A ~& B")
	
	self:SetInputs({
		{"A", TYPE_NUMBER, 2, 12}, --inport 1
		{"B", TYPE_NUMBER, 2, 36}  --inport 2
	})
	
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth() - 2, 12} --outport 1
	})
end

local a,b
function Component:onInputsTriggered()
	a,b = self:GetAllInputValues()
	self:TriggerOutput(1, IFELSE( (a ~= 0) and (b ~= 0), 0, 1))
end
