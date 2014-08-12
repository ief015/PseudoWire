local myKey = 0
local outval_down = 1
local outval_up = 0
local down = false

local txt = Text()

function Component:onSpawn()
	self:SetImage("io_key.png")
	self:SetOutputs({
		{"Out", TYPE_NUMBER, self:GetWidth()-2, self:GetHeight()/2}
	})
	
	txt:SetColor( Color(0,0,0) )
	txt:SetSize( 10 )
	txt:SetPos( Vector(self:GetWidth()/2, self:GetHeight()/2 - 8) )
end

function Component:SetText(keycode)
	local test = KeycodeToString(keycode)
	txt:SetText(test)
	
	if(txt:GetWidth() > self:GetWidth()) then
		local pos = string.find(test, " ")
		if(pos ~= nil) then
			test = string.sub(test, 1, pos-1) .. "\n" .. string.sub(test, pos+1)
			txt:SetText(test)
		end
	end
	
	txt:SetOrigin( Vector(math.floor(txt:GetWidth()/2), math.floor(txt:GetHeight()/2)) )
end

function Component:onBuildEdit()
	self:AddUIKey("key", "Keyboard Character", myKey)
	
	self:AddUISeparator()
	
	self:AddUINumber("upval", "Up Value", outval_up)
	self:AddUINumber("downval", "Down Value", outval_down)
	
	return true
end

function Component:onEditSubmit(settings)
	myKey = settings["key"]
	self:SetText(myKey)
	
	outval_up = settings["upval"]
	outval_down = settings["downval"]
	
	if(down) then
		self:TriggerOutput(1, outval_down)
	else
		self:TriggerOutput(1, outval_up)
	end
end

function Component:onGameSave()
	local data = {}
	
	data.upval = outval_up
	data.downval = outval_down
	
	data.key = myKey
	
	return data
end

function Component:onGameLoad(data)
	outval_up = data.upval
	outval_down = data.downval
	
	myKey = data.key
	self:SetText(myKey)
end

function Component:onDraw()
	self:Draw(txt)
end

function Component:onKeyDown(keycode, shift, ctrl, alt)
	if(keycode == myKey) then
		if(not down) then
			self:TriggerOutput(1, outval_down)
			down = true
		end
	end
end

function Component:onKeyUp(keycode, shift, ctrl, alt)
	if(keycode == myKey) then
		self:TriggerOutput(1, outval_up)
		down = false
	end
end