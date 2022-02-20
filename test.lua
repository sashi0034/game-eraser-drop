-- resorce.lua
InstanceTable = {}

function Luacall(spr, obj)
    if InstanceTable[spr]==nil then
        local str=""
        InstanceTable[spr], str = obj:new(spr)
        return str
    else
        return InstanceTable[spr]:Update()
    end
end


-- class Test
Test = {
    spr = 0,
    x = 0,
    y = 0,
    time = 0,

    new = function(self, spr)
        self.spr = spr
        self.x = 50
        self.y = 100
        self.spr:SetXY(50, 100)
        return self, "created Test\n"
    end,

    Update = function(self)
        --self.x = self.x + 0.5
        self.x = self.x + 0.50
        self.y = self.y + 1.5
        self.time = self.time+1

        self.spr:SetXY(self.spr, self.x, self.y)
        
        --return "hogehoge" .. self.time
        return "x = ".. self.x .. ",y = " .. self.y .."\n"
    end,
}

-- テストオブジェクトの生成
function LuacallTest(spr)
    return Luacall(spr, Test)
end








