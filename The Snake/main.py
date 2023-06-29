from random import randint

from pyglet import app
from pyglet.window import Window
from pyglet import image
from pyglet import clock
from pyglet.window import key
from pyglet import text


# Default game init
def default_game():
    global wall_num, slow_flag, speed_flag, split_flag, speed, split_tick, sp_tick, sl_tick, game_end

    speed = 3
    wall_num = 5
    slow_flag = True
    speed_flag = True
    split_flag = True
    split_tick, sp_tick, sl_tick = 0, 0, 0
    game_end = False


# CLI for game configuration
def set_configuration():

    global snake_color, wall_num, slow_flag, speed_flag, split_flag, speed, split_tick, sp_tick, sl_tick, game_end

    snake_color = (0, 255, 0, 0)

    option = 0

    default_game()

    while option != '1':
        print(" \"The Snake\"\nConfiguration:")
        print("1. Play")
        print("2. Choose snake color")
        print("3. Gameplay options")
        print("4. How to play")
        print("5. Exit\n")
        option = input()

        if option == '2':
            print("Choose color")
            print("1. Green")
            print("2. Yellow")
            print("3. Pink")
            print("4. Custom\n")

            option = input()
            if option == "1":
                snake_color = (0, 255, 0, 0)
            elif option == "2":
                snake_color = (255, 255, 0, 0)
            elif option == "3":
                snake_color = (255, 203, 219, 0)
            elif option == "4":
                print("Put colors (0-255)")
                print("Red:\n")
                red = input()
                while int(red) > 255 or int(red) < 0:
                    print("Put correct values (0-255)")
                    red = input()
                print("Green:\n")
                green = input()
                while int(green) > 255 or int(green) < 0:
                    print("Put correct values (0-255)")
                    green = input()
                print("Blue:\n")
                blue = input()
                while int(blue) > 255 or int(blue) < 0:
                    print("Put correct values (0-255)")
                    blue = input()

                snake_color = (int(red), int(green), int(blue), 0)
            option = 0

        elif option == '3':
            option = 0
            while option != "4":
                print("Game modes:")
                print("1. Default")
                print("2. Without bonuses")
                print("3. Custom")
                print("4. Exit\n")

                option = input()

                if option == "1":
                    default_game()
                    option = 0

                elif option == "2":
                    slow_flag = False
                    sl_tick = -1
                    speed_flag = False
                    sp_tick = -1
                    split_flag = False
                    split_tick = -1
                    option = 0

                elif option == "3":

                    while option != "6":

                        print("Game Features:")
                        print("1. Splitter", split_flag)
                        print("2. Slower", slow_flag)
                        print("3. Speeder", speed_flag)
                        print("4. Number of walls (0 - 20)", wall_num)
                        print("5. Initial speed (0-6)", speed)
                        print("6. Exit\n")
                        option = input()

                        if option == "1":
                            split_flag = not split_flag
                            if split_flag:
                                split_tick = 0
                            else:
                                split_tick = -1
                        elif option == "2":
                            slow_flag = not slow_flag
                            if slow_flag:
                                sl_tick = 0
                            else:
                                sl_tick = -1
                        elif option == "3":
                            speed_flag = not speed_flag
                            if speed_flag:
                                sp_tick = 0
                            else:
                                sp_tick = -1
                        elif option == "4":
                            print("Put walls number:")
                            option = input()
                            while int(option) not in range(0, 21):
                                print("Put correct number (0-20):", int(option))
                                option = input()
                            wall_num = int(option)

                        elif option == "5":
                            print("Put initial speed (0-6)")
                            option = input()
                            while int(option) not in range(0, 7):
                                print("Put correct number (0-6):")
                                option = input()
                            speed = int(option)
                            option = 0
        elif option == "4":
            option = 0
            print(" Game rules:")
            print("Eat tiles to earn points")
            print("White tile - adds 10 points and increase snake's size")
            print("Red tile - adds 20 points and increase snake's speed")
            print("Blue tile - adds 5 points and slows down snake")
            print("Brown tile - decrease your points by 30 and reduce snake size by half")
            print("Green tile - obstacle")
            print("Game ends when snake eats itself or collides obstacle")
            print(" Controls:")
            print("Use arrow keys to move snake")
            print("Use spacebar to pause game")
            print("Enter 1 to main menu")
            while option != "1":
                option = input()
            option = 0
        elif option == "5":
            game_end = True
            return


global tile_size, snake_dx, snake_dy, food_x, food_y, tail, prev_key, prev_food_x, \
    prev_food_y, snake_x, snake_y, game_end, block_key, speed, pause, split_x, split_y, split_flag, sl_x, sl_y, \
    slow_flag, walls, sp_x, sp_y, speed_flag, \
    points, ticks, prev_split_tick, split_tick,\
    prev_sl_tick, sl_tick, sp_tick, prev_sp_tick,\
    speeds, snake_color, wall_num, init_speed,\
    init_sl, init_sp, init_split, init_walls


window = Window(800, 800, visible=False)


window.set_caption("The Snake")


# Draw function
def draw_tile(x, y, size, color):
    tile = image.create(size, size, image.SolidColorImagePattern(color))

    tile.blit(x, y)


def draw_walls():
    for x in walls:
        draw_tile(x[0], x[1], tile_size, (140, 200, 120, 0))


def draw_snake():
    global snake_x, snake_y, tile_size, tail, snake_color
    draw_tile(snake_x, snake_y, tile_size, snake_color)
    col = 50
    for x in tail:

        draw_tile(x[0], x[1], tile_size, (snake_color[0], col, snake_color[2], 0))
        if col != 230:
            col += 10


def draw_food():
    global food_x, food_y, tile_size
    draw_tile(food_x, food_y, tile_size, (255, 255, 255, 0))


def draw_bonuses():
    if split_flag:
        draw_tile(split_x, split_y, tile_size, (200, 100, 0, 0))

    if slow_flag:
        draw_tile(sl_x, sl_y, tile_size, (0, 0, 255, 0))

    if speed_flag:
        draw_tile(sp_x, sp_y, tile_size, (255, 0, 0, 0))


def draw_game_over():
    global points

    screen = text.Label(f"Score: {points}\n(Press Enter to restart)", font_size=30, x=window.width // 2,
                        y=window.height // 2, width=window.width, align="center", anchor_x="center", anchor_y="center",
                        multiline=True)

    screen.draw()


def draw_score():
    global points

    screen = text.Label(f"Score: {points}\n", font_size=30, x=70,
                        y=760, width=window.width, align="right", anchor_x="center", anchor_y="center",
                        multiline=True)

    screen.draw()


def set_pause():
    global pause, block_key
    if not pause:
        pause = True
        block_key = True

    else:
        pause = False
        block_key = False


# Render function
@window.event
def on_draw():
    window.clear()
    draw_food()
    draw_walls()
    draw_bonuses()
    draw_snake()
    draw_score()
    if game_end:
        draw_game_over()


# Key event handler
@window.event
def on_key_press(input_key, modifiers):
    global snake_dx, snake_dy, prev_key, block_key, game_end, pause

    if not block_key and not pause:

        if input_key == key.RIGHT and prev_key != key.LEFT:

            snake_dx = tile_size
            snake_dy = 0
            prev_key = input_key

        elif input_key == key.LEFT and prev_key != key.RIGHT:

            snake_dx = -tile_size
            snake_dy = 0
            prev_key = input_key

        elif input_key == key.DOWN and prev_key != key.UP:

            snake_dx = 0
            snake_dy = -tile_size
            prev_key = input_key

        elif input_key == key.UP and prev_key != key.DOWN:

            snake_dx = 0
            snake_dy = tile_size
            prev_key = input_key

    if input_key == key.SPACE:
        set_pause()

    if game_end and input_key == key.ENTER:
        new_game()


# Checks if game ends
def check_game():
    return (snake_x, snake_y) in tail or (snake_x, snake_y) in walls


# Generate random offset
def random_offset():
    x = randint(0, (window.width // tile_size) - 1) * tile_size
    y = randint(0, (window.height // tile_size) - 1) * tile_size

    return x, y


# Move snake and check if it collides on something
def check_collisions():
    global tail, split_flag, prev_split_tick, ticks, slow_flag, speed, speed_flag, snake_x, snake_y, points, \
        prev_sl_tick, prev_sp_tick, speeds

    tail.append((snake_x, snake_y))

    snake_x += snake_dx
    snake_y += snake_dy

    if snake_x == window.width:
        snake_x = 0
    elif snake_x == -tile_size:
        snake_x = window.width - tile_size
    if snake_y == window.height:
        snake_y = 0
    elif snake_y == -tile_size:
        snake_y = window.height

    if snake_x == split_x and snake_y == split_y and split_flag:
        if len(tail) != 0:
            tail = tail[len(tail) // 2:]
        split_flag = False
        prev_split_tick = ticks
        points -= 30

    if snake_x == sl_x and snake_y == sl_y and slow_flag:
        if speed != 0:
            speed -= 1
        clock.unschedule(update)
        clock.schedule_interval(update, speeds[speed])
        slow_flag = False
        prev_sl_tick = ticks
        points += 5

    if snake_x == sp_x and snake_y == sp_y and speed_flag:
        if speed != len(speeds) - 1:
            speed += 1
        clock.unschedule(update)
        clock.schedule_interval(update, speeds[speed])
        speed_flag = False
        prev_sp_tick = ticks
        points += 20

    if snake_x == food_x and snake_y == food_y:
        place_food()
        points += 10
    else:
        tail.pop(0)


def place_food():
    global food_x, food_y, prev_food_x, prev_food_y, walls, \
        sl_x, sl_y, split_x, split_y, sp_x, sp_y

    food_x, food_y = random_offset()

    while (food_x == prev_food_x and food_y == prev_food_y) or (food_x, food_y) in walls or \
            (food_x == snake_x and food_y == snake_y) or \
            (food_x == sl_x and food_y == sl_y) or \
            (food_x == sp_x and food_y == sp_y) or \
            (food_x == split_x and food_y == split_y):

        food_x, food_y = random_offset()

    prev_food_x = food_x
    prev_food_y = food_y


def place_splitter():
    global food_x, food_y, split_x, split_y, split_flag

    split_x, split_y = random_offset()

    while (split_x == food_x and split_y == food_y) or \
            (split_x, split_y) in walls or \
            (split_x == snake_x and split_y == snake_y) or \
            (split_x == sl_x and split_y == sl_y) or \
            (split_x == sp_x and split_y == sp_y):

        split_x, split_y = random_offset()


def place_slower():
    global food_x, food_y, sl_x, sl_y, split_x, split_y, sp_x, sp_y

    sl_x, sl_y = random_offset()

    while (sl_x == food_x and sl_y == food_y) or \
            (sl_x == split_x and sl_y == split_y) or \
            (sl_x, sl_y) in walls or \
            (sl_x == snake_x and sl_y == snake_y) or \
            (sl_x == sp_x and sl_y == sp_y):

        sl_x, sl_y = random_offset()


def place_speeder():
    global food_x, food_y, sl_x, sl_y, split_x, split_y, sp_x, sp_y

    sp_x, sp_y = random_offset()

    while (sp_x == food_x and sp_y == food_y) or \
            (sp_x == split_x and sp_y == split_y) or \
            (sp_x, sp_y) in walls or \
            (sp_x == snake_x and sp_y == snake_y) or \
            (sp_x == sl_x and sp_y == sl_y):

        sp_x, sp_y = random_offset()


# Controls bonuses spawn
def random_events():
    global split_flag, slow_flag, speed_flag, ticks, split_tick, sp_tick, sl_tick, \
        prev_split_tick, prev_sl_tick, slow_flag, prev_sp_tick, speed_flag, sp_tick

    if not split_flag and split_tick == 0:
        split_tick = randint(40, 100)

    if ticks == (prev_split_tick + split_tick):
        place_splitter()
        split_flag = True
        split_tick = 0

    if not slow_flag and sl_tick == 0:
        sl_tick = randint(50, 90)

    if ticks == (prev_sl_tick + sl_tick):
        place_slower()
        slow_flag = True
        sl_tick = 0

    if not speed_flag and sp_tick == 0:
        sp_tick = randint(50, 120)

    if ticks == (prev_sp_tick + sp_tick):
        place_speeder()
        speed_flag = True
        sp_tick = 0


# Update game state
def update(dt):
    global snake_x, snake_y, food_x, food_y, game_end,\
        points, block_key, pause, tail, split_flag, slow_flag, speed, sl_x, sl_y, speed_flag, \
        points, ticks, split_tick, sl_tick, sp_tick, prev_split_tick, prev_sl_tick, slow_flag, tile_size

    ticks += 1

    if pause:
        return

    if check_game():
        game_end = True
        block_key = True

    if game_end:
        return

    check_collisions()

    random_events()


# Init new game , set all game objects in start state.
def new_game():
    global tile_size, snake_dx, snake_dy, food_x, food_y, tail, prev_key, prev_food_x, \
        prev_food_y, snake_x, snake_y, game_end, block_key, speed, pause, split_x, split_y,\
        split_flag, sl_x, sl_y, slow_flag, walls, sp_x, sp_y, speed_flag, \
        points, ticks, prev_split_tick, split_tick, prev_sl_tick, sl_tick,\
        sp_tick, prev_sp_tick, speeds, wall_num, init_speed, init_sl, init_sp,\
        init_split, init_walls

    if not game_end:
        init_speed, init_sl, init_sp, init_split, init_walls = speed, slow_flag, speed_flag, split_flag, wall_num

    else:
        speed, slow_flag, speed_flag, split_flag, wall_num = init_speed, init_sl, init_sp, init_split, init_walls
        if not init_sp:
            sp_tick = -1
        if not init_sl:
            sl_tick = -1
        if not init_split:
            split_tick = -1

    speeds = [1 / 3, 1 / 4, 1 / 5, 1 / 6, 1 / 7, 1 / 8, 1 / 9]

    walls = []

    tile_size = 40

    clock.unschedule(update)
    clock.schedule_interval(update, speeds[speed])

    snake_dx, snake_dy = 0, 0

    food_x, food_y = 0, 0

    sl_x, sl_y = 0, 0

    sp_x, sp_y = 0, 0

    split_x, split_y = 0, 0

    tail = []

    prev_key = 0

    prev_food_x, prev_food_y = 0, 0

    points = 0
    ticks = 0
    prev_split_tick = 0

    prev_sl_tick = 0

    prev_sp_tick = 0

    snake_x = window.width // tile_size // 2 * tile_size

    snake_y = window.height // tile_size // 2 * tile_size

    for i in range(wall_num):
        wall_x, wall_y = 0, 0
        while (wall_x, wall_y) in walls or (wall_x == snake_x and wall_y == snake_y):
            wall_x, wall_y = random_offset()

        walls.append((wall_x, wall_y))

    block_key = False

    game_end = False

    pause = False

    place_food()
    if split_flag:
        place_splitter()
    if slow_flag:
        place_slower()
    if speed_flag:
        place_speeder()


if __name__ == '__main__':

    set_configuration()

    if not game_end:

        window.set_location(400, 100)
        window.set_visible()
        new_game()

        app.run()
