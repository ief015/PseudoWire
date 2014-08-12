function Component:onSpawn()
	self:SetImage("trig_arcos.png")
	self:SetToolTip("Arc Cosine (Rads)\nacos(a)")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.acos(self:GetInputValue(1)))
end
