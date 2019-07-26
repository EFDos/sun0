function update(entity, delta)
    print("Hello from " .. entity:get_name() .. "!")
    body = entity:get_rigid_body("body")

    if Keyboard.is_key_pressed(Key.Right) then
        body:apply_linear_impulse(Vector2:new(20, 0))
    end
    if Keyboard.is_key_pressed(Key.Up) then
        body:apply_linear_impulse(Vector2:new(0, -50))
    end

    if Keyboard.is_key_pressed(Key.R) then
        entity:set_position(0, 0)
        body:move_to_entity()
    end
end
