import main


main.default_game()


def test_collision():
    main.new_game()
    main.snake_x = main.food_x
    main.snake_y = main.food_y
    main.check_collisions()
    assert main.points == 10
    assert len(main.tail) == 1
    assert not main.game_end

    main.snake_x = main.sl_x
    main.snake_y = main.sl_y
    main.check_collisions()
    assert main.points == 15
    assert len(main.tail) == 1
    assert main.speed == 2
    assert not main.game_end

    main.snake_x = main.sp_x
    main.snake_y = main.sp_y
    main.check_collisions()
    assert main.points == 35
    assert len(main.tail) == 1
    assert main.speed == 3
    assert not main.game_end

    for i in range(5):
        main.snake_x = main.food_x
        main.snake_y = main.food_y
        main.check_collisions()

    main.snake_x = main.split_x
    main.snake_y = main.split_y
    main.check_collisions()
    assert main.points == 55
    assert len(main.tail) == 3
    assert main.speed == 3
    assert not main.game_end


def test_end_game():
    main.snake_x = main.walls[0][0]
    main.snake_y = main.walls[0][1]
    main.check_collisions()
    assert main.points == 55
    assert len(main.tail) == 3
    assert main.speed == 3
    assert main.check_game()

    main.default_game()
    main.new_game()
    main.walls.clear()
    main.tail.append((main.snake_x, main.snake_y - main.tile_size))
    main.tail.append((main.snake_x - main.tile_size, main.snake_y - main.tile_size))
    main.tail.append((main.snake_x - main.tile_size, main.snake_y))
    main.tail.append((main.snake_x - main.tile_size, main.snake_y + main.tile_size))
    main.snake_dx = -main.tile_size
    main.check_collisions()
    assert main.check_game()


def test_slow_bonus_spawn():
    main.default_game()
    main.new_game()
    main.snake_x = main.sl_x
    main.snake_y = main.sl_y
    main.check_collisions()
    main.random_events()
    assert not main.slow_flag
    main.ticks = main.sl_tick
    main.check_collisions()
    main.random_events()
    assert main.slow_flag


def test_speed_bonus_spawn():
    main.default_game()
    main.new_game()
    main.snake_x = main.sp_x
    main.snake_y = main.sp_y
    main.check_collisions()
    main.random_events()
    assert not main.speed_flag
    main.ticks = main.sp_tick
    main.check_collisions()
    main.random_events()
    assert main.speed_flag


def test_splitter_bonus_spawn():
    main.default_game()
    main.new_game()
    main.snake_x = main.split_x
    main.snake_y = main.split_y
    main.check_collisions()
    main.random_events()
    assert not main.split_flag
    main.ticks = main.split_tick
    main.check_collisions()
    main.random_events()
    assert main.split_flag


def test_speed_range():
    main.default_game()
    main.new_game()
    main.speed = 6
    main.snake_x = main.sp_x
    main.snake_y = main.sp_y
    main.check_collisions()
    assert main.speed == 6

    main.speed = 0
    main.snake_x = main.sl_x
    main.snake_y = main.sl_y
    main.check_collisions()
    assert main.speed == 0


def test_new_game():
    main.default_game()
    main.new_game()

    main.snake_dx = main.tile_size
    main.check_collisions()
    main.points = 100
    main.speed = 1
    main.split_flag = False
    main.slow_flag = False
    main.speed_flag = False
    first_game = (main.snake_x, main.food_x, main.food_y, main.points, main.walls, main.speed)
    main.game_end = True
    main.new_game()

    assert main.snake_x != first_game[0] and\
           main.food_x != first_game[1] and\
           main.food_y != first_game[2] and\
           main.points != first_game[3] and\
           main.walls != first_game[4] and\
           main.speed != first_game[5]

    assert main.split_flag and main.slow_flag and main.speed_flag


def test_custom_new_game_config():
    main.split_flag = False
    main.split_tick = -1
    main.speed_flag = True
    main.slow_flag = False
    main.sl_tick = -1
    main.wall_num = 17
    main.speed = 6
    main.new_game()
    first_game = (main.split_flag, main.speed_flag, main.slow_flag, main.wall_num, main.speed)

    main.game_end = True

    main.new_game()

    assert main.split_flag == first_game[0] and \
           main.speed_flag == first_game[1] and \
           main.slow_flag == first_game[2] and \
           main.wall_num == first_game[3] and \
           main.speed == first_game[4] and \
           main.split_tick == -1 and \
           main.sp_tick == 0 and \
           main.sl_tick == -1


def test_window_edge_move():
    main.default_game()
    main.new_game()
    main.walls.clear()
    main.snake_x = main.window.width - main.tile_size
    main.snake_y = 400
    main.snake_dx = main.tile_size
    main.check_collisions()

    assert main.snake_x == 0 and main.snake_y == 400

    main.snake_x = main.tile_size
    main.snake_dx = -main.tile_size
    main.check_collisions()
    main.check_collisions()
    assert main.snake_x == main.window.width - main.tile_size and main.snake_y == 400

    main.snake_x = 400
    main.snake_dx = 0
    main.snake_y = main.window.height - main.tile_size
    main.snake_dy = main.tile_size
    main.check_collisions()

    assert main.snake_x == 400 and main.snake_y == 0

    main.snake_y = main.tile_size
    main.snake_dy = -main.tile_size
    main.check_collisions()
    main.check_collisions()

    assert main.snake_x == 400 and main.snake_y == main.window.height
