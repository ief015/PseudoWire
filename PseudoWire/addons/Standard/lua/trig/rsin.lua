function Component:onSpawn()
	self:SetImage("trig_rsin.png")
	self:SetToolTip("Sine (Rads)\nsin(a)")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.sin(self:GetInputValue(1)))
end
