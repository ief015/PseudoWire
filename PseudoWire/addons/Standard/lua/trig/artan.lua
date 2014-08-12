function Component:onSpawn()
	self:SetImage("trig_artan.png")
	self:SetToolTip("Arc Tangent (Rads)\natan(a)")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.atan(self:GetInputValue(1)))
end
