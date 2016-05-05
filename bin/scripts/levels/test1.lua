mapLoader = TiledTxtMapLoader.new(level, 'test_map')
map = mapLoader:loadMap();
level:setMap(map)

player = map:getEntityPlayer(1)
player:setX(6)
player:setY(4)
player:setOnMoveScript('playermove')

print(map:getWidth())