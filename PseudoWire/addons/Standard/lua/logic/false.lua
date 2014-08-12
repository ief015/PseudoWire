function Component:onSpawn()
	-- self:SetImage("default.png")
	self:SetOutputs({
		{ "False", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2 }
	})
	self:SetToolTip("False")
	self:TriggerOutput(1, 0)
end
