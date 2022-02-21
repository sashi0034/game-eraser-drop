

-- class Test
Test = {
    spr = nil,
    x = 0,
    y = 0,
    time = 0,

    new = function(self, spr)
        self.spr = spr
        self.x = 50
        self.y = 100
        self.spr:setXY(self.x, self.y)
        return "created Test\n"
    end,

    update = function(self)
        --self.x = self.x + 0.5
        self.x = self.x + 0.50
        self.y = self.y + 1.5
        self.time = self.time+1

        self.spr:setXY(self.x, self.y)
        
        --return "hogehoge" .. self.time
        return "x = ".. self.x .. ", y = " .. self.y .."\n"
    end,
}








