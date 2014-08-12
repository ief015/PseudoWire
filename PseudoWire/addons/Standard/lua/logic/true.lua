function Component:onSpawn()
	-- self:SetImage("default.png")
	self:SetOutputs({
		{ "True", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2 }
	})
	self:SetToolTip("True")
	self:TriggerOutput(1, 1)
end
