function Component:onSpawn()
	self:SetImage("trig_arsin.png")
	self:SetToolTip("Arc Sine (Rads)\nasin(a)")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.asin(self:GetInputValue(1)))
end
