project('SAWC', 'cpp')
inc = include_directories('include')
sdl2_dep = dependency('sdl2', required : false)


# if not sdl2_dep.found()
#   homebrew_prefix = '/opt/homebrew'
#   sdl2_dep = declare_dependency(
#     include_directories: include_directories(homebrew_prefix / 'include'),
#     link_args: ['-L' + homebrew_prefix / 'lib', '-lSDL2']
#   )
# endif

executable(
  'walk', 
  ['main.cpp', 'src/walk.cpp'], 
  include_directories: inc,
  dependencies : sdl2_dep
)
