mapLoader = TiledTxtMapLoader.new('test_map')
map = mapLoader:loadMap();
this:setMap(map)

player = map:getEntityPlayer(1)
player:setX(6)
player:setY(4)