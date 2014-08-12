function Component:onSpawn()
	self:SetImage("math_pi.png")
	self:SetToolTip("Pi\n~3.14159")
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
	
	self:TriggerOutput(1, math.pi)
end
