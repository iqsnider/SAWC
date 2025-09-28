project('SAWC', 'cpp')
inc = include_directories('include')
sdl2_dep = dependency('sdl2', required : false)

executable(
  'walk', 
  ['main.cpp', 'src/walk.cpp'], 
  include_directories: inc,
  dependencies : [sdl2_dep]
)
