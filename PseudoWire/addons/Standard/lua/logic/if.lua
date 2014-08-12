function Component:onSpawn()
	self:SetImage("logic_cond.png")
	
	self:SetInputs({
		{ "Condition", TYPE_NUMBER, 2, 12 },
		
		{ "True Value", TYPE_NUMBER, 2, 28 },
		{ "False Value", TYPE_NUMBER, 2, 36 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 12 }
	})
	
	self:SetToolTip("If/Condition")
end

function Component:onInputsTriggered()
	if( self:GetInputValue(1) ~= 0 ) then
		self:TriggerOutput(1, self:GetInputValue(2))
	else
		self:TriggerOutput(1, self:GetInputValue(3))
	end
end
