- [x] make dino look like its running by continuously changing its sprite to point to the one with the left leg lifted and then the one with the right leg lifted
- [x] rename sky.png to bg.png
- [x] change dino.h/dino.c to use "tile" for the 8x8pixel squares in charblocks and "screenblock entry" for the 8x8bit pixel squares that are shown on the screen
- [x] make floor look more realistic
- [x] add cactus
- [x] stop using magic numbers for cactus state
- [x] get rid of unused variables in world.c
- [x] figure out where to put `cactus_state` struct
- [x] add collision detection if dino crashes into cactus
- [x] make cacti spawn in random places
- [x] fix collision detection to be less-square. right now if the bottom corner of the dino sprite which is just empty space hits the top corner of a cactus sprite, which is just empty space, that will count as a collision
- [x] move `cactus_left_lt_dino_right` function
- [x] maybe make cacti update object buffer
- [ ] run on an actual gameboy
- [ ] dont do collision detection for cacti that are too far
- [ ] figure out how to remove `& 0x01FF` suffix from a buncha places, feels hacky
- [ ] maybe add gravity to dinosaur
- [ ] maybe add a way for hard presses to make dino jump higher and soft presses to lower
- [ ] add more floor tiles or make them spawn randomly so its harder to notice a pattern
- [ ] add score that increments
- [ ] save high score somehow
- [ ] add ability to pause and it opens a menu
- [ ] add clouds in second background
- [ ] add vultures
- [ ] make select change to dark mode
- [ ] add spy vs spy theme song

# probably wont do

- [ ] figure out if there's a way to automatically determine the `x_start`/`x_end`/`y_start`/`y_end` of structs
