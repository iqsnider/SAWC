project('SAWC', 'cpp')
inc = include_directories('include')
sdl2_dep = dependency('sdl2', required : false)

srcs = files('src/walk.cpp', 'src/self_avoiding_walk.cpp', 'src/pathing_test.cpp')

executable(
  'walk', 
  ['main.cpp', srcs], 
  include_directories: inc,
  dependencies : [sdl2_dep]
)
