function Component:onSpawn()
	self:SetImage("trig_adcos.png")
	self:SetToolTip("Arc Cosine (Deg)\nacos(rad(a))")
	
	self:SetInputs({
		{ "A", TYPE_NUMBER, 2, 20 }
	})
	
	self:SetOutputs({
		{ "Out", TYPE_NUMBER, self:GetWidth()-2, 20 }
	})
end

function Component:onInputsTriggered()
	self:TriggerOutput(1, math.acos(math.rad(self:GetInputValue(1))))
end
