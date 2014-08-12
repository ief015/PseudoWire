local toggleable = false
local val_off = 0
local val_on = 1

local btn_text = Text()
local show_text = false

local down = false

local img_open = "io_button_open.png"
local img_close = "io_button_closed.png"

function Component:onSpawn()
	self:SetImage(img_open)
	self:SetToolTip("Button\nOff")
	
	self:SetOutputs({
		{ "Out", 0, self:GetWidth()-2, self:GetHeight()/2 }
	})
	
	btn_text:SetPos(Vector( self:GetWidth()/2, self:GetHeight()/2 ))
	btn_text:SetSize(10)
	btn_text:SetColor(Color(0,0,0))
end

function Component:SetText(text)
	show_text = (text == "")
	btn_text:SetText(text)
	btn_text:SetOrigin(Vector(math.floor(btn_text:GetWidth()/2), math.floor(btn_text:GetHeight()/2)))
end

function Component:onGameSave()
	local data = {}
	data.toggleable = toggleable
	data.val_on = val_on
	data.val_off = val_off
	data.text = btn_text:GetText()
	data.down = down
	return data
end

function Component:onGameLoad(data)
	toggleable = data.toggleable
	val_on = data.val_on
	val_off = data.val_off
	self:SetText(data.text)
	
	if(data.down) then
		--simulate a click (just makes my life easier)
		self:onClicked(0,0,MOUSEBUTTON_LEFT)
	end
end

function Component:onBuildEdit()
	self:AddUIString("text", "Button Text", true, btn_text:GetText(), 2)
	
	self:AddUISeparator()
	
	self:AddUICheck("toggle", "Toggleable", toggleable)
	
	self:AddUISeparator()
	
	self:AddUINumber("offval", "Off Value", val_off)
	self:AddUINumber("onval", "On Value", val_on)
	
	return true
end

function Component:onEditSubmit(settings)
	
	self:SetText(settings["text"])
	
	toggleable = settings["toggle"]
	
	local prev_off = val_off
	local prev_on = val_on
	
	val_off = settings["offval"]
	val_on = settings["onval"]
	
	if(down) then
		if(val_on ~= prev_on) then
			self:TriggerOutput(1, val_on)
		end
	else
		if(val_off ~= prev_off) then
			self:TriggerOutput(1, val_off)
		end
	end
end

function Component:onDraw()
	self:Draw(btn_text)
end

function Component:onClicked(x, y, b)
	if(b ~= MOUSEBUTTON_LEFT) then return end
	
	if(toggleable) then
		if(down) then
			self:SetImage(img_open)
			down = false
			self:SetToolTip("Button\nOff")
			self:TriggerOutput(1, val_off)
			return
		end
	end
	
	self:SetImage(img_close)
	down = true
	self:SetToolTip("Button\nOn")
	self:TriggerOutput(1, val_on)
end

-- user may have unclicked outside of the button, so we use this instead of onUnclicked
function Component:onMouseReleased(x, y, b)
	if(b ~= MOUSEBUTTON_LEFT) then return end
	
	if(down and not(toggleable)) then
		self:SetImage(img_open)
		down = false
		self:SetToolTip("Button\nOff")
		self:TriggerOutput(1, val_off)
	end
	
end
