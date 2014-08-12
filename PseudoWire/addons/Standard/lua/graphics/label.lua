local label = Text()

function Component:SetText(txt)
	label:SetText(txt)
	
	local width = label:GetWidth()
	local height = label:GetHeight()
	
	if(width == 0) then width = 12 end
	if(height == 0) then height = 16 end
	self:SetBoundsSize(Vector(width,height))
end

function Component:onSpawn()
	self:SetImage("")
	
	label:SetSize(12)
	label:SetColor( Color(164, 164, 164) )
	self:SetText("Sample Label")
end

function Component:onBuildEdit()
	self:AddUIString("text", "Label", true, label:GetText(), 10)
	return true
end

function Component:onEditSubmit(settings)
	if(settings["text"] == "") then
		self:SetText("*empty*")
	else
		self:SetText(settings["text"])
	end
end

function Component:onGameSave()
	local data = {}
	data.text = label:GetText()
	return data
end

function Component:onGameLoad(data)
	self:SetText(data.text)
end

function Component:onDraw()
	self:Draw(label)
end
