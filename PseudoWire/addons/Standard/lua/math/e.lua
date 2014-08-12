function Component:onSpawn()
	self:SetImage("math_e.png")
	self:SetToolTip("e\n~2.718281")
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, 12}
	})
	
	self:TriggerOutput(1, 2.71828182845904523536028747135266249775724709369995)
end
