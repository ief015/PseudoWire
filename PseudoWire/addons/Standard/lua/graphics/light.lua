local on = false
local r = 255
local g = 255
local b = 255
local img_glow = Image()
local spr_glow = Sprite()

function Component:onSpawn()
	self:SetImage("display_light_base.png")
	img_glow:Load("display_light_glow.png")
	spr_glow:SetImage(img_glow)
	spr_glow:SetBlendMode(BLEND_ADD)
	
	self:SetToolTip("Light\nOff")
	
	self:SetInputs({
		{"On", TYPE_NUMBER, 2, 12},
		{"R", TYPE_NUMBER, 2, 20},
		{"G", TYPE_NUMBER, 2, 28},
		{"B", TYPE_NUMBER, 2, 36}
	})
end

local prev_on = false
local inputs = nil
function Component:onInputsTriggered()
	prev_on = on
	inputs = self:GetInputsTable()
	on = (inputs[1] ~= 0)
	
	if(on) then
		if(self:IsConnected(2) or self:IsConnected(3) or self:IsConnected(4)) then
			r = inputs[2]
			g = inputs[3]
			b = inputs[4]
		end
		spr_glow:SetColor( Color(r*inputs[1], g*inputs[1], b*inputs[1]) )
	end
	
	-- if state didn't change, don't bother setting the tooltip. (optimization)
	if(on == prev_on) then return end
	
	if( on ) then
		self:SetToolTip("Light\nOn")
	else
		self:SetToolTip("Light\nOff")
	end
end

function Component:onDraw()
	if(on) then
		self:Draw(spr_glow)
	end
end
